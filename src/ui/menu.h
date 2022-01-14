#ifndef ui_menu_h
#define ui_menu_h

#include "util/array.h"

typedef void (ui_menu_item_callback)(void);

typedef struct ui_menu_item ui_menu_item;
typedef struct ui_menu ui_menu;

struct ui_menu_item {
  const char* const label;
  const ui_menu_item_callback* const on_selected; // NULL, if this has-a sub-menu.
  ui_menu* const sub_menu; // !NULL, if this is-a sub-menu.
};

#define UI_MENU_MAKE_ITEM(label, on_selected) { label, on_selected, 0 }
#define UI_MENU_MAKE_ITEM_WITH_SUB_MENU(label, sub_menu) { label, 0, sub_menu }

struct ui_menu {
  ui_menu* const parent_menu; // !NULL, if this is-a sub-menu.
  unsigned char render_pos;
  unsigned char item_pos;
  const unsigned char item_count;
  const ui_menu_item* const item_array;
};

#define UI_MENU_MAKE_MENU(item_array) { 0, 0, 0, ARRAY_SIZE(item_array), item_array }
#define UI_MENU_MAKE_SUB_MENU(parent_menu, item_array) { parent_menu, 0, 0, ARRAY_SIZE(item_array), item_array }

/** Run the specified menu. */
void ui_menu_enter(ui_menu* menu);

/** Run the current menu. */
void ui_menu_enter_current(void);

void ui_menu_up(void);
void ui_menu_down(void);
void ui_menu_left(void);
void ui_menu_right(void);
void ui_menu_select(void);
void ui_menu_render(void);
void ui_menu_render_line(const char* buf, unsigned char count, unsigned char display_resolution_x);

#endif // ifndef ui_menu_h
