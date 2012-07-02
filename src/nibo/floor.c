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
#ifndef NIBO_USE_FLOOR
#error NIBO_USE_FLOOR must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_ADC
#error NIBO_USE_ADC must be defined in niboconfig.h when this file is compiled
#endif

#include "nibo/iodefs.h"
#include "nibo/floor.h"
#include "nibo/adc.h"
#include "nibo/delay.h"
#include <avr/eeprom.h>

#define LINE_EEPROM ((void *)0)

#ifdef __cplusplus
extern "C" {
#endif

uint16_t floor_absolute[4];
uint16_t floor_relative[4];
//static uint16_t cal_black[4];
//static uint16_t cal_white[4];
static uint16_t cal_gain[4];

typedef struct {
  uint16_t black[4];
  uint16_t white[4];
} line_cal_t;

line_cal_t line_cal;


void floor_init(void) {
  floor_readPersistent();
  clear_output_bit(IO_LINE_FLOOR_EN);
  activate_output_bit(IO_LINE_FLOOR_EN);
  adc_init(AN_FLOOR_L);
  adc_init(AN_FLOOR_R);
  adc_init(AN_LINE_L);
  adc_init(AN_LINE_R);
}

static void floor_measure() {
  floor_relative[0] = 1023-adc_read(AN_LINE_L);
  floor_relative[1] = 1023-adc_read(AN_LINE_R);
  floor_relative[2] = 1023-adc_read(AN_FLOOR_L);
  floor_relative[3] = 1023-adc_read(AN_FLOOR_R);
  floor_relative[3] += 1023-adc_read(AN_FLOOR_R);
  floor_relative[2] += 1023-adc_read(AN_FLOOR_L);
  floor_relative[1] += 1023-adc_read(AN_LINE_R);
  floor_relative[0] += 1023-adc_read(AN_LINE_L);
}


static void floor_measure_diff() {
  floor_disable_ir();
  delay(1);
  floor_measure();
  floor_absolute[0] = floor_relative[0];
  floor_absolute[1] = floor_relative[1];
  floor_absolute[2] = floor_relative[2];
  floor_absolute[3] = floor_relative[3];
  
  floor_enable_ir();
  delay(1);
  floor_measure();
  floor_relative[0] -= (floor_relative[0]>floor_absolute[0])?floor_absolute[0]:floor_relative[0];
  floor_relative[1] -= (floor_relative[1]>floor_absolute[1])?floor_absolute[1]:floor_relative[1];
  floor_relative[2] -= (floor_relative[2]>floor_absolute[2])?floor_absolute[2]:floor_relative[2];
  floor_relative[3] -= (floor_relative[3]>floor_absolute[3])?floor_absolute[3]:floor_relative[3];
}


void floor_update() {
  floor_measure_diff();
  floor_relative[0] = (floor_relative[0]-(line_cal.black[0]/0x10))*(uint32_t)cal_gain[0]/0x100;
  floor_relative[1] = (floor_relative[1]-(line_cal.black[1]/0x10))*(uint32_t)cal_gain[1]/0x100;
  floor_relative[2] = (floor_relative[2]-(line_cal.black[2]/0x10))*(uint32_t)cal_gain[2]/0x100;
  floor_relative[3] = (floor_relative[3]-(line_cal.black[3]/0x10))*(uint32_t)cal_gain[3]/0x100;
  
  floor_absolute[0] = floor_absolute[0]*(uint32_t)cal_gain[0]/0x100;
  floor_absolute[1] = floor_absolute[1]*(uint32_t)cal_gain[1]/0x100;
  floor_absolute[2] = floor_absolute[2]*(uint32_t)cal_gain[2]/0x100;
  floor_absolute[3] = floor_absolute[3]*(uint32_t)cal_gain[3]/0x100;
}


static void floor_calibrateUpdate() {
  cal_gain[0] = ((uint32_t)0x100000L)/(line_cal.white[0]-line_cal.black[0]);
  cal_gain[1] = ((uint32_t)0x100000L)/(line_cal.white[1]-line_cal.black[1]);
  cal_gain[2] = ((uint32_t)0x100000L)/(line_cal.white[2]-line_cal.black[2]);
  cal_gain[3] = ((uint32_t)0x100000L)/(line_cal.white[3]-line_cal.black[3]);
}


void floor_calibrateBlack() {
  uint8_t i;
  line_cal.black[0]=0;
  line_cal.black[1]=0;
  line_cal.black[2]=0;
  line_cal.black[3]=0;
  for (i=0; i<0x10; ++i) {
    floor_measure_diff();
    line_cal.black[0] += floor_relative[0];
    line_cal.black[1] += floor_relative[1];
    line_cal.black[2] += floor_relative[2];
    line_cal.black[3] += floor_relative[3];
  }
  floor_calibrateUpdate();
}


void floor_calibrateWhite() {
  uint8_t i;
  line_cal.white[0]=0;
  line_cal.white[1]=0;
  line_cal.white[2]=0;
  line_cal.white[3]=0;
  for (i=0; i<0x10; ++i) {
    floor_measure_diff();
    line_cal.white[0] += floor_relative[0];
    line_cal.white[1] += floor_relative[1];
    line_cal.white[2] += floor_relative[2];
    line_cal.white[3] += floor_relative[3];
  }
  floor_calibrateUpdate();
}

void floor_readPersistent() {
  eeprom_read_block (&line_cal, LINE_EEPROM, sizeof(line_cal_t));
  if (line_cal.black[0]==0xffff) {
    line_cal.black[0]=0;
    line_cal.black[1]=0;
    line_cal.black[2]=0;
    line_cal.black[3]=0;
    line_cal.white[0]=0x100;
    line_cal.white[1]=0x100;
    line_cal.white[2]=0x100;
    line_cal.white[3]=0x100;
  }
  floor_calibrateUpdate();
}


void floor_writePersistent() {
  eeprom_write_block (&line_cal, LINE_EEPROM, sizeof(line_cal_t));

}


#ifdef __cplusplus
} // extern "C"
#endif
