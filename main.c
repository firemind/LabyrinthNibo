/**
 * Name:  MP Nibo2
 * Datum: 02.07.12
 * Authors: Victor Ruch, Mike Gerber
 **/

#include "nibo/niboconfig.h"
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
#include "blink.h"
#include "turn.h"
#include "labyrinth.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)
#define MAX_DISTANCE 0xe5

uint8_t dist[5];

// prints int as hex 
void print_hex (uint8_t val) {
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}

// Logs the distance from all proximity sensors in hex.
void log_distance() {
    gfx_move(25, 20);
    print_hex(dist[0]);
    gfx_print_char(' ');
    print_hex(dist[1]);
    gfx_print_char(' ');
    print_hex(dist[2]);
    gfx_print_char(' ');
    print_hex(dist[3]);
    gfx_print_char(' ');
    print_hex(dist[4]);
}

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

  copro_ir_startMeasure();
  delay(10);
  //motco_setSpeedParameters(5, 4, 6); // ki, kp, kd
  copro_setSpeedParameters(15, 20, 10); // ki, kp, kd
 
  fillLabyrinth();
  moveOne();
  
}



void moveOne(){
  copro_setSpeed(25, 25); 
  delay(1100);
  copro_stopImmediate();
  delay(50000);
}

struct position {
  int x;
  int y;
};
struct position gedAdjacent (int x, int y, char dir){
  struct position mypos;
  mypos.x = x;
  mypos.y = y;
  switch(dir){
    case WEST:  mypos.x -=1; break;
    case EAST:  mypos.x +=1; break;
    case SOUTH: mypos.y +=1; break;
    case NORTH: mypos.y -=1; break;
  }
  return mypos;
}

void calc_value(struct field state, int level){
  if(state.value != NULL){
    return;
  }
  for(i=0x8;i>=i%=2;
  

}
