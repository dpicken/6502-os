#include "timer.h"

#include "kernel/system_time.h"
#include "panic/panic.h"

#define MAX_TIMER_COUNT 8

typedef struct {
  unsigned long time_point_ticks;
  timer_callback callback;
  unsigned long next_ttl_ticks;
  unsigned char is_special;
} timer_entry;

static timer_entry timers[MAX_TIMER_COUNT];

void timer_add(timer_callback callback, unsigned long ttl_ms, unsigned long next_ttl_ms, unsigned char is_special) {
  timer_entry* entry;

  unsigned char i = 0;

  for (; i != MAX_TIMER_COUNT; ++i) {
    entry = &timers[i];
    if (entry->callback != 0) {
      continue;
    }

    entry->time_point_ticks = system_time_get_ticks() + system_time_ms_to_ticks(ttl_ms);
    entry->callback = callback;
    entry->next_ttl_ticks = system_time_ms_to_ticks(next_ttl_ms);
    entry->is_special = is_special;
    return;
  }
  panic("timer queue full");
}

void timer_add_one_shot(timer_callback callback, unsigned long ttl_ms) {
  timer_add(callback, ttl_ms, 0, 0);
}

void timer_add_one_shot_special(timer_callback callback, unsigned long ttl_ms) {
  timer_add(callback, ttl_ms, 0, 1);
}

void timer_add_fixed_rate(timer_callback callback, unsigned long ttl_ms) {
  timer_add(callback, ttl_ms, ttl_ms, 0);
}

void timer_add_fixed_rate_special(timer_callback callback, unsigned long ttl_ms) {
  timer_add(callback, ttl_ms, ttl_ms, 1);
}

void timer_clear_all_non_special(void) {
  timer_entry* entry;

  unsigned char i = 0;

  for (; i != MAX_TIMER_COUNT; ++i) {
    entry = &timers[i];
    if (!entry->is_special) {
      entry->callback = 0;
    }
  }
}

void timer_on_system_time_ticked(void) {
  timer_entry* entry;

  unsigned char i = 0;
  unsigned long time_point_ticks = system_time_get_ticks();

  for (; i != MAX_TIMER_COUNT; ++i) {
    entry = &timers[i];
    if (entry->callback == 0) {
      continue;
    }

    if (entry->time_point_ticks < time_point_ticks) {
      entry->time_point_ticks += entry->next_ttl_ticks;
      entry->callback();
      if (entry->next_ttl_ticks == 0) {
        entry->callback = 0;
      }
    }
  }
}
