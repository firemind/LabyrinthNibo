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
#ifndef NIBO_USE_ADC
#error NIBO_USE_ADC must be defined in niboconfig.h when this file is compiled
#endif

#include "nibo/iodefs.h"
#include "nibo/adc.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t adc_random_seed;

void adc_init(uint8_t channel){
  DDRF &= ~((uint8_t)1 << channel);  // Pin als input
  PORTF &= ~((uint8_t)1 << channel); // Pullup aus.
}

uint16_t adc_read(uint8_t channel){
  uint16_t result = 0x00;

  // Reference voltage: AVCC (5V)
  ADMUX= _BV(REFS0);

  ADMUX |= (channel & 0x07);  // Und jetzt Kanal waehlen, nur single ended

  ADCSRA = _BV(ADPS2)  // prescale faktor = 128 ADC laeuft
         | _BV(ADPS1)  // mit 14,7456MHz/ 128 = 115,2kHz
         | _BV(ADPS0)
         | _BV(ADEN)   // ADC an
         | _BV(ADSC);  // Beginne mit der Konvertierung

  // Warten bis konvertierung beendet, das sollte 25 ADC-Zyklen dauern,
  // also 1/4608 s
  while ( (ADCSRA & _BV(ADSC)) != 0) {
    __asm volatile("nop");
  }

  // Ergebnis zusammenbauen
  result= ADCL;
  result+=(ADCH<<8);

  // Entropiequelle aktualisieren
  adc_random_seed+=result;

  return result;
}

uint16_t adc_get_random_seed()
{
  return adc_random_seed;
}


#ifdef __cplusplus
} // extern "C"
#endif
