#include "system_time.h"

#include "hw/map.h"

#define MS_PER_TICK 5

static unsigned long tick_count;
static unsigned char ticked_event;

unsigned long system_time_get_ticks(void) {
  return tick_count;
}

unsigned long system_time_ticks_to_ms(unsigned long ticks) {
  return ticks * MS_PER_TICK;
}

unsigned long system_time_ms_to_ticks(unsigned long ms) {
  return ms / MS_PER_TICK;
}

unsigned long system_time_get_ms(void) {
  return system_time_ticks_to_ms(system_time_get_ticks());
}

unsigned char system_time_reset_ticked_event(void) {
  if (ticked_event) {
    ticked_event = 0;
    return 1;
  }
  return 0;
}

void system_time_init(void) {
  unsigned long tick_frequency = MS_PER_TICK * (hw_cpu_get_frequency() / 1000);
  hw_timer_fixed_rate_start(tick_frequency);
}

void system_time_on_tick(void) {
  ++tick_count;
  ticked_event = 1;
}
