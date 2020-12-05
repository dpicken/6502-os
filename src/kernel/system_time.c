#include "system_time.h"

#include "hw/map.h"

#define TICK_FREQUENCY 50000UL

static unsigned long tick_count;
static unsigned long ms_per_tick;
static unsigned char ticked_event;

unsigned long system_time_get(void) {
  return tick_count;
}

unsigned long system_time_get_ms(void) {
  return system_time_get() * ms_per_tick;
}

unsigned char system_time_reset_ticked_event(void) {
  if (ticked_event) {
    ticked_event = 0;
    return 1;
  }
  return 0;
}

void system_time_init(void) {
  hw_timer_fixed_rate_start(TICK_FREQUENCY);
  ms_per_tick = TICK_FREQUENCY / (hw_cpu_get_frequency() / 1000);
}

void system_time_on_tick(void) {
  ++tick_count;
  ticked_event = 1;
}
