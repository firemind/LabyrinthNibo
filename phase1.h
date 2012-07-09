#ifndef PHASE1_H
#define PHASE1_H

#include "labyrinth.h"
#include "move.h"

void addEast(struct coord c){
  labyrinth[c.x][c.y].actions += EAST;
  if(!(labyrinth[c.x+1][c.y].actions & WEST)){
    labyrinth[c.x+1][c.y].actions += WEST;
  }
}

void addWest(struct coord c){
  labyrinth[c.x][c.y].actions += WEST;
  if(!(labyrinth[c.x-1][c.y].actions & EAST)){
    labyrinth[c.x-1][c.y].actions += EAST;
  }
}

void addNorth(struct coord c){
  labyrinth[c.x][c.y].actions += NORTH;
  if(!(labyrinth[c.x][c.y-1].actions & SOUTH)){
    labyrinth[c.x][c.y-1].actions += SOUTH;
  }
}

void addSouth(struct coord c){
  labyrinth[c.x][c.y].actions += SOUTH;
  if(!(labyrinth[c.x][c.y+1].actions & NORTH)){
    labyrinth[c.x][c.y+1].actions += NORTH;
  }
}


void readActions(){
  labyrinth[current_position.x][current_position.y].value = 1;
  labyrinth[current_position.x][current_position.y].actions = 0;
  switch(current_direction){
    case WEST:
      addEast(current_position);
      if(!wallFront()){
        addWest(current_position);
      }
      if(!wallLeft()){
        addSouth(current_position);
      }
      if(!wallRight()){
        addNorth(current_position);
      }
      break;
    case EAST:
      addWest(current_position);
      if(!wallFront()){
        addEast(current_position);
      }
      if(!wallLeft()){
        addNorth(current_position);
      }
      if(!wallRight()){
        addSouth(current_position);
      }
      break;
    case SOUTH:
      addNorth(current_position);
      if(!wallFront()){
        addSouth(current_position);
      }
      if(!wallLeft()){
        addEast(current_position);
      }
      if(!wallRight()){
        addWest(current_position);
      }
      break;
    case NORTH:
      addSouth(current_position);
      if(!wallFront()){
        addNorth(current_position);
      }
      if(!wallLeft()){
        addWest(current_position);
      }
      if(!wallRight()){
        addEast(current_position);
      }
      break;
  }
}

int onGoal(){
  return 0;//groundBlack();
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
