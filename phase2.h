#ifndef PHASE2_H
#define PHASE2_H

#include "labyrinth.h"
#include "move.h"

void moveTo(struct coord c){
  /*printf("moving from %i,%i to %i,%i\n", c.x, c.y, current_position.x, current_position.y);*/
  struct environment e;
  e.goal_x = current_position.x;
  e.goal_y = current_position.y;
  struct environment *env = &e;
  env->fc = 0;
  env->ec = 0;
  memset(env->frontier, 0, sizeof(struct node) * FRONTIER_SIZE);
  memset(env->explored, 0, sizeof(struct node) * EXPLORED_SIZE);
  memset(env->states, 0, sizeof(int) * LAB_WIDTH*LAB_HEIGHT);

  addToFrontier(c.x, c.y, 0, NULL, env);
  while(0==0) {
    struct node *next = getNext(env);
    struct coord n = next->coord;
    /*printf("cp   %i, %i\n", current_position.x, current_position.y);*/
    /*printf("next %i, %i\n", n.x, n.y);*/
    /*printf("goal %i, %i\n", env->goal_x, env->goal_y);*/
    if (n.x == env->goal_x && n.y == env->goal_y){ 
      while(next != NULL){
        /*printf("movto: %i,%i\n", next->coord.x, next->coord.y);*/
        if (next->coord.x > current_position.x){
          move_east();
        }else if(next->coord.x < current_position.x){
          move_west();
        }else if (next->coord.y > current_position.y){
          move_south();
        }else if(next->coord.y < current_position.y){
          /*printf("move north\n");*/
          move_north();
        }
        /*printf("curpos: %i,%i\n", current_position.x, current_position.y);*/
        next = next->parent;
      }
      readActions();
      return; 
    }
    struct field f = labyrinth[n.x][n.y];
    if(f.actions & NORTH){
      addToFrontier(n.x,n.y-1, next->cost + MOVING_COST, next, env);
    }
    if(f.actions & SOUTH){
      addToFrontier(n.x,n.y+1, next->cost + MOVING_COST, next, env);
    }
    if(f.actions & WEST){
      addToFrontier(n.x-1,n.y, next->cost + MOVING_COST, next, env);
    }
    if(f.actions & EAST){
      addToFrontier(n.x+1,n.y, next->cost + MOVING_COST, next, env);
    }
  }
}


void doPhase2(){
  struct environment e;
  e.goal_x = start_x;
  e.goal_y = start_y;
  struct environment *env = &e;
  env->fc = 0;
  env->ec = 0;
  memset(env->frontier, 0, sizeof(struct node) * FRONTIER_SIZE);
  memset(env->explored, 0, sizeof(struct node) * EXPLORED_SIZE);
  memset(env->states, 0, sizeof(int) * LAB_WIDTH*LAB_HEIGHT);

  addToFrontier(current_position.x, current_position.y, 0, NULL, env);
  while(0==0) {
    struct node *next = getNext(env);
    struct coord n = next->coord;
    moveTo(n);
    if (n.x == env->goal_x && n.y == env->goal_y){ 
      while(next != NULL){
        labyrinth[next->coord.x][next->coord.y].value = 100;
        next = next->parent;
      }
      return; 
    }
    struct field f = labyrinth[n.x][n.y];
    if(f.actions & NORTH){
      addToFrontier(n.x,n.y-1, next->cost + MOVING_COST, next, env);
    }
    if(f.actions & SOUTH){
      addToFrontier(n.x,n.y+1, next->cost + MOVING_COST, next, env);
    }
    if(f.actions & WEST){
      addToFrontier(n.x-1,n.y, next->cost + MOVING_COST, next, env);
    }
    if(f.actions & EAST){
      addToFrontier(n.x+1,n.y, next->cost + MOVING_COST, next, env);
    }
  }
}




#endif /*PHASE2_H*/
