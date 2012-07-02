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

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
#include "nibo/display.h"
#include "nibo/gfx.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
#include "nibo/nds3.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

enum {
  STATE_RESET,
  STATE_INIT,
  STATE_DIST1,
  STATE_DIST2,
  STATE_DIST3,
  STATE_DIST_SAVE,
  
  STATE_CENTER,
  STATE_LEFT,
  STATE_RIGHT,
  STATE_SERVO_SAVE
  
};


uint8_t state = 0;

uint8_t key_cnt = 0;

float volt_flt = 10.0;

void print_hex (uint8_t val)
{
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}

uint16_t dist_raw;

float d0, d1, d2, d3;
float v0, v1, v2, v3;
float alpha;

uint16_t servo_raw, servo_raw_left, servo_raw_center, servo_raw_right;

void calculate() {
  d0 = - (( (d3-d1)*(d2*v2-d1*v1) + (d1*v1-d3*v3)*(d2-d1) ) / ( (v2-v1)*(d3-d1)-(v3-v1)*(d2-d1) ));
  v0 = (d0*(v2-v1)-d1*v1+d2*v2) / (d2-d1);
  alpha = d1*v1 - d1*v0 - d0*v0 + d0*v1;
};

char txt[16];

void handle_key_pressed() {
  // enable INPUT_3 pullup
  deactivate_output_bit(IO_INPUT_3);
  set_output_bit(IO_INPUT_3);
  
  // output GND to INPUT_2
  activate_output_bit(IO_INPUT_2);
  clear_output_bit(IO_INPUT_2);
  delay(1);
  uint8_t jp = !get_input_bit(IO_INPUT_3); 
  
  switch (state) {
    case STATE_RESET:      
      state=STATE_INIT;
      break;

    case STATE_INIT:
      delay(1);
      nds3_servocalib_update();
      delay(1);
      servo_raw_left   = nds3_servocalib_lo;
      servo_raw_center = nds3_servocalib_mid;
      servo_raw_right  = nds3_servocalib_hi;
      
      state = jp?STATE_DIST1:STATE_CENTER;
      break;
      
    case STATE_DIST1:
      d1 = 10;
      v1 = dist_raw;      
      dist_raw = 0;
      state = STATE_DIST2;
      break;
      
    case STATE_DIST2:
      d2 = 30;
      v2 = dist_raw;      
      dist_raw = 0;
      state = STATE_DIST3;
      break;
      
    case STATE_DIST3:
      d3 = 70;
      v3 = dist_raw;      
      state = STATE_DIST_SAVE;
      break;
      
    case STATE_DIST_SAVE:
      nds3_distcalib_v0 = v0;
      nds3_distcalib_d0 = d0;
      nds3_distcalib_alpha = alpha;
      delay(1);
      nds3_distcalib_commit();
      delay(1);      
      state=STATE_INIT;
      break;

    case STATE_CENTER:
      if (key_cnt>4) {
        state=STATE_LEFT;
      } else {
        if (jp) servo_raw_center++; else servo_raw_center--;
      }
      break;

    case STATE_LEFT:
      if (key_cnt>4) {
        state=STATE_RIGHT;
      } else {
        if (jp) servo_raw_left++; else servo_raw_left--;
      }
      break;

    case STATE_RIGHT:
      if (key_cnt>4) {
        state = STATE_SERVO_SAVE;        
      } else {
        if (jp) servo_raw_right++; else servo_raw_right--;
      }
      break;

    case STATE_SERVO_SAVE:
      state = STATE_INIT;
      nds3_servocalib_lo = servo_raw_left;
      nds3_servocalib_mid = servo_raw_center;
      nds3_servocalib_hi = servo_raw_right;
      delay(1);
      nds3_servocalib_commit();
      delay(2);        
      nds3_move(90);
      delay(1);
      break;       
      
      
  }  
  
  
  gfx_term_clear();
  gfx_set_proportional(1);
  switch (state) {
    case STATE_INIT:
      gfx_move(0, 8);
      gfx_print_text("activate JP1 and press S3 for");
      gfx_move(0, 16);
      gfx_print_text("Distance calibration");
      gfx_move(0, 24);
      gfx_print_text("deactivate JP1 and press S3 for");
      gfx_move(0, 32);
      gfx_print_text("Servo calibration");
      break;

    case STATE_DIST1:
      gfx_move(0, 8);
      gfx_print_text("place object in 10 cm range");
      gfx_move(0, 16);
      gfx_print_text("press S1 when ready");
      break;

    case STATE_DIST2:
      gfx_move(0, 8);
      gfx_print_text("place object in 30 cm range");
      gfx_move(0, 16);
      gfx_print_text("press S1 when ready");
      break;

    case STATE_DIST3:
      gfx_move(0, 8);
      gfx_print_text("place object in 70 cm range");
      gfx_move(0, 16);
      gfx_print_text("press S1 when ready");
      break;

    case STATE_DIST_SAVE:
      calculate();
      gfx_move(0, 8);
      gfx_print_text("press S3 to save calibration");
      gfx_move(0, 16);
      sprintf(txt, "d0 = %i", d0);
      gfx_print_text(txt);
      gfx_move(0, 24);
      sprintf(txt, "v0 = %i", v0);
      gfx_print_text(txt);
      gfx_move(0, 32);
      sprintf(txt, "alpha = %i", alpha);
      gfx_print_text(txt);
      break;
      
      
    case STATE_CENTER:
      servo_raw=servo_raw_center;
      nds3_move_servo(servo_raw_center);
      gfx_move(0, 8);
      gfx_print_text("calibrate servo center");
      gfx_move(0, 16);
      gfx_print_text("press S3 long when ready");
      break;

    case STATE_LEFT:
      servo_raw=servo_raw_left;
      nds3_move_servo(servo_raw_left);
      gfx_move(0, 8);
      gfx_print_text("calibrate servo left");
      gfx_move(0, 16);
      gfx_print_text("press S3 long when ready");
      break;

    case STATE_RIGHT:
      servo_raw=servo_raw_right;
      nds3_move_servo(servo_raw_right);
      gfx_move(0, 8);
      gfx_print_text("calibrate servo right");
      gfx_move(0, 16);
      gfx_print_text("press S3 long when ready");
      break;
      
    case STATE_SERVO_SAVE:
      gfx_move(0, 8);
      gfx_print_text("press S3 to save calibration");
      gfx_move(0, 16);
      sprintf(txt, "left = %i", servo_raw_left);
      gfx_print_text(txt);
      gfx_move(0, 24);
      sprintf(txt, "center = %i", servo_raw_center);
      gfx_print_text(txt);
      gfx_move(0, 32);
      sprintf(txt, "right = %i", servo_raw_right);
      gfx_print_text(txt);
      break;
      
  }
  
  
}



int main(void) {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  i2c_init();

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

  nds3_move(90);
  
  handle_key_pressed();
  
  while (1) {
    gfx_draw_mode(GFX_DM_JAM2);


    // Spannung
    bot_update();
    char text[20];
    float volt = 0.0160 * bot_supply;
    sprintf(text, "%2.1fV ", (double)volt);
    gfx_set_proportional(0);
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

    if (!get_input_bit(IO_INPUT_1)) {
      delay(100);
      key_cnt=0;
      while (!get_input_bit(IO_INPUT_1)) {
        delay(100);
        key_cnt++;
      }
      handle_key_pressed();
    }

    gfx_set_proportional(0);
    switch (state) {
      case STATE_INIT: {
        delay(1);
        uint8_t d = nds3_get_dist();
        gfx_move(25, 56);
        sprintf(text, "%3i cm ", (int)d);
        gfx_print_text(text);
      } break;

      case STATE_DIST1: case STATE_DIST2: case STATE_DIST3: {
        delay(1);
        uint16_t d = nds3_get_analog(4);
        if (dist_raw==0) {
          dist_raw=d;
        } else {
          dist_raw *= 7;
          dist_raw += d;
          dist_raw /= 8;
        }
        gfx_move(25, 56);
        sprintf(text, "raw: %5i", (int)dist_raw);
        gfx_print_text(text);
      } break;
      
      case STATE_CENTER: case STATE_LEFT: case STATE_RIGHT: {
        gfx_move(25, 56);
        sprintf(text, "servo: %5i", (int)servo_raw);
        gfx_print_text(text);
      } break;
      
    }
    delay(100);
  }
}

