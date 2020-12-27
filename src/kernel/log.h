#ifndef kernel_log_h
#define kernel_log_h

/** Log a message prior to lcd initialization. */
void kernel_log_early(const char* const str);

/** Log a message and echo it to the lcd. */
void kernel_log(const char* const str);

#endif // ifndef kernel_log_h
