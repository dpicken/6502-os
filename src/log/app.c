#include "app.h"

#include "log.h"

#include "io/io.h"

void log_app_enter(void) {
  log_app_dump();
}

void log_app_dump(void) {
  log_dump(stderr);
  log_dump(vidiprinter);
}
