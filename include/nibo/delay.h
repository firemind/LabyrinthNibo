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

/*! @file    delay.h
 *  @brief   Routinen f&uuml;r Verz&ouml;gerungen
 *  @author  Nils Springob (nils@nicai-systems.de)
 *  @date    2007-07-13
 */

#ifndef NIBO_DELAY_H_
#define NIBO_DELAY_H_
#if defined(NIBO_USE_DELAY) || defined (DOXYGEN)

#include <util/delay.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Verzögerung um die angegebene Zeit.
 * @param ms Zeit(Fliesskomma) in Milisekunden, maximal 16,383 ms
 */
#define delay_ms(ms) _delay_ms(ms)

/*!
 * Verzögerung um die angegebene Zeit.
 * @param us Zeit(Fliesskomma) in Mikrosekunden, maximal 48 µs
 */
#define delay_us(us) _delay_us(us)

/*!
 * Verzögerung um die angegebene Zeit.
 * @param ms Zeit(Ganzzahl) in Milisekunden, maximal 65535 ms
 */
void delay(uint16_t ms);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NIBO_USE_DELAY
#endif // NIBO_DELAY_H_
