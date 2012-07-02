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

/*! @file    display.hpp
 *  @brief   Low-Level Routinen zum Ansprechen eines Displays
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBOCC_DISPLAY_HPP_
#define NIBOCC_DISPLAY_HPP_
#if defined(NIBOCC_USE_DISPLAY) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/display.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zum Ansprechen eines Displays
 */
class Display
{
  public:

    /*!
     * Initialisierung des Displays
     */
    static void init() {::display_init();}

    /*!
     * Überträgt Daten / Kommando zum Display
     * @param mode Auswahl des Controllers und Umschaltung zuwischen Kommando und Daten
     * @param data Zu übertragende Daten / Kommando
     */
    static void write(uint8_t mode, uint8_t data) {::display_write(mode, data);}

    /*!
     * Liest Daten / Status vom Display
     * @param mode Auswahl des Controllers und Umschaltung zuwischen Kommando und Daten
     * @return Übertragende Daten / Status
     */
    static uint8_t read(uint8_t mode) {return ::display_read(mode);}

    /*!
     * Warten bis das Display empfangsbereit ist
     */
    static void display_wait(uint8_t mode) {::display_wait(mode);}

};

} // namespace
#endif // NIBOCC_USE_DISPLAY
#endif // NIBOCC_DISPLAY_HPP_
