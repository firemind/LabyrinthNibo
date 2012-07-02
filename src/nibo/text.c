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
#ifndef NIBO_USE_TEXT
#error NIBO_USE_TEXT must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_DISPLAY
#error NIBO_USE_DISPLAY must be defined in niboconfig.h when this file is compiled
#endif

#include <stdarg.h>
#include <stdio.h>

#include "nibo/text.h"
#include "nibo/display.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t cursor_x;
uint8_t cursor_y;

void text_print_char(char c) {
  if ((cursor_x>15)||(cursor_y>1))
    return;
  display_wait(0x00);
  display_write(0x02, c);
  cursor_x++;
  if ((cursor_x==16)&&(cursor_y==0))
  {
    cursor_x=0;
    cursor_y=1;
    display_wait(0x00);
    display_write(0x00, 0x80+0x40);
  }
}


void text_init(void) {
  display_init();
  //clear display
  display_wait(0x00);
  display_write(0x00, 0x01);
  //display on, blink cursor
  display_wait(0x00);
  display_write(0x00, 0x0f);
  // 8 bit mode, 2-line, 5x7 font
  display_wait(0x00);
  display_write(0x00, 0x38);
}

void text_clear() {
  display_wait(0x00);
  display_write(0x00, 0x01);
}

void text_gotoXY(int x, int y) {
    cursor_x=x;
    cursor_y=y;
    
    if ((cursor_x>15)||(cursor_y>1))
      return;
      
    display_wait(0x00);
    if (cursor_y==0)
      display_write(0x00, 0x80+0x00+cursor_x);
    else
      display_write(0x00, 0x80+0x40+cursor_x);
}

void text_print(const char * str) {
  while(*str)
    text_print_char(*str++);
}

void text_setCursorMode(int mode) {
  display_wait(0x00);
  if (mode==0)
    display_write(0x00, 0x0c);
  else if (mode==1)
    display_write(0x00, 0x0e);
  else if (mode==2)
    display_write(0x00, 0x0f);
}

#ifdef __cplusplus
} // extern "C"
#endif
