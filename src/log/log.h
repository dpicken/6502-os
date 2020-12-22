#ifndef log_log_h
#define log_log_h

/** Log a message; message must be a static string literal. */
void log(const char* const message);

typedef void(*log_dumper)(char);

/** Dump the log entry at the specified offset from the tail of the log; returns 0 if the head of the log is reached. */
unsigned char log_dump(log_dumper dumper, unsigned char offset);

unsigned char log_dump_entry(log_dumper dumper, unsigned char index);

#endif // ifndef log_log_h
