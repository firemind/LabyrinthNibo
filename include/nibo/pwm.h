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

/*! @file    pwm.h
 *  @brief   Funktionen zur Erzeugung der PWM-Signale und f&uuml;r die Systemzeit
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBO_PWM_H_
#define NIBO_PWM_H_
#if defined(NIBO_USE_PWM) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * PWM Wert für den Kanal A aus dem Bereich 0 bis 1023
 */
extern uint16_t pwm_channel_A;

/*!
 * PWM Wert für den Kanal B aus dem Bereich 0 bis 1023
 */
extern uint16_t pwm_channel_B;

#ifdef _NIBO_2_
/*!
 * PWM Wert für den Kanal C aus dem Bereich 0 bis 1023
 */
extern uint16_t pwm_channel_C;
#endif

/*!
 * Zähler für die Zeit in Milisekunden, zählt bis 1000(=1s) in 1ms Schritten.
 */
extern uint16_t pwm_timer_ms;

/*!
 * Zähler für die Zeit in Sekunden, zählt die Sekunden seit dem Systemstart.
 */
extern uint32_t pwm_timer_sec;

/*!
 * Initialiserung für PWM und Zeitberechnung.
 */
void pwm_init(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_PWM
#endif // NIBO_PWM_H_
