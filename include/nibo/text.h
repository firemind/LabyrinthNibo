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

/*! @file    text.h
 *  @brief   Routinen zum Ansteuerung eines LC-Text-Displays
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-16
 */

#ifndef NIBO_TEXT_H_
#define NIBO_TEXT_H_
#if defined(NIBO_USE_TEXT) || defined (DOXYGEN)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Text Display initialisieren
 */
void text_init(void);

/*!
 * Text Display löschen
 */
void text_clear(void);

/*!
 * Position des Text-Cursors setzen
 * @param x X-Position des Cursors
 * @param y Y-Position des Cursors
 */
void text_gotoXY(int x, int y);

/*!
 * Zeichen an aktueller Cursorposition ausgeben
 * @param c Ausgabezeichen
 */
void text_print_char(char c);

/*!
 * 0-terminierten String an aktueller Cursorposition ausgeben
 * @param str Nullterminierter String
 */
void text_print(const char * str);

/*!
 * Cursor Modus setzen
 * @param mode Cursormodus
 */
void text_setCursorMode(int mode);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_TEXT
#endif // NIBO_TEXT_H_
