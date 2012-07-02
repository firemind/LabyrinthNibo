#include <nibo/niboconfig.h>
#include <nibo/leds.h>
#include <nibo/delay.h>

int main() {
  leds_init();
  while (1==1) {
    int farbe;
    for (farbe=0; farbe<4; farbe++) {
      int ledNr;
      for (ledNr=0; ledNr<8; ledNr++) {
        leds_set_status(farbe, ledNr);
        delay(150);
      }
    }
  }
  return 0;
}
