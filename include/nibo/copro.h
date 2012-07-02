/*  BSD-License:

Copyright (c) 2008 by Nils Springob, nicai-systems, Germany

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

/*! @file    copro.h
 *  @brief   Routinen zum Ansprechen des Coprozessors.
 *  Dieses Modul erlaubt den Zugriff auf die Motoransteuerung und die
 *  Distanzmessung mittels IR, beide Funktionen sind im ATmega88 integriert.
 *  Da die Kommunikation über die SPI Schnittstelle abgewickelt wird, muss vor
 *  der Benutzung der Funktionen @ref spi_init() aufgerufen werden.
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2008-11-16
 */

#ifndef NIBO_COPRO_H_
#define NIBO_COPRO_H_
#if defined(NIBO_USE_COPRO) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Aktuelle Geschwindigkeit des linken Rades in Ticks pro Sekunde, 40 Ticks entsprechen einer Radumdrehung.
 */
extern int16_t copro_speed_l;

/*!
 * Aktuelle Geschwindigkeit des rechten Rades in Ticks pro Sekunde, 40 Ticks entsprechen einer Radumdrehung.
 */
extern int16_t copro_speed_r;

/*!
 * Aktuelle Inkrements des linken Rades in Ticks, 40 Ticks entsprechen einer Radumdrehung.
 */
extern int16_t copro_ticks_l;

/*!
 * Aktuelle Inkrements des rechten Rades in Ticks, 40 Ticks entsprechen einer Radumdrehung.
 */
extern int16_t copro_ticks_r;

/*!
 * Aktueller Strom im linken Motor in Milli-Ampere, der genaue Umrechnungsfaktor ist 1.0742
 */
extern int16_t copro_current_l;

/*!
 * Aktuelle Strom im rechten Motor in Milli-Ampere, der genaue Umrechnungsfaktor ist 1.0742
 */
extern int16_t copro_current_r;

/*!
 * Verbleibende Distanz zum Ziel bei Durchführung von copro_setTargetAbs() bzw. copro_setTargetRel()
 * Die Distanz ist der maximale Wert von den verbleibenden linken und rechten Ticks.
 */
extern uint16_t copro_target_distance;

/*!
 * Aktualisiert alle Daten
 */
uint8_t copro_update();

/*!
 * Motoren anhalten (passiv Stoppen - rollt aus)
 */
uint8_t copro_stop();

/*!
 * Motoren unverzüglich stoppen (aktiv Stoppen - bremst)
 */
uint8_t copro_stopImmediate();

/*!
 * PWM Werte für die beiden Motoren setzen. Die PWM Werte sind bei idealisierter
 * Mechanik und Elektonik proportional zum Drehmoment.
 * @param left Wert für linkes Rad (-1024 ... +1024)
 * @param right Wert für rechets Rad (-1024 ... +1024)
 */
uint8_t copro_setPWM(int16_t left, int16_t right);

/*!
 * Geschwindigkeit für die beiden Motoren setzen. Die Werte werden in Ticks/Sekunde
 * angegeben. 40 Ticks entsprechen einer Radumdrehung. Beispiel:
 \code
   // mit 29 Ticks/s (=10cm/s) vorwärts fahren
   copro_setSpeed(29, 29);
 \endcode
 * @param left Sollwert für linkes Rad
 * @param right Sollwert für rechets Rad
 */
uint8_t copro_setSpeed(int16_t left, int16_t right);

/*!
 * Zielposition für Räder setzen. Die Werte werden in Ticks
 * angegeben. 40 Ticks entsprechen einer Radumdrehung.
 * @param left absoluter Sollwert für linkes Rad
 * @param right absoluter Sollwert für rechets Rad
 * @param speed Wert für maximale Geschwindigkeit
 */
uint8_t copro_setTargetAbs(int16_t left, int16_t right, uint16_t speed);

/*!
 * Zielposition für Räder setzen. Die Werte werden in Ticks relativ zur
 * aktuellen Position angegeben. 40 Ticks entsprechen einer Radumdrehung.
 \code
   // mit 29 Ticks/s (=10cm/s) 290 Ticks (1m) vorwärts fahren
   copro_setTargetRel(290, 290, 29);
 \endcode
 * Beispiel für eine Drehung:
 \code
   // mit 10 Ticks/s (=3cm/s) um 90 Grad drehen:
   copro_setTargetRel(27, -27, 10);
 \endcode
 * @param left relativer Sollwert für linkes Rad
 * @param right relativer Sollwert für rechets Rad
 * @param speed Wert für maximale Geschwindigkeit
 */
uint8_t copro_setTargetRel(int16_t left, int16_t right, uint16_t speed);

/*!
 * Regelungsparameter für @see copro_setTarget, @see copro_setTargetRel und @see copro_setSpeed setzen. 
 * Die Standardwerte sind im Normalfall ausreichend
 * Die Werte werden im EEPROM gespeichert. Weitere Infos im Wiki unter
 * http://www.nibo-roboter.de/wiki/Motorcontroller-Firmware
 * @param ki Integralanteil, Standardwert: 6
 * @param kp Proportionalanteil, Standardwert: 5
 * @param kd Differentialanteil, Standardwert: 7
 */
uint8_t copro_setSpeedParameters(int8_t kp, int8_t ki, int8_t kd);

/*!
 * Regelungsparameter setzen.
 * @see copro_setTarget und @see copro_setTargetRel verwenden jetzt die selben Parameter wie @see copro_setSpeed !
 * @param ki Integralanteil
 * @param kp Proportionalanteil
 * @param kd Differentialanteil
 * \deprecated
 */
uint8_t copro_setPositionParameters(int8_t ki, int8_t kp, int8_t kd);

/*!
 * Odometriewerte auf angegebene Werte zurücksetzen
 * @param left Wert für linkes Rad
 * @param right Wert für rechets Rad
 */
uint8_t copro_resetOdometry(int16_t left, int16_t right);



enum {
  COPRO_IR_PASSIVE = 0,
  COPRO_IR_RC5TX   = 1,
  COPRO_IR_MEASURE = 2,
};

/*!
 * Aktueller Modus des IR-Controllers
 */
extern uint8_t copro_ir_mode;

/*!
 * Aktuelle Reflexionswerte der Distanzsensoren
 */
extern uint16_t copro_distance[5];

/*!
 * Zuletzt empfangenes RC5 Kommando
 */
extern uint16_t copro_rc5_cmd;

/*!
 * Aussenden von IR-Licht stoppen, nur Empfang
 */
uint8_t copro_ir_stop();

/*!
 * Reflexionsmessung starten
 */
uint8_t copro_ir_startMeasure();


/*!
 * Aussenden von IR-Licht stoppen und einen RC5 Code übertragen
 * RC5Code: [SXTAAAAACCCCCC] - 14 bit
 * S: Startbit, immer 1
 * X: Command Extension Bit, 1: Command 0-63, 0: Command 64-127
 * T: Toggle Bit, wechselt bei jedem neuen Tastendruck zwischen 0 und 1
 * AAAAA: 5 Address Bits (0-32)
 * CCCCCC: 6 Command Bits (0-63) bzw. (64-127)
 * @param code RC5 Code 0x0000 - 0x1fff, 13 Bit - das Startbit wird automatisch hinzugefügt
 */
uint8_t copro_transmitRC5(uint16_t code);






#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_COPRO
#endif // NIBO_COPRO_H_
