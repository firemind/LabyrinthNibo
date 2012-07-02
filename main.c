/**
 * Name:  Pruefung P3d Nibo2
 * Datum: 11.06.12
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

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdlib.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)
#define MAX_DISTANCE 0xe5

uint8_t dist[5];
uint8_t turns = 0;

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

  while (1) {
    delay(50);
    leds_set_displaylight(1024);

    // Spannung
    bot_update();
    
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
    
    if(dist[2] >= MAX_DISTANCE){  // if wall in front
      turns = turns + 1;
      if(dist[1] < dist[3]){ // if wall is closer to front left sensor
        turn_right();
      } else {  // or closer to front right sensor
        turn_left();
      }
    } else if (dist[1] >= MAX_DISTANCE) { // if too close to front right sensor
      turns = turns + 1;
      turn_left();
    } else if (dist[3] >= MAX_DISTANCE) { // if too close to front left sensor
      turns = turns + 1;
      turn_right();
    } else { // else just drive straight on
      copro_setSpeed(10,10);
    }

    if(turns >= 4) { // if 4 turns have been made
      turns = 0;
      // dance crazy
      blink();
    }
  }
}
