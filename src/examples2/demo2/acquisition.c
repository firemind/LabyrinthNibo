
#include "acquisition.h"

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
#include "nibo/display.h"
#include "nibo/gfx.h"
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/adc.h"
#include "nibo/floor.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
#include "nibo/xbee.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

/* Analog channels 
#define AN_FLOOR_R    0
#define AN_FLOOR_L    1
#define AN_LINE_L     2
#define AN_LINE_R     3
*/

// Raw values from floor sensors on calibration plate
#define CORR_FR 0x3e0
#define CORR_LR 0x3e0
#define CORR_LL 0x3e0
#define CORR_FL 0x3e0

uint16_t acquisition_calibration[4] = {0x3ff-CORR_FR, 0x3ff-CORR_FL, 0x3ff-CORR_LL, 0x3ff-CORR_LR};

// absys
#define LIMIT_STREET_MIN 8
// street
#define LIMIT_STREET_MAX 32
// line

void schedule_behaviour();
volatile uint8_t acquisition_schedule_lock;

volatile int16_t acquisition_val[8];
volatile int16_t acquisition_diff[4];

uint8_t acquisition_counter;
//volatile uint8_t acquisition_floor_bin;
volatile int8_t acquisition_floorstates[4];

uint8_t acquisition_frame_cnt;

static uint16_t acquisition_normalize_floor(uint8_t channel, uint16_t rawvalue) {
  uint32_t value = 0x3ff-rawvalue;
  // align to 10.6 V
  uint16_t vbat = acquisition_val[AN_VBAT];
  uint16_t divisor = vbat-306;
  value *= 355 * 0x40;
  
  divisor *= acquisition_calibration[channel];

  value /= divisor;
  return value;
}


static uint8_t acquisition_calc_diff(int16_t on_value, int16_t off_value) {
    int16_t value = on_value-off_value;
    if (value<0) return 0;
    if (value>255) return 255;
    return value;
}

volatile uint8_t acquisition_linestate = STATE_NO_LINE;

#define LINE_BOTH (_BV(AN_LINE_R) | _BV(AN_LINE_L))
#define LINE_LEFT (_BV(AN_LINE_L))
#define LINE_RIGHT (_BV(AN_LINE_R))

static void update_line_state() {
  uint8_t r = (acquisition_floorstates[AN_LINE_R]==FLOOR_LINE);
  uint8_t l = (acquisition_floorstates[AN_LINE_L]==FLOOR_LINE);
  if (l&&r) {
    acquisition_linestate=STATE_LINE;
  } else if (l) {
    acquisition_linestate=STATE_LINE_L;
  } else if (r) {
    acquisition_linestate=STATE_LINE_R;
  } else {
    switch (acquisition_linestate) {
      case STATE_NO_LINE:
      case STATE_LINE:   acquisition_linestate=STATE_NO_LINE; break;

      case STATE_NO_LINE_L:
      case STATE_LINE_L: acquisition_linestate=STATE_NO_LINE_L; break;

      case STATE_NO_LINE_R:
      case STATE_LINE_R: acquisition_linestate=STATE_NO_LINE_R; break;
    }
  }
}

static uint8_t calc_floorstate(uint8_t oldstate, uint16_t value) {
  switch (oldstate) {
    case FLOOR_LINE:   return (value>(LIMIT_STREET_MAX-3))?FLOOR_LINE:((value>=(LIMIT_STREET_MIN+0))?FLOOR_STREET:FLOOR_ABYSS);
    case FLOOR_STREET: return (value>(LIMIT_STREET_MAX+1))?FLOOR_LINE:((value>=(LIMIT_STREET_MIN-1))?FLOOR_STREET:FLOOR_ABYSS);
    case FLOOR_ABYSS:  return (value>(LIMIT_STREET_MAX+0))?FLOOR_LINE:((value>=(LIMIT_STREET_MIN+1))?FLOOR_STREET:FLOOR_ABYSS);
  }
}

uint8_t acquisition_calculate_linestate() {
  // normalize
  uint16_t value_fl = acquisition_diff[AN_FLOOR_L];
  uint16_t value_fr = acquisition_diff[AN_FLOOR_R];
  uint16_t value_ll = acquisition_diff[AN_LINE_L];
  uint16_t value_lr = acquisition_diff[AN_LINE_R];

  acquisition_floorstates[AN_LINE_L] = calc_floorstate(acquisition_floorstates[AN_LINE_L], value_ll);
  acquisition_floorstates[AN_LINE_R] = calc_floorstate(acquisition_floorstates[AN_LINE_R], value_lr);
  acquisition_floorstates[AN_FLOOR_L] = calc_floorstate(acquisition_floorstates[AN_FLOOR_L], value_fl);
  acquisition_floorstates[AN_FLOOR_R] = calc_floorstate(acquisition_floorstates[AN_FLOOR_R], value_fr);

  update_line_state(value_ll, value_lr);
}



void acquisition_setup() {
  ADCSRA = _BV(ADPS2)  // prescale faktor = 128 ADC laeuft
         | _BV(ADPS1)  // mit 16 MHz / 128 = 125 kHz
         | _BV(ADPS0)
         | _BV(ADEN)   // ADC an
         | _BV(ADFR)   // free running
         | _BV(ADIE)   // enable interrupt
         | _BV(ADSC);  // Beginne mit der Konvertierung
  adc_init(0);
  adc_init(1);
  adc_init(2);
  adc_init(3);
  adc_init(7);
  clear_output_bit(IO_LINE_FLOOR_EN);
  activate_output_bit(IO_LINE_FLOOR_EN);
}

ISR(ADC_vect) {
  if (spi_frame_cnt==acquisition_frame_cnt) {
    // skip sample...
    return;
  }

  // 104 us
  // roundtrip: 1.664 ms == 600 Hz
  uint16_t adcval; // = ADC;
  adcval = ADCL;
  adcval +=(ADCH<<8);
  ADMUX = _BV(REFS0) | ((acquisition_counter-2) & 0x07);
  
  if (acquisition_counter==0) {
    set_output_bit(IO_LINE_FLOOR_EN);
  } else if (acquisition_counter==8) {
    clear_output_bit(IO_LINE_FLOOR_EN);
    // start waiting for next spi irq...
    acquisition_frame_cnt=spi_frame_cnt;
  }

  // after dealing with the hardware store the result with irq enabled:
  sei();
  uint8_t ch = acquisition_counter&0x07;
  if (acquisition_counter&0x08) {
    switch (ch) {
      case 0:
      case 1:
      case 2:
      case 3:
        adcval = acquisition_normalize_floor(ch, adcval);
        break;
      case 4:
      case 5:
      case 6:
        break;
      case 7:
        // filter supply voltage
        adcval += 7*acquisition_val[ch];
        adcval /= 8;
        break;
    }
    cli();
    acquisition_val[ch] = adcval;
    sei();
  } else {
    switch (ch) {
      case 0:
      case 1:
      case 2:
      case 3:
        adcval = acquisition_normalize_floor(ch, adcval);
        // filter 0.125 / 0.875
        int16_t val = acquisition_calc_diff(acquisition_val[ch], adcval);
        val += acquisition_diff[ch]*7;
        val /= 8;
        cli();
        acquisition_diff[ch] = val;
        sei();
        break;
    }
  }

  // prepare next
  acquisition_counter = ((acquisition_counter-1) & 0x0f);

  if (acquisition_counter==0x07) {
    acquisition_calculate_linestate();
    cli();
    if (acquisition_schedule_lock==0) {
      acquisition_schedule_lock=1;
      sei();
      schedule_behaviour();
      cli();
      acquisition_schedule_lock=0;
    }
    sei();
  }
}


