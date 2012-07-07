#ifndef CALCULATE_H
#define CALCULATE_H

#define MOVING_COST 7

#define FRONTIER_SIZE 24
#define EXPLORED_SIZE 24

struct node {
  struct node *parent;
  struct coord coord;
  int cost;
} frontier[FRONTIER_SIZE], explored[EXPLORED_SIZE];

struct node *getNext();
void addToFrontier(int x, int y, int cost, struct node *p);
int estimatedMovingCost(int fx, int fy, int tx, int ty);
int totalCost(struct node n);

int fc = 0; // Counter for nodes in frontier array
int ec = 0; // Counter for nodes in explored array


int calculateValues(){
  addToFrontier(0, 0, 0, NULL);
  while(0==0) {
    struct node *next = getNext();
    struct coord n = next->coord;
    //printf("next: %i, %i\n",n.x, n.y);
    if (n.x == goal_x && n.y == goal_y){ 
      while(next != NULL){
         labyrinth[next->coord.x][next->coord.y].value = 100;

         //printf("parent coord: %i,%i\n", next->coord.x, next->coord.y);
         next = next->parent;
      }
      return 0; }
    struct field f = labyrinth[n.x][n.y];
    if(f.actions & NORTH){
      addToFrontier(n.x,n.y-1, next->cost + MOVING_COST, next);
    }
    if(f.actions & SOUTH){
      addToFrontier(n.x,n.y+1, next->cost + MOVING_COST, next);
    }
    if(f.actions & WEST){
      addToFrontier(n.x-1,n.y, next->cost + MOVING_COST, next);
    }
    if(f.actions & EAST){
      addToFrontier(n.x+1,n.y, next->cost + MOVING_COST, next);
    }
  }
}

void addToFrontier(int x, int y, int cost, struct node *p){
  if (labyrinth[x][y].state == UNKNOWN){
    struct node n;
    struct coord c;
    c.x = x, c.y = y;
    n.parent = p;
    n.coord = c;
    n.cost = cost;
    int pos = fc++;
    frontier[pos] = n;
    if (&n == n.parent){
       // this should never happen
       printf("ERROR!\n");
    }
  }
}

struct node *getNext(){
  int i, rmp = 0;
  struct node best = frontier[0];
  for(i=0;i<fc;i++){
    struct node alt = frontier[i];
    if (totalCost(alt) < totalCost(best)
    ){
      best = alt;
      rmp = i;
    }
  }
  explored[ec++] = best;
  // cleanup frontier array
  for(i=0;rmp+i<fc;i++){
    frontier[rmp+i] = frontier[rmp+i+1];
  }
  fc--;
  labyrinth[best.coord.x][best.coord.y].state = EXPLORED;
  return &explored[ec-1];
}

int totalCost(struct node n){
 return n.cost + estimatedMovingCost(n.coord.x, n.coord.y, goal_x, goal_y) - costOf(n.coord);
}

int costOf(struct coord c){
  return labyrinth[c.x][c.y].value;
}

int estimatedMovingCost(int fx, int fy, int tx, int ty){
  return (abs(fx -tx) + abs(fy - ty)) * MOVING_COST;
}

void cleanup(){
  fc = 0;
  ec = 0;
  memset(labyrinth, 0, sizeof(struct field) * LAB_WIDTH*LAB_HEIGHT);
  memset(frontier, 0, sizeof(struct node) * FRONTIER_SIZE);
  memset(explored, 0, sizeof(struct node) * EXPLORED_SIZE);
}

#endif /*CALCULATE_H*/
