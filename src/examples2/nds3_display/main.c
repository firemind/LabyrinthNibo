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
#include <math.h>

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


void do_plot() {
  delay(1);
  for (int i=0; i<181; i+=15) {
    nds3_read(i, 1);
    delay(1);    
  }

  
  gfx_term_clear();
  uint8_t idx=0;
  uint8_t step=1;
  uint8_t idist_last=0;
  
  while (idx<=180) {
    uint8_t x=64;
    uint8_t y=0;
    uint8_t idist = nds3_distances[idx];
    
    if (idist<8) idist=4;
    if (idist>70) idist=70;
    
    float dir = idx*M_PI/180;
    float dist = idist*(63.0/70);

    y += sin(dir) * dist;
    x += cos(dir) * dist;
    if (((idist>idist_last)?(idist-idist_last):(idist_last-idist)) > 6) {
      gfx_move(x,y);
      gfx_pixel(1);
    } else {
      gfx_lineTo(x,y);
    }
    idist_last = idist;
    idx+=step;
  }


  idx=0;
 
  while (idx<=180) {
    uint8_t x=64;
    uint8_t y=0;
    
    float dir = idx*M_PI/180;
    float dist = 63.0;
    y += sin(dir) * dist;
    x += cos(dir) * dist;
    if (idx==0) {
      gfx_move(x,y);
    } else {
      gfx_lineTo(x,y);
    }    
    idx+=step;
  }

  char txt[10];
  int i;

  gfx_term_goto(0,0);

  i=nds3_distances[0];
  sprintf(txt, "%2i\n", i);
  gfx_term_print(txt);

  i=nds3_distances[14];
  sprintf(txt, "%2i\n", i);
  gfx_term_print(txt);

  i=nds3_distances[15];
  sprintf(txt, "%2i\n", i);
  gfx_term_print(txt);
  
}


void check_voltage() {
  bot_update();
  char text[20];
  static float volt_flt = 10.0;
  float volt = 0.0160 * bot_supply;

  // filter
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

  uint8_t pos=0;
  uint8_t state=0;

  gfx_term_print("Move 0\n");
  nds3_move(0);
  while (nds3_get_busy()) {
    delay(1);
  }
  
  while (1) {
    gfx_draw_mode(GFX_DM_JAM2);

    check_voltage();

   
    //gfx_term_print("Measure 90, 15\n");
      delay(1);
    nds3_measure(180, 1);
    while (nds3_get_busy()) {
      delay(1);
    }

      delay(1);
    //gfx_term_print("Read -90, 15\n");
    do_plot();

      delay(1);
    nds3_measure(0, 1);
    while (nds3_get_busy()) {
      delay(1);
    }

      delay(1);
    //gfx_term_print("Read 90, 15\n");
    nds3_read(0, 15);
    do_plot();
    
    
  }
}

