#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <nibo/floor.h>
#include <stdio.h>

int main() {
  bot_init();
  floor_init();
  display_init();
  gfx_init();

  gfx_move(22, 0);
  gfx_set_proportional(1);
  gfx_print_text("Floor sensor test");
  gfx_set_proportional(0);
  gfx_move(5, 10);
  gfx_print_char('R');
  gfx_move(118, 10);
  gfx_print_char('L');

  while (1==1) {
    delay(10);
    char text[20]="--  --  --  --  --";

    // Bodensensoren
    floor_update();
    sprintf(text, "%02x  %02x  %02x  %02x",
        (uint16_t)(floor_absolute[FLOOR_RIGHT]/8),
        (uint16_t)(floor_absolute[LINE_RIGHT]/8),
        (uint16_t)(floor_absolute[LINE_LEFT]/8),
        (uint16_t)(floor_absolute[FLOOR_LEFT]/8));
    gfx_move(22, 30);
    gfx_print_text(text);
    
    sprintf(text, "%02x  %02x  %02x  %02x",
        (uint16_t)(floor_relative[FLOOR_RIGHT]/8),
        (uint16_t)(floor_relative[LINE_RIGHT]/8),
        (uint16_t)(floor_relative[LINE_LEFT]/8),
        (uint16_t)(floor_relative[FLOOR_LEFT]/8));
    gfx_move(22, 40);
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
