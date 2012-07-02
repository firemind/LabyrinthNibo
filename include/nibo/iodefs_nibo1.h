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

/*! @file    iodefs_nibo1.h
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

/*! Red LEDs */
#define IO_LEDS_RED_PORT  PORTE
#define IO_LEDS_RED_MASK  0xfc
#define IO_LEDS_RED_DDR   DDRE

/*! Green LEDs */
#define IO_LEDS_GREEN_PORT  PORTC
#define IO_LEDS_GREEN_MASK  0xfc
#define IO_LEDS_GREEN_DDR   DDRC

/*! White LED pair */
#define IO_LED_WHITE_PORT  PORTB
#define IO_LED_WHITE_BIT   5
#define IO_LED_WHITE_DDR   DDRB

/*! Display illumination */
#define IO_DISP_LIGHT_PORT  PORTB
#define IO_DISP_LIGHT_BIT   6
#define IO_DISP_LIGHT_DDR   DDRB



/*! Line and floor sensor */
#define IO_LINE_FLOOR_EN IO_LINE_FLOOR_EN
#define IO_LINE_FLOOR_EN_PORT  PORTG
#define IO_LINE_FLOOR_EN_BIT   0
#define IO_LINE_FLOOR_EN_DDR   DDRG


/*! Analog channels */
#define AN_FLOOR_R    0        /*!< ADC-PIN floorsensor right */
#define AN_FLOOR_L    1        /*!< ADC-PIN floorsensor left  */
#define AN_LINE_L     2        /*!< ADC-PIN liniesensor left  */
#define AN_LINE_R     3        /*!< ADC-PIN liniesensor right */
#define AN_VBAT       7        /*!< ADC-PIN battery voltage   */


/*! Display register select */
#define IO_DISPLAY_RS       IO_DISPLAY_RS
#define IO_DISPLAY_RS_PORT  PORTG
#define IO_DISPLAY_RS_BIT   3
#define IO_DISPLAY_RS_DDR   DDRG

/*! Display read/write */
#define IO_DISPLAY_RW       IO_DISPLAY_RW
#define IO_DISPLAY_RW_PORT  PORTG
#define IO_DISPLAY_RW_BIT   4
#define IO_DISPLAY_RW_DDR   DDRG

/*! Display enable */
#define IO_DISPLAY_EN       IO_DISPLAY_EN
#define IO_DISPLAY_EN_PORT  PORTG
#define IO_DISPLAY_EN_BIT   2
#define IO_DISPLAY_EN_DDR   DDRG

/*! Display chip select 1 */
#define IO_DISPLAY_CS1       IO_DISPLAY_CS1
#define IO_DISPLAY_CS1_PORT  PORTB
#define IO_DISPLAY_CS1_BIT   4
#define IO_DISPLAY_CS1_DDR   DDRB

/*! Display chip select 2 */
#define IO_DISPLAY_CS2       IO_DISPLAY_CS2
#define IO_DISPLAY_CS2_PORT  PORTB
#define IO_DISPLAY_CS2_BIT   7
#define IO_DISPLAY_CS2_DDR   DDRB

/*! Display reset */
#define IO_DISPLAY_RST       IO_DISPLAY_RST
#define IO_DISPLAY_RST_PORT  PORTB
#define IO_DISPLAY_RST_BIT   0
#define IO_DISPLAY_RST_DDR   DDRB

/*! Display data port */
#define IO_DISPLAY_PORT  PORTA
#define IO_DISPLAY_PIN   PINA
#define IO_DISPLAY_DDR   DDRA


/*! Reset Ext A */
#define IO_RESET_A_PORT  PORTD
#define IO_RESET_A_BIT   4
#define IO_RESET_A_DDR   DDRD

/*! Reset Ext B */
#define IO_RESET_B_PORT  PORTD
#define IO_RESET_B_BIT   5
#define IO_RESET_B_DDR   DDRD

/*! Reset IC5 / IRCO */
#define IO_RESET_5_PORT  PORTD
#define IO_RESET_5_BIT   6
#define IO_RESET_5_DDR   DDRD

/*! Reset IC3 / MOTCO */
#define IO_RESET_3_PORT  PORTD
#define IO_RESET_3_BIT   7
#define IO_RESET_3_DDR   DDRD


/*! ISP SCK */
#define IO_ISP_SCK_PORT  PORTB
#define IO_ISP_SCK_BIT   1
#define IO_ISP_SCK_DDR   DDRB

/*! ISP MOSI */
#define IO_ISP_MOSI_PORT  PORTB
#define IO_ISP_MOSI_BIT   2
#define IO_ISP_MOSI_DDR   DDRB

/*! ISP MISO */
#define IO_ISP_MISO_PORT  PORTB
#define IO_ISP_MISO_PIN   PINB
#define IO_ISP_MISO_BIT   3
#define IO_ISP_MISO_DDR   DDRB


/*! I2C Interface configuration */
#define I2C_BUF_SIZE 16
//#define I2C_TWBR_INIT 72   /* 100 kHz */
#define I2C_TWBR_INIT 12   /* 400 kHz */


/*! I2C Bus ID for IRCO */
#define IRCO_I2C_ID 42

/*! I2C Bus ID for MOTCO */
#define MOTCO_I2C_ID 44

