#include "app.h"

#include "log.h"

#include "button/event.h"

void log_app_enter(void) {
  button_released_set_handler(log_dump, button_code_fire1);

  log_dump();
}
