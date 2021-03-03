#include "app.h"

#include "button/event.h"
#include "console/console.h"
#include "switcher/app.h"
#include "ui/menu.h"

#include <stdio.h>

static const ui_menu_item configure_menu_items[] = {
  UI_MENU_MAKE_ITEM("16x2", configure_app_console_16x2),
  UI_MENU_MAKE_ITEM("20x4", configure_app_console_20x4),
  UI_MENU_MAKE_ITEM("40x4", configure_app_console_40x4),
  UI_MENU_MAKE_ITEM("Exit", configure_app_exit)
};

static ui_menu configure_menu = UI_MENU_MAKE_MENU(configure_menu_items);

void configure_app_enter(void) {
  ui_menu_enter(&configure_menu);
}

void configure_app_console_16x2(void) {
  console_set_resolution(16, 2);
  configure_app_enter();
}

void configure_app_console_20x4(void) {
  console_set_resolution(20, 4);
  configure_app_enter();
}

void configure_app_console_40x4(void) {
  console_set_resolution(40, 4);
  configure_app_enter();
}

void configure_app_exit(void) {
  switcher_app_enter();
}
