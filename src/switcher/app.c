#include "app.h"

#include "button/app.h"
#include "button/event.h"
#include "configure/app.h"
#include "console/console.h"
#include "distraction/scroll/app.h"
#include "distraction/wisdom/app.h"
#include "kernel/kernel.h"
#include "lcd/app.h"
#include "led/app.h"
#include "log/app.h"
#include "memtest/app.h"
#include "panic/app.h"
#include "timer/timer.h"
#include "uptime/app.h"

#include <stdio.h>

ui_menu switcher_menu;

static const ui_menu_item switcher_distraction_menu_items[] = {
  UI_MENU_MAKE_ITEM("Scroll", switcher_app_enter_scroll),
  UI_MENU_MAKE_ITEM("Wisdom", switcher_app_enter_wisdom),
};

static ui_menu switcher_distraction_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_distraction_menu_items);

static const ui_menu_item switcher_system_menu_items[] = {
  UI_MENU_MAKE_ITEM("Log", switcher_app_enter_log),
  UI_MENU_MAKE_ITEM("Uptime", switcher_app_enter_uptime),
  UI_MENU_MAKE_ITEM("Shutdown", switcher_app_exit),
};

static ui_menu switcher_system_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_system_menu_items);

static const ui_menu_item switcher_test_menu_items[] = {
  UI_MENU_MAKE_ITEM("Button", switcher_app_enter_button),
  UI_MENU_MAKE_ITEM("LCD", switcher_app_enter_lcd),
  UI_MENU_MAKE_ITEM("LED", switcher_app_enter_led),
  UI_MENU_MAKE_ITEM("Memtest", switcher_app_enter_memtest),
  UI_MENU_MAKE_ITEM("Panic", switcher_app_enter_panic)
};

static ui_menu switcher_test_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_test_menu_items);

static const ui_menu_item switcher_menu_items[] = {
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Configure", &configure_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Distraction", &switcher_distraction_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("System", &switcher_system_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Test", &switcher_test_menu)
};

ui_menu switcher_menu = UI_MENU_MAKE_MENU(switcher_menu_items);

void switcher_app_enter(void) {
  switcher_app_reset();
  UI_MENU_HOME(switcher_menu);
  ui_menu_enter(&switcher_menu);
}

void switcher_app_enter_button(void) {
  switcher_app_reset();
  button_app_enter();
}

void switcher_app_enter_lcd(void) {
  switcher_app_reset();
  lcd_app_enter();
}

void switcher_app_enter_led(void) {
  switcher_app_reset();
  led_app_enter();
}

void switcher_app_enter_log(void) {
  switcher_app_reset();
  log_app_enter();
}

void switcher_app_enter_memtest(void) {
  switcher_app_reset();
  memtest_app_enter();
}

void switcher_app_enter_panic(void) {
  switcher_app_reset();
  panic_app_enter();
}

void switcher_app_enter_scroll(void) {
  switcher_app_reset();
  scroll_app_enter();
}

void switcher_app_enter_wisdom(void) {
  switcher_app_reset();
  wisdom_app_enter();
}

void switcher_app_enter_uptime(void) {
  switcher_app_reset();
  uptime_app_enter();
}

void switcher_app_exit(void) {
  switcher_app_reset();
  kernel_shutdown();
}

void switcher_app_reset(void) {
  button_clear_all_non_special();
  timer_clear_all_non_special();
  console_clear();
}
