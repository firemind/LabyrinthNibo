#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/copro.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <nibo/spi.h>
#include <avr/interrupt.h>
#include <stdio.h>

int main() {
  sei();
  bot_init();
  spi_init();
  display_init();
  gfx_init();

  gfx_move(15, 0);
  gfx_set_proportional(1);
  gfx_print_text("Distance sensor test");
  gfx_set_proportional(0);
  gfx_move(5, 10);
  gfx_print_char('R');
  gfx_move(118, 10);
  gfx_print_char('L');

  delay(50);
  copro_ir_startMeasure();

  while (1==1) {
    delay(10);
    char text[20]="--  --  --  --  --";

    // Co-Prozessor
    if (copro_update()) {
      sprintf(text, "%02x  %02x  %02x  %02x  %02x",
          (uint16_t)copro_distance[0]/256,
          (uint16_t)copro_distance[1]/256,
          (uint16_t)copro_distance[2]/256,
          (uint16_t)copro_distance[3]/256,
          (uint16_t)copro_distance[4]/256);
    }
    gfx_move(10, 55);
    gfx_print_text(text);

    // Spannung
    bot_update();
    float volt = 0.0166 * bot_supply - 1.19;
    sprintf(text, "%3.1fV", (double)volt);
    gfx_move(30, 10);
    gfx_set_proportional(1);
    gfx_print_text("supply: ");
    gfx_set_proportional(0);
    gfx_print_text(text);

  }

  return 0;
}
