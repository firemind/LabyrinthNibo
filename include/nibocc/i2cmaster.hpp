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

/*! @file    i2cmaster.hpp
 *  @brief   Routinen zur Kommunikation ueber den I2C Bus
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBOCC_I2CMASTER_HPP_
#define NIBOCC_I2CMASTER_HPP_
#if defined(NIBOCC_USE_I2CMASTER) || defined (DOXYGEN)

#include <inttypes.h>
#include "nibo/i2cmaster.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zur Kommunikation ueber den I2C Bus
 */
class I2CMaster
{
  public:

    /*!
     * Anzahl der zu sendenden / zu empfangenden Zeichen
     */
    static uint8_t get_size() { return ::i2c_size;}

    /*!
     * Puffer zur Datenübertragung
     */
    static uint8_t * get_buffer() { return ::i2c_buf; }

    /*!
     * I2C Status
     */
    enum {
      BUSY    = ::I2C_BUSY,
      IDLE    = ::I2C_IDLE,
      SUCCESS = ::I2C_SUCCESS,
      ERROR   = ::I2C_ERROR
    };

    /*!
     * Initialisierung der I2C Schnittstelle
     */
    static void init() {::i2c_init();}

    /*!
     * Start der Datenübertragung. Die Daten sind im @ref i2c_buf gespeichert
     */
    static uint8_t start_transmission() {return ::i2c_start_transmission();}

    /*!
     * Hole den aktuellen I2C-Status
     */
    static uint8_t status() { return ::i2c_status();}

    /*!
     * Warte bis Übertragung beendet wurde
     */
    static uint8_t wait_transmission() { return ::i2c_wait_transmission();}

};

template<int ADDR> class I2CAddress
{
  public:
    enum { TX = ((ADDR)<<1),
           RX = (((ADDR)<<1)+1) };
};

} // namespace
#endif // NIBOCC_USE_I2CMASTER
#endif // NIBOCC_I2CMASTER_HPP_
