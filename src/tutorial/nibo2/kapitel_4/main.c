#include <nibo/niboconfig.h>
#include <nibo/leds.h>
#include <nibo/delay.h>

int main() {
  leds_init();
  while (1==1) {
    delay(500);
    leds_set_status(LEDS_RED, 0);
    delay(500);
    leds_set_status(LEDS_GREEN, 0);
  }
  return 0;
}
