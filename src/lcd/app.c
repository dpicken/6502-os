#include "app.h"

#include "control.h"
#include "io.h"

#include "button/event.h"
#include "util/algorithm.h"

#include <stdint.h>

#define CHAR_SET_END (1 << 8)

static int char_set_pos;

void lcd_app_enter(void) {
  button_released_set_handler(lcd_app_char_set_previous_page, button_code_up);
  button_released_set_handler(lcd_app_char_set_next_page, button_code_down);
  button_released_set_handler(lcd_display_shift_left, button_code_left);
  button_released_set_handler(lcd_display_shift_right, button_code_right);
  button_released_set_handler(lcd_app_char_set_home, button_code_fire1);
  button_released_set_handler(lcd_display_off, button_code_fire2);
  button_released_set_handler(lcd_display_on, button_code_fire3);

  lcd_app_char_set_home();
}

void lcd_app_char_set_home(void) {
  lcd_home();
  char_set_pos = 0;
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_next_page(void) {
  if (char_set_pos == (CHAR_SET_END - lcd_get_char_count())) {
    return;
  }

  char_set_pos = min(char_set_pos + lcd_get_char_count(), CHAR_SET_END);
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_previous_page(void) {
  char_set_pos = max(char_set_pos - lcd_get_char_count(), 0);
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_dump_page(void) {
  int char_set_page_end = char_set_pos + lcd_get_char_count();
  int char_set_end = min(char_set_page_end, CHAR_SET_END);

  int i;

  for (i = char_set_pos; i != char_set_end; ++i) {
    lcd_putc(i);
  }

  for (; i != char_set_page_end; ++i) {
    lcd_putc(' ');
  }
}
