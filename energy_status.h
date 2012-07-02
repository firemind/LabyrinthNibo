#ifndef ENERGY_STATUS_H
#define ENERGY_STATUS_H

float volt_flt = 10.0;

void energy_status_check() {
  // Spannung
  bot_update();
  volt_flt = 0.9 * volt_flt + 0.00160 * bot_supply;

  if (volt_flt<8.0) {
    gfx_fill(0x00);
    gfx_move(25, 20);
    gfx_set_proportional(1);
    gfx_print_text("Please recharge");
    gfx_move(35, 30);
    gfx_print_text("batteries!");
    gfx_set_proportional(0);
    while(1) {
      leds_set_headlights(0);
      clear_output_bit(IO_RESET_CO);
      //motco_stop();
      leds_set_displaylight(500);
      IO_LEDS_RED_PORT = 0xaa;
      IO_LEDS_GREEN_PORT = 0;
      delay(500);
      //motco_setSpeed(0, 0);
      leds_set_displaylight(0);
      IO_LEDS_RED_PORT = 0x55;
      IO_LEDS_GREEN_PORT = 0;
      delay(500);
    }
  }
}

#endif /*ENERGY_STATUS_H*/
