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
#ifndef NIBO_USE_I2CMASTER
#error NIBO_USE_I2CMASTER must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_DELAY
#error NIBO_USE_DELAY must be defined in niboconfig.h when this file is compiled
#endif

#include "nibo/iodefs.h"
#include "nibo/i2cmaster.h"
#include <avr/interrupt.h>
#include "nibo/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NIBO_I2C_TXRX
volatile uint8_t i2c_tx_size;
volatile uint8_t i2c_rx_size;
#endif

volatile uint8_t i2c_state;

volatile uint8_t i2c_size;
volatile uint8_t i2c_pos;

uint8_t i2c_buf[I2C_BUF_SIZE];
volatile uint8_t i2c_last_TWSR;

enum {
  I2C_Idle,
  I2C_Address,
  I2C_Rx,
  I2C_Rx_Ack,
  I2C_Tx,
  I2C_Tx_Ack,
  I2C_Locked
};

////////////////////////////////////////////////////////////////////////////////

static inline void twi_start() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE) | _BV(TWSTA);
}

static inline void twi_send(uint8_t data) {
  TWDR = data;
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
}

static inline void twi_ack() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE) | _BV(TWEA);
}

static inline void twi_nak() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWIE);
}

static inline void twi_stop() {
  TWCR = _BV(TWEN) | _BV(TWINT) | _BV(TWSTO);
}

static inline void twi_abort() {
  TWCR = _BV(TWEN) | _BV(TWINT);
}

////////////////////////////////////////////////////////////////////////////////

uint8_t i2c_start_transmission(void) {
  uint8_t res = 0;
  cli();
  do {
    if (i2c_state == I2C_BUSY) {
      i2c_state = I2C_ERROR+1;
      break;
    }
  
    if (TWCR & _BV(TWSTO)) {
      i2c_state = I2C_ERROR+2;
      break;
    }
    if (TWCR & _BV(TWSTA)) {
      TWCR = 0;
      TWCR = _BV(TWEN);
      i2c_state = I2C_ERROR+8;
      break;
    }
    i2c_state = I2C_BUSY;
#ifdef NIBO_I2C_TXRX
    i2c_size = i2c_tx_size;
#endif
    twi_start();
    res = 1;
  } while(0);
  sei();
  return res;
}

uint8_t i2c_wait_transmission(void) {
  uint16_t i = 10000;
  while (i--) {
    if ((TWCR & _BV(TWIE))||(TWCR & _BV(TWSTO))) {
      delay_us(1);
    } else {
      return 1;
    }
  }
  cli();
  TWCR = 0;
  TWCR = _BV(TWEN);
  if (i2c_state < I2C_ERROR) {
    i2c_state = I2C_ERROR+3;
  }
  sei();
  return 0;
}


uint8_t i2c_status(void) {
  uint8_t res = i2c_state;
  return res;
}


void i2c_init(void) {
  TWCR = 0;
  TWBR = I2C_TWBR_INIT;
  TWDR = 0xff;
  TWCR = _BV(TWEN);
  i2c_state = I2C_IDLE;
}


ISR(TWI_vect) {
  uint8_t res = TWSR & 0xf8;
  i2c_last_TWSR = res;
  switch (res) {
    case 0x08: // Start condition
    case 0x10: // Repeated start condition
      i2c_pos=0;
    case 0x18: // W-Address ACK received
    case 0x28: // Data tx and ACK received
      if (i2c_pos<i2c_size) {
        twi_send(i2c_buf[i2c_pos++]);
      } else {
#ifdef NIBO_I2C_TXRX
        if (i2c_rx_size) {
          i2c_size = i2c_rx_size;
          i2c_buf[0] |= 0x01;
          twi_start();
          break;
        }
#endif
        i2c_state = I2C_SUCCESS;
        twi_stop();
      }
      break;
    
    case 0x50: // Data rx and ACK sent
      i2c_buf[i2c_pos++] = TWDR;
    case 0x40: // R-Address ACK received
      if (i2c_pos+1<i2c_size) {
        // Receive next byte, and send ACK
        twi_ack();
      } else {
        // Receive last byte, and send NAK
        twi_nak();
      }
      break;
    
    case 0x58: // Data rx and NACK sent
      // Last byte was received
      i2c_buf[i2c_pos] = TWDR;
      i2c_state = I2C_SUCCESS;
      twi_stop();
      break;
      
    case 0x38: // Arbitration was lost
      twi_abort();
      i2c_state = I2C_ERROR+5;
      break;

    case 0x00: // Bus Error
    case 0x20: // W-Address NACK received
    case 0x48: // R-Address NACK received
    case 0x30: // Data tx and NACK received
      twi_stop();
      i2c_state = I2C_ERROR+6;
      break;
    
    default:
      twi_abort();
      i2c_state = I2C_ERROR+7;
  }
}

#ifdef __cplusplus
} // extern "C"
#endif
