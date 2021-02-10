#include "timer.h"

#include "kernel/system_time.h"

#define MAX_TIMER_COUNT 8

typedef struct {
  unsigned int time_point_ms;
  timer_callback callback;
  unsigned int next_ttl_ms;
  unsigned char is_special;
} timer_entry;

timer_entry timers[MAX_TIMER_COUNT];

void timer_add(timer_callback callback, unsigned int ttl_ms, unsigned int next_ttl_ms, unsigned char is_special) {
  timer_entry* entry;

  int i = 0;

  for (; i != MAX_TIMER_COUNT; ++i) {
    entry = &timers[i];
    if (entry->callback != 0) {
      continue;
    }

    entry->time_point_ms = system_time_get_ms() + ttl_ms;
    entry->callback = callback;
    entry->next_ttl_ms = next_ttl_ms;
    entry->is_special = is_special;
    return;
  }
  // TODO: brk
}

void timer_add_one_shot(timer_callback callback, unsigned int ttl_ms) {
  timer_add(callback, ttl_ms, 0, 0);
}

void timer_add_fixed_rate(timer_callback callback, unsigned int ttl_ms) {
  timer_add(callback, ttl_ms, ttl_ms, 0);
}

void timer_add_fixed_rate_special(timer_callback callback, unsigned int ttl_ms) {
  timer_add(callback, ttl_ms, ttl_ms, 1);
}

void timer_clear_all_non_special(void) {
  timer_entry* entry;

  int i = 0;

  for (; i != MAX_TIMER_COUNT; ++i) {
    entry = &timers[i];
    if (!entry->is_special) {
      entry->callback = 0;
    }
  }
}

void timer_on_system_time_ticked(void) {
  timer_entry* entry;

  int i = 0;
  unsigned long time_point_ms = system_time_get_ms();

  for (; i != MAX_TIMER_COUNT; ++i) {
    entry = &timers[i];

    if (entry->callback == 0) {
      continue;
    }

    if (entry->time_point_ms < time_point_ms) {
      entry->time_point_ms += entry->next_ttl_ms;
      entry->callback();
      if (entry->next_ttl_ms == 0) {
        entry->callback = 0;
      }
    }
  }
}
