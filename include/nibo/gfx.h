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

/*! @file    gfx.h
 *  @brief   Routinen zur Ansteuerung eines LC-Grafik-Displays
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 *
 *  Dieser Teil stellt Funktionen zur Textausgabe auf dem Grafikdisplay zur
 *  Verfügung.
 *  Alle Funktionen arbeiten blockierend, das heisst sie kehren erst zurück
 *  wenn die Daten übertragen wurden.
 */

#ifndef NIBO_GFX_H_
#define NIBO_GFX_H_
#if defined(NIBO_USE_GFX) || defined (DOXYGEN)

#include <stdint.h>
#include <avr/pgmspace.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
  GFX_DM_JAM1=0,
  GFX_DM_JAM2=1,
  GFX_DM_COMP=2,
  GFX_DM_JAM1_INV=4,
  GFX_DM_JAM2_INV=5,
  GFX_DM_COMP_INV=6
};

/*!
 * Initialisiert das Grafik Display. Die Funktion @ref display_init() muss
 * vorher aufgerufen werden!
 */
void gfx_init();

/*!
 * Liefert aktuelle X Position des Grafikcursors
 */
uint8_t gfx_get_x();

/*!
 * Liefert aktuelle Y Position des Grafikcursors
 */
uint8_t gfx_get_y();

/*!
 * Setzt Grafikcursor auf angegebene Position
 * @param x X-Position des Cursors (0..127)
 * @param y Y-Position des Cursors (0..63)
 */
void gfx_move(uint8_t x, uint8_t y);

/*!
 * Setzt den Zeichenmodus
 * @param mode Zeichenmodus
 */
void gfx_draw_mode(uint8_t mode);

/*!
 * Fulle gesamten Bildschirm mit angegebener Bitfolge
 * @param val Bitmaske
 */
void gfx_fill(uint8_t val);

/*!
 * Liefert Breite in Pixeln für angegebenes Zeichen
 * @param ch Zeichen
 * @return Breite in Pixeln
 */
uint8_t gfx_char_width(char ch);

/*!
 * Liefert Breite in Pixeln für Nullterminierten String
 * @param txt Nullterminierter String
 * @return Breite in Pixeln
 */
uint8_t gfx_text_width(const char *txt);

/*!
 * Liefert Breite in Pixeln für Nullterminierten String im FLASH
 * @param txt Nullterminierter String im FLASH
 * @return Breite in Pixeln
 */
uint8_t gfx_text_width_P(PGM_P txt);

/*!
 * Gibt angegebenes Zeichen an der aktuellen Grafikposition aus.
 * @param c Zeichen
 */
void gfx_print_char(char c);

/*!
 * Gibt angegebenen Nullterminierten String an der aktuellen Grafikposition aus.
 * @param txt Nullterminierter String
 */
void gfx_print_text(const char *txt);

/*!
 * Gibt angegebenen Nullterminierten String im FLASH an der aktuellen Grafikposition aus.
 * @param txt Nullterminierter String im FLASH
 */
void gfx_print_text_P(PGM_P txt);

/*!
 * Gibt angegebenes XBM Bild im RAM an der aktuellen Grafikposition aus.
 * @param width Breite
 * @param height Höhe
 * @param data XBM Daten im RAM
 */
void gfx_draw_xbm(uint8_t width, uint8_t height, uint8_t * data);

/*!
 * Gibt angegebenes XBM Bild im Flash an der aktuellen Grafikposition aus.
 * @param width Breite
 * @param height Höhe
 * @param data XBM Daten im Flash
 */
void gfx_draw_xbm_P(uint8_t width, uint8_t height, PGM_P data);

/*!
 * Setzt den Proportionalitaetsmodus.
 * @param prop 0=fix, 1=Proportional
 */
void gfx_set_proportional(uint8_t prop);

/*!
 * Liefert den aktuellen Proportionalitaetsmodus.
 * @return 0=fix, 1=Proportional
 */
uint8_t gfx_get_proportional(void);

/*!
 * Zeichnet eine horizontale Linie von der aktuellen Position nach rechts
 * @param length Länge der Linie
 */
void gfx_hline(uint8_t length);

/*!
 * Zeichnet eine vertikale Linie von der aktuellen Position nach unten
 * @param length Länge der Linie
 */
void gfx_vline(uint8_t length);

/*!
 * Zeichnet ein Rechteck.
 * @param width Breite
 * @param height Höhe
 */
void gfx_box(uint8_t width, uint8_t height);

/*!
 * Zeichnet eine Linie zur angegebenen Position.
 * @param x X-Position des Zielpunktes (0..127)
 * @param y Y-Position des Zielpunktes (0..63)
 */
void gfx_lineTo(uint8_t x, uint8_t y);

/*!
 * Setzt bzw. löscht den Pixel an der aktuellen Position.
 * @param value 0: löschen, anderer Wert: setzen
 */
void gfx_pixel(uint8_t value);

/*!
 * Löscht den Bildschirm und setzt den Terminal-Cursor an die Position (0/0).
 */
void gfx_term_clear();

/*!
 * Setzt den Terminal-Cursor an die angegebene Position.
 * @param col Spalte
 * @param col Zeile
 */
void gfx_term_goto(uint8_t col, uint8_t row);

/*!
 * Scrollt den Bildschirm um die angegebene Zeilenanzahl.
 * @param lines Anzahl der Zeilen
 */
void gfx_term_scroll(uint8_t lines);

/*!
 * Gibt den Text im Terminal-Modus aus.
 * @param text Nullterminierter String im RAM
 */
void gfx_term_print(char * text);

/*!
 * Gibt den Text im Terminal-Modus aus.
 * @param text Nullterminierter String im FLASH
 */
void gfx_term_print_P(PGM_P text);


#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_GFX
#endif // NIBO_GFX_H_

