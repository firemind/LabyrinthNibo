/*
Dieses Programm testet das Grafikdisplay und die Grafikfunktionen der NiboLib
dazu werden verschiedene Bilder nacheinander angezeigt und der Terminal Modus
getestet.
*/

#include <nibo/niboconfig.h>  /* muss immer als erste Datei eingebunden werden */
#include <nibo/display.h>
#include <nibo/gfx.h>
#include <nibo/copro.h>
#include <nibo/delay.h>
#include <nibo/iodefs.h>
#include <nibo/bot.h>
#include <nibo/leds.h>
#include <nibo/pwm.h>
#include <avr/interrupt.h>
#include <nibo/i2cmaster.h>
#include <stdio.h>
#include "images.h"

void drawImage (uint8_t id) {
  gfx_draw_xbm_P(image_getWidth(id), image_getHeight(id), image_getBits(id));
}

/* NIBO-Logo ausgeben */
void plot_logo(uint8_t x, uint8_t y) {
  gfx_move(x+6, y+0);  gfx_box(14, 3);
  gfx_move(x+6, y+5);  gfx_box(14, 6);
  gfx_move(x+0, y+7);  gfx_box(4, 6);
  gfx_move(x+22, y+7);  gfx_box(4, 6);
}

/* Pfeil als Vektorgrafik ausgeben */
void plot_vector(uint8_t x, uint8_t y) {
  gfx_move(x-6, y+17);  
  gfx_lineTo(x   , y+14);
  gfx_lineTo(x+ 6, y+17);
  gfx_lineTo(x+ 4, y- 6);
  gfx_lineTo(x+12, y- 4);
  gfx_lineTo(x   , y-25);
  gfx_lineTo(x-12, y- 4);  
  gfx_lineTo(x- 4, y- 6);
  gfx_lineTo(x- 6, y+17);
}

/* Pfeil als Vektorgrafik mit Schriftzug ausgeben, vorher Bildschirm löschen*/
void drawVector() {
  gfx_fill(0x00);
  plot_vector(64, 32);
  gfx_move(36, 56);
  gfx_print_text("Vector graphic");
}


/* NIBO Begrüßungsbildschirm ausgeben */
void drawInfo() {
  gfx_fill(0x00);
  gfx_set_proportional(1);
  gfx_draw_mode(GFX_DM_JAM2);
  gfx_move(16, 0);
  drawImage(IMG_NICAI_LOGO);
  gfx_move(38, 5);
  gfx_print_text("nicai-systems");
  gfx_move(0, 19);
  gfx_hline(128);
  plot_logo(52,25);
  gfx_set_proportional(0);
  gfx_move(52,40);
  gfx_print_text("N");
  gfx_move(59,40);
  gfx_print_text("I");
  gfx_move(66,40);
  gfx_print_text("B");
  gfx_move(73,40);
  gfx_print_text("O");
  
  gfx_set_proportional(1);
  gfx_move(0, 52);
  gfx_hline(128);
  gfx_move(46, 56);
  gfx_print_text("(c) 2010");
}

/* Terminal Ausgabemodus testen */
void drawTerminal() {
  gfx_term_goto(0, 7);
  gfx_term_print("\n");
  uint8_t speed=8;
  for (uint8_t j=0; j<6; ++j) {
    for (uint8_t i=0x20; i<0x80; ++i) {
      char text[2];
      text[1]=0;
      text[0]=i;
      gfx_term_print(text);
      if (speed&&((i&0x0f)==0x0f)) speed--;
      for (uint8_t d=speed; d; --d) {
        _delay_ms(20);
      }
    }
    gfx_term_print("\n");
  }
  for (uint8_t j=0; j<2; ++j) {
    for (uint8_t i=0x20; i<0x80; ) {
      char text[5];
      text[0]=i++;
      text[1]=i++;
      text[2]=i++;
      text[3]=i++;
      text[4]=0;
      gfx_term_print(text);
    }
    gfx_term_print("\n");
  }
  gfx_term_print("\nTerminal test\nfinished!\n");
  
}

void sleep(uint8_t secs) {
  while(secs--) {
    _delay_ms(1000);
  }
}
    


int main() {
  leds_init();
  pwm_init();
  sei();
  leds_set_displaylight(1024);

  display_init();
  gfx_init();

  drawInfo();
  sleep(2);

  while(1==1) {
    sei();
    drawInfo();
    sleep(3);

    gfx_move(0, 0);
    drawImage(1);
    sleep(2);
    
    gfx_move(0, 0);
    drawImage(3);
    sleep(2);

    drawVector();
    sleep(3);
    
    gfx_move(0, 0);
    drawImage(2);
    sleep(2);

    drawTerminal();
    sleep(2);
    
  }
  return 0;
}

