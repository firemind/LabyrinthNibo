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

#ifndef NIBOCONFIG_H_
#define NIBOCONFIG_H_

#include <avr/version.h>

#if (__AVR_LIBC_VERSION__ < 10607UL)
#error "Minimum AVR Libc version: 1.6.7 (WinAVR-20100110)"
#endif


// low level drivers
#define NIBO_USE_DISPLAY
#define NIBO_USE_PWM
#define NIBO_USE_DELAY
#define NIBO_USE_I2CMASTER
#define NIBO_USE_ADC
#define NIBO_USE_UART0
#ifdef _NIBO_2_
#define NIBO_USE_SPISLAVE
#define NIBO_USE_SOUND
#define NIBO_USE_UART1
#endif

// high level drivers
#define NIBO_USE_TEXT
#define NIBO_USE_GFX
#define NIBO_USE_LEDS
#ifdef _NIBO_2_
#define NIBO_USE_COPRO
#else
#define NIBO_USE_IRCO
#define NIBO_USE_MOTCO
#endif
#define NIBO_USE_FLOOR
#define NIBO_USE_BOT
#define NIBO_USE_XBEE

#endif // NIBOCONFIG_H_

/******************************************************************************/

/*! @file    niboconfig.h
 *  @brief   Konfiguration der Nibo-Bibliothek
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

/*! \def NIBO_USE_DISPLAY
 *  Define this macro to use low level Display module. Depends on @ref NIBO_USE_DELAY
 */

/*! \def NIBO_USE_PWM
 *  Define this macro to use low level PWM module.
 */

/*! \def NIBO_USE_DELAY
 *  Define this macro to use low level Delay module.
 */

/*! \def NIBO_USE_I2CMASTER
 *  Define this macro to use low level I2C master module. Depends on @ref NIBO_USE_DELAY
 */

/*! \def NIBO_USE_ADC
 *  Define this macro to use low level adc module.
 */

/*! \def NIBO_USE_TEXT
 *  Define this macro to use high level text-display module. Depends on @ref NIBO_USE_DISPLAY
 */

/*! \def NIBO_USE_GFX
 *  Define this macro to use high level graphic-display module. Depends on @ref NIBO_USE_DISPLAY
 */

/*! \def NIBO_USE_LEDS
 *  Define this macro to use high level LEDs module. Depends on @ref NIBO_USE_PWM
 */

/*! \def NIBO_USE_IRCO
 *  Define this macro to use high level IR-controller module. Depends on @ref NIBO_USE_I2CMASTER
 */

/*! \def NIBO_USE_MOTCO
 *  Define this macro to use high level motor-controller module. Depends on @ref NIBO_USE_I2CMASTER
 */

/*! \def NIBO_USE_FLOOR
 *  Define this macro to use high level floor sensor module. Depends on @ref NIBO_USE_ADC
 */

/*! \def NIBO_USE_BOT
 *  Define this macro to use high level bot module. Depends on @ref NIBO_USE_ADC
 */
