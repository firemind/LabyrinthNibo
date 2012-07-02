#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/copro.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <avr/interrupt.h>
#include <nibo/spi.h>
#include <stdio.h>


int main() {
  sei();
  bot_init();
  spi_init();
  display_init();
  gfx_init();

  gfx_move(62, 0);
  gfx_set_proportional(1);
  gfx_print_text("motion");
  gfx_set_proportional(0);

  gfx_move(5, 0);
  gfx_print_char('R');

  gfx_move(118, 0);
  gfx_print_char('L');

  delay(50);
  copro_ir_startMeasure();
  copro_setSpeedParameters(5, 6, 7);

  int counter=0;

  while (1==1) {
    delay(10);
    char text[20]="";

    bot_update();
    float volt = 0.0166 * bot_supply - 1.19;
    sprintf(text, "%3.1fV", (double)volt);
    gfx_move(25, 0);
    gfx_print_text(text);

    switch(++counter) {
      case 200:
        copro_setSpeed(20, 20);
        break;

      case 400:
        copro_stop();
        break;

      case 600:
        copro_setSpeed(-20, -20);
        break;

      case 800:
        copro_stop();
        counter=0;
        break;
    }
  }
  return 0;
}
