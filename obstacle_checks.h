#ifndef OBSTACLECHECKS_H
#define OBSTACLECHECKS_H

#include "nibo/iodefs.h"
#include <stdio.h>
#define FALSE 0
#define TRUE 1

int pillar_count = 0;
char pillar_started = FALSE;

void pillar_check(uint8_t *dist){
  // seeting leds
  uint8_t ledred = 0x00;
  uint8_t ledgreen = 0x00;

  // left sensor: led 2
  if(dist[4] > 0xCC && !pillar_started) pillar_started = TRUE;
  else if(dist[4] <= 0xCC && pillar_started){
    pillar_count += 1; 
    pillar_started = FALSE;
  }
  char text[255] ="";
  sprintf(text, "Count: %i", pillar_count);
  mylog(text);
}

char wallFront(){
  int truecount = 0;
  int falsecount = 0;
  copro_update();
  uint8_t dist = copro_distance[2]/128;
  int i;
  for(i; i < 5; i++){
    if(dist > 0x40){
      truecount++;
    }else{
      falsecount++;
    }
    delay(50);
  }
  return truecount > falsecount;
}

char wallLeft(){
  int truecount = 0;
  int falsecount = 0;
  copro_update();
  uint8_t dist = copro_distance[4]/128;
  int i;
  for(i; i < 5; i++){
    if(dist > dist > 0x3A){
      truecount++;
    }else{
      falsecount++;
    }
    delay(50);
  }
  return truecount > falsecount;
}

char wallRight(){
  int truecount = 0;
  int falsecount = 0;
  copro_update();
  uint8_t dist = copro_distance[0]/128;
  int i;
  for(i; i < 5; i++){
    if(dist > dist > 0x3A){
      truecount++;
    }else{
      falsecount++;
    }
    delay(50);
  }
  return truecount > falsecount;
}

#endif /*OBSTACLECHECKS_H*/
