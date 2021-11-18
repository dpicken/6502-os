#include "app.h"

#include "lcd.h"

#include "controller/controller.h"
#include "timer/timer.h"
#include "util/algorithm.h"
#include "util/hex.h"

#include <stdint.h>

#define CHAR_SET_END (1 << 8)

static int char_set_pos;

void lcd_app_enter(void) {
  controller_button_set_demuxed_depressed_handler(lcd_app_char_set_previous_page, controller_button_up);
  controller_button_set_demuxed_depressed_handler(lcd_app_char_set_next_page, controller_button_down);
  lcd_app_char_set_home();
}

void lcd_app_char_set_home(void) {
  lcd->set_pos_home();
  char_set_pos = 0;
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_next_page(void) {
  if (char_set_pos < CHAR_SET_END) {
    char_set_pos += lcd->get_resolution_cell_count();
  }
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_previous_page(void) {
  if (char_set_pos > 0) {
    char_set_pos -= lcd->get_resolution_cell_count();
  }
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_dump_page(void) {
  int char_set_page_end = char_set_pos + lcd->get_resolution_cell_count();
  int char_set_end = min(char_set_page_end, CHAR_SET_END);

  int i;

  lcd->clear();
  for (i = char_set_pos; i < char_set_end; ++i) {
    lcd->putchar(i);
  }
}
