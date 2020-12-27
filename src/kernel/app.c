#include "app.h"

#include "button/app.h"
#include "button/event.h"
#include "lcd/init.h"
#include "log/app.h"
#include "uptime/app.h"

void kernel_button_down_released(void) {
  button_app_enter();
}

void kernel_button_left_released(void) {
  uptime_app_enter();
}

void kernel_button_right_released(void) {
  log_app_enter();
}

void kernel_app_start(void) {
  lcd_clear();
  button_released_set_handler(kernel_button_down_released, button_code_down);
  button_released_set_handler(kernel_button_left_released, button_code_left);
  button_released_set_handler(kernel_button_right_released, button_code_right);
}
