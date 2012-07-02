void make_sound(int interval, int time) {
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


void play_music() {
  cli();
  SPCR = 0;
  set_output_bit(IO_DISPLAY_RST);
  activate_output_bit(IO_DISPLAY_RST);
  clear_output_bit(IO_RESET_CO);
  
  activate_output_bit(IO_AUDIO);
  make_sound(127/4, 15000);
  delay_ms(10);
  make_sound(191/4, 15000);
  delay_ms(10);
  make_sound(255/4, 10000);
  delay_ms(10);
  make_sound(255/4, 10000);
  delay_ms(10);
  make_sound(191/4, 15000);
  delay_ms(1);
  
  sei();
  bot_init();
  deactivate_output_bit(IO_DISPLAY_RST);
  clear_output_bit(IO_DISPLAY_RST);
  spi_init();
}
