#include "app.h"

#include "panic.h"

void panic_app_enter(void) {
  panic("test panic");
}
