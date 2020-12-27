#include "app.h"

#include "button/event.h"
#include "lcd/init.h"
#include "lcd/io.h"
#include "timer/timer.h"

#define DEFINE_BUTTON_HANDLERS(button_name, depressed_char, released_char) \
  void button_app_button_##button_name##_depressed(void) { \
    lcd_putc(depressed_char); \
  } \
\
  void button_app_button_##button_name##_released(void) { \
    lcd_putc(released_char); \
  }

DEFINE_BUTTON_HANDLERS(up,    'u', 'U')
DEFINE_BUTTON_HANDLERS(down,  'd', 'D')
DEFINE_BUTTON_HANDLERS(left,  'l', 'L')
DEFINE_BUTTON_HANDLERS(right, 'r', 'R')
DEFINE_BUTTON_HANDLERS(fire,  'f', 'F')

#define SET_BUTTON_HANDLERS(button_name) \
  button_depressed_set_handler(button_app_button_##button_name##_depressed, button_code_##button_name); \
  button_released_set_handler(button_app_button_##button_name##_released, button_code_##button_name)

void button_app_enter(void) {
  lcd_clear();
  button_clear_handlers();
  SET_BUTTON_HANDLERS(up);
  SET_BUTTON_HANDLERS(down);
  SET_BUTTON_HANDLERS(left);
  SET_BUTTON_HANDLERS(right);
  SET_BUTTON_HANDLERS(fire);
  timer_add_fixed_rate(&lcd_clear, 10000);
}
