/*
Dieses Programm zeigt das zuletzt empfangene RC5-Kommando an. Wenn der Taster S3 gedrückt wird sendet es einen RC5 Code,
der verwendete Code wird bei jedem Senden um eins erhöht (0x3500, 0x3501, 0x3502, ...)
*/

#include <nibo/niboconfig.h>            /* muss immer als erste Datei
                                           eingebunden werden */
#include <nibo/display.h>               /* stellt die grundlegenden Routinen
                                           zur Displayansteuerung zur
                                           Verfügung */
#include <nibo/gfx.h>                   /* stellt die Routinen zur Ansteuerung
                                           eines Grafikdisplays zur Verfügung */
#include <nibo/copro.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <avr/interrupt.h>
#include <nibo/i2cmaster.h>
#include <stdio.h>


int main() {
  sei(); // enable interrupts
  bot_init();
  spi_init();
  display_init();                       /* Displayansteuerung initialisieren */
  gfx_init();                           /* Grafikdisplay initialisieren */

  gfx_move(62, 0);
  gfx_set_proportional(1);
  gfx_print_text("rc5test");
  gfx_set_proportional(0);

  delay(50);

  uint16_t code = 0x3500;

  while (1) {
    if (!get_input_bit(IO_INPUT_1)) {
      delay(20);
      while (!get_input_bit(IO_INPUT_1)) {
        delay(20);
      }
      copro_transmitRC5(code++);
    }
    
    // IR Controller
    delay(10);

    char text[20]="RC5: ----";
    if (copro_update()) {
      sprintf(text, "RC5: %04x", (int)copro_rc5_cmd);
    }
    gfx_move(35, 45);
    gfx_print_text(text);

    // Spannung
    bot_update();
    float volt = 0.0166 * bot_supply - 1.19;
    sprintf(text, "%2.1fV ", (double)volt);
    gfx_move(25, 0);
    gfx_print_text(text);

  }
  return 0;
}
