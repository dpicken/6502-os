#include "app.h"

#include "log.h"

#include "button/event.h"
#include "lcd/init.h"
#include "lcd/io.h"

unsigned char log_offset;

void log_app_button_up_released(void) {
  lcd_home();
  if (log_dump(&lcd_puts, log_offset + 1) != 0) {
    log_dump(&lcd_puts, log_offset);
    ++log_offset;
  } else {
    log_dump(&lcd_puts, log_offset);
    log_dump(&lcd_puts, log_offset + 1);
  }
}

void log_app_button_down_released(void) {
  if (log_offset > 1) {
    --log_offset;
  }
  lcd_home();
  log_dump(&lcd_puts, log_offset);
  log_dump(&lcd_puts, log_offset - 1);
}

void log_app_tail(void) {
  lcd_home();
  log_offset = 1;
  log_dump(&lcd_puts, log_offset);
  log_dump(&lcd_puts, log_offset - 1);
}

void log_app_enter(void) {
  log_app_tail();
  button_released_set_handler(log_app_button_up_released, button_code_up);
  button_released_set_handler(log_app_button_down_released, button_code_down);
  button_released_set_handler(log_app_tail, button_code_fire);
}
