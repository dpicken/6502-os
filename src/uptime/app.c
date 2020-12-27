#include "app.h"

#include "button/event.h"
#include "kernel/system_time.h"
#include "lcd/init.h"
#include "lcd/io.h"
#include "timer/timer.h"

#include <stdio.h>

void uptime_print_system_time(void) {
  unsigned long total_seconds = system_time_get_ms() / 1000;
  unsigned int seconds = total_seconds % 60;
  unsigned int minutes = (total_seconds / 60) % 60;
  unsigned int hours = (total_seconds / (60 * 60)) % 24;
  unsigned int days = (total_seconds / (60 * 60 * 24));
  char timestamp[12];
  snprintf(timestamp, sizeof(timestamp), "%02u-%02u:%02u:%02u", days, hours, minutes, seconds);
  lcd_puts(timestamp);
}

void uptime_print_system_time_pretty(void) {
  lcd_home();
  uptime_print_system_time();
}

void uptime_app_enter(void) {
  lcd_clear();
  button_clear_handlers();
  uptime_print_system_time_pretty();
  timer_add_fixed_rate(&uptime_print_system_time_pretty, 1000);
}
