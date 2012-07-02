#include "nibo/iodefs.h"

//Blinks all LEDs clockwise with a 500ms delay
void blink(void){

  IO_LEDS_RED_PORT = 0x80;
  IO_LEDS_GREEN_PORT = 0x80;
  _delay_ms(500);

  IO_LEDS_RED_PORT = 0x40;
  IO_LEDS_GREEN_PORT = 0x40;
  _delay_ms(500);
 
  leds_set_headlights(1024);

  IO_LEDS_RED_PORT = 0x20;
  IO_LEDS_GREEN_PORT = 0x20;
  _delay_ms(500);
 
  IO_LEDS_RED_PORT = 0x10;
  IO_LEDS_GREEN_PORT = 0x10;
  _delay_ms(500);
 
  leds_set_headlights(0);

  IO_LEDS_RED_PORT = 0x08;
  IO_LEDS_GREEN_PORT = 0x08;
  _delay_ms(500); 

  IO_LEDS_RED_PORT = 0x04;
  IO_LEDS_GREEN_PORT = 0x04;
  _delay_ms(500);
  
  IO_LEDS_RED_PORT = 0x02;
  IO_LEDS_GREEN_PORT = 0x02;
  _delay_ms(500);
 
  IO_LEDS_RED_PORT = 0x01;
  IO_LEDS_GREEN_PORT = 0x01;
  _delay_ms(500);

  IO_LEDS_RED_PORT = 0x00;
  IO_LEDS_GREEN_PORT = 0x00;
}
