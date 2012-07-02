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

/*! @file       pwm.c
 * @brief       Modul zur Erzeugung der Pwm-Signale und Zeitberechnung
 * @author      Nils Springob (nils@nicai-systems.de)
 * @date        2007-01-05
*/

#include "nibo/niboconfig.h"
#ifndef NIBO_USE_PWM
#error NIBO_USE_PWM must be defined in niboconfig.h when this file is compiled
#endif

#include <avr/interrupt.h>
#include <stdlib.h> 
#include "nibo/pwm.h"
#include "nibo/iodefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PWM_SIGNAL_OV       SIG_OVERFLOW1
#define PWM_TIMER_CNT       TCNT1

#define PWM_TIMER_CCRA      TCCR1A
#ifdef _NIBO_2_
# define PWM_TIMER_CCRA_INIT _BV(WGM11) | _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1)
#else
# define PWM_TIMER_CCRA_INIT _BV(WGM11) | _BV(COM1A1) | _BV(COM1B1)
#endif
#define PWM_TIMER_CCRB      TCCR1B
#define PWM_TIMER_CCRB_INIT  _BV(CS11) | _BV(WGM13)

#define PWM_TIMER_OCRA      OCR1A
#define PWM_TIMER_OCRB      OCR1B
#define PWM_TIMER_OCRC      OCR1C
#define PWM_TIMER_IMSK      _BV(TOIE1)
#define PWM_TIMER_ICR       ICR1
#define PWM_TIMER_ICR_INIT  1024


/*
Der Timer läuft mit einer Frequenz von 2 MHz und zählt bis 2048. Dadurch wird
die Interruptroutine alle 1.024 ms aufgerufen.
*/


// diese Variablen enthalten die Soll-Werte: 
uint16_t pwm_channel_A;
uint16_t pwm_channel_B;
#ifdef _NIBO_2_
uint16_t pwm_channel_C;
#endif

uint16_t pwm_timer_sub;  // zählt bis 1000 (1ms) in 1us Schritten
uint16_t pwm_timer_ms;   // zählt bis 1000 (1s) in 1ms Schritten
uint32_t pwm_timer_sec;  // zählt die Sekunden seit Systemstart

void pwm_init(void)
{
  pwm_timer_sub = 0;
  pwm_timer_ms  = 0;
  pwm_timer_sec = 0;

  pwm_channel_A = 0;
  pwm_channel_B = 0;
#ifdef _NIBO_2_
  pwm_channel_C = 1023;
#endif

  clear_output_bit(IO_LED_WHITE);
  clear_output_bit(IO_DISP_LIGHT);

  activate_output_bit(IO_LED_WHITE);
  activate_output_bit(IO_DISP_LIGHT);

#ifdef _NIBO_2_
  activate_output_bit(IO_LED_RG);
  set_output_bit(IO_LED_RG);
#endif

  PWM_TIMER_CNT = 0;
  PWM_TIMER_CCRA = PWM_TIMER_CCRA_INIT;
  PWM_TIMER_CCRB = PWM_TIMER_CCRB_INIT;
  PWM_TIMER_ICR = PWM_TIMER_ICR_INIT;
  TIMSK |= PWM_TIMER_IMSK;
}

SIGNAL(PWM_SIGNAL_OV)
{
  pwm_timer_sub+=24;
  if (pwm_timer_sub>=1000){
    pwm_timer_sub-=1000;
    pwm_timer_ms++;
  }
  pwm_timer_ms++;

  if (pwm_timer_ms>=1000){
    pwm_timer_ms-=1000;
    pwm_timer_sec++;
  }
  if (pwm_channel_A>=1023)
    PWM_TIMER_OCRA = 1024;
  else
    PWM_TIMER_OCRA = pwm_channel_A;

  if (pwm_channel_B>=1023)
    PWM_TIMER_OCRB = 1024;
  else
    PWM_TIMER_OCRB = pwm_channel_B;

#ifdef _NIBO_2_
  if (pwm_channel_C>=1023)
    PWM_TIMER_OCRC = 1024;
  else
    PWM_TIMER_OCRC = pwm_channel_C;
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif
