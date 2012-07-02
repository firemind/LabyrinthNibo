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

/*! @file    nds3.h
 *  @brief   Routinen zur Kommunikation mit dem NDS3 Modul.
 *  Da die Kommunikation über die I²C Schnittstelle abgewickelt wird, muss vor
 *  der Benutzung der Funktionen @ref i2c_init() aufgerufen werden.
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2009-07-02
 *
 */

#ifndef NIBO_NDS3_H_
#define NIBO_NDS3_H_
#if defined(NIBO_USE_NDS3) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Initialisierung des NDS3
 */
void nds3_init();

/*!
 * Setzt das aktuelle Kommando zurück, nds3_get_busy() wird anschliessend 0 zurück geben.
 */
void nds3_reset();

/*!
 * Startet eine Drehung in die angegeben Richtung.
 * Die Drehung ist abgeschlossen, sobald nds3_get_busy() 0 zurück gibt.
 * @param dir Richtung in Grad [0 ... 180]
 */
void nds3_move(uint8_t dir);

/*!
 * Startet einen Scan. Der Sensor wird von der aktuellen Richtung mit der
 * Schritweite Step zur angegebenen Richtung gedreht.
 * Die Messung ist abgeschlossen, sobald nds3_get_busy() 0 zurück gibt.
 * @param dir Richtung in Grad [0 ... 180]
 * @param step Schrittweite in Grad
 */
void nds3_measure(uint8_t dir, int8_t step);

extern uint8_t nds3_distances[181];

/*!
 * Liefert die Ergebnisse des letzten Scans zurück. Dir ist die Startrichtung,
 * step die Schrittweite. Beispiel:
 * \code
   // Scan von 0 bis 180 Grad in 5 Grad Schritten
   nds3_move(0);
   while(nds3_get_busy());
   nds3_measure(180, 5);
   while(nds3_get_busy());
   nds3_read(0, 5);
   \endcode
 * @param dir Richtung in Grad [-90 ... 90]
 * @param step Schrittweite in Grad
 */
void nds3_read(uint8_t dir, uint8_t step);

/*!
 * Liefert die aktuelle Position
 * @return Richtung in Grad [0 ... 180]
 */
uint8_t nds3_get_pos();

/*!
 * Liefert die aktuell gemessene Distanz
 * @return Distanz in cm
 */
uint8_t nds3_get_dist();

/*!
 * Prüfen, ob das NDS3 Modul ein (Bewegungs-)Kommando bearbeitet.
 * @returns 0: untätig
 */
uint8_t nds3_get_busy();


/*!
 * Data Direction Register (DDR)
 */
uint8_t nds3_ddr;

/*!
 * Ausgabe Register (PORT)
 */
uint8_t nds3_port;

/*!
 * Port Input Register (PIN)
 */
uint8_t nds3_pin;

/*!
 * Akualisiere Data Direction Register (DDR) des NDS3
 */
void nds3_ddr_commit();

/*!
 * Akualisiere Ausgabe Register (PORT) des NDS3
 */
void nds3_port_commit();

/*!
 * Aktualisiere lokales Port Input Register (PIN)
 */
void nds3_pin_update();

/*!
 * Liefert den Wert des Analog-Eingangs Idx zurück
 */
uint16_t nds3_get_analog(uint8_t idx);

/*!
 * Rohpulsweite des Servos in Microsekunden setzen
 */
void nds3_move_servo(uint16_t usecs);

/*!
 * Maximale Pulsweite des Servos in Microsekunden
 */
uint16_t nds3_servocalib_hi;

/*!
 * Mittlere Pulsweite des Servos in Microsekunden
 */
uint16_t nds3_servocalib_mid;

/*!
 * Minimale Pulsweite des Servos in Microsekunden
 */
uint16_t nds3_servocalib_lo;

/*!
 * Übertrage Servo-Kalibrierwerte vom NDS3 in die Variablen
 */
void nds3_servocalib_update();

/*!
 * Übertrage Servo-Kalibrierwerte aus den Variablen zum NDS3
 */
void nds3_servocalib_commit();

/*!
 * Voltage - Offset für Distanzmessung (* 5.0/1024 V)
 */
uint16_t nds3_distcalib_v0;

/*!
 * Voltage - Offset für Distanzmessung (* 1 cm)
 */
uint16_t nds3_distcalib_d0;

/*!
 * Voltage - Faktor
 */
uint16_t nds3_distcalib_alpha;

/*!
 * Übertrage Distanz-Kalibrierwerte vom NDS3 in die Variablen.
 * Die Werte werden im EEPROM gespeichert.
 * Die Formel lautet: d = alpha/(v-v0) - d
 * d: Distanz (*cm)
 * v: Spannung (*5.0/1024 V)
 */
void nds3_distcalib_update();

/*!
 * Übertrage Distanz-Kalibrierwerte aus den Variablen zum NDS3
 */
void nds3_distcalib_commit();


#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_NDS3
#endif // NIBO_NDS3_H_
