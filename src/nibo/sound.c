
#include "nibo/niboconfig.h"
#ifndef NIBO_USE_SOUND
#error NIBO_USE_SOUND must be defined in niboconfig.h when this file is compiled
#endif

#include "nibo/sound.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"

void sound_init() {
  activate_output_bit(IO_AUDIO);
}


void sound_tone(int interval, int time) {
  while (time) {
    int i=interval;
    clear_output_bit(IO_AUDIO);
    while (time && i) {
      time--;
      i--;
      delay_us(10);
    }
    i=interval;
    set_output_bit(IO_AUDIO);
    while (time && i) {
      time--;
      i--;
      delay_us(10);
    }

  }
}







