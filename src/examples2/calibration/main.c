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

/* Dieses Programm dient zur Kalibrierung der Bodensensoren. */

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"
#include "nibo/delay.h"
#include "nibo/display.h"
#include "nibo/gfx.h"
#include "nibo/spi.h"
#include "nibo/copro.h"
#include "nibo/adc.h"
#include "nibo/floor.h"
#include "nibo/leds.h"
#include "nibo/pwm.h"
#include "nibo/bot.h"
#include "nibo/sound.h"

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

uint8_t mode;

float volt_flt = 10.0;

/* Diese Funktion gibt ein Byte (8-Bit Wert) in Hexadezimaler Form (zwei Hex-Ziffern) auf dem Display aus */
void print_hex (uint8_t val) { 
  char c1=val/16;
  char c2=val%16;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c1);
  gfx_print_char(c2);
}

/* Diese Funktion gibt ein 12-Bit Wert in Hexadezimaler Form (drei Hex-Ziffern) auf dem Display aus */
void print_hex12 (uint16_t val) {
  char c0=val/256;
  val %= 256;
  char c1=val/16;
  char c2=val%16;
  if (c0<10) c0+='0'; else c0+='a'-10;
  if (c1<10) c1+='0'; else c1+='a'-10;
  if (c2<10) c2+='0'; else c2+='a'-10;
  gfx_print_char(c0);
  gfx_print_char(c1);
  gfx_print_char(c2);
}

/* Diese Funktion gibt ein 16-Bit Wert in Hexadezimaler Form (vier Hex-Ziffern) auf dem Display aus */
void print_hex16 (uint16_t val) {
  print_hex(HIBYTE(val));
  print_hex(LOBYTE(val));
}


/* Zeige aktuelle Funktion auf dem Display an */
void print_msg(uint8_t mode) {
  gfx_move(20, 45);
  gfx_set_proportional(1);
  switch (mode) {
    case 0: gfx_print_text("Press S3 for  "); break;
    case 1: gfx_print_text("Press S3 for  "); break;
    case 2: gfx_print_text("Press S3 to  "); break;
  };
  gfx_move(20, 55);
  switch (mode) {
    case 0: gfx_print_text("black calibration  "); break;
    case 1: gfx_print_text("white calibration  "); break;
    case 2: gfx_print_text("store calibration  "); break;
  };
}


int main(void) {
  sei(); // enable interrupts
  leds_init();
  pwm_init();
  bot_init();
  spi_init();
  floor_init();
  sound_init();

  if (display_init()!=0) {
    leds_set_displaylight(50);
    if (display_type==2) {
      gfx_init();
    }
  }
  
  gfx_fill(0x00);
  gfx_move(21, 0);
  gfx_set_proportional(1);
  gfx_print_text("Sensor Calibration");
  gfx_set_proportional(0);

  print_msg(0);
  while (1) {
    delay(20);

    // Spannung
    bot_update();
    char text[20];
    float volt = 0.0160 * bot_supply;
    sprintf(text, "%4.1fV ", (double)volt);
    gfx_move(29, 10);
    gfx_set_proportional(1);
    gfx_print_text("Supply: ");
    gfx_set_proportional(0);
    gfx_print_text(text);

    // Spannung filtern: 90% alter Wert, 10% neuer Wert.
    volt_flt = 0.9*volt_flt+0.1*volt;

    // Wenn die gefilterete Spannung unter 9V sinkt sollten die Akkus geladen werden
    if (volt_flt<9.0) {
      gfx_fill(0x00);
      gfx_move(25, 20);
      gfx_set_proportional(1);
      gfx_print_text("Please recharge");
      gfx_move(35, 30);
      gfx_print_text("batteries!");
      gfx_set_proportional(0);
      while(1) {
        leds_set_headlights(0);
        clear_output_bit(IO_RESET_CO);
        //motco_stop();
        leds_set_displaylight(500);
        IO_LEDS_RED_PORT = 0xaa;
        IO_LEDS_GREEN_PORT = 0;
        delay(500);
        //motco_setSpeed(0, 0);
        leds_set_displaylight(0);
        IO_LEDS_RED_PORT = 0x55;
        IO_LEDS_GREEN_PORT = 0;
        delay(500);
      }
    }
    
    // Bodensensoren aktualisieren:
    floor_update();

    uint16_t value_afr = floor_absolute[FLOOR_RIGHT];
    uint16_t value_afl = floor_absolute[FLOOR_LEFT];
    uint16_t value_alr = floor_absolute[LINE_RIGHT];
    uint16_t value_all = floor_absolute[LINE_LEFT];

    uint16_t value_rfr = floor_relative[FLOOR_RIGHT];
    uint16_t value_rfl = floor_relative[FLOOR_LEFT];
    uint16_t value_rlr = floor_relative[LINE_RIGHT];
    uint16_t value_rll = floor_relative[LINE_LEFT];

    // Werte der Bodensensoren auf dem Display ausgeben:
    gfx_move(5, 20);
    print_hex16(value_afr);
    gfx_print_char(' ');
    print_hex16(value_alr);
    gfx_print_char(' ');
    print_hex16(value_all);
    gfx_print_char(' ');
    print_hex16(value_afl);
    
    gfx_move(5, 30);
    print_hex16(value_rfr);
    gfx_print_char(' ');
    print_hex16(value_rlr);
    gfx_print_char(' ');
    print_hex16(value_rll);
    gfx_print_char(' ');
    print_hex16(value_rfl);

    uint8_t ledr = 0x00;
    uint8_t ledg = 0x00;

    // vordere grüne LEDs in Abhängigkeit der Bodensensoren einschalten:
    if (value_rll>0x50) ledg |= 0x10;
    if (value_rlr>0x50) ledg |= 0x20;
    if (value_rfl>0x50) ledg |= 0x08;
    if (value_rfr>0x50) ledg |= 0x40;

    // hintere grüne LEDs in Abhängigkeit des aktuellen Zustands einschalten:
    switch (mode) {
      case 0: ledg |= 0x01; break;
      case 1: ledg |= 0x02; break;
      case 2: ledg |= 0x03; break;
    }

    IO_LEDS_RED_PORT = ledr;
    IO_LEDS_GREEN_PORT = ledg;

    // Wenn der Taster S3 gedrückt wurde, ...
    if (!get_input_bit(IO_INPUT_1)) {
      // ... 20 ms entprellen ...
      delay(20);
      if (!get_input_bit(IO_INPUT_1)) {
        while (!get_input_bit(IO_INPUT_1)) {
          
        }
        // ... und der Taster wieder losgelassen wurde:

        // Modus weiterschalten und mit den roten LEDs die Funktion anzeigen
        mode++;
        switch (mode) {
          case 1:
            IO_LEDS_GREEN_PORT = 0x00;
            IO_LEDS_RED_PORT = 0x01|0x78;
            floor_calibrateBlack();
            break;
          case 2:
            IO_LEDS_GREEN_PORT = 0x00;
            IO_LEDS_RED_PORT = 0x02|0x78;
            floor_calibrateWhite();
            break;
          case 3:
            IO_LEDS_GREEN_PORT = 0x00;
            IO_LEDS_RED_PORT = 0x04|0x78;
            floor_writePersistent();
            mode=0;
            break;
        }
        print_msg(mode);
	// eine 
        delay(1000);
	
      }
    }

  }
}

