#include "menu.h"

#include "button/event.h"
#include "lcd/lcd.h"
#include "util/algorithm.h"

#include <stdio.h>

static ui_menu* current_menu;

void ui_menu_enter(ui_menu* menu) {
  current_menu = menu;

  button_depressed_set_handler(ui_menu_button_up_depressed, button_code_up);
  button_depressed_set_handler(ui_menu_button_down_depressed, button_code_down);
  button_depressed_set_handler(ui_menu_button_left_depressed, button_code_left);
  button_depressed_set_handler(ui_menu_button_right_depressed, button_code_right);
  button_depressed_set_handler(ui_menu_button_fire1_depressed, button_code_fire1);

  ui_menu_render();
}

void ui_menu_button_up_depressed(void) {
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

void ui_menu_button_down_depressed(void) {
  if (current_menu->item_pos + 1 == current_menu->item_count) {
    return;
  }

  ++current_menu->item_pos;

  if (current_menu->item_pos - current_menu->render_pos == lcd->get_resolution_y()) {
    ++current_menu->render_pos;
  }
  ui_menu_render();
}

void ui_menu_button_left_depressed(void) {
  if (current_menu->parent_menu != 0) {
    current_menu->item_pos = 0;
    current_menu->render_pos = 0;
    ui_menu_enter(current_menu->parent_menu);
  }
}

void ui_menu_button_right_depressed(void) {
  if (current_menu->item_array[current_menu->item_pos].sub_menu != 0) {
    ui_menu_enter(current_menu->item_array[current_menu->item_pos].sub_menu);
  }
}

void ui_menu_button_fire1_depressed(void) {
  unsigned char item_pos;

  if (current_menu->item_array[current_menu->item_pos].sub_menu != 0) {
    ui_menu_enter(current_menu->item_array[current_menu->item_pos].sub_menu);
  } else {
    current_menu->item_array[current_menu->item_pos].on_selected();
  }
}

void ui_menu_render(void) {
  unsigned char render_item_count_max = current_menu->item_count - current_menu->render_pos;
  unsigned char render_item_count = uc_min(render_item_count_max, lcd->get_resolution_y());
  unsigned char render_item_pos_end = current_menu->render_pos + render_item_count;
  unsigned char render_end = current_menu->render_pos + uc_max(render_item_count, lcd->get_resolution_y());

  unsigned char i;
  unsigned char buffer[16 + 1];

  for (i = current_menu->render_pos; i != render_item_pos_end; ++i) {
    ui_menu_render_line(buffer, snprintf(buffer, sizeof(buffer), "%c %-12s %c",
        i == current_menu->item_pos ? '*' : ' ',
        current_menu->item_array[i].label,
        current_menu->item_array[i].sub_menu ? '>' : ' '));
  }

  for (; i != render_end; ++i) {
    ui_menu_render_line(" ", 1);
  }
}

void ui_menu_render_line(const char* buf, unsigned int count) {
  unsigned int remaining = lcd->get_resolution_x();

  int i;

  count |= ~-1U;
  count = min(count, remaining);

  for (i = 0; i != count; ++i) {
    lcd->putchar(buf[i]);
  }

  if (remaining > count) {
    remaining -= count;
    for (; remaining != 0; --remaining) {
      lcd->putchar(' ');
    }
  }
}
