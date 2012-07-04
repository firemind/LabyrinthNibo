/**
 * Name:  MP Nibo2
 * Datum: 02.07.12
 * Authors: Victor Ruch, Mike Gerber
 * Robot: 12
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
#include "nibo/iodefs.h"

#include "mylog.h"
#include "move.h"
#include "labyrinth.h"
#include "energy_status.h"

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

<<<<<<< HEAD
  while (1) {
    delay(50);
    leds_set_displaylight(1024);

    energy_status_check();

    // Request distance data
    copro_update();

    dist[4] = copro_distance[4]/128; // left
    dist[3] = copro_distance[3]/128; // front left
    dist[2] = copro_distance[2]/128; // front 
    dist[1] = copro_distance[1]/128; // front right
    dist[0] = copro_distance[0]/128; // right

    dist[4] = (dist[4]<250)? (dist[4]+5):255;
    dist[0] = (dist[0]<250)? (dist[0]+5):255;
    dist[2] = (dist[2]>5)? (dist[2]-5):0;

    log_distance();
    
    move_forward();
    delay(3000);
    turn_left();
    delay(3000);
    //turn_right();
  }
}

=======
void fillLabyrinth() {
  labyrinth[0][0].actions = SOUTH;
  labyrinth[0][1].actions = EAST+NORTH;
  labyrinth[0][2].actions = EAST;

  labyrinth[1][0].actions = EAST+SOUTH;
  labyrinth[1][1].actions = NORTH+WEST+SOUTH;
  labyrinth[1][2].actions = NORTH+WEST;

  labyrinth[2][0].actions = EAST+SOUTH+WEST;
  labyrinth[2][1].actions = NORTH+SOUTH;
  labyrinth[2][2].actions = NORTH;

  labyrinth[3][0].actions = EAST+SOUTH+WEST;
  labyrinth[3][1].actions = NORTH+EAST;
  labyrinth[3][2].actions = EAST;

  labyrinth[4][0].actions = EAST+WEST;
  labyrinth[4][1].actions = EAST+SOUTH+WEST;
  labyrinth[4][2].actions = EAST+NORTH+WEST;

  labyrinth[5][0].actions = EAST+WEST;
  labyrinth[5][1].actions = EAST+WEST;
  labyrinth[5][2].actions = EAST+SOUTH+WEST;
  labyrinth[5][2].value   = 100;

  labyrinth[6][0].actions = SOUTH+WEST;
  labyrinth[6][1].actions = EAST+NORTH+WEST;
  labyrinth[6][2].actions = EAST+WEST;

  labyrinth[7][0].actions = 0;
  labyrinth[7][1].actions = SOUTH+WEST;
  labyrinth[7][2].actions = NORTH+WEST;
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
>>>>>>> 25b2f908ded57aecef37594b412c88329262408e
