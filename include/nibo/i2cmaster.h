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

/*! @file    i2cmaster.h
 *  @brief   Routinen zur Kommunikation ueber den I2C Bus
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBO_I2CMASTER_H_
#define NIBO_I2CMASTER_H_
#if defined(NIBO_USE_I2CMASTER) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef NIBO_I2C_TXRX

/*!
 * Anzahl der zu sendenden Zeichen
 */
extern volatile uint8_t i2c_tx_size;

/*!
 * Anzahl der zu empfangenden Zeichen
 */
extern volatile uint8_t i2c_rx_size;

#else

/*!
 * Anzahl der zu sendenden / zu empfangenden Zeichen
 */
extern volatile uint8_t i2c_size;

#endif


/*!
 * Puffer zur Datenübertragung
 */
extern uint8_t i2c_buf[I2C_BUF_SIZE];

/*!
 * I2C Status
 */
enum {
  I2C_BUSY,
  I2C_IDLE,
  I2C_SUCCESS,
  I2C_ERROR=0x10
};

/*!
 * Initialisierung der I2C Schnittstelle
 */
void i2c_init(void);

/*!
 * Start der Datenübertragung. Die Daten sind im @ref i2c_buf gespeichert
 */
uint8_t i2c_start_transmission(void);

/*!
 * Hole den aktuellen I2C-Status
 */
uint8_t i2c_status(void);

extern volatile uint8_t i2c_last_TWSR;
extern volatile uint8_t i2c_pos;

/*!
 * Warte bis Übertragung beendet wurde
 */
uint8_t i2c_wait_transmission(void);

#define I2C_TX(ADDR) ((ADDR)<<1)
#define I2C_RX(ADDR) (((ADDR)<<1)+1)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_I2CMASTER
#endif // NIBO_I2CMASTER_H_
