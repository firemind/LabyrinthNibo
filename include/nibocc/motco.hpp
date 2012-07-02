//
//  Nibo C drivers - C functions for the Nibo robot
//
//  Copyright (C) 2007 by Nils Springob <nils@nicai-systems.de>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

/*! @file    motco.hpp
 *  @brief   Routinen zum Ansprechen des Mikrocontrollers zur Motorsteuerung.
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBOCC_MOTCO_HPP_
#define NIBOCC_MOTCO_HPP_
#if defined(NIBOCC_USE_MOTCO) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/motco.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zum Ansprechen des Mikrocontrollers zur Motorsteuerung.
 */
class MotCo
{
  public:
    /*!
     * Aktuelle Geschwindigkeit des linken Rades
     */
    static int16_t get_speed_l() {return ::motco_speed_l;}

    /*!
     * Aktuelle Geschwindigkeit des rechten Rades
     */
    static int16_t get_speed_r() {return ::motco_speed_r;}

    /*!
     * Aktuelle Inkrements des linken Rades
     */
    static int16_t get_ticks_l() {return ::motco_ticks_l;}

    /*!
     * Aktuelle Inkrements des rechten Rades
     */
    static int16_t get_ticks_r() {return ::motco_ticks_r;}

    /*!
     * Aktualisiert alle Daten
     */
    static uint8_t update() {return ::motco_update();}

    /*!
     * Motoren anhalten
     */
    static uint8_t stop() {return ::motco_stop();}

    /*!
     * PWM Werte für die beiden Motoren setzen. Die PWM Werte sind bei idealisierter
     * Mechanik und Elektonik proportional zum Drehmoment.
     * @param left Wert für linkes Rad (-1024 ... +1024)
     * @param right Wert für rechets Rad (-1024 ... +1024)
     */
    static uint8_t set_pwm(int16_t left, int16_t right) {return ::motco_setPWM(left, right);}

    /*!
     * Geschwindigkeit für die beiden Motoren setzen. Die Werte werden in Ticks/Sekunde
     * angegeben. Zwanzig Ticks entsprechen einer Radumdrehung.
     * @param left Sollwert für linkes Rad
     * @param right Sollwert für rechets Rad
     */
    static uint8_t set_speed(int16_t left, int16_t right) {return ::motco_setSpeed(left, right);}

    /*!
     * Regelungsparameter setzen. Weitere Infos im Wiki unter
     * http://nibo.editthis.info/wiki/Motorcontroller-Firmware
     * @param ki Integralanteil
     * @param kp Proportionalanteil
     * @param kd Differentialanteil
     */
    static uint8_t set_speed_parameters(int8_t ki, int8_t kp, int8_t kd) {return ::motco_setSpeedParameters(ki, kp, kd);}

    /*!
     * Odometriewerte auf angegebene Werte zurücksetzen
     * @param left Wert für linkes Rad
     * @param right Wert für rechets Rad
     */
    static uint8_t reset_odometry(int16_t left, int16_t right) {return ::motco_resetOdometry(left, right);}

};

} // namespace
#endif // NIBOCC_USE_MOTCO
#endif // NIBOCC_MOTCO_HPP_
