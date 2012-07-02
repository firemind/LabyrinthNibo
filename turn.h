//turns right 90 degrees blinks right led and stops
void turn_right(){
  IO_LEDS_GREEN_PORT = 0x80;
  copro_setSpeed(20,-20);
  _delay_ms(750);
  copro_stopImmediate();
  IO_LEDS_GREEN_PORT = 0x00;
}

//turns left 90 degrees blinks left led and stops
void turn_left(){
  IO_LEDS_GREEN_PORT = 0x04;
  copro_setSpeed(-20,20);
  _delay_ms(550);
  copro_stopImmediate();
  IO_LEDS_GREEN_PORT = 0x00;
}
