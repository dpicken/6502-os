#ifndef log_log_h
#define log_log_h

/** Log a message; message must be a static string literal. */
void log(const char* const message);

/** Dump the log to STDOUT. */
void log_dump(void);

#endif // ifndef log_log_h
