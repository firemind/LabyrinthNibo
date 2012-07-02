/*
  Dieses Testprogramm läßt beim Nibo eine Status-LED abwechselnd rot und grün
  blinken.
*/


#include <nibo/niboconfig.h>            /* muss immer als erste Datei
                                           eingebunden werden */
#include <nibo/leds.h>                  /* stellt die leds_xxx Routinen zur
                                           Verfügung */
#include <nibo/delay.h>                 /* stellt die delay Routine zur
                                           Verfügung */

int main()
{
  leds_init();                          /* IO-Ports der LEDs als Ausgänge
                                           konfigurieren */

  while(1==1)                           /* Endlosschleife (1==1 ist immer wahr!)
                                        */
  {
    delay(500);                         /* Eine halbe Sekunde warten */
    leds_set_status(LEDS_RED, 0);       /* Status LED 0 auf rot setzen */
    delay(500);                         /* Eine halbe Sekunde warten */
    leds_set_status(LEDS_GREEN, 0);     /* Status LED 0 auf grün setzen */
  }
  return 0;                             /* Diese Zeile ist eigentlich nicht
                                           notwendig, da das Programm sie wegen
                                           der Endlosschleife nie erreichen
                                           wird, der Compiler gibt jedoch
                                           ansonsten eine Warnung aus. */
}
