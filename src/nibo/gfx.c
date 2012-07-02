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
#ifndef NIBO_USE_GFX
#error NIBO_USE_GFX must be defined in niboconfig.h when this file is compiled
#endif
#ifndef NIBO_USE_DISPLAY
#error NIBO_USE_DISPLAY must be defined in niboconfig.h when this file is compiled
#endif

#include <stdarg.h>
#include <stdio.h>

#include "nibo/gfx.h"
#include "nibo/display.h"

#ifdef __cplusplus
extern "C" {
#endif

// The font is 8 pixels high and 5 pixels wide.
// The first byte of each symbol is the proportional mask. The columns are
// encoded in the following 5 bytes.

const uint8_t fontTiles[] = {
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, // <space>
  0x04, 0x00, 0x00, 0x5f, 0x00, 0x00, // !
  0x0e, 0x00, 0x07, 0x00, 0x07, 0x00, // "
  0x1f, 0x14, 0x7f, 0x14, 0x7f, 0x14, // #
  0x1f, 0x24, 0x2a, 0x7f, 0x2a, 0x12, // $
  0x1f, 0x23, 0x13, 0x08, 0x64, 0x62, // %
  0x1f, 0x36, 0x49, 0x55, 0x22, 0x50, // &
  0x06, 0x00, 0x05, 0x03, 0x00, 0x00, // '
  0x0e, 0x00, 0x1c, 0x22, 0x41, 0x00, // (
  0x0e, 0x00, 0x41, 0x22, 0x1c, 0x00, // )
  0x1f, 0x14, 0x08, 0x3e, 0x08, 0x14, // *
  0x1f, 0x08, 0x08, 0x3e, 0x08, 0x08, // +
  0x06, 0x00, 0xa0, 0x60, 0x00, 0x00, // ,
  0x0f, 0x08, 0x08, 0x08, 0x08, 0x08, // -
  0x06, 0x00, 0x60, 0x60, 0x00, 0x00, // .
  0x0e, 0x00, 0x60, 0x1c, 0x03, 0x00, // /
  
  0x1f, 0x3e, 0x51, 0x49, 0x45, 0x3e, // 0
  0x0e, 0x00, 0x42, 0x7f, 0x40, 0x00, // 1
  0x1f, 0x42, 0x61, 0x51, 0x49, 0x46, // 2
  0x1d, 0x21, 0x41, 0x45, 0x4b, 0x31, // 3
  0x1f, 0x18, 0x14, 0x12, 0x7f, 0x10, // 4
  0x1d, 0x27, 0x45, 0x45, 0x45, 0x39, // 5
  0x17, 0x3c, 0x4a, 0x49, 0x49, 0x30, // 6
  0x1e, 0x01, 0x71, 0x09, 0x05, 0x03, // 7
  0x1d, 0x36, 0x49, 0x49, 0x49, 0x36, // 8
  0x1d, 0x06, 0x49, 0x49, 0x29, 0x1e, // 9
  0x06, 0x00, 0x36, 0x36, 0x00, 0x00, // :
  0x06, 0x00, 0xac, 0x6c, 0x00, 0x00, // ;
  0x0f, 0x08, 0x14, 0x22, 0x41, 0x00, // <
  0x0f, 0x14, 0x14, 0x14, 0x14, 0x14, // =
  0x1e, 0x00, 0x41, 0x22, 0x14, 0x08, // >
  0x1f, 0x02, 0x01, 0x51, 0x09, 0x06, // ?
  
  0x1f, 0x32, 0x49, 0x79, 0x41, 0x3e, // @
  0x1f, 0x7c, 0x12, 0x11, 0x12, 0x7c, // A
  0x17, 0x7f, 0x49, 0x49, 0x49, 0x36, // B
  0x17, 0x3e, 0x41, 0x41, 0x41, 0x22, // C
  0x1f, 0x7f, 0x41, 0x41, 0x22, 0x1c, // D
  0x17, 0x7f, 0x49, 0x49, 0x49, 0x41, // E
  0x17, 0x7f, 0x09, 0x09, 0x09, 0x01, // F
  0x17, 0x3e, 0x41, 0x49, 0x49, 0x7a, // G
  0x17, 0x7f, 0x08, 0x08, 0x08, 0x7f, // H
  0x0e, 0x00, 0x41, 0x7f, 0x41, 0x00, // I
  0x1f, 0x20, 0x40, 0x41, 0x3f, 0x01, // J
  0x1f, 0x7f, 0x08, 0x14, 0x22, 0x41, // K
  0x17, 0x7f, 0x40, 0x40, 0x40, 0x40, // L
  0x1f, 0x7f, 0x02, 0x0c, 0x02, 0x7f, // M
  0x1f, 0x7f, 0x04, 0x08, 0x10, 0x7f, // N
  0x17, 0x3e, 0x41, 0x41, 0x41, 0x3e, // O
  
  0x17, 0x7f, 0x09, 0x09, 0x09, 0x06, // P
  0x17, 0x3e, 0x41, 0x41, 0x41, 0xbe, // Q
  0x1d, 0x7f, 0x09, 0x19, 0x29, 0x46, // R
  0x17, 0x46, 0x49, 0x49, 0x49, 0x31, // S
  0x1f, 0x01, 0x01, 0x7f, 0x01, 0x01, // T
  0x17, 0x3f, 0x40, 0x40, 0x40, 0x3f, // U
  0x1f, 0x1f, 0x20, 0x40, 0x20, 0x1f, // V
  0x1f, 0x3f, 0x40, 0x38, 0x40, 0x3f, // W
  0x1f, 0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x1f, 0x07, 0x08, 0x70, 0x08, 0x07, // Y
  0x1f, 0x61, 0x51, 0x49, 0x45, 0x43, // Z
  0x0e, 0x00, 0x7f, 0x41, 0x41, 0x00, // [
  0x0e, 0x00, 0x03, 0x1c, 0x60, 0x00, // <backslash>
  0x0e, 0x00, 0x41, 0x41, 0x7f, 0x00, // ]
  0x0e, 0x04, 0x02, 0x01, 0x02, 0x04, // ^
  0x1f, 0x40, 0x40, 0x40, 0x40, 0x40, // _

  0x0c, 0x00, 0x00, 0x03, 0x04, 0x00, // `
  0x17, 0x20, 0x54, 0x54, 0x54, 0x78, // a
  0x17, 0x7f, 0x48, 0x44, 0x44, 0x38, // b
  0x17, 0x38, 0x44, 0x44, 0x44, 0x20, // c
  0x1d, 0x38, 0x44, 0x44, 0x48, 0x7f, // d
  0x17, 0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x17, 0x08, 0x7e, 0x09, 0x01, 0x02, // f
  0x17, 0x18, 0xa4, 0xa4, 0xa4, 0x78, // g
  0x17, 0x7f, 0x08, 0x04, 0x04, 0x78, // h
  0x0e, 0x00, 0x44, 0x7d, 0x40, 0x00, // i
  0x0d, 0x40, 0x80, 0x84, 0x7d, 0x00, // j
  0x0f, 0x7f, 0x10, 0x28, 0x44, 0x00, // k
  0x0e, 0x00, 0x41, 0x7f, 0x40, 0x00, // l
  0x1f, 0x7c, 0x04, 0x78, 0x04, 0x78, // m
  0x17, 0x7c, 0x08, 0x04, 0x04, 0x78, // n
  0x17, 0x38, 0x44, 0x44, 0x44, 0x38, // o
  
  0x17, 0xfc, 0x24, 0x24, 0x24, 0x18, // p
  0x17, 0x18, 0x24, 0x24, 0x24, 0xfc, // q
  0x17, 0x7c, 0x08, 0x04, 0x04, 0x08, // r
  0x17, 0x48, 0x54, 0x54, 0x54, 0x20, // s
  0x17, 0x04, 0x3f, 0x44, 0x40, 0x20, // t
  0x1d, 0x3c, 0x40, 0x40, 0x20, 0x7c, // u
  0x1f, 0x1c, 0x20, 0x40, 0x20, 0x1c, // v
  0x1f, 0x3c, 0x40, 0x30, 0x40, 0x3c, // w
  0x1f, 0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x17, 0x1c, 0xa0, 0xa0, 0xa0, 0x7c, // y
  0x1f, 0x44, 0x64, 0x54, 0x4c, 0x44, // z
  0x0e, 0x00, 0x08, 0x36, 0x41, 0x00, // {
  0x04, 0x00, 0x00, 0x7f, 0x00, 0x00, // |
  0x0e, 0x00, 0x41, 0x36, 0x08, 0x00, // }
  0x1f, 0x02, 0x01, 0x02, 0x04, 0x02, // ~
  0x1f, 0xaa, 0x55, 0xaa, 0x55, 0xaa, // <del>
};

uint8_t gfx_x;
uint8_t gfx_y;
uint8_t gfx_proportional;
uint8_t gfx_dm;

uint8_t gfx_tile[8];
uint8_t gfx_term_row;
uint8_t gfx_term_col;


static void gfx_load_disp_tile(uint8_t col, uint8_t row) {
  if (col<8) {
    col *= 8;
    display_wait(0);
    display_write(0, 0xb8+row);
    display_wait(0);
    display_write(0, 0x40+col);
    display_read(2);
    for (uint8_t i=0; i<8; ++i) {
      //display_wait(2);
      gfx_tile[i]=display_read(2);
    }
  } else {
    col -= 8;
    col *= 8;
    display_wait(1);
    display_write(1, 0xb8+row);
    display_wait(1);
    display_write(1, 0x40+col);
    display_read(3);
    for (uint8_t i=0; i<8; ++i) {
      //display_wait(3);
      gfx_tile[i]=display_read(3);
    }
  }
}


static void gfx_store_disp_tile(uint8_t col, uint8_t row) {
  if (col<8) {
    col *= 8;
    display_wait(0);
    display_write(0, 0xb8+row);
    display_wait(0);
    display_write(0, 0x40+col);
    for (uint8_t i=0; i<8; ++i) {
      display_wait(2);
      display_write(2, gfx_tile[i]);
    }
  } else {
    col -= 8;
    col *= 8;
    display_wait(1);
    display_write(1, 0xb8+row);
    display_wait(1);
    display_write(1, 0x40+col);
    for (uint8_t i=0; i<8; ++i) {
      display_wait(3);
      display_write(3, gfx_tile[i]);
    }
  }
}


static void gfx_reset_tile() {
  for (uint8_t i=0; i<8; ++i) {
    gfx_tile[i]=0;
  }
}


uint8_t gfx_get_x() {
  return gfx_x;
}


uint8_t gfx_get_y() {
  return gfx_y;
}


void gfx_draw_mode(uint8_t mode) {
  gfx_dm = mode;
}


void gfx_move(uint8_t x, uint8_t y) {
  gfx_x = x;
  gfx_y = y;

  display_wait(0);
  display_wait(1);
  display_write(0, 0xb8+y/8);
  display_write(1, 0xb8+y/8);
  
  display_wait(0);
  display_wait(1);
  if (x>64) {
    x-=64;
    display_write(0, 0x40);
    display_write(1, 0x40+x);
  } else {
    display_write(0, 0x40+x);
    display_write(1, 0x40);
  }
}


void gfx_fill(uint8_t val) {
  uint8_t b;
  for (b=0; b<8; ++b) {
    display_wait(0);
    display_wait(1);
    display_write(0, 0xb8+b);
    display_write(1, 0xb8+b);
    uint8_t i;
    for (i=0; i<64; ++i) {
      display_wait(2);
      display_wait(3);
      display_write(2, val);
      display_write(3, val);
    }
  }
  display_wait(0);
  display_wait(1);
  display_write(0, 0xb8);
  display_write(1, 0xb8);
}


void gfx_display_write2(uint8_t data, uint8_t mask) {
  uint8_t ch=0;
  if (gfx_x>=128)
    return;
  if (gfx_x>=64)
    ch++;

  display_wait(ch);
  display_read(ch+2); // dummy read
  
  uint8_t tp = display_read(ch+2);
  uint8_t dm = gfx_dm;
  if (dm&GFX_DM_JAM1_INV) {
    data = ~data;
    dm &= ~GFX_DM_JAM1_INV;
  }
  data &= mask;
  if (dm==GFX_DM_JAM1) {
    data |= tp;
  } else if (dm==GFX_DM_JAM2) {
    data |= tp & ~mask;
  } else /*if (dm==GFX_DM_COMP)*/ {
    data ^= tp;
  }
  
  display_wait(ch);
  display_write(ch, 0x40|gfx_x);
  
  display_wait(ch);
  display_write(ch+2, data);
  gfx_x++;
}



uint8_t gfx_char_width(char ch) {
  uint8_t res = 0;
  uint8_t c = (uint8_t) ch;
  if ((c<0x20) || (c>=0x80))
    c=0x80;
  c-=0x20;

  const uint8_t * ftp = fontTiles;
  ftp += (6 * (int)c);
  
  uint8_t mask = *ftp++;
  if (gfx_proportional==0)
    mask=0x1f;

  uint8_t i;
  for (i=0; i<5; ++i) {
    if (mask&0x01)
      res++;
    mask>>=1;
    ftp++;
  }
  return res;
}


void gfx_print_char_base(char ch, int8_t shval) {
  uint8_t c = (uint8_t) ch;
  if ((c<0x20) || (c>=0x80))
    c=0x80;
  c-=0x20;

  const uint8_t * ftp = fontTiles;
  ftp += (6 * (int)c);
  
  uint8_t mask = *ftp++;
  if (gfx_proportional==0)
    mask=0x1f;
    
  uint8_t i;
  for (i=0; i<5; ++i) {
    if (mask&0x01) {
      if (shval>=0) {
        uint8_t tv = (uint8_t)*ftp << shval;
        uint8_t tm = (uint8_t)0xff << shval;
        gfx_display_write2(tv, tm);
      } else {
        uint8_t tv = ((uint8_t)*ftp >> -shval);
        uint8_t tm = ((uint8_t)0xff >> -shval);
        gfx_display_write2(tv, tm);
      }
    }
    mask>>=1;
    ftp++;
  }
  if (shval>=0) {
    uint8_t tm = (uint8_t)0xff << shval;
    gfx_display_write2(0x00, tm);
  } else {
    uint8_t tm = ((uint8_t)0xff >> -shval);
    gfx_display_write2(0x00, tm);
  }
}


void gfx_print_char(char ch) {
  uint8_t x = gfx_x;
  int8_t shval = gfx_y%8;
  gfx_print_char_base(ch, shval);
  if (shval) {
    gfx_move(x, gfx_y+8);
    gfx_print_char_base(ch, shval-8);
    gfx_move(gfx_x, gfx_y-8);
  }
}


void gfx_print_text(const char *txt) {
  while(*txt)
    gfx_print_char(*txt++);
}


uint8_t gfx_text_width(const char *txt) {
  uint8_t res=0;
  while(*txt)
    res += gfx_char_width(*txt++) + 1;
  return res;
}


void gfx_print_text_P(PGM_P txt) {
  while(pgm_read_byte(txt))
    gfx_print_char(pgm_read_byte(txt++));
}


uint8_t gfx_text_width_P(PGM_P txt) {
  uint8_t res=0;
  while(pgm_read_byte(txt))
    res += gfx_char_width(pgm_read_byte(txt++)) + 1;
  return res;
}


void gfx_init() {
  display_wait(0x00);
  display_write(0x00, 0x3f);
  display_wait(0x00);
  display_write(0x00, 0xc0);
  display_wait(0x00);
  display_write(0x00, 0x40);
  display_wait(0x00);
  display_write(0x00, 0xb8);

  display_wait(0x01);
  display_write(0x01, 0x3f);
  display_wait(0x01);
  display_write(0x01, 0xc0);
  display_wait(0x01);
  display_write(0x01, 0x40);
  display_wait(0x01);
  display_write(0x01, 0xb8);

  gfx_fill(0x00);
  gfx_dm=GFX_DM_JAM2;
}


void gfx_set_proportional(uint8_t prop) {
  gfx_proportional=prop;
}


uint8_t gfx_get_proportional(void) {
  return gfx_proportional;
}


void gfx_hline(uint8_t l) {
  uint8_t data = 0x01 << (gfx_y%8);
  while (l--) {
    gfx_display_write2(data, data);
  }
}


void gfx_vline(uint8_t l) {
  gfx_box(1,l);
}


void gfx_box(uint8_t w, uint8_t h) {
  uint8_t x = gfx_x;
  uint8_t y = gfx_y;
  uint8_t data = ((uint8_t)0xff) << (y&(uint8_t)0x07);
  while (h) {
    uint8_t f = (uint8_t)8-(y&(uint8_t)0x07);
    if (h<f) {
      uint8_t mask = ((uint8_t)0xff) >> (uint8_t)(f-h);
      data = data & mask;
    }
    uint8_t i = w;
    while(i--) {
      gfx_display_write2(data, data);
    }
    
    if (f>h) {
      f = h;
    }
    h = h-f;
    y = y+f;
    gfx_move(x, y);
    data = (uint8_t)0xff;
  }
}


static void gfx_load_tile(uint8_t x8, uint8_t y, uint8_t w, uint8_t h, uint8_t * xbm) {
  uint8_t winc = ((w-1)/8)+1;
  uint8_t * ptr = xbm;
  ptr += x8 + y*winc;
  ptr -= 8*winc;

  for (int i=0; i<8; ++i) {
    uint8_t val = *ptr;
    for (int j=0; j<8; ++j) {
      gfx_tile[j] >>= 1;
      if (val&0x01) {
        gfx_tile[j] |= 0x80;
      }
      val >>= 1;
    }
    ptr += winc;
  }
}


void gfx_draw_xbm(uint8_t w, uint8_t h, uint8_t * xbm) {
  uint8_t x = gfx_x;
  uint8_t y = gfx_y;
  uint8_t iy = 0;
  // mask for first row
  uint8_t data = ((uint8_t)0xff) << (y&(uint8_t)0x07);
  while (h) {
    uint8_t f = (uint8_t)8-(y&(uint8_t)0x07);
    if (h<f) {
      uint8_t mask = ((uint8_t)0xff) >> (uint8_t)(f-h);
      data = data & mask;
    }
    //draw row
    for (uint8_t i=0; i<w; ++i) {
      if ((i&0x07)==0) {
        gfx_load_tile(i/8, iy+f, w, h, xbm);
      }
      uint8_t image_data = gfx_tile[i&0x07];
      image_data &= data;
      gfx_display_write2(image_data, data);

    }

    if (f>h) {
      f = h;
    }
    h = h-f;
    y = y+f;
    iy = iy+f;
    gfx_move(x, y);
    data = (uint8_t)0xff;
  }
}


static void gfx_load_tile_P(uint8_t x8, uint8_t y, uint8_t w, uint8_t h, PGM_P xbm) {
  uint8_t winc = ((w-1)/8)+1;
  PGM_P ptr = xbm;
  ptr += x8 + y*winc;
  ptr -= 8*winc;

  for (int i=0; i<8; ++i) {
    uint8_t val = pgm_read_byte(ptr);
    for (int j=0; j<8; ++j) {
      gfx_tile[j] >>= 1;
      if (val&0x01) {
        gfx_tile[j] |= 0x80;
      }
      val >>= 1;
    }
    ptr += winc;
  }
}


void gfx_draw_xbm_P(uint8_t w, uint8_t h, PGM_P xbm) {
  uint8_t x = gfx_x;
  uint8_t y = gfx_y;
  uint8_t iy = 0;
  // mask for first row
  uint8_t data = ((uint8_t)0xff) << (y&(uint8_t)0x07);
  while (h) {
    uint8_t f = (uint8_t)8-(y&(uint8_t)0x07);
    if (h<f) {
      uint8_t mask = ((uint8_t)0xff) >> (uint8_t)(f-h);
      data = data & mask;
    }
    //draw row
    for (uint8_t i=0; i<w; ++i) {
      if ((i&0x07)==0) {
        gfx_load_tile_P(i/8, iy+f, w, h, xbm);
      }
      uint8_t image_data = gfx_tile[i&0x07];
      image_data &= data;
      gfx_display_write2(image_data, data);

    }

    if (f>h) {
      f = h;
    }
    h = h-f;
    y = y+f;
    iy = iy+f;
    gfx_move(x, y);
    data = (uint8_t)0xff;
  }
}


void gfx_pixel(uint8_t val) {
  uint8_t data = 0x01 << (gfx_y%8);
  if (val) {
    gfx_display_write2(data, data);
  } else {
    gfx_display_write2(data, 0);
  }
}


static void gfx_lineToBase(uint8_t x1, uint8_t y1) {
  uint8_t x = gfx_x;
  uint8_t y = gfx_y;
  uint8_t dx = 0;
  uint8_t dy = 0;
  int8_t xm = 0;
  int8_t ym = 0;
  int8_t count;

  if (gfx_x>x1) {
    xm = -1;
    dx = gfx_x-x1;
  } else if (gfx_x<x1) {
    xm = 1;
    dx = x1-gfx_x;
  }

  if (gfx_y>y1) {
    ym = -1;
    dy = gfx_y-y1;
  } else if (gfx_y<y1) {
    ym = 1;
    dy = y1-gfx_y;
  }

  // Draw start pixel
  gfx_pixel(1);

  if (dx>dy) {
    // less than 45 deg
    count = -(int8_t)(dx/2);

    while (x!=x1) {
      count += dy;
      x += xm;

      if(count>0) {
        count -= dx;
        y += ym;
      }

      gfx_move(x, y);
      gfx_pixel(1);
    }
  } else {
    // more than 45 deg
    count = -(int8_t)(dy/2);

    while (y!=y1) {
      count += dx;
      y += ym;

      if(count>0) {
        count -= dy;
        x += xm;
      }

      gfx_move(x, y);
      gfx_pixel(1);
    }
  }
}


void gfx_lineTo(uint8_t x, uint8_t y) {
  if (x==gfx_x) {
    if (y>gfx_y) {
      gfx_vline(y-gfx_y);
    } else {
      uint8_t l=gfx_y-y;
      gfx_move(x, y);
      gfx_vline(l);
    }
  } else if (y==gfx_y) {
    if (x>gfx_x) {
      gfx_hline(x-gfx_x);
    } else {
      uint8_t l=gfx_x-x;
      gfx_move(x, y);
      gfx_hline(l);
    }
  } else {
    gfx_lineToBase(x, y);
  }
  gfx_move(x, y);
}


void gfx_term_clear() {
  gfx_fill(0);
  gfx_term_row = 0;
  gfx_term_col = 0;
}


void gfx_term_goto(uint8_t col, uint8_t row) {
  if ((col<22)&&(row<8)) {
    gfx_term_row = row;
    gfx_term_col = col;
  }
}


void gfx_term_scroll(uint8_t lines) {
  for (uint8_t row=1; row<8; ++row) {
    for (uint8_t col=0; col<16; ++col) {
      gfx_load_disp_tile(col, row);
      gfx_store_disp_tile(col, row-1);
    }
  }
  gfx_reset_tile();
  for (uint8_t row=7; row<8; ++row) {
    for (uint8_t col=0; col<16; ++col) {
      gfx_store_disp_tile(col, row);
    }
    if (gfx_term_row>0) {
      gfx_term_row--;
    }
  }
}


void gfx_term_print(char * text) {
  gfx_set_proportional(0);
  uint8_t c;
  while (c=*text++) {
    if (c==10) {
      gfx_term_col = 0;
      gfx_term_row++;
      if (gfx_term_row==8) {
        gfx_term_scroll(1);
      }
    } else if (c==13) {

    } else {
      if (gfx_term_col>=21) {
        gfx_term_col = 0;
        gfx_term_row++;
        if (gfx_term_row==8) {
          gfx_term_scroll(1);
        }
      }
      gfx_move(gfx_term_col*6, gfx_term_row*8);
      gfx_print_char(c);
      gfx_term_col++;
    }
  }
}


#ifdef __cplusplus
} // extern "C"
#endif
