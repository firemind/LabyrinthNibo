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
#ifndef NIBO_USE_XBEE
#error NIBO_USE_XBEE must be defined in niboconfig.h when this file is compiled
#endif

#include <avr/interrupt.h>

#include "nibo/xbee.h"

#if defined(XBEE_USE_UART0)
# define XBEE_UBRRxH UBRR0H
# define XBEE_UBRRxL UBRR0L
# define XBEE_UCSRxB UCSR0B
# define XBEE_UDRx UDR0
# define XBEE_USARTx_UDRE_vect USART0_UDRE_vect
# define XBEE_USARTx_RX_vect USART0_RX_vect
#elif defined(XBEE_USE_UART1)
# define XBEE_UBRRxH UBRR1H
# define XBEE_UBRRxL UBRR1L
# define XBEE_UCSRxB UCSR1B
# define XBEE_UDRx UDR1
# define XBEE_USARTx_UDRE_vect USART1_UDRE_vect
# define XBEE_USARTx_RX_vect USART1_RX_vect
#else
# error either XBEE_USE_UART0 or XBEE_USE_UART1 must be defined
#endif


#define HIBYTE(x) ((uint8_t)((((uint16_t)((x)))>>8)&0x00ff))
#define LOBYTE(x) ((uint8_t)((((uint16_t)((x)))>>0)&0x00ff))

#ifdef __cplusplus
extern "C" {
#endif


uint16_t xbee_tx_address;
uint16_t xbee_rx_address;

uint8_t xbee_tx_buf[XBEE_TXBUF_SIZE];
uint8_t xbee_rx_buf[XBEE_RXBUF_SIZE];

int8_t xbee_tx_pos;
int8_t xbee_rx_pos;

uint8_t xbee_rx_apiid;
uint8_t xbee_tx_apiid;

uint8_t xbee_rx_size;
uint8_t xbee_tx_size;

uint8_t xbee_tx_esc;
uint8_t xbee_rx_esc;

uint8_t xbee_tx_chk;
uint8_t xbee_rx_chk;

uint8_t xbee_tx_frame;
uint8_t xbee_rx_frame;

enum {
  XBEE_START = -14,
  XBEE_HSIZE = -13,
  XBEE_LSIZE = -12,

  XBEE_APIID = -11,
  XBEE_FRAME = -10,
  XBEE_ADDR7 = -9,
  XBEE_ADDR6 = -8,
  XBEE_ADDR5 = -7,
  XBEE_ADDR4 = -6,
  XBEE_ADDR3 = -5,
  XBEE_ADDR2 = -4,
  XBEE_ADDR1 = -3,
  XBEE_ADDR0 = -2,
  XBEE_OPT = -1,
  
  XBEE_DATA  = 0
};


void xbee_set_baudrate(uint16_t baud) {
  baud = ((uint32_t)(F_CPU))/((uint32_t)baud*16) - 1;
  XBEE_UBRRxH = (uint8_t)(baud>>8);
  XBEE_UBRRxL = (uint8_t)(baud&0xff);
}


void xbee_tx_start(uint8_t size) {
  xbee_tx_pos = XBEE_START;
  xbee_tx_size = size;
  if (++xbee_tx_frame==0) {
    xbee_tx_frame=1;
  }
  xbee_tx_esc = 0;
  XBEE_UCSRxB |= _BV(UDRIE0);
}


uint8_t xbee_tx_idle() {
  return xbee_tx_size==0;
}


ISR(XBEE_USARTx_UDRE_vect) {
  if (xbee_tx_pos<=xbee_tx_size) {
    uint8_t c;
    if (xbee_tx_pos==xbee_tx_size) {
      c = (uint8_t)0xff-xbee_tx_chk;
    } else if (xbee_tx_pos<0){
      if (xbee_tx_pos==XBEE_ADDR7) xbee_tx_pos=XBEE_ADDR1; // 16 bit address mode

      if (xbee_tx_pos==XBEE_START)      {c=0x7e^0x20; xbee_tx_esc=1;}
      else if (xbee_tx_pos==XBEE_HSIZE) {c=0x00;}
      else if (xbee_tx_pos==XBEE_LSIZE) {c=xbee_tx_size+5;}
      else if (xbee_tx_pos==XBEE_APIID) {c=0x01; xbee_tx_chk=0;}
      else if (xbee_tx_pos==XBEE_FRAME) {c=xbee_tx_frame;}
      else if (xbee_tx_pos==XBEE_ADDR1) {c=HIBYTE(xbee_tx_address);}
      else if (xbee_tx_pos==XBEE_ADDR0) {c=LOBYTE(xbee_tx_address);}
      else if (xbee_tx_pos==XBEE_OPT)   {c=0x00;}
    } else {
      c = xbee_tx_buf[xbee_tx_pos];
    }
    
    if (xbee_tx_esc) {
      c ^= 0x20;
      xbee_tx_esc = 0;
      xbee_tx_pos++;
    } else {
      if (xbee_tx_pos!=xbee_tx_size) {
        xbee_tx_chk += c;
      }
      if ((c==0x7e)||(c==0x7d)||(c==0x11)||(c==0x13)) {
        c = 0x7d;
        xbee_tx_esc = 1;
      } else {
        xbee_tx_pos++;
      }
    }
    XBEE_UDRx = c;
  } else {
    XBEE_UCSRxB &= ~_BV(UDRIE0);
    xbee_tx_size = 0;
  }
}






/*

char xbee_getchar() {
  cli();
  uint8_t result = uart0_rxbuf[uart0_rxbuf_begin];
  if (uart0_rxbuf_end==UART0_RXBUF_SIZE) {
    uart0_rxbuf_end=uart0_rxbuf_begin;
  }
  
  if (++uart0_rxbuf_begin>=UART0_RXBUF_SIZE) {
    uart0_rxbuf_begin=0;
  }
  sei();
  return result;
}

static char uart0_rx_putchar(char c) {
  uart0_rxbuf[uart0_rxbuf_end] = c;
  if (++uart0_rxbuf_end>=UART0_RXBUF_SIZE) {
    uart0_rxbuf_end=0;
  }
  if (uart0_rxbuf_end==uart0_rxbuf_begin) {
    uart0_rxbuf_end=UART0_RXBUF_SIZE;
  }
  return c;
}
*/

void xbee_enable() {
  XBEE_UCSRxB = _BV(RXCIE0) + _BV(UDRIE0) + _BV(RXEN0) + _BV(TXEN0);
}


void xbee_disable() {
  XBEE_UCSRxB = 0;
}

ISR(XBEE_USARTx_RX_vect) {
  uint8_t c = XBEE_UDRx;
}



#ifdef __cplusplus
} // extern "C"
#endif


