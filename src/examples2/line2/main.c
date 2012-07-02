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
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/adc.h"
#include "nibo/floor.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
#include "nibo/sound.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

void print_hex (uint8_t val)
{
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}


int main() {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  spi_init();
  floor_init();

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
  int16_t speed_flt_l=0;
  int16_t speed_flt_r=0;

  while(1) {
    sei();
    _delay_ms(1);
    int16_t speed_l=0;
    int16_t speed_r=0;

    floor_update();
    char text[20]="--  --  --  --  --";

    // Bodensensoren
    floor_update();
    sprintf(text, "%02x  %02x  %02x  %02x",
        (uint16_t)(floor_absolute[FLOOR_RIGHT]/8),
        (uint16_t)(floor_absolute[LINE_RIGHT]/8),
        (uint16_t)(floor_absolute[LINE_LEFT]/8),
        (uint16_t)(floor_absolute[FLOOR_LEFT]/8));
    gfx_move(22, 20);
    gfx_print_text(text);

    sprintf(text, "%02x  %02x  %02x  %02x",
        (uint16_t)(floor_relative[FLOOR_RIGHT]/8),
        (uint16_t)(floor_relative[LINE_RIGHT]/8),
        (uint16_t)(floor_relative[LINE_LEFT]/8),
        (uint16_t)(floor_relative[FLOOR_LEFT]/8));
    gfx_move(22, 30);
    gfx_print_text(text);
    
    int16_t lval = floor_relative[LINE_LEFT];
    int16_t flval = floor_relative[FLOOR_LEFT];
    int16_t frval = floor_relative[FLOOR_RIGHT];
    int16_t rval = floor_relative[LINE_RIGHT];
    int16_t cval = (flval>frval)?flval:frval;

    if (lval+cval+rval < 20) {
      leds_set_status(LEDS_OFF, 4);
      leds_set_status(LEDS_OFF, 5);
      speed_r=0, speed_l=0;
    } else if ((lval<cval) && (lval<rval)) {
      // lval is minimum
      leds_set_status(LEDS_RED, 4);
      leds_set_status(LEDS_OFF, 5);
      speed_r=350, speed_l=250-1*(cval-lval);
    } else if ((rval<cval) && (rval<lval)) {
      // rval is minimum
      leds_set_status(LEDS_OFF, 4);
      leds_set_status(LEDS_RED, 5);
      speed_r=250-1*(cval-rval), speed_l=350;
    } else {
      // cval is minimum
      leds_set_status(LEDS_RED, 4);
      leds_set_status(LEDS_RED, 5);
      speed_r=550 + 1*(rval-cval), speed_l=550 + 1*(lval-cval);
    }

    //speed_l/=2;
    //speed_r/=2;

    speed_flt_l*=3; speed_flt_l+=speed_l; speed_flt_l/=4;
    speed_flt_r*=3; speed_flt_r+=speed_r; speed_flt_r/=4;
   
    if (!copro_setPWM(speed_flt_l, speed_flt_r)) {
      gfx_move(10, 10);
      gfx_set_proportional(1);
      gfx_print_text("MOTCO_Error");
      gfx_set_proportional(0);
      continue;
    }
   
    gfx_move(25, 40);
    gfx_set_proportional(1);
    gfx_print_text("r:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(speed_flt_r));
    print_hex(LOBYTE(speed_flt_r));
    gfx_set_proportional(1);
    gfx_print_text(" l:");
    gfx_set_proportional(0);
    print_hex(HIBYTE(speed_flt_l));
    print_hex(LOBYTE(speed_flt_l));

    delay(10);
    if (!copro_update()) {
      gfx_move(10, 10);
      gfx_set_proportional(1);
      gfx_print_text("MOTCO-Error");
      gfx_set_proportional(0);
      continue;
    }

    gfx_move(25, 50);
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

  }
  return 0;
}
