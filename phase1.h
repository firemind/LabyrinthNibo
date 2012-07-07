#ifndef PHASE1_H
#define PHASE1_H

#include "labyrinth.h"
#include "move.h"

void readActions(){
  labyrinth[current_position.x][current_position.y].actions = 0;
  switch(current_direction){
    case WEST:
      labyrinth[current_position.x][current_position.y].actions += EAST;
      if(!wallFront()){
        labyrinth[current_position.x][current_position.y].actions += WEST;
      }
      if(!wallLeft()){
        labyrinth[current_position.x][current_position.y].actions += SOUTH;
      }
      if(!wallRight()){
        labyrinth[current_position.x][current_position.y].actions += NORTH;
      }
      break;
    case EAST:
      labyrinth[current_position.x][current_position.y].actions += WEST;
      if(!wallFront()){
        labyrinth[current_position.x][current_position.y].actions += EAST;
      }
      if(!wallLeft()){
        labyrinth[current_position.x][current_position.y].actions += NORTH;
      }
      if(!wallRight()){
        labyrinth[current_position.x][current_position.y].actions += SOUTH;
      }
      break;
    case SOUTH:
      labyrinth[current_position.x][current_position.y].actions += NORTH;
      if(!wallFront()){
        labyrinth[current_position.x][current_position.y].actions += SOUTH;
      }
      if(!wallLeft()){
        labyrinth[current_position.x][current_position.y].actions += EAST;
      }
      if(!wallRight()){
        labyrinth[current_position.x][current_position.y].actions += WEST;
      }
      break;
    case NORTH:
      labyrinth[current_position.x][current_position.y].actions += SOUTH;
      if(!wallFront()){
        labyrinth[current_position.x][current_position.y].actions += NORTH;
      }
      if(!wallLeft()){
        labyrinth[current_position.x][current_position.y].actions += WEST;
      }
      if(!wallRight()){
        labyrinth[current_position.x][current_position.y].actions += EAST;
      }
      break;
  }
}

int onGoal(){
  return groundBlack();
}

void doPhase1(){
  while(!onGoal()){
    if(!wallLeft()){
      turn_left();
    }
    while(wallFront()){
       turn_right();   
    }
    move_forward();
    readActions();
  }
}
#endif /*PHASE1_H*/
