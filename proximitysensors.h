#ifndef PROXIMITYSENSORS_H
#define PROXIMITYSENSORS_H

#include "nibo/iodefs.h"

void proximity_check(uint8_t *dist){
  // seeting leds
  uint8_t ledred = 0x00;
  uint8_t ledgreen = 0x00;

  // right sensor: led 7
  if(dist[0] > 0xCC) ledred += 0x80;
  else if(dist[0] > 0x30) ledgreen += 0x80; 
  
  // frontright sensor: led 6
  if(dist[1] > 0xCC) ledred += 0x40;
  else if(dist[1] > 0x30) ledgreen += 0x40; 
  
  // front sensor: led 4 & 5
  if(dist[2] > 0xCC) ledred += 0x30;
  else  if(dist[2] > 0x30) ledgreen += 0x30; 
  
  // frontleft sensor: led 3
  if(dist[3] > 0xCC) ledred += 0x08;
  else if(dist[3] > 0x30) ledgreen += 0x08;

  // left sensor: led 2
  if(dist[4] > 0xCC) ledred += 0x04;
  else if(dist[4] > 0x30) ledgreen += 0x04;

  IO_LEDS_RED_PORT = ledred;
  IO_LEDS_GREEN_PORT = ledgreen;
}

#endif /*PROXIMITYSENSORS_H*/
