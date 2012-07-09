#ifndef LABYRINTH_H
#define LABYRINTH_H

#define LAB_WIDTH 8
#define LAB_HEIGHT 3

#define start_x 0
#define start_y 0

#define UNKNOWN  0 
#define EXPLORED 1

/*int goal_x = 5;*/
/*int goal_y = 2;*/

struct field {
  char actions;
  int value;
} labyrinth[LAB_WIDTH][LAB_HEIGHT];

void fillLabyrinth(struct field labyrinth[LAB_WIDTH][LAB_HEIGHT]) {

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

}

char dirsym(){
  switch(current_direction){
    case WEST:
      return '<';
    case EAST:
      return '>';
    case SOUTH:
      return 'v';
    case NORTH:
      return '^';
  }

}

void printLabyrinth(struct field labyrinth[LAB_WIDTH][LAB_HEIGHT]){
  printf("\n");
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
        if (current_position.x == j && current_position.y == i){
          printf("  %c ", dirsym());
        }else{
          printf(" %3i", labyrinth[j][i].value);
        }
      }else{
        if (current_position.x == j && current_position.y == i){
          printf("| %c ", dirsym());
        }else{
          printf("|%3i", labyrinth[j][i].value);
        }
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

#endif /*LABYRINTH_H*/
