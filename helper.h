#ifndef HELPER_H
#define HELPER_H

#define NORTH 0x01
#define SOUTH 0x02
#define WEST 0x04
#define EAST 0x08

struct coord {
  int x;
  int y;
};

char current_direction = SOUTH;
struct coord current_position = {0,0};

#endif /*HELPER_H*/
