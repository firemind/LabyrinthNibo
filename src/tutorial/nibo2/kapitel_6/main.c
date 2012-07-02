#include <nibo/niboconfig.h>
#include <nibo/display.h>
#include <nibo/gfx.h>

int main() {
  display_init();
  gfx_init();
  gfx_move(30, 20);
  gfx_set_proportional(0);
  gfx_print_text("hello world!");
  gfx_move(30, 30);
  gfx_set_proportional(1);
  gfx_print_text("hello world!");
  return 0;
}

