#include "app.h"

#include "button/app.h"
#include "button/event.h"
#include "lcd/init.h"
#include "log/app.h"
#include "memtest/app.h"
#include "timer/timer.h"
#include "uptime/app.h"

#include <stdio.h>

void switcher_button_up_released(void) {
  switcher_app_reset();
  memtest_app_enter();
}

void switcher_button_down_released(void) {
  switcher_app_reset();
  button_app_enter();
}

void switcher_button_left_released(void) {
  switcher_app_reset();
  uptime_app_enter();
}

void switcher_button_right_released(void) {
  switcher_app_reset();
  log_app_enter();
}

void switcher_app_reset(void) {
  button_clear_all_non_special();
  timer_clear_all_non_special();
  lcd_clear();
}

void switcher_app_enter(void) {
  switcher_app_reset();

  printf("U:memtest  D:btn\n");
  printf("L:uptime   R:log\n");

  button_released_set_handler(switcher_button_up_released, button_code_up);
  button_released_set_handler(switcher_button_down_released, button_code_down);
  button_released_set_handler(switcher_button_left_released, button_code_left);
  button_released_set_handler(switcher_button_right_released, button_code_right);
}
