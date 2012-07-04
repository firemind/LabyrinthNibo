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

struct field {
  char actions;
  int value;
} labyrinth[LAB_WIDTH][LAB_HEIGHT];

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

  labyrinth[death_x][death_y].value = -99;
  labyrinth[goal_x][goal_y].value   = 100;
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

int valueOf(int x, int y){
  return labyrinth[x][y].value; 
}

void calculateValues(){
  calculateAdjacentValues(start_x, start_y);
}
#endif /*LABYRINTH_H*/
