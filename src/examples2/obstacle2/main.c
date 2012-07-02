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
  Mit diesem Programm weicht der NIBO 2 Hindernissen aus, er fährt jedoch nur wenn er auf dem Boden 
  steht. Hebt man ihn auf, drehen sich die Räder nicht mehr.

*/

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

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)


/*
  Hier werden die Zustände definiert, in denen sich der NIBO 2 befinden kann.
*/
typedef enum {
  MODE_STOP,
  MODE_STRAIGHTON,
  MODE_TURN_LEFT,
  MODE_TURN_RIGHT,
  MODE_ROTATE_LEFT,
  MODE_ROTATE_RIGHT,
  MODE_BACK
} mode_t;

int16_t tspeed_l = 0;
int16_t tspeed_r = 0;

uint8_t dist_l;
uint8_t dist_fl;
uint8_t dist_f;
uint8_t dist_fr;
uint8_t dist_r;

uint8_t dist_max_ls; // max leftside
uint8_t dist_max_fs; // max frontside
uint8_t dist_max_rs; // max rightside

uint8_t headlight_target;
uint8_t headlight_act;

int8_t mode = MODE_STOP;
int8_t last_mode = MODE_STOP;
int8_t mode_cnt = 0;

float volt_flt = 10.0;

uint16_t stress;

void print_hex (uint8_t val) {
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}

typedef enum {
  DIST_L  = 0x01,
  DIST_FL = 0x02,
  DIST_F  = 0x04,
  DIST_FR = 0x08,
  DIST_R  = 0x10,
  DIST_ALL = 0x1f
} distnames_t;


uint8_t check_le(uint8_t dist, int8_t delta, uint8_t mask) {
  if (delta<0) {
    delta=-delta;
    dist = (dist>delta)? (dist-delta):0;
  } else {
    dist = (dist<255-delta)? (dist+delta):255;
  }

  return (!(mask&DIST_L) || (dist<=dist_l))
       & (!(mask&DIST_FL) || (dist<=dist_fl))
       & (!(mask&DIST_F) || (dist<=dist_f))
       & (!(mask&DIST_FR) || (dist<=dist_fr))
       & (!(mask&DIST_R) || (dist<=dist_r));
}

uint8_t back_cnt;

int main(void) {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  spi_init();
  floor_init();
  floor_readPersistent();

  if (display_init()!=0) {
    leds_set_displaylight(50);
    if (display_type==2) {
      gfx_init();
    }
  }
  
  gfx_fill(0x00);
  gfx_move(15, 0);
  gfx_set_proportional(1);
  gfx_print_text("nibo");
  gfx_set_proportional(0);

  copro_ir_startMeasure();
  delay(10);
  //motco_setSpeedParameters(5, 4, 6); // ki, kp, kd
  copro_setSpeedParameters(15, 20, 10); // ki, kp, kd

  stress = 0;
  
  
  while (1) {
    delay(50);

    gfx_draw_mode(GFX_DM_JAM2);

    leds_set_displaylight(1024);

    // Spannung
    bot_update();
    char text[20];
    float volt = 0.0160 * bot_supply;
    sprintf(text, "%2.1fV ", (double)volt);
    gfx_move(45, 0);
    gfx_print_text(text);

    volt_flt = 0.9*volt_flt+0.1*volt;

    if (volt_flt<8.0) {
      gfx_fill(0x00);
      gfx_move(25, 20);
      gfx_set_proportional(1);
      gfx_print_text("Please recharge");
      gfx_move(35, 30);
      gfx_print_text("batteries!");
      gfx_set_proportional(0);
      while(1) {
        leds_set_headlights(0);
        clear_output_bit(IO_RESET_CO);
        //motco_stop();
        leds_set_displaylight(500);
        IO_LEDS_RED_PORT = 0xaa;
        IO_LEDS_GREEN_PORT = 0;
        delay(500);
        //motco_setSpeed(0, 0);
        leds_set_displaylight(0);
        IO_LEDS_RED_PORT = 0x55;
        IO_LEDS_GREEN_PORT = 0;
        delay(500);
      }
    }
    
    // Analog values:
    floor_update();
    int16_t value_fl = floor_l;
    int16_t value_fr = floor_r;
    int16_t value_ll = line_l;
    int16_t value_lr = line_r;

    if (value_fl<0) value_fl=0;
    if (value_fr<0) value_fr=0;
    if (value_ll<0) value_ll=0;
    if (value_lr<0) value_lr=0;
    if (value_fl>255) value_fl=255;
    if (value_fr>255) value_fr=255;
    if (value_ll>255) value_ll=255;
    if (value_lr>255) value_lr=255;

    gfx_move(30, 10);
    print_hex(value_fr);
    gfx_print_char(' ');
    print_hex(value_lr);
    gfx_print_char(' ');
    print_hex(value_ll);
    gfx_print_char(' ');
    print_hex(value_fl);

    // Distance

    int16_t speed_l = 0;
    int16_t speed_r = 0;

    // Request distance data
    if (!copro_update()) {
      gfx_move(10, 10);
      gfx_set_proportional(1);
      gfx_print_text("COPRO Error   ");
      gfx_set_proportional(0);
      continue;
    }

    dist_l  = copro_distance[4]/128;
    dist_fl = copro_distance[3]/128;
    dist_f  = copro_distance[2]/128;
    dist_fr = copro_distance[1]/128;
    dist_r  = copro_distance[0]/128;

    dist_l = (dist_l<250)? (dist_l+5):255;
    dist_r = (dist_r<250)? (dist_r+5):255;
    dist_f = (dist_f>5)? (dist_f-5):0;


    dist_max_ls = (dist_l>dist_fl)?dist_l:dist_fl;
    dist_max_rs = (dist_r>dist_fr)?dist_r:dist_fr;
    dist_max_fs = (dist_fr>dist_fl)?dist_fr:dist_fl;
    if (dist_f>dist_max_fs) dist_max_fs = dist_f;

    gfx_move(25, 20);
    print_hex(dist_r);
    gfx_print_char(' ');
    print_hex(dist_fr);
    gfx_print_char(' ');
    print_hex(dist_f);
    gfx_print_char(' ');
    print_hex(dist_fl);
    gfx_print_char(' ');
    print_hex(dist_l);

    uint8_t ledr = 0x00;
    uint8_t ledg = 0x00;

    uint16_t sum = dist_r + dist_fr
                 + dist_f + dist_fl
                 + dist_l;
    sum /= 5;
    
    uint8_t dmax = (sum<0xe0)?(sum+0x20):0xff;
    uint8_t dmin = (sum>0x20)?(sum-0x20):0x00;

    ledr=0;
    ledg=0;

    stress = (stress>10)?(stress-10):(0);

    stress += copro_current_l;
    stress += copro_current_r;

    switch (mode) {
      case MODE_STOP:
        if (check_le(dist_l, 0, DIST_ALL))
          mode = MODE_ROTATE_LEFT;
        if (check_le(dist_r, 0, DIST_ALL))
          mode = MODE_ROTATE_RIGHT;
        if (check_le(dist_fl, 0, DIST_ALL))
          mode = MODE_TURN_LEFT;
        if (check_le(dist_fr, 0, DIST_ALL))
          mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, 0, DIST_ALL))
          mode = MODE_STRAIGHTON;
        break;
      
      case MODE_STRAIGHTON:
        if (check_le(dist_fl, 0, DIST_FL|DIST_F|DIST_FR))
          mode = MODE_TURN_LEFT;
        if (check_le(dist_fr, 0, DIST_FL|DIST_F|DIST_FR))
          mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, -2, DIST_FL|DIST_F|DIST_FR))
          mode = MODE_STRAIGHTON;
        if (!check_le(dist_max_fs, -40, DIST_ALL))
          mode = MODE_STOP;
        if (dist_max_fs>0xa0)
          mode = MODE_BACK;
        break;
      
      case MODE_TURN_LEFT:
        if (check_le(dist_l, 0, DIST_L|DIST_FL|DIST_F))
          mode = MODE_ROTATE_LEFT;
        if (check_le(dist_fl, -3, DIST_L|DIST_FL|DIST_F))
          mode = MODE_TURN_LEFT;
        if (check_le(dist_f, 0, DIST_L|DIST_FL|DIST_F))
          mode = MODE_STRAIGHTON;
        if (!check_le(dist_max_fs, -40, DIST_ALL))
          mode = MODE_STOP;
        if (dist_max_fs>0xa0)
          mode = MODE_BACK;
        break;
      
      case MODE_TURN_RIGHT:
        if (check_le(dist_r, 0, DIST_R|DIST_FR|DIST_F))
          mode = MODE_ROTATE_RIGHT;
        if (check_le(dist_fr, -3, DIST_R|DIST_FR|DIST_F))
          mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, 0, DIST_R|DIST_FR|DIST_F))
          mode = MODE_STRAIGHTON;
        if (!check_le(dist_max_fs, -40, DIST_ALL))
          mode = MODE_STOP;
        if (dist_max_fs>0xa0)
          mode = MODE_BACK;
        break;
      
      case MODE_ROTATE_LEFT:
        if (check_le(dist_l, 0, DIST_L|DIST_FL|DIST_F))
          mode = MODE_ROTATE_LEFT;
        if (check_le(dist_fl, -1, DIST_L|DIST_FL|DIST_F))
          mode = MODE_TURN_LEFT;
        if (check_le(dist_f, -1, DIST_L|DIST_FL|DIST_F))
          mode = MODE_TURN_LEFT;
        if (!check_le(dist_max_ls, -40, DIST_ALL))
          mode = MODE_BACK;
        break;
      
      case MODE_ROTATE_RIGHT:
        if (check_le(dist_r, 0, DIST_R|DIST_FR|DIST_F))
          mode = MODE_ROTATE_RIGHT;
        if (check_le(dist_fr, -1, DIST_R|DIST_FR|DIST_F))
          mode = MODE_TURN_RIGHT;
        if (check_le(dist_f, -1, DIST_R|DIST_FR|DIST_F))
          mode = MODE_TURN_RIGHT;
        if (!check_le(dist_max_rs, -40, DIST_ALL))
          mode = MODE_BACK;
        break;
      
      case MODE_BACK:
        if (++back_cnt>20) {
          back_cnt = 0;
          mode = MODE_STOP;
        }
        break;
    }

    if (stress>100) {
      mode = MODE_BACK;
      stress = 0;
    }
    
    if (mode!=last_mode) {
      mode_cnt+=2;
      if (mode_cnt>50) {
        mode = MODE_BACK;
      }
      last_mode=mode;
    } else {
      if (mode_cnt>0) {
        mode_cnt--;
      }
    }
    
      
    gfx_move(100, 0);
    print_hex(mode);
      
    switch (mode) {
      case MODE_STOP:
        speed_l=0, speed_r=0, ledg= 0x00, ledr=0x84;
        break;
      
      case MODE_STRAIGHTON:
        headlight_target=10;
        speed_l=60, speed_r=60, ledg= 0x30;
        break;
      
      case MODE_TURN_LEFT:
        headlight_target=10;
        speed_l=40, speed_r=50, ledg= 0x08;
        break;
      
      case MODE_TURN_RIGHT:
        headlight_target=10;
        speed_l=50, speed_r=40, ledg= 0x40;
        break;
      
      case MODE_ROTATE_LEFT:
        headlight_target=0;
        speed_l=-10, speed_r=10, ledg= 0x04;
        break;
      
      case MODE_ROTATE_RIGHT:
        headlight_target=0;
        speed_l=10, speed_r=-10, ledg= 0x80;
        break;
      
      case MODE_BACK:
        headlight_target=0;
        speed_l=-10, speed_r=-10, ledg= 0x00, ledr=0x03;
        break;
      
    }

      
    IO_LEDS_RED_PORT = ledr;
    IO_LEDS_GREEN_PORT = ledg;

    if (headlight_act<headlight_target)
      headlight_act++;
    if (headlight_act>headlight_target)
      headlight_act--;

    if (headlight_act)
      leds_set_headlights(1<<(headlight_act-1));
    else
      leds_set_headlights(0);
    

    if (mode==MODE_STRAIGHTON) {
      int16_t lval = 2*dist_fl+dist_l;
      int16_t rval = 2*dist_fr+dist_r;
      int16_t delta = lval-rval;

      if (abs(delta)>5) {
        if (lval>rval) speed_r-=10; // rechts mehr Platz
        if (rval>lval) speed_l-=10; // links mehr Platz
      } else {
        if (lval>rval) speed_r-=3; // rechts mehr Platz
        if (rval>lval) speed_l-=3; // links mehr Platz
      }
    
    } else {
      uint16_t lval = dist_fl+dist_l;
      uint16_t rval = dist_fr+dist_r;
      if (lval>rval) speed_r-=3; // rechts mehr Platz
      if (rval>lval) speed_l-=3; // links mehr Platz
    }

    uint8_t emergency=0;

    if ((value_fl<0x10)||(value_fr<0x10)) {
      speed_l=0;
      speed_r=0;
      emergency = 1;
    }

    if (((tspeed_l<0) && (speed_l>0)) ||
        ((tspeed_l>0) && (speed_l<0)) ||
        ((tspeed_r<0) && (speed_r>0)) ||
        ((tspeed_r>0) && (speed_r<0)) ||
        emergency) {
      tspeed_l = 0;
      tspeed_r = 0;
      
      if (!copro_stop()) {
        gfx_move(10, 10);
        gfx_set_proportional(1);
        gfx_print_text("MOTCO Error");
        gfx_set_proportional(0);
        continue;
      }
      
    } else {
      tspeed_l = (3*tspeed_l+speed_l)/4;
      tspeed_r = (3*tspeed_r+speed_r)/4;
    
      if (!copro_setSpeed(tspeed_l, tspeed_r)) {
        gfx_move(10, 10);
        gfx_set_proportional(1);
        gfx_print_text("MOTCO_Error");
        gfx_set_proportional(0);
        continue;
      }
    }

    gfx_move(25, 30);
    gfx_set_proportional(1);
    gfx_print_text("r:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(tspeed_r));
    print_hex(LOBYTE(tspeed_r));
    gfx_set_proportional(1);
    gfx_print_text(" l:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(tspeed_l));
    print_hex(LOBYTE(tspeed_l));

    delay(10);
    if (!copro_update()) {
      gfx_move(10, 10);
      gfx_set_proportional(1);
      gfx_print_text("COPRO-Error");
      gfx_set_proportional(0);
      continue;
    }

    gfx_move(25, 40);
    gfx_set_proportional(1);
    gfx_print_text("r:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(copro_speed_r));
    print_hex(LOBYTE(copro_speed_r));
    gfx_set_proportional(1);
    gfx_print_text(" l:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(copro_speed_l));
    print_hex(LOBYTE(copro_speed_l));

    gfx_move(25, 50);
    gfx_set_proportional(1);
    gfx_print_text("r:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(copro_current_r));
    print_hex(LOBYTE(copro_current_r));
    gfx_set_proportional(1);
    gfx_print_text(" l:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(copro_current_l));
    print_hex(LOBYTE(copro_current_l));
    
  }
}

