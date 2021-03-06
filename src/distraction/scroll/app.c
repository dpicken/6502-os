#include "app.h"

#include "button/event.h"
#include "lcd/control.h"
#include "lcd/io.h"
#include "timer/timer.h"

#include <stdlib.h>

void scroll_app_enter(void) {
  int i;

  button_depressed_set_handler(scroll_left, button_code_left);
  button_depressed_set_handler(scroll_right, button_code_right);

  for (i = 0; i < lcd_get_char_count(); ++i) {
    rand() % 2 ? lcd_putc(' ') : lcd_putc(0xFF);
  }

  scroll_left();
}

void scroll_left(void) {
  timer_clear_all_non_special();
  timer_add_fixed_rate(lcd_display_shift_left, 60);
}

void scroll_right(void) {
  timer_clear_all_non_special();
  timer_add_fixed_rate(lcd_display_shift_right, 60);
}
