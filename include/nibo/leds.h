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

/*! @file    leds.h
 *  @brief   Routinen zur Ansteuerung der LEDs.
 *
 *  Vor Aufruf einer Funktion muss die Funktion @ref leds_init() ausgeführt
 *  werden. Um die Displaybeleuchtung und die Scheinwerfer anzusteuern muss
 *  zusätzlich das PWM (Pulsweitenmodulation)-Modul durch Aufruf der Funktion
 *  @ref pwm_init() initialisiert werden und die Interrupts durch Aufruf der
 *  Funktion @ref sei() aktiviert werden.
 */

#ifndef NIBO_LEDS_H_
#define NIBO_LEDS_H_
#if defined(NIBO_USE_LEDS) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Farben der LED
 */
enum {
  LEDS_OFF    = 0,
  LEDS_GREEN  = 1,
  LEDS_RED    = 2,
  LEDS_ORANGE = 3
};

/*!
 * LEDs initialisieren
 */
void leds_init(void);

/*!
 * Status-LED setzen
 * Vor dem ersten Aufruf der Funktion muss @ref leds_init() aufgerufen werden
 * @param color Farbe der LED
 * @param led ID der LED
 */
void leds_set_status(uint8_t color, uint8_t led);

#ifdef _NIBO_2_
/*!
 * Wert für die Statusbeleuchtung (rot/grüne LEDs) setzen
 * Vor dem ersten Aufruf der Funktion muss @ref pwm_init() aufgerufen werden und
 * die Interrupts müssen mit @ref sei() aktiviert sein.
 * @param light Helligkeit (0..1024)
 */
void leds_set_status_intensity(int light);

/*!
 * Holt Wert der Statusbeleuchtung (rot/grüne LEDs)
 * @return Helligkeit (0..1024)
 */
int leds_get_status_intensity();
#endif

/*!
 * Wert für Scheinwerferbeleuchtung setzen
 * Vor dem ersten Aufruf der Funktion muss @ref pwm_init() aufgerufen werden und
 * die Interrupts müssen mit @ref sei() aktiviert sein.
 * @param light Helligkeit (0..1024)
 */
void leds_set_headlights(int light);

/*!
 * Holt Wert der Scheinwerferbeleuchtung
 * @return Helligkeit (0..1024)
 */
int leds_get_headlights();

/*!
 * Wert für Displaybeleuchtung setzen
 * Vor dem ersten Aufruf der Funktion muss @ref pwm_init() aufgerufen werden und
 * die Interrupts müssen mit @ref sei() aktiviert sein.
 * @param light Helligkeit (0..1024)
 */
void leds_set_displaylight(int light);

/*!
 * Holt Wert der Displaybeleuchtung
 * @return Helligkeit (0..1024)
 */
int leds_get_displaylight();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_LEDS
#endif // NIBO_LEDS_H_
