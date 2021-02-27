#include "app.h"

#include "button/event.h"
#include "console/console.h"

#include <stdio.h>

void configure_app_enter(void) {
  button_released_set_handler(console_init_40x4, button_code_fire1);
  button_released_set_handler(console_init_16x2, button_code_fire2);

  puts("A:40x4");
  puts("B:16x2");
}
