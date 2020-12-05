#ifndef kernel_system_time_h
#define kernel_system_time_h

/** Get the system time in timer ticks. */
unsigned long system_time_get(void);

/** Get the sysem time in ms. */
unsigned long system_time_get_ms(void);

/** Reset the system time ticked event, if set; returns true if it was set. */
unsigned char system_time_reset_ticked_event(void);

void system_time_init(void);

void system_time_on_tick(void);

#endif // ifndef kernel_system_time_h
