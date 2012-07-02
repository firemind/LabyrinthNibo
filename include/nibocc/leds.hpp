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
 *  @brief   Routinen zur Ansteuerung der LEDs
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBOCC_LEDS_HPP_
#define NIBOCC_LEDS_HPP_
#if defined(NIBOCC_USE_LEDS) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/leds.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zur Ansteuerung der LEDs
 */
class Leds
{
  public:

    enum {
      OFF    = ::LEDS_OFF,
      GREEN  = ::LEDS_GREEN,
      RED    = ::LEDS_RED,
      ORANGE = ::LEDS_ORANGE
    };


    /*!
     * LEDs initialisieren
     */
    static void init() { ::leds_init(); }

    /*!
     * Status-LED setzen
     * @param color Farbe der LED
     * @param led ID der LED
     */
    static void set_status(uint8_t color, uint8_t led) { ::leds_set_status(color, led); }

    /*!
     * Wert für Scheinwerferbeleuchtung setzen
     * @param light Helligkeit (0..1024)
     */
    static void set_headlights(int light) { ::leds_set_headlights(light); }

    /*!
     * Holt Wert der Scheinwerferbeleuchtung
     * @return Helligkeit (0..1024)
     */
    static int get_headlights() { return ::leds_get_headlights(); }

    /*!
     * Wert für Displaybeleuchtung setzen
     * @param light Helligkeit (0..1024)
     */
    static void set_displaylight(int light) { ::leds_set_displaylight(light); }

    /*!
     * Holt Wert der Displaybeleuchtung
     * @return Helligkeit (0..1024)
     */
    static int leds_get_displaylight() { return ::leds_get_displaylight(); }

};

} // namespace
#endif // NIBOCC_USE_LEDS
#endif // NIBOCC_LEDS_HPP_
