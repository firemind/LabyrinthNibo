#ifndef MOVE_H
#define MOVE_H

#include "labyrinth.h"

char current_direction = SOUTH;

void move_forward() {
  copro_setSpeed(20, 20); 
  delay(1250);
  copro_stopImmediate();
}

void turn_right() {
    copro_setSpeed(20,-20);
    _delay_ms(1000);
    copro_stopImmediate();
}

void turn_left() {
    copro_setSpeed(-15,20);
    _delay_ms(1000);
    copro_setSpeed(-20, 20);
    _delay_ms(1000);
    copro_stopImmediate();
}

#endif /*MOVE_H*/
