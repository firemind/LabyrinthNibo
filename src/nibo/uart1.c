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

#include "nibo/niboconfig.h"
#ifndef NIBO_USE_UART1
#error NIBO_USE_UART1 must be defined in niboconfig.h when this file is compiled
#endif

#include <avr/interrupt.h>

#include "nibo/uart1.h"

#ifdef __cplusplus
extern "C" {
#endif



uint8_t uart1_txbuf[UART1_TXBUF_SIZE];
uint8_t uart1_rxbuf[UART1_RXBUF_SIZE];

uint8_t uart1_txbuf_begin;
uint8_t uart1_txbuf_end;

uint8_t uart1_rxbuf_begin;
uint8_t uart1_rxbuf_end;



void uart1_set_baudrate(uint16_t baud) {
  baud = ((uint32_t)(F_CPU))/((uint32_t)baud*16) - 1;
  UBRR1H = (uint8_t)(baud>>8);
  UBRR1L = (uint8_t)(baud&0xff);
}


char uart1_getchar() {
  cli();
  uint8_t result = uart1_rxbuf[uart1_rxbuf_begin];
  if (uart1_rxbuf_end==UART1_RXBUF_SIZE) {
    uart1_rxbuf_end=uart1_rxbuf_begin;
  }
  
  if (++uart1_rxbuf_begin>=UART1_RXBUF_SIZE) {
    uart1_rxbuf_begin=0;
  }
  sei();
  return result;
}


char uart1_putchar(char c) {
  cli();
  uart1_txbuf[uart1_txbuf_end] = c;
  if (++uart1_txbuf_end>=UART1_TXBUF_SIZE) {
    uart1_txbuf_end=0;
  }
  if (uart1_txbuf_end==uart1_txbuf_begin) {
    uart1_txbuf_end=UART1_TXBUF_SIZE;
  }
  // reenable transmit register empty interrupt
  UCSR1B |= _BV(UDRIE1);
  sei();
  return c;
}


static char uart1_tx_getchar() {
  uint8_t result = uart1_txbuf[uart1_txbuf_begin];
  if (uart1_txbuf_end==UART1_TXBUF_SIZE) {
    uart1_txbuf_end=uart1_txbuf_begin;
  }
  
  if (++uart1_txbuf_begin>=UART1_TXBUF_SIZE) {
    uart1_txbuf_begin=0;
  }
  return result;
}


static char uart1_rx_putchar(char c) {
  uart1_rxbuf[uart1_rxbuf_end] = c;
  if (++uart1_rxbuf_end>=UART1_RXBUF_SIZE) {
    uart1_rxbuf_end=0;
  }
  if (uart1_rxbuf_end==uart1_rxbuf_begin) {
    uart1_rxbuf_end=UART1_RXBUF_SIZE;
  }
  return c;
}


void uart1_enable() {
  UCSR1B = _BV(RXCIE1) + _BV(UDRIE1) + _BV(RXEN1) + _BV(TXEN1);
}


void uart1_disable() {
  UCSR1B = 0;
}


ISR(USART1_RX_vect) {
  uint8_t c = UDR1;
  if (!uart1_rxfull()) {
    uart1_rx_putchar(c);
  }
}


ISR(USART1_UDRE_vect) {
  if (!uart1_txempty()) {
    UDR1 = uart1_tx_getchar();
  } else {
    UCSR1B &= ~_BV(UDRIE1);
  }
}


#ifdef __cplusplus
} // extern "C"
#endif


