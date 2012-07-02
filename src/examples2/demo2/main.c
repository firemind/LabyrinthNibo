/*  BSD-License:

Copyright (c) 2007 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
 * Dieses Programm wurde auf dem Messe-Parcours in der Dortmunder Westfalenhalle auf der Intermodellbau 2008 verwendet. 
 * Auf den grünen Rasenflächen führt es eine Hindernisvermeidung durch, auf den grauen Straßenstücken folgt das Programm
 * der weißen Mittellinie. Die Umschaltung zwischen den beiden Vrhalten und die Ausrichtung erfolgt auf den schwarzen 
 * Teilstücken. 
 * Programm in Aktion als Video: http://www.youtube.com/watch?v=7avyqZg0uKE
 */

#include "acquisition.h"

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
#include "nibo/display.h"
#include "nibo/gfx.h"
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
#include "nibo/xbee.h"

#include "behaviour.h"


#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)


#define ORI_TICKS_FULL 0xcb

/*
#define CORR_FL 0x1d
#define CORR_FR 0x24
#define CORR_LL 0x16
#define CORR_LR 0x1e
*/

/*
#define CORR_FL 0x20
#define CORR_FR 0x20
#define CORR_LL 0x20
#define CORR_LR 0x20
*/

// correction values are raw values from white line
/*
#define CORR_FL 0x20
#define CORR_FR 0x20
#define CORR_LL 0x19
#define CORR_LR 0x1b
*/


typedef struct {
  uint16_t calibration[4];
} persistant_t;

EEMEM persistant_t persistant;


uint8_t behaviour = BEHAVIOUR_LINE;


int16_t tspeed_l = 0;
int16_t tspeed_r = 0;

uint16_t dist_l;
uint16_t dist_fl;
uint16_t dist_f;
uint16_t dist_fr;
uint16_t dist_r;

uint8_t headlight_target;
uint8_t headlight_act;

int8_t mode;
int8_t last_mode;
int8_t mode_cnt;

float volt_flt = 10.0;

uint16_t stress;

uint8_t ori; // Range: 0-31
uint16_t ori_deg; // Range: 0-360
uint8_t ori_opt;
uint16_t pos_x;
uint16_t pos_y;

uint16_t back_cnt;

void make_sound(int interval, int time) {
  while (time) {
    int i=interval;
    clear_output_bit(IO_AUDIO);
    while (time && i) {
      time--;
      i--;
      delay_us(10);
    }
    i=interval;
    set_output_bit(IO_AUDIO);
    while (time && i) {
      time--;
      i--;
      delay_us(10);
    }

  }
}


void play_music() {
  cli();
  SPCR = 0;
  set_output_bit(IO_DISPLAY_RST);
  activate_output_bit(IO_DISPLAY_RST);
  clear_output_bit(IO_RESET_CO);
  
  activate_output_bit(IO_AUDIO);
  make_sound(127/4, 15000);
  delay_ms(10);
  make_sound(191/4, 15000);
  delay_ms(10);
  make_sound(255/4, 10000);
  delay_ms(10);
  make_sound(255/4, 10000);
  delay_ms(10);
  make_sound(191/4, 15000);
  delay_ms(1);
  
  sei();
  bot_init();
  deactivate_output_bit(IO_DISPLAY_RST);
  clear_output_bit(IO_DISPLAY_RST);
  spi_init();
}


void print_hex (uint8_t val) {
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}


void print_prop_text(uint8_t x, uint8_t y, const char * text) {
  gfx_move(x, y);
  gfx_set_proportional(1);
  gfx_print_text(text);
}


void print_hex8(uint8_t x, uint8_t y, uint8_t val) {
  gfx_move(x, y);
  gfx_set_proportional(0);
  print_hex(val);
}


void print_hex16(uint8_t x, uint8_t y, uint16_t val) {
  gfx_move(x, y);
  gfx_set_proportional(0);
  print_hex(HIBYTE(val));
  print_hex(LOBYTE(val));
}


void update_ori() {
  static int16_t last_ticks_l = 0;
  static int16_t last_ticks_r = 0;
  static int16_t ori_ticks = 0;
  
  int8_t delta_l = copro_ticks_l-last_ticks_l;
  int8_t delta_r = copro_ticks_r-last_ticks_r;

  last_ticks_l = copro_ticks_l;
  last_ticks_r = copro_ticks_r;

  ori_ticks += delta_l-delta_r;

  while (ori_ticks<0) ori_ticks += ORI_TICKS_FULL;
  while (ori_ticks>=ORI_TICKS_FULL) ori_ticks -= ORI_TICKS_FULL;

  ori = (ori_ticks*32)/ORI_TICKS_FULL;
  ori_deg = (ori_ticks*(360/4))/(ORI_TICKS_FULL/4);
}


void transmit_update1() {
  xbee_tx_address = 0xffff;
  
  xbee_tx_buf[0] = 1; // type

  // position
  xbee_tx_buf[1] = HIBYTE(pos_x);
  xbee_tx_buf[2] = LOBYTE(pos_x);
  xbee_tx_buf[3] = HIBYTE(pos_y);
  xbee_tx_buf[4] = LOBYTE(pos_y);
  xbee_tx_buf[5] = HIBYTE(ori_deg);
  xbee_tx_buf[6] = LOBYTE(ori_deg);

  // distance sensors
  xbee_tx_buf[7] = HIBYTE(copro_distance[0]);
  xbee_tx_buf[8] = LOBYTE(copro_distance[0]);
  xbee_tx_buf[9] = HIBYTE(copro_distance[1]);
  xbee_tx_buf[10] = LOBYTE(copro_distance[1]);
  xbee_tx_buf[11] = HIBYTE(copro_distance[2]);
  xbee_tx_buf[12] = LOBYTE(copro_distance[2]);
  xbee_tx_buf[13] = HIBYTE(copro_distance[3]);
  xbee_tx_buf[14] = LOBYTE(copro_distance[3]);
  xbee_tx_buf[15] = HIBYTE(copro_distance[4]);
  xbee_tx_buf[16] = LOBYTE(copro_distance[4]);
  xbee_tx_buf[17] = HIBYTE(copro_distance[5]);
  xbee_tx_buf[18] = LOBYTE(copro_distance[5]);

  // line/floor sensors
  xbee_tx_buf[17] = acquisition_getDiff(AN_FLOOR_L);
  xbee_tx_buf[18] = acquisition_getDiff(AN_FLOOR_R);
  xbee_tx_buf[19] = acquisition_getDiff(AN_LINE_L);
  xbee_tx_buf[20] = acquisition_getDiff(AN_LINE_R);

  // mode
  xbee_tx_buf[21] = mode;

  xbee_tx_start(22);
}

void show_recharge_screen() {
  gfx_fill(0x00);
  print_prop_text(25, 20, "Please recharge");
  print_prop_text(35, 30, "batteries!");
  gfx_set_proportional(0);
  while (1) {
    leds_set_headlights(0);
    clear_output_bit(IO_RESET_CO);
    leds_set_displaylight(500);
    IO_LEDS_RED_PORT = 0xaa;
    IO_LEDS_GREEN_PORT = 0;
    delay(500);
    leds_set_displaylight(0);
    IO_LEDS_RED_PORT = 0x55;
    IO_LEDS_GREEN_PORT = 0;
    delay(500);
  }
}


int main(void) {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  spi_init();
  acquisition_setup();
  xbee_set_baudrate(9600);
  xbee_enable();

  if (display_init()!=0) {
    leds_set_displaylight(1024);
    if (display_type==2) {
      gfx_init();
    }
  }
  
  eeprom_read_block (acquisition_calibration, persistant.calibration, sizeof(acquisition_calibration));
  
  gfx_fill(0x00);
  print_prop_text(15, 0, "nibo");
  gfx_set_proportional(0);

  //copro_ir_startMeasure();
  delay(10);
  //motco_setSpeedParameters(5, 4, 6); // ki, kp, kd
  copro_setSpeedParameters(15, 20, 10); // ki, kp, kd

  stress = 0;

  //Setup Jumper...
  deactivate_output_bit(IO_INPUT_2);
  activate_output_bit(IO_INPUT_3);
  set_output_bit(IO_INPUT_2);
  clear_output_bit(IO_INPUT_3);


  if (!get_input_bit(IO_INPUT_1)) {
    behaviour = BEHAVIOUR_CALIB;
    
    acquisition_calibration[0]=0x20;
    acquisition_calibration[1]=0x20;
    acquisition_calibration[2]=0x20;
    acquisition_calibration[3]=0x20;
    
    print_prop_text(21, 30, "CALIBRATION");
    
    delay(100);
    // Floor

    uint16_t values[4];
    values[0]=0;
    values[1]=0;
    values[2]=0;
    values[3]=0;

    for (uint8_t i = 0; i<5; i++) {
      for (uint8_t j = 0; j<10; j++) {
        if (j>2) {
          values[0] += acquisition_getValue(0);
          values[1] += acquisition_getValue(1);
          values[2] += acquisition_getValue(2);
          values[3] += acquisition_getValue(3);
        }
        gfx_set_proportional(0);
        gfx_draw_mode(GFX_DM_JAM2);
        gfx_move(31, 41);
        print_hex(acquisition_getValue(AN_FLOOR_R));
        gfx_print_char(' ');
        print_hex(acquisition_getValue(AN_LINE_R));
        gfx_print_char(' ');
        print_hex(acquisition_getValue(AN_LINE_L));
        gfx_print_char(' ');
        print_hex(acquisition_getValue(AN_FLOOR_L));
        delay(100);
      }
      gfx_move(81+4*i, 30);
      gfx_print_char('.');
    }

    acquisition_calibration[0] = values[0]/71;
    acquisition_calibration[1] = values[1]/71;
    acquisition_calibration[2] = values[2]/71;
    acquisition_calibration[3] = values[3]/71;
    
    eeprom_write_block (acquisition_calibration, persistant.calibration, sizeof(acquisition_calibration));
    
    
    delay(100);
    gfx_print_text(" OK");
    IO_LEDS_GREEN_PORT = 0xff;
    while (1) {
      // Floor
      gfx_set_proportional(0);
      gfx_draw_mode(GFX_DM_JAM2);
      gfx_move(31, 49);
      print_hex(acquisition_getValue(AN_FLOOR_R));
      gfx_print_char(' ');
      print_hex(acquisition_getValue(AN_LINE_R));
      gfx_print_char(' ');
      print_hex(acquisition_getValue(AN_LINE_L));
      gfx_print_char(' ');
      print_hex(acquisition_getValue(AN_FLOOR_L));
      delay(10);
    }
  }
  
  while (1) {

    //while (!uart0_rxempty() && !uart0_txfull()) {
    //  uart0_putchar(uart0_getchar());
    //}

    gfx_draw_mode(GFX_DM_JAM2);

    //*
    if (xbee_tx_idle()) {
      transmit_update1();
    }
    //delay(50);
    //*/
    delay(10);

    // Spannung
    //bot_update();
    char text[20];
    float volt = 0.0160 * acquisition_getValue(AN_VBAT);
    sprintf(text, "%2.1fV ", (double)volt);
    gfx_move(45, 0);
    gfx_print_text(text);

    volt_flt = 0.9*volt_flt+0.1*volt;

    if (volt_flt<8.0) {
      show_recharge_screen();
    }

    // Ori
    gfx_move(0, 25);
    sprintf(text, "or:%3i ", (int)ori_deg);
    gfx_print_text(text);
    sprintf(text, "op:%2i/%2i ", (int)ori_opt, (int)ori);
    gfx_print_text(text);
    sprintf(text, "m:%1i", (int)mode);
    gfx_print_text(text);
    
    // Floor
    gfx_move(31, 49);
    print_hex(acquisition_getDiff(AN_FLOOR_R));
    gfx_print_char(' ');
    print_hex(acquisition_getDiff(AN_LINE_R));
    gfx_print_char(' ');
    print_hex(acquisition_getDiff(AN_LINE_L));
    gfx_print_char(' ');
    print_hex(acquisition_getDiff(AN_FLOOR_L));

    // Distance
    gfx_move(4, 57);
    gfx_print_text("R  ");
    print_hex(dist_r);
    gfx_print_char(' ');
    print_hex(dist_fr);
    gfx_print_char(' ');
    print_hex(dist_f);
    gfx_print_char(' ');
    print_hex(dist_fl);
    gfx_print_char(' ');
    print_hex(dist_l);
    gfx_print_text("  L");

////////////////////////////

    print_hex16(0, 33, copro_ticks_r);
    print_hex16(105, 33, copro_ticks_l);

    print_hex16(0, 41, copro_speed_r);
    print_hex16(105, 41, copro_speed_l);

    print_hex16(0, 49, copro_current_r);
    print_hex16(105, 49, copro_current_l);

  }
}


void schedule_behaviour() {
  // called at 100 Hz / 10ms
    
  if (behaviour == BEHAVIOUR_CALIB){
    return;
  }

  if (!get_input_bit(IO_INPUT_1)) {
    play_music();
  }
  
  copro_update();
  update_ori();
  uint8_t new_behaviour = behaviour;
  
  switch (behaviour) {
    case BEHAVIOUR_LINE:     new_behaviour = behaviour_follow_line(); break;
    case BEHAVIOUR_OBSTACLE: new_behaviour = behaviour_obstacle_avoidance(); break;
  }

  if (new_behaviour != behaviour) {
    behaviour=new_behaviour;
    switch (behaviour) {
      case BEHAVIOUR_LINE:     behaviour_follow_line_start(); break;
      case BEHAVIOUR_OBSTACLE: behaviour_obstacle_avoidance_start(); break;
    }
  }
}


