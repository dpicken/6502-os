#include "menu.h"

#include "controller/controller.h"
#include "display/display.h"
#include "util/algorithm.h"

#include <stdio.h>

static ui_menu* current_menu;

void ui_menu_enter(ui_menu* menu) {
  current_menu = menu;

  current_menu->item_pos = 0;
  current_menu->render_pos = 0;

  ui_menu_enter_current();
}

void ui_menu_enter_current(void) {
  controller_button_set_demuxed_depressed_handler(ui_menu_up, controller_button_up);
  controller_button_set_demuxed_depressed_handler(ui_menu_down, controller_button_down);
  controller_button_set_demuxed_depressed_handler(ui_menu_left, controller_button_left);
  controller_button_set_demuxed_depressed_handler(ui_menu_right, controller_button_right);
  controller_button_set_demuxed_depressed_handler(ui_menu_select, controller_button_a);

  ui_menu_render();
}

void ui_menu_up(void) {
  if (current_menu->item_pos == 0) {
    return;
  }

  --current_menu->item_pos;

  if (current_menu->item_pos < current_menu->render_pos) {
    if (current_menu->render_pos != 0) {
      --current_menu->render_pos;
    }
  }
  ui_menu_render();
}

void ui_menu_down(void) {
  if (current_menu->item_pos + 1 == current_menu->item_count) {
    return;
  }

  ++current_menu->item_pos;

  if (current_menu->item_pos - current_menu->render_pos == display->get_resolution_y()) {
    ++current_menu->render_pos;
  }
  ui_menu_render();
}

void ui_menu_left(void) {
  if (current_menu->parent_menu == 0) {
    return;
  }

  ui_menu_enter(current_menu->parent_menu);
}

void ui_menu_right(void) {
  if (current_menu->item_array[current_menu->item_pos].sub_menu == 0) {
    ui_menu_select();
    return;
  }

  ui_menu_enter(current_menu->item_array[current_menu->item_pos].sub_menu);
}

void ui_menu_select(void) {
  unsigned char item_pos = current_menu->item_pos;

  if (current_menu->item_array[item_pos].sub_menu != 0) {
    ui_menu_enter(current_menu->item_array[item_pos].sub_menu);
  } else {
    current_menu->item_array[item_pos].on_selected();
  }
}

void ui_menu_render(void) {
  unsigned char display_resolution_x = display->get_resolution_x();
  unsigned char display_resolution_y = display->get_resolution_y();
  unsigned char render_item_count_max = current_menu->item_count - current_menu->render_pos;
  unsigned char render_item_count = uc_min(render_item_count_max, display_resolution_y);
  unsigned char render_item_pos_end = current_menu->render_pos + render_item_count;
  unsigned char render_end = current_menu->render_pos + uc_max(render_item_count, display_resolution_y);

  unsigned char i;
  char buffer[16 + 1];

  for (i = current_menu->render_pos; i != render_item_pos_end; ++i) {
    ui_menu_render_line(buffer
        , snprintf(buffer, sizeof(buffer)
            , "%c %-12s %c", i == current_menu->item_pos ? '*' : ' '
            , current_menu->item_array[i].label, current_menu->item_array[i].sub_menu ? '>' : ' ')
        , display_resolution_x);
  }

  for (; i != render_end; ++i) {
    ui_menu_render_line(" ", 1, display_resolution_x);
  }
}

void ui_menu_render_line(const char* buf, unsigned char count, unsigned char display_resolution_x) {
  unsigned char i;

  count = uc_min(count, display_resolution_x);

  for (i = 0; i < count; ++i) {
    display->putchar(buf[i]);
  }

  for (; i < display_resolution_x; ++i) {
    display->putchar(' ');
  }
}
