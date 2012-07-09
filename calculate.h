#ifndef CALCULATE_H
#define CALCULATE_H

#define MOVING_COST 7

#define FRONTIER_SIZE 24
#define EXPLORED_SIZE 24
#define NULL 0

struct node {
  struct node *parent;
  struct coord coord;
  int cost;
} ;

struct environment {
  int goal_x;
  int goal_y;
  int fc; // Counter for nodes in frontier array
  int ec; // Counter for nodes in explored array
  struct node frontier[FRONTIER_SIZE];
  struct node explored[EXPLORED_SIZE];
  int states[LAB_WIDTH][LAB_HEIGHT];
};

struct node *getNext(struct environment *env);
void addToFrontier(int x, int y, int cost, struct node *p, struct environment *env);
int estimatedMovingCost(int fx, int fy, int tx, int ty);
int totalCost(struct node n, struct environment *env);

int calculateValues(struct environment *env){
  addToFrontier(0, 0, 0, NULL, env);
  while(0==0) {
    struct node *next = getNext(env);
    struct coord n = next->coord;
    /*printf("next: %i, %i\n",n.x, n.y);*/
    /*printf("next: %i, %i\n",env->goal_x, env->goal_x);*/
    if (n.x == env->goal_x && n.y == env->goal_y){ 
      while(next != NULL){
         labyrinth[next->coord.x][next->coord.y].value = 100;

         next = next->parent;
      }
      return 0; }
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

void addToFrontier(int x, int y, int cost, struct node *p, struct environment *env){
  if (env->states[x][y] == UNKNOWN){
    /*printf("Adding to frontier %i,%i!\n", x,y);*/
    struct node n;
    struct coord c;
    c.x = x; c.y = y;
    n.parent = p;
    n.coord = c;
    n.cost = cost;
    int pos = env->fc++;
    env->frontier[pos] = n;
    if (&n == n.parent){
       // this should never happen
       printf("ERROR!\n");
    }
  }
}

struct node *getNext(struct environment *env){
  if (env->fc < 0){
     // this should never happen
     printf("ERROR!\n");
  }
  int i, rmp = 0;
  struct node best = env->frontier[0];
  for(i=0;i<env->fc;i++){
    struct node alt = env->frontier[i];
    if (totalCost(alt, env) < totalCost(best, env)
    ){
      best = alt;
      rmp = i;
    }
  }
  env->explored[env->ec++] = best;
  // cleanup frontier array
  for(i=0;rmp+i<env->fc;i++){
    env->frontier[rmp+i] = env->frontier[rmp+i+1];
  }
  env->fc--;
  env->states[best.coord.x][best.coord.y] = EXPLORED;
  return &(env->explored)[env->ec-1];
}

int totalCost(struct node n, struct environment *env){
 return n.cost + estimatedMovingCost(n.coord.x, n.coord.y, env->goal_x, env->goal_y) - costOf(n.coord);
}

int costOf(struct coord c){
  return labyrinth[c.x][c.y].value;
}

int estimatedMovingCost(int fx, int fy, int tx, int ty){
  return (abs(fx -tx) + abs(fy - ty)) * MOVING_COST;
}

void cleanup(struct environment *env){
  env->fc = 0;
  env->ec = 0;
  memset(labyrinth, 0, sizeof(struct field) * LAB_WIDTH*LAB_HEIGHT);
  memset(env->states, 0, sizeof(int) * LAB_WIDTH*LAB_HEIGHT);
  memset(env->frontier, 0, sizeof(struct node) * FRONTIER_SIZE);
  memset(env->explored, 0, sizeof(struct node) * EXPLORED_SIZE);
}

#endif /*CALCULATE_H*/
