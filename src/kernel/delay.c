#include "delay.h"

#include "interrupt_wait.h"
#include "system_time.h"

void delay_ms(unsigned int ms) {
  unsigned long time_point = system_time_get_ms() + ms;
  while (system_time_get_ms() < time_point) {
    interrupt_wait();
  }
}
