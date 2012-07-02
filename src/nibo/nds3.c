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

/*! @file       nds3.c
 * @brief       Modul zur Kommunikation mit der NDS3 Erweiterung
 * @author      Nils Springob (nils@nicai-systems.de)
 * @date        2009-06-28
*/

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/nds3.h"
#include "nibo/i2cmaster.h"


#define NDS3_I2C_ADDR (42<<1)


#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) (((uint8_t)hi*(uint16_t)0x100)+(uint8_t)lo)


uint8_t nds3_ddr;
uint8_t nds3_port;
uint8_t nds3_pin;
uint16_t nds3_servocalib_hi;
uint16_t nds3_servocalib_mid;
uint16_t nds3_servocalib_lo;
int16_t nds3_distcalib_v0;
int16_t nds3_distcalib_d0;
int16_t nds3_distcalib_alpha;

uint8_t nds3_distances[181];


enum {
  CMD_RESET = 0x00,
  CMD_MOVE = 0x01,
  CMD_MEASURE = 0x02,
  CMD_READ = 0x03,
  CMD_GETPOS = 0x04,
  CMD_GETDIST = 0x05,
  CMD_GETBUSY = 0x06,
  CMD_SETDDR = 0x10,
  CMD_SETPORT = 0x11,
  CMD_GETPIN = 0x12,
  CMD_GETANALOG = 0x13,
  CMD_MOVESERVO = 0x20,
  CMD_GETSERVOCALIB = 0x21,
  CMD_SETSERVOCALIB = 0x22,
  CMD_GETDISTCALIB = 0x23,
  CMD_SETDISTCALIB = 0x24
};


static void nds3_transaction(uint8_t txsize, uint8_t rxsize) {
  i2c_tx_size = txsize;
  i2c_rx_size = rxsize;
  i2c_start_transmission();
  i2c_wait_transmission();
}


void nds3_init() {
}

void nds3_reset() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_RESET;
  nds3_transaction(2, 2);
}

void nds3_move(uint8_t dir) {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_MOVE;
  i2c_buf[2] = dir;
  nds3_transaction(3, 2);
}

void nds3_measure(uint8_t dir, int8_t step) {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_MEASURE;
  i2c_buf[2] = dir;
  i2c_buf[3] = step;
  nds3_transaction(4, 2);
}

void nds3_read(uint8_t dir, uint8_t step) {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_READ;
  i2c_buf[2] = dir;
  i2c_buf[3] = step;
  nds3_transaction(4, 17);
  
  uint8_t idx=1;
  while ((dir<=180) && (idx<17)) {
    nds3_distances[dir] = i2c_buf[idx];
    idx++;
    dir += step;
  }
  
}

uint8_t nds3_get_pos() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETPOS;
  nds3_transaction(2, 2);
  return i2c_buf[1];
}

uint8_t nds3_get_dist() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETDIST;
  nds3_transaction(2, 2);
  return i2c_buf[1];
}

uint8_t nds3_get_busy() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETBUSY;
  nds3_transaction(2, 2);
  return i2c_buf[1];
}



void nds3_ddr_commit() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_SETDDR;
  i2c_buf[2] = nds3_ddr;
  nds3_transaction(3, 2);
}

void nds3_port_commit() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_SETPORT;
  i2c_buf[2] = nds3_port;
  nds3_transaction(3, 2);
}

void nds3_pin_update() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETPIN;
  nds3_transaction(2, 2);
  nds3_pin = i2c_buf[1];
}

uint16_t nds3_get_analog(uint8_t idx) {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETANALOG;
  i2c_buf[2] = idx;
  nds3_transaction(3, 3);
  return MAKE_WORD(i2c_buf[1], i2c_buf[2]);
}

void nds3_move_servo(uint16_t usecs) {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_MOVESERVO;
  i2c_buf[2] = HIBYTE(usecs);
  i2c_buf[3] = LOBYTE(usecs);
  nds3_transaction(4, 2);
}


void nds3_servocalib_update() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETSERVOCALIB;
  nds3_transaction(2, 7);
  nds3_servocalib_lo = MAKE_WORD(i2c_buf[1], i2c_buf[2]);
  nds3_servocalib_mid = MAKE_WORD(i2c_buf[3], i2c_buf[4]);
  nds3_servocalib_hi = MAKE_WORD(i2c_buf[5], i2c_buf[6]);
}

void nds3_servocalib_commit() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_SETSERVOCALIB;
  i2c_buf[2] = HIBYTE(nds3_servocalib_lo);
  i2c_buf[3] = LOBYTE(nds3_servocalib_lo);
  i2c_buf[4] = HIBYTE(nds3_servocalib_mid);
  i2c_buf[5] = LOBYTE(nds3_servocalib_mid);
  i2c_buf[6] = HIBYTE(nds3_servocalib_hi);
  i2c_buf[7] = LOBYTE(nds3_servocalib_hi);
  nds3_transaction(8, 2);
}

void nds3_distcalib_update() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_GETDISTCALIB;
  nds3_transaction(2, 7);
  nds3_distcalib_v0 = MAKE_WORD(i2c_buf[1], i2c_buf[2]);
  nds3_distcalib_d0 = MAKE_WORD(i2c_buf[3], i2c_buf[4]);
  nds3_distcalib_alpha = MAKE_WORD(i2c_buf[5], i2c_buf[6]);
}

void nds3_distcalib_commit() {
  i2c_buf[0] = NDS3_I2C_ADDR;
  i2c_buf[1] = CMD_SETDISTCALIB;
  i2c_buf[2] = HIBYTE(nds3_distcalib_v0);
  i2c_buf[3] = LOBYTE(nds3_distcalib_v0);
  i2c_buf[4] = HIBYTE(nds3_distcalib_d0);
  i2c_buf[5] = LOBYTE(nds3_distcalib_d0);
  i2c_buf[6] = HIBYTE(nds3_distcalib_alpha);
  i2c_buf[7] = LOBYTE(nds3_distcalib_alpha);
  nds3_transaction(8, 2);
}

