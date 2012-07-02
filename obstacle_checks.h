#ifndef OBSTACLECHECKS_H
#define OBSTACLECHECKS_H

#include "nibo/iodefs.h"
#include <stdio.h>

int pillar_count = 0;
bool pillar_started = false;

void pillar_check(uint8_t *dist){
  // seeting leds
  uint8_t ledred = 0x00;
  uint8_t ledgreen = 0x00;

  // left sensor: led 2
  if(dist[4] > 0xCC && !pillar_started) pillar_started = true;
  else if(dist[4] <= 0xCC && pillar_started){
    pillar_count += 1; 
    pillar_started = false;
  }
  char text[255] ="";
  sprintf(text, "Count: %i", pillar_count);
  mylog(text);
}

#endif /*OBSTACLECHECKS_H*/
