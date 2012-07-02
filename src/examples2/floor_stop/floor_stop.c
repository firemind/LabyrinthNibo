/*
  Dieses Programm fährt auf einer weissen Fläche bis zu einem Abgrund und
  hält dann an. Damit das Programm funktioniert müssen die Bodensensoren 
  kalibriert sein! Man sollte natürlich trotzdem aufpassen, das der NIBO
  nicht runterfällt - VERWENDUNG AUF EIGENE GEFAHR!
*/

#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/copro.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <nibo/spi.h>
#include <nibo/leds.h>
#include <nibo/floor.h>
#include <avr/interrupt.h>
#include <nibo/i2cmaster.h>
#include <stdio.h>

uint16_t time;

#define THRESHOLD 150

int main() {
  sei(); // enable interrupts
  bot_init();
  spi_init();
  display_init();                       /* Displayansteuerung initialisieren */
  gfx_init();                           /* Grafikdisplay initialisieren */
  leds_init();
  floor_init();
  
  gfx_move(62, 0);
  gfx_set_proportional(1);
  gfx_print_text("floor stop");
  gfx_set_proportional(0);

  delay(50);
  copro_ir_startMeasure();
  copro_setSpeedParameters(5, 6, 7);

  copro_update();  
  uint8_t go = 0;
  
  while (1) {
    sei();
    _delay_ms(1);
    floor_update();
    if (go==0) {
      if ((floor_relative[FLOOR_LEFT]>THRESHOLD) && (floor_relative[FLOOR_RIGHT]>THRESHOLD)) {
        // Boden links und rechts vorhanden -> losfahren!
        go=1;
        copro_setSpeed(40, 40);
        IO_LEDS_RED_PORT=0x00;
        IO_LEDS_GREEN_PORT=0x84;
      }
    } else {
      if ((floor_relative[FLOOR_LEFT]<THRESHOLD) || (floor_relative[FLOOR_RIGHT]<THRESHOLD)) {
        // Boden links oder rechts verloren -> sofort stoppen!
        go=0;
        copro_stopImmediate();
        IO_LEDS_RED_PORT=0x84;
        IO_LEDS_GREEN_PORT=0x00;
      }
    }
    
  }
  return 0;
}
