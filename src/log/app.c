#include "app.h"

#include "log.h"

#include "cc65/write.h"

void log_app_enter(void) {
  log_app_dump();
}

void log_app_dump(void) {
  log_dump(vidiprinter);
}
