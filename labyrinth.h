#ifndef LABYRINTH_H
#define LABYRINTH_H

#define NORTH 0x01
#define SOUTH 0x02
#define WEST 0x04
#define EAST 0x08

#define LAB_WIDTH 8
#define LAB_HEIGHT 3

struct field {
  char actions;
  int value;
} labyrinth[LAB_WIDTH][LAB_HEIGHT];

struct position {
  int x;
  int y;
};

#endif /*LABYRINTH_H*/
