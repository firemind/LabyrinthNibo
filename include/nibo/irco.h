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

/*! @file    irco.h
 *  @brief   Routinen zum Ansprechen des Infrarot Mikrocontrollers
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBO_IRCO_H_
#define NIBO_IRCO_H_
#if defined(NIBO_USE_IRCO) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  IRCO_PASSIVE = 0,
  IRCO_RC5TX   = 1,
  IRCO_MEASURE = 2,
};

/*!
 * Aktueller Modus des IR-Controllers
 */
extern uint8_t irco_mode;

/*!
 * Aktuelle Reflexionswerte der Distanzsensoren
 */
extern uint8_t irco_distance[5];

/*!
 * Zuletzt empfangenes RC5 Kommando
 */
extern uint16_t irco_rc5_cmd;

/*!
 * Daten des IRCO aktualisieren
 */
uint8_t irco_update();

/*!
 * Aussenden von IR-Licht stoppen, nur Empfang
 */
uint8_t irco_stop();

/*!
 * Reflexionsmessung starten
 */
uint8_t irco_startMeasure();

/*!
 * RC5 Code übertragen
 * @param rc5 RC5 Code
 */
uint8_t irco_transmitRC5(uint16_t rc5);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_IRCO
#endif // NIBO_IRCO_H_
