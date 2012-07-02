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

/*! @file    irco.hpp
 *  @brief   Routinen zum Ansprechen des Infrarot Mikrocontrollers
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBOCC_IRCO_HPP_
#define NIBOCC_IRCO_HPP_
#if defined(NIBOCC_USE_IRCO) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/irco.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zum Ansprechen des Infrarot Mikrocontrollers
 */
class IrCo
{
  public:
    enum {
      PASSIVE = 0,
      RC5TX   = 1,
      MEASURE = 2,
    };
    /*!
     * Aktueller Modus des IR-Controllers
     */
    static uint8_t get_mode() {return ::irco_mode;}

    /*!
     * Aktuelle Reflexionswerte der Distanzsensoren
     */
    static uint8_t get_distance(uint8_t id) {return ::irco_distance[id];}

    /*!
     * Zuletzt empfangenes RC5 Kommando
     */
    static uint16_t get_rc5_cmd() {return ::irco_rc5_cmd;}

    /*!
     * Daten des IRCO aktualisieren
     */
    static uint8_t update() {return ::irco_update();}

    /*!
     * Aussenden von IR-Licht stoppen, nur Empfang
     */
    static uint8_t stop() {return ::irco_stop();}

    /*!
     * Reflexionsmessung starten
     */
    static uint8_t start_measure() {return ::irco_startMeasure();}

    /*!
     * RC5 Code übertragen
     * @param rc5 RC5 Code
     */
    static uint8_t transmit_rc5(uint16_t rc5) {return ::irco_transmitRC5(rc5);}

};

} // namespace
#endif // NIBOCC_USE_IRCO
#endif // NIBOCC_IRCO_HPP_
