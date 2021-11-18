#include "app.h"

#include "controller.h"

#include "console/console.h"
#include "timer/timer.h"

#include <stdio.h>

#define DEFINE_BUTTON_HANDLERS(button_name, depressed_char, released_char) \
  void button_app_##button_name##_depressed(void) { \
    controller_led_off(); \
    putchar(depressed_char); \
  } \
\
  void button_app_##button_name##_released(void) { \
    putchar(released_char); \
    controller_led_on(); \
  }

DEFINE_BUTTON_HANDLERS(up,    'u', 'U')
DEFINE_BUTTON_HANDLERS(down,  'd', 'D')
DEFINE_BUTTON_HANDLERS(left,  'l', 'L')
DEFINE_BUTTON_HANDLERS(right, 'r', 'R')
DEFINE_BUTTON_HANDLERS(a,     'a', 'A')
DEFINE_BUTTON_HANDLERS(b,     'b', 'B')
DEFINE_BUTTON_HANDLERS(c,     'c', 'C')

#define SET_BUTTON_HANDLERS(button_name) \
  controller_button_set_demuxed_depressed_handler(button_app_##button_name##_depressed, controller_button_##button_name); \
  controller_button_set_demuxed_released_handler(button_app_##button_name##_released, controller_button_##button_name)

void controller_app_enter(void) {
  SET_BUTTON_HANDLERS(up);
  SET_BUTTON_HANDLERS(down);
  SET_BUTTON_HANDLERS(left);
  SET_BUTTON_HANDLERS(right);
  SET_BUTTON_HANDLERS(a);
  SET_BUTTON_HANDLERS(b);
  SET_BUTTON_HANDLERS(c);
  timer_add_fixed_rate(&console_clear, 30000);
}
