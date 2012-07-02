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

/*!
 *  @brief  Routinen zur Displaysteuerung
 *  @author Nils Springob (nils@nicai-systems.de)
 *  @date   2007-01-26
 */

#include "nibo/niboconfig.h"
#ifndef NIBO_USE_DISPLAY
#error NIBO_USE_DISPLAY must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_DELAY
#error NIBO_USE_DELAY must be defined in niboconfig.h when this file is compiled
#endif


#include "nibo/iodefs.h"
#include "nibo/display.h"
#include "nibo/delay.h"

#ifdef __cplusplus
extern "C" {
#endif

/*! Puffergroesse fuer eine Zeile in bytes */
#define DISPLAY_BUFFER_SIZE	(DISPLAY_LENGTH + 1)

#define DISPLAY_CLEAR 0x01      /*!< Kommando zum Löschen */
#define DISPLAY_CURSORHOME 0x02 /*!< Kommando für den Cursor */

/**! Output-Pins des Displays */
#define DISPLAY_OUT (_BV(IO_DISPLAY_RS_BIT)|_BV(IO_DISPLAY_RW_BIT)|_BV(IO_DISPLAY_EN_BIT))

/**! Input-Pins des Displays */
#define DISPLAY_IN (_BV(IO_DISPLAY_READY_BIT))

#define clear_display_output_bits() do {IO_DISPLAY_PORT &= ~DISPLAY_OUT;} while(0)

uint8_t display_type;

uint8_t display_init(){
  display_type=0xff;
  
  activate_output_bit(IO_DISPLAY_RS);
  activate_output_bit(IO_DISPLAY_RW);
  activate_output_bit(IO_DISPLAY_EN);
  activate_output_bit(IO_DISPLAY_CS1);
  activate_output_bit(IO_DISPLAY_CS2);
  activate_output_bit(IO_DISPLAY_RST);
  
  IO_DISPLAY_DDR = 0x00;

  clear_output_bit(IO_DISPLAY_RS);
  clear_output_bit(IO_DISPLAY_RW);
  clear_output_bit(IO_DISPLAY_EN);
  clear_output_bit(IO_DISPLAY_CS1);
  clear_output_bit(IO_DISPLAY_CS2);

  clear_output_bit(IO_DISPLAY_RST);
#ifdef _NIBO_2_
  if (get_output_bit(IO_RESET_CO)) {
    clear_output_bit(IO_RESET_CO);
    delay_ms(1);
    set_output_bit(IO_RESET_CO);
  } else {
    delay_ms(1);
  }
#else
  delay_ms(1);
#endif
  set_output_bit(IO_DISPLAY_RST);

  if (display_wait(0)) {
    if (display_wait(1))
      display_type=2;
    else
      display_type=1;
  }

  if (display_type==0xff)
    display_type=0;

  return display_type;
}


void display_write(uint8_t mode, uint8_t data){
  set_output_bitval(IO_DISPLAY_CS1, !(mode&0x01));
  set_output_bitval(IO_DISPLAY_CS2, mode&0x01);
  set_output_bitval(IO_DISPLAY_RS, mode&0x02);
  clear_output_bit(IO_DISPLAY_RW);
  IO_DISPLAY_DDR = 0xff;
  IO_DISPLAY_PORT = data;
  delay_us(1);
  set_output_bit(IO_DISPLAY_EN);
  delay_us(1);
  clear_output_bit(IO_DISPLAY_EN);
  delay_us(1);
  IO_DISPLAY_PORT = 0x00;
  IO_DISPLAY_DDR = 0x00;
}

uint8_t display_read(uint8_t mode){
  set_output_bitval(IO_DISPLAY_CS1, !(mode&0x01));
  set_output_bitval(IO_DISPLAY_CS2, mode&0x01);
  set_output_bitval(IO_DISPLAY_RS, mode&0x02);
  set_output_bit(IO_DISPLAY_RW);
  IO_DISPLAY_DDR = 0x00;
  delay_us(1);
  set_output_bit(IO_DISPLAY_EN);
  delay_us(4);
  uint8_t data = IO_DISPLAY_PIN;
  clear_output_bit(IO_DISPLAY_EN);
  delay_us(1);
  return data;
}


// high level fuctions

uint8_t display_wait(uint8_t mode){
  if (display_type==0)
    return 0;
  mode &= 0x01;
  uint16_t timeout=0xffff;
  while (timeout--){
    if ((display_read(mode)&0x80)==0x00)
      return 1;
  }
  return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif
