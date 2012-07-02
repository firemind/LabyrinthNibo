/*  BSD-License:

Copyright (c) 2007 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "nibo/niboconfig.h"
#ifndef NIBO_USE_LEDS
#error NIBO_USE_LEDS must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_PWM
#error NIBO_USE_PWM must be defined in niboconfig.h when this file is compiled
#endif


#include "nibo/iodefs.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

void leds_init(void) {
#ifdef _NIBO_2_
  activate_output_bit(IO_LED_RG);
  set_output_bit(IO_LED_RG);
#endif
  activate_output_bit(IO_LED_WHITE);
  activate_output_bit(IO_DISP_LIGHT);
  IO_LEDS_RED_DDR = IO_LEDS_RED_MASK;
  IO_LEDS_GREEN_DDR = IO_LEDS_GREEN_MASK;
}

void leds_set_status(uint8_t color, uint8_t led) {
#ifdef _NIBO_2_
# define LEDBASEVAL 0x01
#else
# define LEDBASEVAL 0x04
#endif
  if (color&0x01)
    IO_LEDS_GREEN_PORT |= LEDBASEVAL<<led;
  else
    IO_LEDS_GREEN_PORT &= ~(LEDBASEVAL<<led);
    
  if (color&0x02)
    IO_LEDS_RED_PORT |= LEDBASEVAL<<led;
  else
    IO_LEDS_RED_PORT &= ~(LEDBASEVAL<<led);
}

#ifdef _NIBO_2_
void leds_set_status_intensity(int light) {
    pwm_channel_C = light;
}

int leds_get_status_intensity() {
  return pwm_channel_C;
}
#endif


void leds_set_headlights(int light) {
    pwm_channel_A = light;
}

int leds_get_headlights() {
  return pwm_channel_A;
}

void leds_set_displaylight(int light) {
    pwm_channel_B = light;
}

int leds_get_displaylight() {
  return pwm_channel_B;
}

#ifdef __cplusplus
} // extern "C"
#endif
