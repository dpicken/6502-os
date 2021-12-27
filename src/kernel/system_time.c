#include "system_time.h"

#include "hw/configuration.h"

#define MS_PER_TICK 5

static unsigned long tick_count;
static unsigned char ticked_event;

unsigned long system_time_get_tick_frequency(void) {
  return MS_PER_TICK * (HW_CPU_FREQUENCY / 1000);
}

unsigned long system_time_get_ticks(void) {
  return tick_count;
}

unsigned long system_time_ticks_to_ms(unsigned long ticks) {
  return ticks * MS_PER_TICK;
}

unsigned long system_time_ms_to_ticks(unsigned long ms) {
  if (ms == 0) {
    return 0;
  } else if (ms < MS_PER_TICK) {
    return 1;
  } else {
    return ms / MS_PER_TICK;
  }
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

void system_time_on_tick(void) {
  ++tick_count;
  ticked_event = 1;
}
