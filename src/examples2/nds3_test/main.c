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

  uint8_t pos=90;
  uint8_t state=0;
  while (1) {
    gfx_draw_mode(GFX_DM_JAM2);


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

    if (!get_input_bit(IO_INPUT_1)) {
      delay(100);
      while (!get_input_bit(IO_INPUT_1)) {
        delay(100);
      }
      switch (state) {
        case 0:  state = 1; pos =  135; break;
        case 1:  state = 2; pos =  180; break;
        case 2:  state = 3; pos =    0; break;
        case 3:  state = 4; pos =   45; break;
        case 4:  state = 0; pos =   90; break;
        default: state = 0; pos =   90;
      }
    }


    uint8_t d = nds3_get_dist();

    gfx_move(25, 20);
    sprintf(text, "%3i cm ", (int)d);
    gfx_print_text(text);
    //print_hex(d);
    nds3_move(pos);
    
    delay(100);
  }
}

