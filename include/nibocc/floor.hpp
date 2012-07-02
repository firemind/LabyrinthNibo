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

/*! @file    floor.hpp
 *  @brief   Routinen zum Auslesen der Werte der Bodensensoren
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBOCC_FLOOR_HPP_
#define NIBOCC_FLOOR_HPP_
#if defined(NIBOCC_USE_FLOOR) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/floor.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zum Auslesen der Werte der Bodensensoren
 */
class Floor
{
  public:
    
    enum {
      LINE_LEFT     = ::LINE_LEFT,
      LINE_RIGHT    = ::LINE_RIGHT,
      FLOOR_LEFT    = ::FLOOR_LEFT,
      FLOOR_RIGHT   = ::FLOOR_RIGHT
    };


    /*!
     * Floor IR-Licht einschalten
     */
    static void enable_ir() {floor_enable_ir();}

    /*!
     * Floor IR-Licht ausschalten
     */
    static void disable_ir() {floor_disable_ir();}

    /*!
     * Verzögerung der Sensoren, nach ein/ausschalten des IR-Lichts
     */
    static const int SENS_DALAY_US = FLOOR_SENS_DALAY_US;

    /*!
     * Wert linker äußerer Sensor
     */
    static uint16_t getAbsolute(uint8_t index) {return ::floor_absolute[index];}

    /*!
     * Wert linker äußerer Sensor
     */
    static uint16_t getRelative(uint8_t index) {return ::floor_relative[index];}

    /*!
     * Bodensensoren initialisieren
     */
    static void init() {::floor_init();}

    /*!
     * Messung durchführen und Ergebnisse normalisieren
     */
    static void update() {::floor_update();}

    /*!
     * Bodensensoren werden auf schwarzer Fläche kalibriert
     */
    static void calibrateBlack() {::floor_calibrateBlack();}

    /*!
     * Bodensensoren werden auf weisser Fläche kalibriert
     */
    static void calibrateWhite() {::floor_calibrateWhite();}

    /*!
     * Kalibrierdaten aus EEPROM lesen
     */
    static void readPersistent() {::floor_readPersistent();}

    /*!
     * Kalibrierdaten ins EEPROM schreiben
     */
    static void writePersistent() {::floor_writePersistent();}

};

} // namespace
#endif // NIBO_USE_FLOOR
#endif // NIBO_FLOOR_H_
