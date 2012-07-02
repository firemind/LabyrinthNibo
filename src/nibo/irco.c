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
#ifndef NIBO_USE_IRCO
#error NIBO_USE_IRCO must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_I2CMASTER
#error NIBO_USE_I2CMASTER must be defined in niboconfig.h when this file is compiled
#endif


#include "nibo/iodefs.h"
#include "nibo/irco.h"
#include "nibo/i2cmaster.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) (((uint8_t)hi*(uint16_t)0x100)+(uint8_t)lo)

uint8_t irco_mode;
uint8_t irco_distance[5];
uint8_t irco_rc5_count;
uint16_t irco_rc5_cmd;

uint8_t irco_transmit()
{
  if (i2c_start_transmission()==0)
    return 0;
  if (i2c_wait_transmission()==0)
    return 0;
  if (i2c_status()!=I2C_SUCCESS)
    return 0;
  return 1;
}

uint8_t irco_update()
{
  i2c_buf[1]=0xaf;
  i2c_buf[2]=0xfe;
  i2c_buf[3]=0xaf;
  i2c_buf[4]=0xfe;
  i2c_buf[5]=0xaf;
  i2c_buf[6]=0xfe;
  i2c_buf[7]=0xaf;
  i2c_buf[8]=0xfe;

  i2c_size = 9;
  i2c_buf[0] = I2C_RX(IRCO_I2C_ID);
  if (!irco_transmit())
    return 0;

  irco_distance[0] = i2c_buf[1];
  irco_distance[1] = i2c_buf[2];
  irco_distance[2] = i2c_buf[3];
  irco_distance[3] = i2c_buf[4];
  irco_distance[4] = i2c_buf[5];
  irco_rc5_count = i2c_buf[6];

  uint16_t cmd = MAKE_WORD(i2c_buf[7], i2c_buf[8]);
  if (cmd)
    irco_rc5_cmd=cmd;
  return 1;
}

uint8_t irco_stop()
{
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(IRCO_I2C_ID);
  i2c_buf[1] = 0x00;
  return irco_transmit();
}

uint8_t irco_startMeasure()
{
  i2c_size = 2;
  i2c_buf[0] = I2C_TX(IRCO_I2C_ID);
  i2c_buf[1] = 0x01;
  return irco_transmit();
}

uint8_t irco_transmitRC5(uint16_t rc5)
{
  i2c_size = 4;
  i2c_buf[0] = I2C_TX(IRCO_I2C_ID);
  i2c_buf[1] = 0x02;
  i2c_buf[2] = HIBYTE(rc5);
  i2c_buf[3] = LOBYTE(rc5);
  return irco_transmit();
}

#ifdef __cplusplus
} // extern "C"
#endif
