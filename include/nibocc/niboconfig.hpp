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

#ifndef NIBOCONFIG_HPP_
#define NIBOCONFIG_HPP_

#include "nibo/niboconfig.h"
#include "nibo/iodefs.h"

// low level drivers
#define NIBOCC_USE_DISPLAY
#define NIBOCC_USE_PWM
#define NIBOCC_USE_DELAY
#define NIBOCC_USE_I2CMASTER
#define NIBOCC_USE_ADC
#define NIBOCC_USE_UART0
#ifdef _NIBO_2_
#define NIBOCC_USE_SPI
#define NIBOCC_USE_SOUND
#define NIBOCC_USE_UART1
#endif

// high level drivers
#define NIBOCC_USE_TEXTDISPLAY
#define NIBOCC_USE_GRAPHICDISPLAY
#define NIBOCC_USE_LEDS
#ifdef _NIBO_2_
#define NIBOCC_USE_COPRO
#else
#define NIBOCC_USE_IRCO
#define NIBOCC_USE_MOTCO
#endif
#define NIBOCC_USE_FLOOR
#define NIBOCC_USE_BOT
#define NIBOCC_USE_XBEE

#endif // NIBOCONFIG_HPP_

/******************************************************************************/

/*! @file    niboconfig.h
 *  @brief   Konfiguration der Nibo-Bibliothek
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

/*! \def NIBOCC_USE_DISPLAY
 *  Define this macro to use low level Display module. Depends on @ref NIBO_USE_DELAY
 */

/*! \def NIBOCC_USE_PWM
 *  Define this macro to use low level PWM module.
 */

/*! \def NIBOCC_USE_DELAY
 *  Define this macro to use low level Delay module.
 */

/*! \def NIBOCC_USE_I2CMASTER
 *  Define this macro to use low level I2C master module. Depends on @ref NIBO_USE_DELAY
 */

/*! \def NIBOCC_USE_ADC
 *  Define this macro to use low level adc module.
 */

/*! \def NIBOCC_USE_TEXTDISPLAY
 *  Define this macro to use high level text-display module. Depends on @ref NIBO_USE_DISPLAY
 */

/*! \def NIBOCC_USE_GRAFICDISPLAY
 *  Define this macro to use high level graphic-display module. Depends on @ref NIBO_USE_DISPLAY
 */

/*! \def NIBOCC_USE_LEDS
 *  Define this macro to use high level LEDs module. Depends on @ref NIBO_USE_PWM
 */

/*! \def NIBOCC_USE_IRCO
 *  Define this macro to use high level IR-controller module. Depends on @ref NIBO_USE_I2CMASTER
 */

/*! \def NIBOCC_USE_MOTCO
 *  Define this macro to use high level motor-controller module. Depends on @ref NIBO_USE_I2CMASTER
 */

/*! \def NIBOCC_USE_FLOOR
 *  Define this macro to use high level floor sensor module. Depends on @ref NIBO_USE_ADC
 */

/*! \def NIBOCC_USE_BOT
 *  Define this macro to use high level bot module. Depends on @ref NIBO_USE_ADC
 */
