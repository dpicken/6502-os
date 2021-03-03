#include "app.h"

#include "log.h"

#include "button/event.h"
#include "cc65/write.h"

void log_app_enter(void) {
  button_released_set_handler(log_app_dump, button_code_fire1);

  log_app_dump();
}

void log_app_dump(void) {
  log_dump(vidiprinter);
}
