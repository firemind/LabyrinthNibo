#ifndef PHASE3_H
#define PHASE3_H

#include "labyrinth.h"
#include "move.h"

char nextAction(){
  char bestAction = 0;
  int bestValue = -10000;
  char actions = labyrinth[current_position.x][current_position.y].actions;
  
  if ((actions & NORTH) && labyrinth[current_position.x][current_position.y - 1].value > bestValue){
    bestAction  = NORTH;
    bestValue = labyrinth[current_position.x][current_position.y - 1].value;
  }

  if ((actions & WEST) && labyrinth[current_position.x - 1][current_position.y].value > bestValue){
    bestAction  = WEST;
    bestValue = labyrinth[current_position.x - 1][current_position.y].value;
  }

  if ((actions & SOUTH) && labyrinth[current_position.x][current_position.y + 1].value > bestValue){
    bestAction = SOUTH;
    bestValue = labyrinth[current_position.x][current_position.y + 1].value;
  }

  if ((actions & EAST) && labyrinth[current_position.x - 1][current_position.y].value > bestValue){
    bestAction = EAST;
    bestValue = labyrinth[current_position.x - 1][current_position.y].value;
  }

  return bestAction;
}


void doPhase3(){
  while(!onGoal()){
    switch(nextAction()){
      case SOUTH:
        move_south();break;
      case NORTH:
        move_north();break;
      case WEST:
        move_west();break;
      case EAST:
        move_east();break;
    }
  }
}
#endif /*PHASE3_H*/
