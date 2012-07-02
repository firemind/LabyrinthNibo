/*
Dieses Programm testet die Motoransteuerung. Zus�tzlich wird 
noch die Versorgungsspannung und das zuletzt empfangene
RC5-Kommando angezeigt.
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


int main()
{
  sei(); // enable interrupts
  bot_init();
  spi_init();
  display_init();                       /* Displayansteuerung initialisieren */
  gfx_init();                           /* Grafikdisplay initialisieren */

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

  while (1)
  {
    // IR Controller
    delay(10);

    char text[20]="RC5: ----";
    if (copro_update())
    {
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

    switch(++counter)
    {
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
  return 0;                             /* Fertig.... */
}
