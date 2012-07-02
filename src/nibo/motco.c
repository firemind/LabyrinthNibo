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
#ifndef NIBO_USE_MOTCO
#error NIBO_USE_MOTCO must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_I2CMASTER
#error NIBO_USE_I2CMASTER must be defined in niboconfig.h when this file is compiled
#endif

#include "nibo/iodefs.h"
#include "nibo/motco.h"
#include "nibo/i2cmaster.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) (((uint8_t)hi*(uint16_t)0x100)+(uint8_t)lo)

int16_t motco_speed_l;
int16_t motco_speed_r;

int16_t motco_ticks_l;
int16_t motco_ticks_r;

float motco_pos_x;
float motco_pos_y;
float motco_pos_ori;


uint8_t motco_transmit() {
  if (i2c_start_transmission()==0)
    return 0;
  if (i2c_wait_transmission()==0)
    return 0;
  if (i2c_status()!=I2C_SUCCESS)
    return 0;
  return 1;
}


uint8_t motco_update() {
  i2c_size=9;
  i2c_buf[0] = I2C_RX(MOTCO_I2C_ID);
  if (motco_transmit()==0)
    return 0;
  motco_ticks_l = MAKE_WORD(i2c_buf[1], i2c_buf[2]);
  motco_ticks_r = MAKE_WORD(i2c_buf[3], i2c_buf[4]);
  motco_speed_l = MAKE_WORD(i2c_buf[5], i2c_buf[6]);
  motco_speed_r = MAKE_WORD(i2c_buf[7], i2c_buf[8]);
  return 1;
}

uint8_t motco_stop() {
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x00;
  return motco_transmit();
}

uint8_t motco_stopImmediate() {
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x07;
  return motco_transmit();
}

uint8_t motco_setPWM(int16_t left, int16_t right) {
  i2c_size = 6;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x01;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  return motco_transmit();
}

uint8_t motco_setSpeed(int16_t left, int16_t right) {
  i2c_size = 6;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x02;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  return motco_transmit();
}

uint8_t motco_setPosition(int16_t left, int16_t right, uint16_t speed) {
  i2c_size = 8;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x06;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  i2c_buf[6] = HIBYTE(speed);
  i2c_buf[7] = LOBYTE(speed);
  return motco_transmit();
}

uint8_t motco_setSpeedParameters(int8_t ki, int8_t kp, int8_t kd) {
  i2c_size = 5;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x03;
  i2c_buf[2] = ki;
  i2c_buf[3] = kp;
  i2c_buf[4] = kd;
  return motco_transmit();
}

uint8_t motco_setPositionParameters(int8_t ki, int8_t kp, int8_t kd) {
  i2c_size = 5;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x05;
  i2c_buf[2] = ki;
  i2c_buf[3] = kp;
  i2c_buf[4] = kd;
  return motco_transmit();
}

uint8_t motco_resetOdometry(int16_t left, int16_t right) {
  i2c_size = 6;
  i2c_buf[0] = I2C_TX(MOTCO_I2C_ID);
  i2c_buf[1] = 0x04;
  i2c_buf[2] = HIBYTE(left);
  i2c_buf[3] = LOBYTE(left);
  i2c_buf[4] = HIBYTE(right);
  i2c_buf[5] = LOBYTE(right);
  return motco_transmit();
}

#ifdef __cplusplus
} // extern "C"
#endif
