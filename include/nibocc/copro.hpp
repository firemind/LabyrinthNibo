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

/*! @file    copro.hpp
 *  @brief   Routinen zum Ansprechen des Mikrocontrollers zur Motorsteuerung.
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBOCC_COPRO_HPP_
#define NIBOCC_COPRO_HPP_
#if defined(NIBOCC_USE_COPRO) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/copro.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zum Ansprechen des Mikrocontrollers zur Motorsteuerung.
 */
class Copro
{
  public:
    /*!
     * Aktuelle Geschwindigkeit des linken Rades
     */
    static int16_t get_speed_l() {return ::copro_speed_l;}

    /*!
     * Aktuelle Geschwindigkeit des rechten Rades
     */
    static int16_t get_speed_r() {return ::copro_speed_r;}

    /*!
     * Aktuelle Inkrements des linken Rades
     */
    static int16_t get_ticks_l() {return ::copro_ticks_l;}

    /*!
     * Aktuelle Inkrements des rechten Rades
     */
    static int16_t get_ticks_r() {return ::copro_ticks_r;}

    /*!
     * Aktueller Strom des linken Motors
     */
    static int16_t get_current_l() {return ::copro_current_l;}

    /*!
     * Aktueller Strom des rechten Motors
     */
    static int16_t get_current_r() {return ::copro_current_r;}

    /*!
     * Aktualisiert alle Daten
     */
    static uint8_t update() {return ::copro_update();}

    /*!
     * Motoren anhalten
     */
    static uint8_t stop() {return ::copro_stop();}

    /*!
     * PWM Werte für die beiden Motoren setzen. Die PWM Werte sind bei idealisierter
     * Mechanik und Elektonik proportional zum Drehmoment.
     * @param left Wert für linkes Rad (-1024 ... +1024)
     * @param right Wert für rechets Rad (-1024 ... +1024)
     */
    static uint8_t set_pwm(int16_t left, int16_t right) {return ::copro_setPWM(left, right);}

    /*!
     * Geschwindigkeit für die beiden Motoren setzen. Die Werte werden in Ticks/Sekunde
     * angegeben. Zwanzig Ticks entsprechen einer Radumdrehung.
     * @param left Sollwert für linkes Rad
     * @param right Sollwert für rechets Rad
     */
    static uint8_t set_speed(int16_t left, int16_t right) {return ::copro_setSpeed(left, right);}

    /*!
     * Zielposition für Räder setzen. Die Werte werden in Ticks
     * angegeben. 40 Ticks entsprechen einer Radumdrehung.
     * @param left absoluter Sollwert für linkes Rad
     * @param right absoluter Sollwert für rechets Rad
     * @param speed Wert für maximale Geschwindigkeit
     */
    static uint8_t set_target_abs(int16_t left, int16_t right, uint16_t speed) {return ::copro_setTargetAbs(left, right, speed);}

    /*!
     * Zielposition für Räder setzen. Die Werte werden in Ticks relativ zur
     * aktuellen Position angegeben. 40 Ticks entsprechen einer Radumdrehung.
     * @param left relativer Sollwert für linkes Rad
     * @param right relativer Sollwert für rechets Rad
     * @param speed Wert für maximale Geschwindigkeit
     */
    static uint8_t set_target_rel(int16_t left, int16_t right, uint16_t speed) {return ::copro_setTargetRel(left, right, speed);}

    /*!
     * Regelungsparameter setzen. Weitere Infos im Wiki unter
     * http://nibo.editthis.info/wiki/Motorcontroller-Firmware
     * @param ki Integralanteil
     * @param kp Proportionalanteil
     * @param kd Differentialanteil
     */
    static uint8_t set_speed_parameters(int8_t ki, int8_t kp, int8_t kd) {return ::copro_setSpeedParameters(ki, kp, kd);}

    /*!
     * Odometriewerte auf angegebene Werte zurücksetzen
     * @param left Wert für linkes Rad
     * @param right Wert für rechets Rad
     */
    static uint8_t reset_odometry(int16_t left, int16_t right) {return ::copro_resetOdometry(left, right);}


    enum {
      PASSIVE = 0,
      RC5TX   = 1,
      MEASURE = 2,
    };
    /*!
     * Aktueller Modus des IR-Controllers
     */
    static uint8_t get_ir_mode() {return ::copro_ir_mode;}

    /*!
     * Aktuelle Reflexionswerte der Distanzsensoren
     */
    static uint8_t get_distance(uint8_t id) {return ::copro_distance[id];}

    /*!
     * Zuletzt empfangenes RC5 Kommando
     */
    static uint16_t get_rc5_cmd() {return ::copro_rc5_cmd;}

    /*!
     * Aussenden von IR-Licht stoppen, nur Empfang
     */
    static uint8_t ir_stop() {return ::copro_ir_stop();}

    /*!
     * Reflexionsmessung starten
     */
    static uint8_t ir_start_measure() {return ::copro_ir_startMeasure();}

    /*!
     * RC5 Code übertragen
     * @param rc5 RC5 Code
     */
    static uint8_t transmit_rc5(uint16_t rc5) {return ::copro_transmitRC5(rc5);}



};

} // namespace
#endif // NIBOCC_USE_COPRO
#endif // NIBOCC_COPRO_HPP_
