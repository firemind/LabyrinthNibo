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
#include <nibo/pwm.h>                  /* stellt die leds_xxx Routinen zur
                                           Verfügung */

#include <avr/interrupt.h>

int main()
{
  leds_init();                          /* IO-Ports der LEDs als Ausgänge
                                           konfigurieren */
  pwm_init();

  while(1==1)                           /* Endlosschleife (1==1 ist immer wahr!)
                                        */
  {
    sei();
    
    delay(1000);                         /* Eine halbe Sekunde warten */

    leds_set_status(LEDS_RED, 0);       /* Status LED 0 auf rot setzen */
    leds_set_status(LEDS_RED, 4);       /* Status LED 4 auf rot setzen */

    delay(1000);                         /* Eine halbe Sekunde warten */
    
    leds_set_status_intensity(20);
    
    delay(1000);                         /* Eine halbe Sekunde warten */

    leds_set_status(LEDS_GREEN, 0);       /* Status LED 0 auf rot setzen */
    leds_set_status(LEDS_GREEN, 4);       /* Status LED 4 auf rot setzen */

    delay(1000);                         /* Eine halbe Sekunde warten */
    
    leds_set_status_intensity(1023);
  }
  return 0;                             /* Diese Zeile ist eigentlich nicht
                                           notwendig, da das Programm sie wegen
                                           der Endlosschleife nie erreichen
                                           wird, der Compiler gibt jedoch
                                           ansonsten eine Warnung aus. */
}
