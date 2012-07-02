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

/*! @file    motco.h
 *  @brief   Routinen zum Ansprechen des Mikrocontrollers zur Motorsteuerung.
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBO_MOTCO_H_
#define NIBO_MOTCO_H_
#if defined(NIBO_USE_MOTCO) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Aktuelle Geschwindigkeit des linken Rades
 */
extern int16_t motco_speed_l;

/*!
 * Aktuelle Geschwindigkeit des rechten Rades
 */
extern int16_t motco_speed_r;

/*!
 * Aktuelle Inkrements des linken Rades
 */
extern int16_t motco_ticks_l;

/*!
 * Aktuelle Inkrements des rechten Rades
 */
extern int16_t motco_ticks_r;

/*!
 * Aktualisiert alle Daten
 */
uint8_t motco_update();

/*!
 * Motoren anhalten (passiv Stoppen - rollt aus)
 */
uint8_t motco_stop();

/*!
 * Motoren unverzüglich stoppen (aktiv Stoppen - bremst)
 */
uint8_t motco_stopImmediate();


/*!
 * PWM Werte für die beiden Motoren setzen. Die PWM Werte sind bei idealisierter
 * Mechanik und Elektonik proportional zum Drehmoment.
 * @param left Wert für linkes Rad (-1024 ... +1024)
 * @param right Wert für rechets Rad (-1024 ... +1024)
 */
uint8_t motco_setPWM(int16_t left, int16_t right);

/*!
 * Geschwindigkeit für die beiden Motoren setzen. Die Werte werden in Ticks/Sekunde
 * angegeben. Zwanzig Ticks entsprechen einer Radumdrehung.
 * @param left Sollwert für linkes Rad
 * @param right Sollwert für rechets Rad
 */
uint8_t motco_setSpeed(int16_t left, int16_t right);

/*!
 * Zielposition für Räder setzen. Die Werte werden in Ticks
 * angegeben. Zwanzig Ticks entsprechen einer Radumdrehung.
 * @param left Sollwert für linkes Rad
 * @param right Sollwert für rechets Rad
 * @param speed Sollwert für maximale Geschwindigkeit
 */
uint8_t motco_setPosition(int16_t left, int16_t right, uint16_t speed);

/*!
 * Regelungsparameter setzen. Weitere Infos im Wiki unter
 * http://www.nibo-roboter.de/wiki/Motorcontroller-Firmware
 * @param ki Integralanteil
 * @param kp Proportionalanteil
 * @param kd Differentialanteil
 */
uint8_t motco_setSpeedParameters(int8_t ki, int8_t kp, int8_t kd);

/*!
 * Regelungsparameter setzen. Weitere Infos im Wiki unter
 * http://www.nibo-roboter.de/wiki/Motorcontroller-Firmware
 * @param ki Integralanteil
 * @param kp Proportionalanteil
 * @param kd Differentialanteil
 */
uint8_t motco_setPositionParameters(int8_t ki, int8_t kp, int8_t kd);

/*!
 * Odometriewerte auf angegebene Werte zurücksetzen
 * @param left Wert für linkes Rad
 * @param right Wert für rechets Rad
 */
uint8_t motco_resetOdometry(int16_t left, int16_t right);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_MOTCO
#endif // NIBO_MOTCO_H_
