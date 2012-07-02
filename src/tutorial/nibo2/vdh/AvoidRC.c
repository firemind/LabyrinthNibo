#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/adc.h>
#include <nibo/bot.h>
#include <avr/interrupt.h>
#include <nibo/spi.h>
#include <nibo/floor.h>
#include <nibo/leds.h>
#include <nibo/copro.h>
#include <nibo/pwm.h>
#include <nibo/uart1.h>
#include <stdio.h>
#include <avr/io.h>

#define PROGRAMNAME "AvoidRC"
#define TRUE 1

//#define USE_UART

/* Thresholds for distances of obstacles */
#define MAX_DISTANCE_FRONT	0x9000
#define MAX_DISTANCE_LEFT	0x8000
#define MAX_DISTANCE_RIGHT	0x8000
#define MIN_GROUND_LEFT		0x00
#define MIN_GROUND_RIGHT	0x00

/* Speed constants */
#define SPEED_FORWARD 20
#define SPEED_BACKWARD 15
#define SPEED_TURN 10

/* RC5 commands */
#define RC5_NONE		0
#define RC5_FORWARD		0x2a
#define RC5_BACKWARD	0x27
#define RC5_LEFT		0x2b
#define RC5_RIGHT		0x28
#define RC5_ONOFF		0x30

/* States of the state machine */
#define STATE_OFF		0
#define STATE_FORWARD	1
#define STATE_BACKWARD	2
#define STATE_TURNLEFT	3AvoidRC.o
#define STATE_TURNRIGHT	4

/* Variables */
uint16_t value_off[2];
uint16_t value_on[2];
uint16_t ground_r, ground_l;
uint16_t distance_l, distance_ml, distance_m, distance_mr, distance_r;
char rs232_cmd;
uint16_t last_rc5_cmd;
uint8_t rc5_cmd;
uint16_t odoLeft, odoRight;
/* Boolean event variables */
uint8_t obstacle_left, obstacle_right, obstacle_front, obstacle_frontleftright;
/* State variable */
uint16_t state;
uint8_t state_changed;

void sendOdometry()
{
	static char str[11] = "----------";
	int16_t left, right;
	copro_update();
	left = copro_ticks_l - odoLeft;
	right = copro_ticks_r - odoRight;
	if (left==0 && right==0)
		return;
	odoLeft += left;
	odoRight += right;
	sprintf(str, "%04xl%04xr", left, right);

	// Send der odometry data via serial interface (RS232)
#ifdef USE_UART
	for (int i=0; i<10; i++) {
		if (!uart1_txfull())
			uart1_putchar(str[i]);
		else
			break;
		delay(1);
	}
	if (!uart1_txfull())
		uart1_putchar('\n');
#endif

    // Output on display
	gfx_move(20, 30);
	gfx_print_text(str);
}

uint8_t clip(int16_t val)
{
	if(val>255)
		val=255;
	if (val<0)
		val=0;
	return val;
}

void measure_ground()
{
/*
	floor_disable_ir();
	delay(1);
	floor_update();
	value_off[0] = floor_r;
	value_off[1] = floor_l;
	floor_enable_ir();
	delay(1);
	floor_update();
	value_on[0] = floor_r;
	value_on[1] = floor_l;
	floor_disable_ir();
	ground_r = clip(value_on[0]-value_off[0]);
	ground_l = clip(value_on[1]-value_off[1]);
*/
	floor_update();
	ground_r = floor_absolute[LINE_RIGHT];
	ground_l = floor_absolute[LINE_LEFT];
}

void read_sensors()
{
	measure_ground();

	// Read data from serial interface (RS232)
#ifdef USE_UART
	if (!uart1_rxempty()) {
		char c = uart1_getchar();
		if (c>='A' && c<='Z')
			rs232_cmd = c;
	}
#endif
	
	if (copro_update()) {
		distance_l = copro_distance[4];
		distance_ml = copro_distance[3];
		distance_m = copro_distance[2];
		distance_mr = copro_distance[1];
		distance_r = copro_distance[0];
		if (copro_rc5_cmd != last_rc5_cmd) {
			rc5_cmd = copro_rc5_cmd & 0xFF;
			last_rc5_cmd = copro_rc5_cmd;
		}
	}

	/* generate events */
	obstacle_front = distance_m > MAX_DISTANCE_FRONT;
	obstacle_left = distance_ml > MAX_DISTANCE_LEFT || ground_l < MIN_GROUND_LEFT 
		|| (obstacle_front && distance_ml >= distance_mr);
	obstacle_right = distance_mr > MAX_DISTANCE_RIGHT || ground_r < MIN_GROUND_RIGHT
		|| (obstacle_front && distance_ml < distance_mr);
	obstacle_frontleftright = obstacle_left && obstacle_right && obstacle_front;
}

void state_transition()
{
	/* First handle RS232 commands, because they have highest priority */
	if (rs232_cmd != 0) {
		switch (rs232_cmd)
		{
			case 'S':
				if (state == STATE_OFF) {
					copro_update();
					odoLeft = copro_ticks_l;
					odoRight = copro_ticks_r;
					state = STATE_FORWARD;
				}
				rs232_cmd = 0;
				return;
			case 'C':
				state = STATE_OFF;
				rs232_cmd = 0;
				return;
			default:
				break;
		}
	}

	/* Secondly handle RC5 commands, because they have second highest priority */
	if (rc5_cmd != RC5_NONE) {
		switch (rc5_cmd)
		{
			case RC5_ONOFF:
				if (state == STATE_OFF) {
					copro_update();
					odoLeft = copro_ticks_l;
					odoRight = copro_ticks_r;
					state = STATE_FORWARD;
				} else
					state = STATE_OFF;
				rc5_cmd = RC5_NONE;
				return;
			case RC5_FORWARD:
				state = STATE_FORWARD;
				return;
			case RC5_BACKWARD:
				state = STATE_BACKWARD;
				return;
			case RC5_LEFT:
				state = STATE_TURNLEFT;
				return;
			case RC5_RIGHT:
				state = STATE_TURNRIGHT;
				return;
			default:
				break;
		}
	}
	
	/* Now handle regular state transitions for avoiding collisions */
	switch (state) 
	{
		case STATE_FORWARD:
			if (obstacle_frontleftright)
				state = STATE_BACKWARD;
			else if (obstacle_left)
				state = STATE_TURNRIGHT;
			else if (obstacle_right)
				state = STATE_TURNLEFT;
			break;
		case STATE_BACKWARD:
			if (!obstacle_left)
				state = STATE_TURNLEFT;
			else if (!obstacle_right)
				state = STATE_TURNRIGHT;
			break;
		case STATE_TURNLEFT:
			if (!(obstacle_left || obstacle_right ||obstacle_front))
				state = STATE_FORWARD;
			else if (obstacle_left)
				state = STATE_BACKWARD;
			break;
		case STATE_TURNRIGHT:
			if (!(obstacle_left || obstacle_right ||obstacle_front))
				state = STATE_FORWARD;
			else if (obstacle_right)
				state = STATE_BACKWARD;
			break;
		default:
			break;
	}
}

void execute_state()
{
	if (!state_changed)
		return;

	sendOdometry();

	switch (state)
	{
		case STATE_OFF:
			copro_stop();
			copro_resetOdometry(0, 0);
			copro_update();
			odoLeft = copro_ticks_l;
			odoRight = copro_ticks_r;
			break;
		case STATE_FORWARD:
			copro_setSpeed(SPEED_FORWARD, SPEED_FORWARD);
			break;
		case STATE_BACKWARD:
			copro_setSpeed(-SPEED_BACKWARD, -SPEED_BACKWARD);
			break;
		case STATE_TURNLEFT:
			copro_setSpeed(-SPEED_TURN, SPEED_TURN);
			break;
		case STATE_TURNRIGHT:
			copro_setSpeed(SPEED_TURN, -SPEED_TURN);
			break;
		default:
			break;
	}
}

void display_output()
{
	char text[20]="-- -- -- -- --";
	char textRC[20]="RC5: ----";
	// LEDs zeigen Distanzen an
	for (int i=0; i<6; i++)
		leds_set_status(copro_distance[(i*5+2)/6]/0x4000, 7-i); // Die Indexformel erspart if-Abfragen
	// Boden-Sensoren
	sprintf(text, "%04x %04x",
		(int)ground_r,
		(int)ground_l);
		gfx_move(35, 40);
		gfx_print_text(text);
	// irco-Sensoren
	sprintf(text, "%02x %02x %02x %02x %02x",
		(int)(distance_r>>8),
		(int)(distance_mr>>8),
		(int)(distance_m>>8),
		(int)(distance_ml>>8),
		(int)(distance_l>>8));
	gfx_move(10, 55);
	gfx_print_text(text);
	// RC-Signal ausgeben
	sprintf(textRC, "RC5: %04x", (int)last_rc5_cmd);
	gfx_move(20, 10);
	gfx_print_text(textRC);
	// Status ausgeben
	sprintf(text, "State: %02d", (int)state);AvoidRC.o
	gfx_move(20, 20);
	gfx_print_text(text);
	// Spannung
	bot_update();
	double volt = 0.0166 * bot_supply - 1.19;
	sprintf(text, "%3.1fV", volt);
	gfx_move(20, 0);
	gfx_print_text(text);
}

/**************************************************************
 * Main program
 **************************************************************/
int main()
{
	// Initializing
	sei();
	bot_init();
	spi_init();
	floor_init();
	display_init();
	leds_init();
	pwm_init();
	gfx_init();
	gfx_move(62, 0);
	gfx_set_proportional(1);
	gfx_print_text(PROGRAMNAME);
	gfx_set_proportional(0);
	gfx_move(5, 0);
	gfx_print_char('R');
	gfx_move(118, 0);
	gfx_print_char('L');
	delay(50);
	copro_ir_startMeasure();
	copro_setSpeedParameters(5, 6, 7);
	copro_resetOdometry(0, 0);
	odoLeft = odoRight = 0;
	// Initialize serial interface (RS232)
#ifdef USE_UART
	uart1_set_baudrate(9600);
	uart1_enable();
#endif
	// Switch on headlights and display light
//	leds_set_headlights(1023);
//	leds_set_displaylight(1023);

	state = STATE_OFF;

	// Operation loop
	while (TRUE)
	{
		read_sensors();
		uint16_t last_state = state;
		state_transition();
		state_changed = state != last_state;
		execute_state();
		display_output();
//	DDRF |= _BV(6);
//	PORTF |= _BV(6);
		delay(10);
	}
	return 0;
}
