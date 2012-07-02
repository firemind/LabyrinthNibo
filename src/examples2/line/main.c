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

uint8_t headlight_target;
uint8_t headlight_act;

int8_t mode = MODE_STOP;
int8_t last_mode = MODE_STOP;
int8_t mode_cnt = 0;

uint16_t old_rc5_cmd;

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



uint8_t and_poss(uint8_t p1, uint8_t p2) {
  return (((uint16_t)p1*p2)+((uint16_t)p1+p2)/2)/0x100;
}

uint8_t or_poss(uint8_t p1, uint8_t p2) {
  return 0xff-and_poss(0xff-p1, 0xff-p2);
}


uint8_t linear_trans(uint8_t val, uint8_t val_false, uint8_t val_true) {
  uint16_t res = (val-val_false)*(0x100)/(val_true-val_false);
  if (val_false<val_true) {
    if (val<=val_false) {
      return 0x00;
    }
    if (val>=val_true) {
      return 0xff;
    }
  } else {
    if (val<=val_false) {
      return 0xff;
    }
    if (val>=val_true) {
      return 0x00;
    }
  }
  if (res>0xff) {
    return 0xff;
  }
  return res;
}


uint8_t back_cnt;
char emergency = 0;
int8_t line_ori;

int main(void) {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  spi_init();
  floor_init();
  sound_init();

  if (display_init()!=0)
  {
    leds_set_displaylight(50);
    if (display_type==2)
    {
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

    sound_tone(127/4, 15000);
    delay_ms(10);
    sound_tone(191/4, 15000);
    delay_ms(10);
    sound_tone(255/4, 10000);
    delay_ms(10);
    sound_tone(255/4, 10000);
    delay_ms(10);
    sound_tone(191/4, 15000);

  
  while (1)
  {
    delay(20);

    // Spannung
    bot_update();
    char text[20];
    float volt = 0.0160 * bot_supply;
    sprintf(text, "%2.1fV ", (double)volt);
    gfx_move(45, 0);
    gfx_print_text(text);

    volt_flt = 0.9*volt_flt+0.1*volt;

    if (volt_flt<9.0) {
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
    floor_disable_ir();
    delay(1);
    floor_update();
    int16_t value_fl = -floor_l;
    int16_t value_fr = -floor_r;
    int16_t value_ll = -line_l;
    int16_t value_lr = -line_r;
    int16_t value_nfl = floor_l/16;
    int16_t value_nfr = floor_r/16;
    int16_t value_nll = line_l/16;
    int16_t value_nlr = line_r/16;
    floor_enable_ir();
    delay(1);
    floor_update();
    int16_t value_pfl = floor_l/16;
    int16_t value_pfr = floor_r/16;
    int16_t value_pll = line_l/16;
    int16_t value_plr = line_r/16;
    value_fl += floor_l;
    value_fr += floor_r;
    value_ll += line_l;
    value_lr += line_r;
    //floor_disable_ir();

    if (value_fl<0) value_fl=0;
    if (value_fr<0) value_fr=0;
    if (value_ll<0) value_ll=0;
    if (value_lr<0) value_lr=0;
    if (value_fl>255) value_fl=255;
    if (value_fr>255) value_fr=255;
    if (value_ll>255) value_ll=255;
    if (value_lr>255) value_lr=255;

    gfx_move(30, 10);
    print_hex(value_nfr);
    gfx_print_char(' ');
    print_hex(value_nlr);
    gfx_print_char(' ');
    print_hex(value_nll);
    gfx_print_char(' ');
    print_hex(value_nfl);
    
    gfx_move(30, 20);
    print_hex(value_pfr);
    gfx_print_char(' ');
    print_hex(value_plr);
    gfx_print_char(' ');
    print_hex(value_pll);
    gfx_print_char(' ');
    print_hex(value_pfl);

    gfx_move(30, 30);
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
    if (!copro_update())
    {
      gfx_move(10, 10);
      gfx_set_proportional(1);
      gfx_print_text("IRCO Error   ");
      gfx_set_proportional(0);
      continue;
    }

    gfx_move(76, 0);
    gfx_set_proportional(1);
    gfx_print_text("RC5: ");
    gfx_set_proportional(0);
    print_hex(HIBYTE(copro_rc5_cmd));
    print_hex(LOBYTE(copro_rc5_cmd));

    if (copro_rc5_cmd!=old_rc5_cmd) {
      old_rc5_cmd = copro_rc5_cmd;
      if ((HIBYTE(copro_rc5_cmd)&0x07)==0x02) {
        switch(LOBYTE(copro_rc5_cmd)) {
          //case 0xad: floor_calibrate(); break;
          case 0xac: emergency = emergency?0:1;
        }
      }
    }


    uint8_t is_line_l = linear_trans(value_ll, 0x16, 0x02);
    uint8_t is_line_r = linear_trans(value_lr, 0x16, 0x02);
    uint8_t is_floor_l = linear_trans(value_ll, 0x10, 0x20);
    uint8_t is_floor_r = linear_trans(value_lr, 0x10, 0x20);


    uint8_t go_l = or_poss(is_line_r, and_poss(is_floor_l, 0x40));
    uint8_t go_r = or_poss(is_line_l, and_poss(is_floor_r, 0x40));

    
    if ((is_line_l>0x80) && (line_ori<+5)) line_ori++;
    if ((is_line_r>0x80) && (line_ori>-5)) line_ori--;
    if ((is_line_l>0x80) && (is_line_r>0x80)) line_ori=0;



    if ((is_line_l<0x20) && (is_line_r<0x20)) {
      //line was lost
      if (line_ori>0) {
        go_l = -20;
        go_r = 160;
      } else if (line_ori<0) {
        go_l = 160;
        go_r = -20;
      }
    }



    uint8_t ledr = 0x00;
    uint8_t ledg = 0x00;

    if (is_line_l>0x80) ledg |= 0x10;
    if (is_line_r>0x80) ledg |= 0x20;
    if (is_floor_l>0x80) ledr |= 0x10;
    if (is_floor_r>0x80) ledr |= 0x20;

    IO_LEDS_RED_PORT = ledr;
    IO_LEDS_GREEN_PORT = ledg;

    speed_l = go_l/20;
    speed_r = go_r/20;

    if (((tspeed_l<0) && (speed_l>0)) ||
        ((tspeed_l>0) && (speed_l<0)) ||
        ((tspeed_r<0) && (speed_r>0)) ||
        ((tspeed_r>0) && (speed_r<0)) ||
        emergency)
    {
      tspeed_l = 0;
      tspeed_r = 0;
      
      if (!copro_stop())
      {
        gfx_move(10, 10);
        gfx_set_proportional(1);
        gfx_print_text("MOTCO Error");
        gfx_set_proportional(0);
        continue;
      }
    }
    else
    {
      tspeed_l = (3*tspeed_l+speed_l)/4;
      tspeed_r = (3*tspeed_r+speed_r)/4;
    
      if (!copro_setSpeed(tspeed_l, tspeed_r))
      {
        gfx_move(10, 10);
        gfx_set_proportional(1);
        gfx_print_text("MOTCO_Error");
        gfx_set_proportional(0);
        continue;
      }
    }

    gfx_move(25, 40);
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
    if (!copro_update())
    {
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


    //gfx_move(10, 10);
    //gfx_print_text("         ");
  }
}

