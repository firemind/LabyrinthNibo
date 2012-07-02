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

/*! @file    bot.hpp
 *  @brief   Routinen fuer allgemeine Funktionen des Roboters
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-14
 */

#ifndef NIBOCC_BOT_HPP_
#define NIBOCC_BOT_HPP_
#if defined(NIBOCC_USE_BOT) || defined (DOXYGEN)

#include <inttypes.h>
#include "nibo/bot.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse fuer allgemeine Funktionen des Roboters
 */
class Bot
{
  public:
  
    /*!
     * Versorgungsspannung des Roboters. Angabe unkorrigiert,
     * als Wert zwischen 0 und 1023
     * Grobe Umrechnung: Ubatt = 0.0166 * bot_supply - 1.19;
     */
    static uint16_t get_supply() { return ::bot_supply; }

    /*!
     * Roboter initialisieren, externe Kontroller zurücksetzen, etc...
     */
    static void init() { ::bot_init(); }

    /*!
     * Werte aktualisieren
     */
    static void update() {::bot_update();}

};

} // namespace
#endif // NIBOCC_USE_BOT
#endif // NIBOCC_BOT_HPP_
