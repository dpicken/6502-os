#ifndef timer_timer_h
#define timer_timer_h

typedef void (*timer_callback)(void);

/** Add a timer that fires once. */
void timer_add_one_shot(timer_callback callback, unsigned long ttl_ms);

/** Add a timer that fires at a fixed rate. */
void timer_add_fixed_rate(timer_callback callback, unsigned long ttl_ms);

/** Add a timer that fires at a fixed rate, and, cannot be cleared. */
void timer_add_fixed_rate_special(timer_callback callback, unsigned long ttl_ms);

/** Clear all non-special timers. */
void timer_clear_all_non_special(void);

void timer_on_system_time_ticked(void);

#endif // ifndef timer_timer_h
