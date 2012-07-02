#ifndef LABYRINTH_H
#define LABYRINTH_H

#define ACTIONS 4
#define NORTH 0x01
#define SOUTH 0x02
#define WEST 0x04
#define EAST 0x08

#define LAB_WIDTH 10
#define LAB_HEIGHT 10

struct field {
  char actions;
  int value;
};

struct field labyrinth[LAB_HEIGHT][LAB_WIDTH];

#endif /*LABYRINTH_H*/
