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

/*! @file    uart1.h
 *  @brief   Routinen zur Kommunikation über die zweite serielle Schnittstelle
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2008-07-30
 */

#ifndef NIBO_UART1_H_
#define NIBO_UART1_H_
#if defined(NIBO_USE_UART1) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define UART1_TXBUF_SIZE 8
#define UART1_RXBUF_SIZE 8

/*!
 * Setzt neue Baudrate
 */
void uart1_set_baudrate(uint16_t baud);

extern uint8_t uart1_txbuf[UART1_TXBUF_SIZE];
extern uint8_t uart1_rxbuf[UART1_RXBUF_SIZE];

extern uint8_t uart1_txbuf_begin;
extern uint8_t uart1_txbuf_end;

extern uint8_t uart1_rxbuf_begin;
extern uint8_t uart1_rxbuf_end;

/*!
 * Senden und empfangen aktivieren.
 */
void uart1_enable();

/*!
 * Senden und empfangen deaktivieren.
 */
void uart1_disable();


/*!
 * Gibt ein einzelnes Zeichen aus dem Empfangspuffer zurück.
 * Darf nur aufgerufen werden wenn (uart1_rxempty()==false) ist.
 */
char uart1_getchar();


/*!
 * Schreibt ein einzelnes Zeichen in den Sendepuffer.
 * Darf nur aufgerufen werden solange (uart1_txfull()==false) ist.
 */
char uart1_putchar(char c);


/*!
 * Liefert true wenn der Empfangspuffer leer ist.
 */
static inline char uart1_rxempty() {
  return uart1_rxbuf_begin==uart1_rxbuf_end;
}


/*!
 * Liefert true wenn der Sendepuffer leer ist.
 */
static inline char uart1_txempty() {
  return uart1_txbuf_begin==uart1_txbuf_end;
}


/*!
 * Liefert true wenn der Empfangspuffer voll ist.
 */
static inline char uart1_rxfull() {
  return uart1_rxbuf_end==UART1_RXBUF_SIZE;
}


/*!
 * Liefert true wenn der Sendepuffer voll ist.
 */
static inline char uart1_txfull() {
  return uart1_txbuf_end==UART1_TXBUF_SIZE;
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_UART1
#endif // NIBO_UART1_H_
