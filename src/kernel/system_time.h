#ifndef kernel_system_time_h
#define kernel_system_time_h

/** Get the number of CPU cycles per tick. */
unsigned long system_time_get_tick_frequency(void);

/** Get the system time in timer ticks. */
unsigned long system_time_get_ticks(void);

/** Convert ticks to ms. */
unsigned long system_time_ticks_to_ms(unsigned long ticks);

/** Convert ms to ticks. */
unsigned long system_time_ms_to_ticks(unsigned long ms);

/** Get the sysem time in ms. */
unsigned long system_time_get_ms(void);

/** Reset the system time ticked event, if set; returns true if it was set. */
unsigned char system_time_reset_ticked_event(void);

void system_time_on_tick(void);

#endif // ifndef kernel_system_time_h
