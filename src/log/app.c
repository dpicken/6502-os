#include "app.h"

#include "log.h"

#include "button/event.h"

void log_app_button_up_released(void) {
}

void log_app_button_down_released(void) {
}

void log_app_button_fire_released(void) {
  log_dump();
}

void log_app_enter(void) {
  button_released_set_handler(log_app_button_up_released, button_code_up);
  button_released_set_handler(log_app_button_down_released, button_code_down);
  button_released_set_handler(log_app_button_fire_released, button_code_fire1);

  log_dump();
}
