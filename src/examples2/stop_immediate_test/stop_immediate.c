/*
  Dieses Programm testet die copro_stopImmediate() Funktion
  Der Roboter fährt ein stück vor und bremst, danach fährt er zurück und bremst.
*/

#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/copro.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <nibo/spi.h>
#include <avr/interrupt.h>
#include <nibo/i2cmaster.h>
#include <stdio.h>

uint16_t time;

int main() {
  sei(); // enable interrupts
  bot_init();
  spi_init();
  display_init();                       /* Displayansteuerung initialisieren */
  gfx_init();                           /* Grafikdisplay initialisieren */
  leds_init();

  gfx_move(62, 0);
  gfx_set_proportional(1);
  gfx_print_text("stopImmediate()");
  gfx_set_proportional(0);

  delay(50);
  copro_ir_startMeasure();
  copro_setSpeedParameters(5, 6, 7);

  copro_update();  
  
  while (1) {
    time++;
    
    switch (time) {
      case 100: copro_setSpeed(60, 60); IO_LEDS_RED_PORT=0x84; break;
      case 200: copro_stopImmediate(); break;
      case 300: copro_setSpeed(-60, -60); break;
      case 400: copro_stopImmediate(); break;
      case 500: copro_setSpeed(60, 60); IO_LEDS_RED_PORT=0x00; break;
      case 600: copro_stop(); break;
      case 700: copro_setSpeed(-60, -60); break;
      case 800: copro_stop(); time=0; break;    
    }
    delay(10);    
    
  }
  return 0;                             /* Fertig.... */
}
