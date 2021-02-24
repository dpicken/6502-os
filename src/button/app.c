#include "app.h"

#include "button/event.h"
#include "console/console.h"
#include "timer/timer.h"

#include <stdio.h>

#define DEFINE_BUTTON_HANDLERS(button_name, depressed_char, released_char) \
  void button_app_button_##button_name##_depressed(void) { \
    putchar(depressed_char); \
  } \
\
  void button_app_button_##button_name##_released(void) { \
    putchar(released_char); \
  }

DEFINE_BUTTON_HANDLERS(up,    'u', 'U')
DEFINE_BUTTON_HANDLERS(down,  'd', 'D')
DEFINE_BUTTON_HANDLERS(left,  'l', 'L')
DEFINE_BUTTON_HANDLERS(right, 'r', 'R')
DEFINE_BUTTON_HANDLERS(fire1,  'a', 'A')
DEFINE_BUTTON_HANDLERS(fire2,  'b', 'B')
DEFINE_BUTTON_HANDLERS(fire3,  'c', 'C')

#define SET_BUTTON_HANDLERS(button_name) \
  button_depressed_set_handler(button_app_button_##button_name##_depressed, button_code_##button_name); \
  button_released_set_handler(button_app_button_##button_name##_released, button_code_##button_name)

void button_app_enter(void) {
  SET_BUTTON_HANDLERS(up);
  SET_BUTTON_HANDLERS(down);
  SET_BUTTON_HANDLERS(left);
  SET_BUTTON_HANDLERS(right);
  SET_BUTTON_HANDLERS(fire1);
  SET_BUTTON_HANDLERS(fire2);
  SET_BUTTON_HANDLERS(fire3);
  timer_add_fixed_rate(&console_clear, 30000);
}
