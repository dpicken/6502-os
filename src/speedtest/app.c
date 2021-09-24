#include "app.h"

#include "button/event.h"
#include "console/console.h"
#include "hw/map.h"
#include "lcd/lcd.h"
#include "kernel/system_time.h"

#include <stdio.h>

void speedtest_app_enter(void) {
  button_depressed_set_handler(speedtest_run, button_code_fire1);

  speedtest_run();
}

void speedtest_run(void) {
  unsigned char i;
  unsigned char j;
  unsigned char k;
  unsigned int start_time;
  unsigned int end_time;
  unsigned char v;

  volatile unsigned char* p = &v;

  start_time = system_time_get_ticks();
  for (i = 0; i != 100; ++i) {
    for (j = 0; j < 100; ++j) {
      lcd->putchar('*');
    }
  }
  end_time = system_time_get_ticks();
  console_clear();
  printf("10K LCD: %lu ms\n", system_time_ticks_to_ms(end_time - start_time));

  start_time = system_time_get_ticks();
  for (i = 0; i != 100; ++i) {
    for (j = 0; j < 100; ++j) {
      for (k = 0; k < 100; ++k) {
      }
    }
  }
  end_time = system_time_get_ticks();
  printf("1M loop: %lu ms\n", system_time_ticks_to_ms(end_time - start_time));

  start_time = system_time_get_ticks();
  for (i = 0; i != 100; ++i) {
    for (j = 0; j < 100; ++j) {
      for (k = 0; k < 100; ++k) {
        *p;
      }
    }
  }
  end_time = system_time_get_ticks();
  printf("1M read: %lu ms\n", system_time_ticks_to_ms(end_time - start_time));

  start_time = system_time_get_ticks();
  for (i = 0; i != 100; ++i) {
    for (j = 0; j < 100; ++j) {
      for (k = 0; k < 100; ++k) {
        *p = 1;
      }
    }
  }
  end_time = system_time_get_ticks();
  printf("1M write: %lu ms", system_time_ticks_to_ms(end_time - start_time));
}
