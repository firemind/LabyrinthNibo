#ifndef STAYONLINE_H
#define STAYONLINE_H

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

#include "mylog.h"

#define THRESHOLD 150
typedef int bool;

bool line_left(void){
  return floor_relative[LINE_LEFT]<THRESHOLD;
}

bool line_right(){
  return floor_relative[LINE_RIGHT]<THRESHOLD;
}

bool floor_left(void){
  return floor_relative[FLOOR_LEFT]<THRESHOLD;
}

bool floor_right(){
  return floor_relative[FLOOR_RIGHT]<THRESHOLD;
}


void drive_and_stay_on_line(){
  while (1) {
    _delay_ms(1);
    floor_update();
    if /*(!line_left() && !line_right()){
      copro_stopImmediate();
      copro_setSpeed(-20, -20);
      _delay_ms(100);
    }else if*/ (floor_right() ) {
      copro_setSpeed(15, 0);
    } else if(floor_left() ) {
      copro_setSpeed(0, 15);
    }else{
      copro_setSpeed(15, 15);
    }
  }
}

#endif /*STAYONLINE_H*/
