#include "kernel.h"

#include "interrupt_control.h"
#include "irq_handler.h"
#include "system_time.h"

#include "button/init.h"
#include "lcd/init.h"
#include "lcd/putc.h"
#include "timer/timer.h"

#include <stdio.h>

void kernel_out(const char* str) {
  while (*str != '\0') {
    lcd_putc(*str++);
  }
}

void kernel_out_system_time(void) {
  long unsigned int total_seconds = system_time_get_ms() / 1000;
  unsigned int seconds = total_seconds % 60;
  unsigned int minutes = (total_seconds / 60) % 60;
  unsigned int hours = (total_seconds / (60 * 60)) % 24;
  unsigned int days = (total_seconds / (60 * 60 * 24));
  char timestamp[12];
  snprintf(timestamp, sizeof(timestamp), "%02u-%02u:%02u:%02u", days, hours, minutes, seconds);
  kernel_out(timestamp);
}

void kernel_display_time(void) {
  lcd_home();
  kernel_out_system_time();
  kernel_out("                             ");
}

void kernel_button_poll(void) {
  unsigned char button_bits = hw_button_get();
  if (button_bits & HW_BUTTON_0) {
    kernel_out("Up                                      ");
  }
  if (button_bits & HW_BUTTON_1) {
    kernel_out("Down                                    ");
  }
  if (button_bits & HW_BUTTON_2) {
    kernel_out("Left                                    ");
  }
  if (button_bits & HW_BUTTON_3) {
    kernel_out("Right                                   ");
  }
  if (button_bits & HW_BUTTON_4) {
    kernel_out("Fire                                    ");
  }
}

void kernel_event_loop(void) {
  for (;;) {
    if (system_time_reset_ticked_event()) {
      timer_on_system_time_ticked();
    }
    kernel_button_poll();
  }
}

void main(void) {
  irq_handler_init();
  interrupt_control_irq_enable();

  system_time_init();

  lcd_init();
  kernel_out("[lcd_init] done                         ");

  button_init();
  kernel_out("[btn_init] done                         ");

  timer_add_fixed_rate(&kernel_display_time, 1000);
  kernel_out("[tme_init] done                         ");

  kernel_out("[evt_loop] start                        ");
  kernel_event_loop();
}
