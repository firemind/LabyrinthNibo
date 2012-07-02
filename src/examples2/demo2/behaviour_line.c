#include "acquisition.h"

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
//#include "nibo/display.h"
//#include "nibo/gfx.h"
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
//#include "nibo/xbee.h"

#include "behaviour.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

uint8_t ramp_counter;

uint16_t start_counter;

uint16_t state_timeout;

extern int16_t tspeed_l;
extern int16_t tspeed_r;
extern int8_t mode;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////
////                       FOLLOW LINE
////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

enum {
  FL_I0,
  FL_I1,
  FL_I2,
  FL_I3,
  FL_S,
  FL_N,
  FL_R0,
  FL_R1,
  FL_R2,
  FL_R3,
  FL_L0,
  FL_L1,
  FL_L2,
  FL_L3
};

static uint8_t state = FL_N;


void behaviour_follow_line_start() {
  start_counter = 3000; // 30s
  state = FL_I0;
  state_timeout = 300;
  tspeed_l=0;
  tspeed_r=0;
  acquisition_linestate = STATE_NO_LINE;
}



uint8_t behaviour_follow_line() {
  copro_ir_stop();
  uint8_t led_g=0x00;
  uint8_t led_r=0x00;

  static uint16_t state_timer;

  static uint8_t old_linestate;
  uint8_t linestate = acquisition_linestate;

  uint8_t old_state = state;

  uint8_t halt=0;
  static uint8_t goto_pos=0;

  static int8_t curve_mode;

  if (state_timeout) {
    state_timeout--;
  }

  switch (state) {
    case FL_I0:
      state=FL_I1;
      break;
    case FL_I1:
      if ((acquisition_getFloorState(AN_LINE_L)!=FLOOR_ABYSS) && (acquisition_getFloorState(AN_LINE_R)!=FLOOR_ABYSS)) {
        state=FL_I2;
        state_timeout=100;
      }
      break;
    case FL_I2:
      if ((acquisition_getFloorState(AN_LINE_L)==FLOOR_ABYSS) && (acquisition_getFloorState(AN_LINE_R)==FLOOR_ABYSS)) {
        state=FL_I3;
        state_timeout=100;
      }
      break;

    case FL_I3:
    case FL_N:
    case FL_R0:
    case FL_L0:
      switch (linestate) {
        case STATE_LINE:      state=FL_N; break;
        case STATE_LINE_L:    state=FL_L0; break;
        case STATE_LINE_R:    state=FL_R0; break;
        case STATE_NO_LINE_L: state=FL_L2; break;
        case STATE_NO_LINE_R: state=FL_R2; break;
      }
      break;

    case FL_R1:
    case FL_R2:
      switch (linestate) {
        case STATE_LINE:      state=FL_N;  break;
        case STATE_LINE_L:    state=FL_L0; halt=1; break;
        case STATE_LINE_R:    state=FL_R1; break;
        //case STATE_NO_LINE_L: state=FL_L2; break;
        case STATE_NO_LINE_R: state=FL_R2; break;
      }
      break;
    
    case FL_L1:
    case FL_L2:
      switch (linestate) {
        case STATE_LINE:      state=FL_N;  break;
        case STATE_LINE_L:    state=FL_L1; break;
        case STATE_LINE_R:    state=FL_R0; halt=1; break;
        case STATE_NO_LINE_L: state=FL_L2; break;
        //case STATE_NO_LINE_R: state=FL_R2; break;
      }
      break;
    
    case FL_R3:
      switch (linestate) {
        case STATE_LINE:      state=FL_N;  halt=1; break;
        case STATE_LINE_R:    state=FL_R0; halt=1; break;
        case STATE_NO_LINE_R: state=FL_R2; halt=1; break;
      }
      break;
      
    case FL_L3:
      switch (linestate) {
        case STATE_LINE:      state=FL_N;  halt=1; break;
        case STATE_LINE_L:    state=FL_L0; halt=1; break;
        case STATE_NO_LINE_L: state=FL_L2; halt=1; break;
      }
      break;
  }

  switch (state) {
    case FL_R0:
    case FL_R1:
    case FL_R2:
      if (acquisition_getFloorState(AN_FLOOR_L)==FLOOR_LINE) {
        //state=FL_L3;
      }
      break;
    
    case FL_L0:
    case FL_L1:
    case FL_L2:
      if (acquisition_getFloorState(AN_FLOOR_R)==FLOOR_LINE) {
        //state=FL_R3;
      }
      break;
  }

  if (start_counter) {
    start_counter--;
    
  } else {
    if ((acquisition_getFloorState(AN_FLOOR_R)==FLOOR_ABYSS) && (acquisition_getFloorState(AN_FLOOR_L)==FLOOR_ABYSS)) {
      ramp_counter++;
      if (ramp_counter>10) {
        return BEHAVIOUR_OBSTACLE;
      }
    } else {
      ramp_counter=0;
    }
  }

  switch (state) {
    case FL_R3:  led_r = 0x80; break;
    case FL_R2:  led_r = 0x40; break;
    case FL_R1:  led_r = 0x60; break;
    case FL_R0:  led_r = 0x20; break;
    case FL_N:   led_r = 0x30; break;
    case FL_L0:  led_r = 0x10; break;
    case FL_L1:  led_r = 0x18; break;
    case FL_L2:  led_r = 0x08; break;
    case FL_L3:  led_r = 0x04; break;
    case FL_S:   led_r = 0xff; break;
  }

  if (acquisition_getFloorState(AN_FLOOR_R)==FLOOR_LINE) led_g |= 0x40;
  if (acquisition_getFloorState(AN_LINE_R)==FLOOR_LINE)  led_g |= 0x20;
  if (acquisition_getFloorState(AN_LINE_L)==FLOOR_LINE)  led_g |= 0x10;
  if (acquisition_getFloorState(AN_FLOOR_L)==FLOOR_LINE) led_g |= 0x08;



  IO_LEDS_RED_PORT = led_r;
  IO_LEDS_GREEN_PORT = led_g;

  if ((state_timer>=150) && ((state==FL_R2)||(state==FL_L2))) {
    // force command again
    //old_state=FL_N;
  }

  if ((state_timer>=250) && ((curve_mode==1)||(curve_mode==-1))) {
    curve_mode = 0;
  }


  if (old_state==state) {
    if (state_timer<30000) state_timer++;
    
  } else {
    state_timer=0;
    switch (state) {
      case FL_I0:  tspeed_l = 10; tspeed_r=10; goto_pos=0; break;
      case FL_I1:  tspeed_l = 10; tspeed_r=10; goto_pos=0; break;
      case FL_I2:  tspeed_l = 10; tspeed_r=10; goto_pos=0; break;
      case FL_R3:  tspeed_l = 15; tspeed_r=10; goto_pos=0; halt=1; break;
      case FL_R2:  tspeed_l = 10; tspeed_r=-10; goto_pos=1; halt=1; break;
      case FL_R1:  tspeed_l = 10; tspeed_r=10; goto_pos=0; halt=1; break;
      case FL_R0:  tspeed_l = 13; tspeed_r=10; goto_pos=0; break;
      case FL_N:   tspeed_l = 10; tspeed_r=10; goto_pos=0; break;
      case FL_L0:  tspeed_l = 10; tspeed_r=13; goto_pos=0; break;
      case FL_L1:  tspeed_l = 10; tspeed_r=10; goto_pos=0; halt=1; break;
      case FL_L2:  tspeed_l = -10; tspeed_r= 10; goto_pos=1; halt=1; break;
      case FL_L3:  tspeed_l = 10; tspeed_r=15; goto_pos=0; halt=1; break;
      case FL_S:   tspeed_l =  0; tspeed_r= 0; goto_pos=0; halt=1; break;
    }



    if (state==FL_I1) {
      uint8_t corr = (line_detect_pos_l+line_detect_pos_r)/10;
      if (corr>5) corr=5;

      if (line_detect_dir>0) {
        tspeed_l += corr;
        tspeed_r -= corr;
      } else if (line_detect_dir<0) {
        tspeed_l -= corr;
        tspeed_r += corr;
      }
    }

    if (state==FL_I3) {
      acquisition_linestate = STATE_NO_LINE;
    }

    if (state==FL_R2) {
      curve_mode++;
      if (curve_mode<0) curve_mode=0;
    } else if (state==FL_L2) {
      curve_mode--;
      if (curve_mode>0) curve_mode=0;
    }
    if (curve_mode>3) curve_mode=3;
    if (curve_mode<-3) curve_mode=-3;

    if ((state==FL_R1) || (state==FL_R0) || (state==FL_N) || (state==FL_L0) || (state==FL_L1)) {
      switch (curve_mode) {
        case  3: tspeed_l+=4; tspeed_r-=3; break;
        case  2: tspeed_l+=3; tspeed_r-=3; break;
        case  1: break;
        case  0: break;
        case -1: break;
        case -2: tspeed_l-=3; tspeed_r+=3; break;
        case -3: tspeed_l-=3; tspeed_r+=4; break;
      }
    }

    if (goto_pos) {
      // new command is gotopos
      //tspeed_l += copro_ticks_l;
      //tspeed_r += copro_ticks_r;

      
      //goto_pos=0;
    }
    
  }

  if ((state==FL_R0) && (state_timer>200)) {
    tspeed_l = 11; tspeed_r=10;
  }

  if ((state==FL_L0) && (state_timer>200)) {
    tspeed_l = 10; tspeed_r=11;
  }

  
  if (halt) {
    halt=0;
    copro_stop();
  
  } else if (goto_pos) {
    copro_setTargetRel(tspeed_l, tspeed_r, 20);

  } else {
    if (acquisition_getDiff(AN_LINE_R)>acquisition_getDiff(AN_LINE_L)) {
       copro_setSpeed(tspeed_l+1, tspeed_r);
    } else if (acquisition_getDiff(AN_LINE_R)<acquisition_getDiff(AN_LINE_L)) {
      copro_setSpeed(tspeed_l+1, tspeed_r);
    } else {
      copro_setSpeed(tspeed_l, tspeed_r);
    }
  }
  mode=state;
  return BEHAVIOUR_LINE;
}

