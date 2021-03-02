#include "app.h"

#include "button/app.h"
#include "button/event.h"
#include "buzzer/app.h"
#include "configure/app.h"
#include "console/console.h"
#include "log/app.h"
#include "memtest/app.h"
#include "timer/timer.h"
#include "ui/menu.h"
#include "uptime/app.h"

#include <stdio.h>

static ui_menu switcher_menu;

static const ui_menu_item switcher_test_menu_items[] = {
  UI_MENU_MAKE_ITEM("Buzzer", switcher_app_enter_buzzer),
  UI_MENU_MAKE_ITEM("Button", switcher_app_enter_button),
  UI_MENU_MAKE_ITEM("Memtest",switcher_app_enter_memtest)
};

static ui_menu switcher_test_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_test_menu_items);

static const ui_menu_item switcher_menu_items[] = {
  UI_MENU_MAKE_ITEM("Uptime", switcher_app_enter_uptime),
  UI_MENU_MAKE_ITEM("Log", switcher_app_enter_log),
  UI_MENU_MAKE_ITEM("Configure", switcher_app_enter_config),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Test", &switcher_test_menu)
};

static ui_menu switcher_menu = UI_MENU_MAKE_MENU(switcher_menu_items);

void switcher_app_enter(void) {
  switcher_app_reset();
  ui_menu_enter(&switcher_menu);
}

void switcher_app_enter_memtest(void) {
  switcher_app_reset();
  memtest_app_enter();
}

void switcher_app_enter_button(void) {
  switcher_app_reset();
  button_app_enter();
}

void switcher_app_enter_uptime(void) {
  switcher_app_reset();
  uptime_app_enter();
}

void switcher_app_enter_log(void) {
  switcher_app_reset();
  log_app_enter();
}

void switcher_app_enter_buzzer(void) {
  switcher_app_reset();
  buzzer_app_enter();
}

void switcher_app_enter_config(void) {
  switcher_app_reset();
  configure_app_enter();
}

void switcher_app_reset(void) {
  button_clear_all_non_special();
  timer_clear_all_non_special();
  console_clear();
}
