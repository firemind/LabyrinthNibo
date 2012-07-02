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

/*! @file    graphicdisplay.hpp
 *  @brief   Routinen zur Ansteuerung eines LC-Grafik-Displays
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBOCC_GRAPHICDISPLAY_HPP_
#define NIBOCC_GRAPHICDISPLAY_HPP_
#if defined(NIBOCC_USE_GRAPHICDISPLAY) || defined (DOXYGEN)

#include <stdint.h>
#include <utilscc/string.hpp>
#include "nibo/gfx.h"

namespace nibocc {

/*!
 *  @brief Klasse zur Kapselung von Bitmapgrafiken
 */
class Bitmap
{
  public:
    uint8_t width;
    uint8_t height;
    uint8_t data[];
};


/*!
 *  @brief Statische Klasse zur Ansteuerung eines LC-Grafik-Displays
 */
class Graphicdisplay
{
  public:
    enum
    {
      DM_JAM1 = ::GFX_DM_JAM1,
      DM_JAM2 = ::GFX_DM_JAM2,
      DM_COMP = ::GFX_DM_COMP,
      DM_JAM1_INV = ::GFX_DM_JAM1_INV,
      DM_JAM2_INV = ::GFX_DM_JAM2_INV,
      DM_COMP_INV = ::GFX_DM_COMP_INV
    };

    /*!
     * Initialisiert das Grafik Display. Die Methode @ref Display::init() muss
     * vorher aufgerufen werden!
     */
    static void init() {::gfx_init();}

    /*!
     * Liefert aktuelle X Position des Grafikcursors
     */
    static uint8_t get_x() {return ::gfx_get_x();}

    /*!
     * Liefert aktuelle Y Position des Grafikcursors
     */
    static uint8_t get_y() {return ::gfx_get_y();}

    /*!
     * Setzt Grafikcursor auf angegebene Position
     * @param x X-Position des Cursors (0..127)
     * @param y Y-Position des Cursors (0..63)
     */
    static void move(uint8_t x, uint8_t y) {::gfx_move(x, y);}

    /*!
     * Setzt den Zeichenmodus
     * @param mode Zeichenmodus
     */
    static void draw_mode(uint8_t mode) {::gfx_draw_mode(mode);}

    /*!
     * Fulle gesamten Bildschirm mit angegebener Bitfolge
     * @param val Bitmaske
     */
    static void fill(uint8_t val) {::gfx_fill(val);}

    /*!
     * Liefert Breite in Pixeln für angegebenes Zeichen
     * @param ch Zeichen
     * @return Breite in Pixeln
     */
    static uint8_t char_width(char ch) {return ::gfx_char_width(ch);}

    /*!
     * Liefert Breite in Pixeln für Nullterminierten String
     * @param txt Nullterminierter String
     * @return Breite in Pixeln
     */
    static uint8_t text_width(const char *txt) {return ::gfx_text_width(txt);}

    /*!
     * Liefert Breite in Pixeln für Nullterminierten String
     * @param txt Nullterminierter String
     * @return Breite in Pixeln
     */
    static uint8_t text_width(const ::utilscc::string & txt) {return ::gfx_text_width(txt.c_str());}

    /*!
     * Gibt angegebenes Zeichen an der aktuellen Grafikposition aus.
     * @param c Zeichen
     */
    static void print_char(char c) {::gfx_print_char(c);}

    /*!
     * Gibt angegebenen Nullterminierten String an der aktuellen Grafikposition aus.
     * @param txt Nullterminierter String
     */
    static void print_text(const char *txt) {::gfx_print_text(txt);}

    /*!
     * Gibt angegebenen Nullterminierten String an der aktuellen Grafikposition aus.
     * @param txt Nullterminierter String
     */
    static void print_text(const ::utilscc::string & txt) {::gfx_print_text(txt.c_str());}


    /*!
     * Gibt angegebene Bitmap an der aktuellen Grafikposition aus.
     */
    static void draw_bitmap(bitmap_t * bmp) {::gfx_draw_bitmap(bmp);}

    /*!
     * Setzt den Proportionalitaetsmodus.
     * @param prop 0=fix, 1=Proportional
     */
    static void set_proportional(uint8_t prop) {::gfx_set_proportional(prop);}

    /*!
     * Liefert den aktuellen Proportionalitaetsmodus.
     * @param prop 0=fix, 1=Proportional
     */
    static uint8_t get_proportional() {return ::gfx_get_proportional();}

};

} // namespace
#endif // NIBOCC_USE_GRAPHICDISPLAY
#endif // NIBOCC_GRAPHICDISPLAY_HPP_

