#include "app.h"

#include "display.h"

#include "controller/controller.h"
#include "timer/timer.h"
#include "util/algorithm.h"

#include <stdint.h>

#define CHAR_SET_END (1 << 8)

static int char_set_pos;

void display_app_enter(void) {
  controller_button_set_demuxed_depressed_handler(display_app_char_set_previous_page, controller_button_up);
  controller_button_set_demuxed_depressed_handler(display_app_char_set_next_page, controller_button_down);
  display_app_char_set_home();
}

void display_app_char_set_home(void) {
  display->set_pos_home();
  char_set_pos = 0;
  display_app_char_set_dump_page();
}

void display_app_char_set_next_page(void) {
  if (char_set_pos < CHAR_SET_END) {
    char_set_pos += display->get_resolution_cell_count();
  }
  display_app_char_set_dump_page();
}

void display_app_char_set_previous_page(void) {
  if (char_set_pos > 0) {
    char_set_pos -= display->get_resolution_cell_count();
  }
  display_app_char_set_dump_page();
}

void display_app_char_set_dump_page(void) {
  int char_set_page_end = char_set_pos + display->get_resolution_cell_count();
  int char_set_end = min(char_set_page_end, CHAR_SET_END);

  int i;

  display->clear();
  for (i = char_set_pos; i < char_set_end; ++i) {
    display->putchar(i);
  }
}
