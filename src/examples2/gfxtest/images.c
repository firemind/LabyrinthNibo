/*
  
*/



#include "images.h"


#include "nicai_logo.xbm"
#include "Nase.xbm"
#include "Biene.xbm"
#include "MarsRover.xbm"

uint8_t image_getWidth(uint8_t id) {
  switch (id) {
    case 0: return nicai_logo_width;
    case 1: return Nase_width;
    case 2: return Biene_width;
    case 3: return MarsRover_width;
  }
  return 0;
}

uint8_t image_getHeight(uint8_t id) {
  switch (id) {
    case 0: return nicai_logo_height;
    case 1: return Nase_height;
    case 2: return Biene_height;
    case 3: return MarsRover_height;
  }
  return 0;
}

PGM_P image_getBits(uint8_t id) {
  switch (id) {
    case 0: return nicai_logo_bits;
    case 1: return Nase_bits;
    case 2: return Biene_bits;
    case 3: return MarsRover_bits;
  }
  return 0;
}
