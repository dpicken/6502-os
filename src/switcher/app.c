#include "app.h"

#include "button/app.h"
#include "button/event.h"
#include "buzzer/app.h"
#include "configure/app.h"
#include "console/console.h"
#include "log/app.h"
#include "memtest/app.h"
#include "timer/timer.h"
#include "uptime/app.h"

#include <stdio.h>

void switcher_app_enter(void) {
  switcher_app_reset();
  button_released_set_handler(switcher_button_up_released, button_code_up);
  button_released_set_handler(switcher_button_down_released, button_code_down);
  button_released_set_handler(switcher_button_left_released, button_code_left);
  button_released_set_handler(switcher_button_right_released, button_code_right);
  button_released_set_handler(switcher_button_fire1_released, button_code_fire1);
  button_released_set_handler(switcher_button_fire2_released, button_code_fire2);

  printf("          U:memtest                     \n");
  printf("L:uptime             R:log    A:buzzer  \n");
  printf("          D:button            B:config  \n");
  printf("                              C:        ");
}

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

void switcher_button_fire1_released(void) {
  switcher_app_reset();
  buzzer_app_enter();
}

void switcher_button_fire2_released(void) {
  switcher_app_reset();
  configure_app_enter();
}

void switcher_app_reset(void) {
  button_clear_all_non_special();
  timer_clear_all_non_special();
  console_clear();
}
