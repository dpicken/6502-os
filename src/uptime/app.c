#include "app.h"

#include "kernel/system_time.h"
#include "timer/timer.h"

#include <stdio.h>

void uptime_app_enter(void) {
  timer_add_fixed_rate(&uptime_print_system_time, 1000);

  uptime_print_system_time();
}

void uptime_print_system_time(void) {
  unsigned long total_seconds = system_time_get_ms() / 1000;
  unsigned int seconds = total_seconds % 60;
  unsigned int minutes = (total_seconds / 60) % 60;
  unsigned int hours = (total_seconds / (60 * 60)) % 24;
  unsigned int days = (total_seconds / (60 * 60 * 24));
  printf("%02u-%02u:%02u:%02u\r", days, hours, minutes, seconds);
}
