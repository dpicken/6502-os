#include "kernel.h"

#include "interrupt_wait.h"
#include "irq_control.h"
#include "irq_handler.h"
#include "system_time.h"

#include "button/event.h"
#include "button/init.h"
#include "lcd/init.h"
#include "lcd/putc.h"
#include "log/log.h"
#include "timer/timer.h"

#include <stdio.h>
#include <string.h>

void kernel_out(const char* str) {
  while (*str != '\0') {
    lcd_putc(*str++);
  }
}

void kernel_out_system_time(void) {
  unsigned long total_seconds = system_time_get_ms() / 1000;
  unsigned int seconds = total_seconds % 60;
  unsigned int minutes = (total_seconds / 60) % 60;
  unsigned int hours = (total_seconds / (60 * 60)) % 24;
  unsigned int days = (total_seconds / (60 * 60 * 24));
  char timestamp[12];
  snprintf(timestamp, sizeof(timestamp), "%02u-%02u:%02u:%02u", days, hours, minutes, seconds);
  kernel_out(timestamp);
}

void kernel_out_system_time_pretty(void) {
  lcd_home();
  kernel_out_system_time();
  kernel_out("                             ");
}

#define KERNEL_DECLARE_BUTTON_HANDLER(button_code, event, str) \
  void kernel_##button_code##_##event##(void); \
  void kernel_##button_code##_##event##(void) { \
    kernel_out(str); \
  }

//KERNEL_DECLARE_BUTTON_HANDLER(button_code_up, depressed, "u")
//KERNEL_DECLARE_BUTTON_HANDLER(button_code_down, depressed, "d")
KERNEL_DECLARE_BUTTON_HANDLER(button_code_left, depressed, "l")
KERNEL_DECLARE_BUTTON_HANDLER(button_code_right, depressed, "r")
//KERNEL_DECLARE_BUTTON_HANDLER(button_code_fire, depressed, "f")

//KERNEL_DECLARE_BUTTON_HANDLER(button_code_up, released, "U")
//KERNEL_DECLARE_BUTTON_HANDLER(button_code_down, released, "D")
KERNEL_DECLARE_BUTTON_HANDLER(button_code_left, released, "L")
KERNEL_DECLARE_BUTTON_HANDLER(button_code_right, released, "R")
//KERNEL_DECLARE_BUTTON_HANDLER(button_code_fire, released, "F")

unsigned char log_offset = 0;
void kernel_button_code_up_released(void) {
  ++log_offset;
  if (log_dump(&lcd_putc, log_offset) == 0) {
    --log_offset;
  }
}
void kernel_button_code_down_released(void) {
  if (log_offset >= 1) {
    --log_offset;
  }
  log_dump(&lcd_putc, log_offset);
}
void kernel_button_code_fire_released(void) {
  log_offset = 0;
  log_dump(&lcd_putc, log_offset);
}


void kernel_button_handlers_set(void) {
  //button_depressed_set_handler(kernel_button_code_up_depressed, button_code_up);
  //button_depressed_set_handler(kernel_button_code_down_depressed, button_code_down);
  button_depressed_set_handler(kernel_button_code_left_depressed, button_code_left);
  button_depressed_set_handler(kernel_button_code_right_depressed, button_code_right);
  //button_depressed_set_handler(kernel_button_code_fire_depressed, button_code_fire);

  button_released_set_handler(kernel_button_code_up_released, button_code_up);
  button_released_set_handler(kernel_button_code_down_released, button_code_down);
  button_released_set_handler(kernel_button_code_left_released, button_code_left);
  button_released_set_handler(kernel_button_code_right_released, button_code_right);
  button_released_set_handler(kernel_button_code_fire_released, button_code_fire);
}

void kernel_event_poll_loop(void) {
  lcd_clear();
  for (;;) {
    if (system_time_reset_ticked_event()) {
      timer_on_system_time_ticked();
    }
    interrupt_wait();
  }
}

void kernel_log_early(const char* const str) {
  log(str);
}

void kernel_log(const char* const str) {
  unsigned char str_len = strlen(str);

  kernel_log_early(str);
  kernel_out(str);
  while (str_len--) {
    kernel_out(" ");
  }
}

void main(void) {
  irq_handler_init();
  irq_enable();
  kernel_log_early("[irq_init] done ");

  system_time_init();
  kernel_log_early("[sys_time] done ");

  lcd_init();
  kernel_log_early("[lcd_init] done ");

  button_init();
  kernel_log("[btn_init] done ");

  timer_add_fixed_rate(&kernel_out_system_time_pretty, 1000);
  kernel_log("[krl_time] done ");

  kernel_button_handlers_set();
  kernel_log("[krl_bttn] done ");

  kernel_log("[krl_poll] start");
  kernel_event_poll_loop();
}
