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
#ifndef NIBO_USE_BOT
#error NIBO_USE_BOT must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_DELAY
#error NIBO_USE_DELAY must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_ADC
#error NIBO_USE_ADC must be defined in niboconfig.h when this file is compiled
#endif

#include "nibo/delay.h"
#include "nibo/iodefs.h"
#include "nibo/adc.h"
#include "nibo/bot.h"

#ifdef __cplusplus
extern "C" {
#endif

const char bot_lib_revision[] PROGMEM = SVN_REVISION;
const char bot_lib_version[] PROGMEM = VERSION;

uint16_t bot_supply;

void bot_init(void) {
  // Reset controllers
#ifdef _NIBO_2_
  activate_output_bit(IO_RESET_CO);
  clear_output_bit(IO_RESET_CO);
  delay(10);
  set_output_bit(IO_RESET_CO);
  delay(100);
#else
  activate_output_bit(IO_RESET_5);
  activate_output_bit(IO_RESET_3);
  clear_output_bit(IO_RESET_5);
  clear_output_bit(IO_RESET_3);
  delay(10);
  set_output_bit(IO_RESET_5);
  set_output_bit(IO_RESET_3);
  delay(100);
#endif
}

void bot_update() {
  bot_supply = adc_read(AN_VBAT);
}

#ifdef __cplusplus
} // extern "C"
#endif
