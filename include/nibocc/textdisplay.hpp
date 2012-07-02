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

/*! @file    textdisplay.hpp
 *  @brief   Routinen zur Ansteuerung eines LC-Text-Displays
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBOCC_TEXTDISPLAY_HPP_
#define NIBOCC_TEXTDISPLAY_HPP_
#if defined(NIBOCC_USE_TEXTDISPLAY) || defined (DOXYGEN)

#include <stdint.h>
#include "nibo/text.h"

namespace nibocc {

/*!
 *  @brief Statische Klasse zur Ansteuerung eines LC-Text-Displays
 */
class TextDisplay
{
  public:

    /*!
     * Initialisiert das Text Display. Die Methode @ref Display::init() muss
     * vorher aufgerufen werden!
     */
    inline static void init(void) {::text_init();}

    /*!
     * Text Display löschen
     */
    inline static void clear(void)  {::text_clear();}

    /*!
     * Position des Text-Cursors setzen
     * @param x X-Position des Cursors
     * @param y Y-Position des Cursors
     */
    inline static void gotoXY(int x, int y)  {::text_gotoXY(x, y);}

    /*!
     * Zeichen an aktueller Cursorposition ausgeben
     * @param c Ausgabezeichen
     */
    inline static void print_char(char c)  {::text_print_char(c);}

    /*!
     * 0-terminierten String an aktueller Cursorposition ausgeben
     * @param str Nullterminierter String
     */
    inline static void print(const char * str) {::text_print(str);}

    /*!
     * Cursor Modus setzen
     * @param mode Cursormodus
     */
    inline static void setCursorMode(int mode) {::text_setCursorMode(mode);}

};

} // namespace
#endif // NIBOCC_USE_TEXTDISPLAY
#endif // NIBOCC_TEXTDISPLAY_HPP_
