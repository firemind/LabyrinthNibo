#ifndef LABYRINTH_H
#define LABYRINTH_H

#ifndef max
  #define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#define NORTH 0x01
#define SOUTH 0x02
#define WEST 0x04
#define EAST 0x08

#define LAB_WIDTH 8
#define LAB_HEIGHT 3

#define MOVING_COST 7

#define start_x 0
#define start_y 0
#define goal_x  5
#define goal_y  2

#define death_x  6
#define death_y  1

#define gamma 0.9
#define epsilon 0.001

#define UNKNOWN  0 
#define EXPLORED 1
#define FRONTIER 2

struct field {
  char actions;
  int value;
  int state; // UNKNOWN, EXPLORED or FRONTIER
} labyrinth[LAB_WIDTH][LAB_HEIGHT];

struct coord {
  int x;
  int y;
};

struct node {
  struct node *parent;
  struct coord coord;
} frontier[24], explored[100];

struct node *getNext();
void addToFrontier(int x, int y, int cost, struct node *p);
int estimatedMovingCost(int fx, int fy, int tx, int ty);
int totalCost(int x, int y);

int fc = 0; // Counter for nodes in frontier array
int ec = 0; // Counter for nodes in explored array

void fillLabyrinth() {
  labyrinth[0][0].actions = SOUTH;
  labyrinth[0][1].actions = EAST+NORTH;
  labyrinth[0][2].actions = EAST;

  labyrinth[1][0].actions = EAST+SOUTH;
  labyrinth[1][1].actions = NORTH+WEST+SOUTH;
  labyrinth[1][2].actions = NORTH+WEST;

  labyrinth[2][0].actions = EAST+SOUTH+WEST;
  labyrinth[2][1].actions = NORTH+SOUTH;
  labyrinth[2][2].actions = NORTH;

  labyrinth[3][0].actions = EAST+SOUTH+WEST;
  labyrinth[3][1].actions = NORTH+EAST;
  labyrinth[3][2].actions = EAST;

  labyrinth[4][0].actions = EAST+WEST;
  labyrinth[4][1].actions = EAST+SOUTH+WEST;
  labyrinth[4][2].actions = EAST+NORTH+WEST;

  labyrinth[5][0].actions = EAST+WEST;
  labyrinth[5][1].actions = EAST+WEST;
  labyrinth[5][2].actions = EAST+WEST;

  labyrinth[6][0].actions = SOUTH+WEST;
  labyrinth[6][1].actions = EAST+NORTH+WEST;
  labyrinth[6][2].actions = EAST+WEST;

  labyrinth[7][0].actions = 0;
  labyrinth[7][1].actions = SOUTH+WEST;
  labyrinth[7][2].actions = NORTH+WEST;

//  labyrinth[death_x][death_y].value = -99;
//  labyrinth[goal_x][goal_y].value   = 100;
  addToFrontier(0, 0, 0, NULL);
}

void printLabyrinth(){
  int i,j;
  for(i = 0;i<LAB_HEIGHT;i++){ // Every Row
    // top
    for(j = 0;j<LAB_WIDTH;j++){ // Every Cell
      if (labyrinth[j][i].actions & NORTH){
        printf("+   ");
      }else{
        printf("+---");
      }
    }
    printf("+\n");
    // middle
    for(j = 0;j<LAB_WIDTH;j++){ // Every Cell
      if (labyrinth[j][i].actions & WEST){
        printf(" %3i", labyrinth[j][i].value);
      }else{
        printf("|%3i", labyrinth[j][i].value);
      }
      if (j == LAB_WIDTH - 1) {
        if (labyrinth[j][i].actions & EAST){
          printf(" \n");
        }else{
          printf("|\n");
        }
      }
    }
    // bottom
    if (i == LAB_HEIGHT - 1) {
      for(j = 0;j<LAB_WIDTH;j++){ // Every Cell
        if (labyrinth[j][i].actions & SOUTH){
          printf("+   ");
        }else{
          printf("+---");
        }
      }
      printf("+\n");
    }
  }
}


int calculateAdjacentValues(int x, int y);

// Calls checkAndSetValue for every adjacent field
// value is the value of field at x and y minus the
// cost of moving
int calculateAdjacentValues(int x, int y){
  struct field f = labyrinth[x][y];
  int delta = 0;
  int val = -100;
  if (!(x == goal_x && y == goal_y) &&
      !(x == death_x && y == death_y)
      ){
    if(f.actions & NORTH){
      val = max(val, valueOf(x,y-1) - MOVING_COST);
    }
    if(f.actions & SOUTH){
      val = max(val, valueOf(x,y+1) - MOVING_COST);
    }
    if(f.actions & WEST){
      val = max(val, valueOf(x-1,y) - MOVING_COST);
    }
    if(f.actions & EAST){
      val = max(val, valueOf(x+1,y) - MOVING_COST);
    }
    /*printf("val: %i", val );*/
    /*printf("dif: %i", abs(val - labyrinth[x][y].value ));*/
    /*if(abs(val - labyrinth[x][y].value ) > epsilon * (1-gamma) / gamma){*/
    if(abs(val - labyrinth[x][y].value ) > 0){
      labyrinth[x][y].value = val;
      if(f.actions & NORTH){
        calculateAdjacentValues(x, y-1);
      }
      if(f.actions & SOUTH){
        calculateAdjacentValues(x, y+1);
      }
      if(f.actions & WEST){
        calculateAdjacentValues(x-1, y);
      }
      if(f.actions & EAST){
        calculateAdjacentValues(x+1, y);
      }
    }
  }
  return labyrinth[x][y].value;
}

int calculateValues(){
  while(0==0) {
    struct node *next = getNext();
    struct coord n = next->coord;
    printf("next: %i, %i\n",n.x, n.y);
    if (n.x == goal_x && n.y == goal_y){ 
      while(next != NULL){
         labyrinth[next->coord.x][next->coord.y].value = 100;

         //printf("parent coord: %i,%i\n", next->coord.x, next->coord.y);
         next = next->parent;
      }
      return 0; }
    struct field f = labyrinth[n.x][n.y];
    if(f.actions & NORTH){
      addToFrontier(n.x,n.y-1, f.value + MOVING_COST, next);
    }
    if(f.actions & SOUTH){
      addToFrontier(n.x,n.y+1, f.value + MOVING_COST, next);
    }
    if(f.actions & WEST){
      addToFrontier(n.x-1,n.y, f.value + MOVING_COST, next);
    }
    if(f.actions & EAST){
      addToFrontier(n.x+1,n.y, f.value + MOVING_COST, next);
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
    int pos = fc++;
    frontier[pos] = n;
    printf("pos: %i\n",pos);

    labyrinth[x][y].value = cost;
    printf("added: %i, %i\n",x, y);
    //printf("coords: %i,%i\n",frontier[fc-1].coord.x,frontier[fc-1].coord.y);
    if (&n == n.parent){
       printf("ERROR!\n");
    }
  }
}

struct node *getNext(){
  int i, rmp = 0;
  struct coord best = frontier[0].coord;
  printf("best: %i,%i\n",best.x,best.y);
  for(i=0;i<fc;i++){
    struct coord alt = frontier[i].coord;
    if (totalCost(alt.x, alt.y) < totalCost(best.x, best.y)
    ){
      best = alt;
      rmp = i;
    }
  }
  printf("fc: %i\n",fc);
  explored[ec++] = frontier[rmp];
  // cleanup frontier array
  for(i=0;rmp+i<fc;i++){
    frontier[rmp+i] = frontier[rmp+i+1];
  }
  fc--;
  labyrinth[best.x][best.y].state = EXPLORED;
  return &explored[ec-1];
}

int totalCost(int x, int y){
 return labyrinth[x][y].value + estimatedMovingCost(x, y, goal_x, goal_y);
}

int estimatedMovingCost(int fx, int fy, int tx, int ty){
  return (abs(fx -tx) + abs(fy - ty)) * MOVING_COST;
}

int valueOf(int x, int y){
  return labyrinth[x][y].value;
}

#endif /*LABYRINTH_H*/
