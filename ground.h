#ifndef GROUND_H
#define GROUND_H

#define THRESHOLD 150

bool line_left(void){
  return floor_relative[LINE_LEFT]<THRESHOLD;
}

bool line_right(){
  return floor_relative[LINE_RIGHT]<THRESHOLD;
}

bool floor_left(void){
  return floor_relative[FLOOR_LEFT]<THRESHOLD;
}

bool floor_right(){
  return floor_relative[FLOOR_RIGHT]<THRESHOLD;
}

bool groundBlack(){
    floor_update();
    return (line_right() && line_left());
}
#endif /*GROUND_H*/

