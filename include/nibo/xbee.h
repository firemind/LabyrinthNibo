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

/*! @file    xbee.h
 *  @brief   Routinen zur Kommunikation über ein XBee Modul
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2009-02-25
 */

#ifndef NIBO_XBEE_H_
#define NIBO_XBEE_H_
#if defined(NIBO_USE_XBEE) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define XBEE_TXBUF_SIZE 40
#define XBEE_RXBUF_SIZE 10

#define XBEE_USE_UART0


/*!
 * Zu versendende Daten
 */
extern uint8_t xbee_tx_buf[XBEE_TXBUF_SIZE];


/*!
 * Empfangene Daten
 */
extern uint8_t xbee_rx_buf[XBEE_RXBUF_SIZE];


/*!
 * Sendeaddresse
 */
extern uint16_t xbee_tx_address;


/*!
 * Empfangsaddresse
 */
extern uint16_t xbee_rx_address;


/*!
 * Setzt neue Baudrate
 */
void xbee_set_baudrate(uint16_t baud);


/*!
 * Senden und empfangen aktivieren.
 */
void xbee_enable();


/*!
 * Senden und empfangen deaktivieren.
 */
void xbee_disable();


/*!
 * Datenpaket übertragen
 */
void xbee_tx_start(uint8_t size);


/*!
 * Senden fertig, neue Übertragung kann beginnen
 */
uint8_t xbee_tx_idle();


/*!
 * Empfangenes Datenpaket steht bereit
 */
uint8_t xbee_rx_avail();


/*!
 * Liefert den aktuellen Status des XBee Moduls
 */
uint8_t xbee_status();




#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_XBEE
#endif // NIBO_XBEE_H_
