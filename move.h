#ifndef MOVE_H
#define MOVE_H

#include "labyrinth.h"

void move_forward() {
  copro_setSpeed(20, 20); 
  delay(1250);
  copro_stopImmediate();
  switch(current_direction){
    case WEST:
      current_position.x -= 1; break;
    case EAST:
      current_position.x += 1; break;
    case SOUTH:
      current_position.y += 1; break;
    case NORTH:
      current_position.y -= 1; break;
  }
}

void turn_right() {
  copro_setSpeed(20,-20);
  _delay_ms(1000);
  copro_stopImmediate();
  switch(current_direction){
    case WEST:
      current_direction = NORTH; break;
    case EAST:
      current_direction = SOUTH; break;
    case SOUTH:
      current_direction = WEST; break;
    case NORTH:
      current_direction = EAST; break;
  }
}

void turn_left() {
  copro_setSpeed(-15,20);
  _delay_ms(1000);
  copro_setSpeed(-20, 20);
  _delay_ms(1000);
  copro_stopImmediate();
  switch(current_direction){
    case WEST:
      current_direction = SOUTH; break;
    case EAST:
      current_direction = NORTH; break;
    case SOUTH:
      current_direction = EAST; break;
    case NORTH:
      current_direction = WEST; break;
  }
}

void move_south() {
  switch(current_direction){
    case WEST:
      turn_left(); break;
    case EAST:
      turn_right(); break;
    case NORTH:
      turn_right(); turn_right(); break;
  }
  move_forward();
}

void move_north() {
  switch(current_direction){
    case WEST:
      turn_right(); break;
    case EAST:
      turn_left(); break;
    case SOUTH:
      turn_right(); turn_right(); break;
  }
  move_forward();
}

void move_west() {
  switch(current_direction){
    case EAST:
      turn_right(); turn_right(); break;
    case NORTH:
      turn_left(); break;
    case SOUTH:
      turn_right(); break;
  }
  move_forward();
}

void move_east() {
  switch(current_direction){
    case WEST:
      turn_right(); turn_right(); break;
    case NORTH:
      turn_right(); break;
    case SOUTH:
      turn_left(); break;
  }
  move_forward();
}
#endif /*MOVE_H*/
