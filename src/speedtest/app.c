#include "app.h"

#include "controller/controller.h"
#include "display/display.h"
#include "io/io.h"
#include "kernel/system_time.h"

#include <stdio.h>

#define SPEEDTEST_1K(test_description, test_expression) \
  start_time = system_time_get_ticks();\
  for (i = 0; i != 100; ++i) {\
    for (j = 0; j != 10; ++j) {\
      test_expression;\
    }\
  }\
  end_time = system_time_get_ticks();\
  fprintf(vidiprinter, "1K %s: %lu ms\n", test_description, system_time_ticks_to_ms(end_time - start_time))

#define SPEEDTEST_1M(test_description, test_expression) \
  start_time = system_time_get_ticks();\
  for (i = 0; i != 100; ++i) {\
    for (j = 0; j != 100; ++j) {\
      for (k = 0; k != 100; ++k) {\
        test_expression;\
      }\
    }\
  }\
  end_time = system_time_get_ticks();\
  fprintf(vidiprinter, "1M %s: %lu ms\n", test_description, system_time_ticks_to_ms(end_time - start_time))

void speedtest_app_enter(void) {
  controller_button_set_demuxed_depressed_handler(speedtest_run, controller_button_a);

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

  SPEEDTEST_1K("vdu", fprintf(vdu, "speedtest "));
  SPEEDTEST_1K("serial", fprintf(serial, "speedtest "));
  SPEEDTEST_1M("loop", ;);
  SPEEDTEST_1M("read", *p);
  SPEEDTEST_1M("write", *p = 1);
}
