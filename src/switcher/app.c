#include "app.h"

#include "configure/app.h"
#include "controller/app.h"
#include "display/app.h"
#include "display/display.h"
#include "distraction/scroll/app.h"
#include "distraction/wisdom/app.h"
#include "ft245r/app.h"
#include "kernel/kernel.h"
#include "loader/app.h"
#include "log/app.h"
#include "memtest/app.h"
#include "panic/app.h"
#include "sd/app.h"
#include "speedtest/app.h"
#include "timer/timer.h"
#include "uptime/app.h"

#include <stdio.h>

#define SWITCHER_APP_ENTER_BUTTON_BITS (CONTROLLER_BUTTON_TO_BIT(controller_button_left) | CONTROLLER_BUTTON_TO_BIT(controller_button_right))

static const ui_menu_item switcher_distraction_menu_items[] = {
  UI_MENU_MAKE_ITEM("Scroll", switcher_app_enter_scroll),
  UI_MENU_MAKE_ITEM("Wisdom", switcher_app_enter_wisdom)
};

static ui_menu switcher_distraction_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_distraction_menu_items);

static const ui_menu_item switcher_system_menu_items[] = {
  UI_MENU_MAKE_ITEM("Log", switcher_app_enter_log),
  UI_MENU_MAKE_ITEM("Uptime", switcher_app_enter_uptime),
  UI_MENU_MAKE_ITEM("Reset", switcher_app_enter_kernel_reset),
  UI_MENU_MAKE_ITEM("Shutdown", switcher_app_enter_kernel_shutdown)
};

static ui_menu switcher_system_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_system_menu_items);

static const ui_menu_item switcher_test_menu_items[] = {
  UI_MENU_MAKE_ITEM("Controller", switcher_app_enter_controller),
  UI_MENU_MAKE_ITEM("Display", switcher_app_enter_display),
  UI_MENU_MAKE_ITEM("FT245R", switcher_app_enter_ft245r),
  UI_MENU_MAKE_ITEM("Memtest", switcher_app_enter_memtest),
  UI_MENU_MAKE_ITEM("Panic", switcher_app_enter_panic),
  UI_MENU_MAKE_ITEM("SD card", switcher_app_enter_sd),
  UI_MENU_MAKE_ITEM("Speedtest", switcher_app_enter_speedtest)
};

static ui_menu switcher_test_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, switcher_test_menu_items);

static const ui_menu_item switcher_menu_items[] = {
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Configure", &configure_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Distraction", &switcher_distraction_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Loader", &loader_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("System", &switcher_system_menu),
  UI_MENU_MAKE_ITEM_WITH_SUB_MENU("Test", &switcher_test_menu)
};

ui_menu switcher_menu = UI_MENU_MAKE_MENU(switcher_menu_items);

void switcher_app_enter(void) {
  switcher_app_reset();
  ui_menu_enter(&switcher_menu);
}

void switcher_app_enter_controller(void) {
  switcher_app_start(controller_app_enter);
}

void switcher_app_enter_display(void) {
  switcher_app_start(display_app_enter);
}

void switcher_app_enter_ft245r(void) {
  switcher_app_start(ft245r_app_enter);
}

void switcher_app_enter_log(void) {
  switcher_app_start(log_app_enter);
}

void switcher_app_enter_memtest(void) {
  switcher_app_start(memtest_app_enter);
}

void switcher_app_enter_panic(void) {
  switcher_app_start(panic_app_enter);
}

void switcher_app_enter_sd(void) {
  switcher_app_start(sd_app_enter);
}

void switcher_app_enter_speedtest(void) {
  switcher_app_start(speedtest_app_enter);
}

void switcher_app_enter_scroll(void) {
  switcher_app_start(scroll_app_enter);
}

void switcher_app_enter_wisdom(void) {
  switcher_app_start(wisdom_app_enter);
}

void switcher_app_enter_uptime(void) {
  switcher_app_start(uptime_app_enter);
}

void switcher_app_enter_kernel_reset(void) {
  switcher_app_start(kernel_reset);
}

void switcher_app_enter_kernel_shutdown(void) {
  switcher_app_start(kernel_shutdown);
}

void switcher_app_start(switcher_app_runnable* app) {
  switcher_app_reset();
  controller_button_set_released_handler(switcher_app_button_released);
  controller_button_set_demuxed_depressed_handler(switcher_app_return, controller_button_left);
  app();
}

void switcher_app_return(void) {
  switcher_app_reset();
  ui_menu_enter_current();
}

void switcher_app_reset(void) {
  controller_button_clear_event_handlers();
  timer_clear_all_non_special();
  display_clear();
}

void switcher_app_button_released(controller_button_bitset buttons) {
  if ((buttons & SWITCHER_APP_ENTER_BUTTON_BITS) == SWITCHER_APP_ENTER_BUTTON_BITS) {
    switcher_app_enter();
  }
}
