#ifndef kernel_kernel_h
#define kernel_kernel_h

void kernel_out(const char* str);

void kernel_out_system_time(void);

void kernel_out_system_time_pretty(void);

void kernel_button_handlers_set(void);

void kernel_event_poll_loop(void);

void main(void);

#endif // ifndef kernel_kernel_h
