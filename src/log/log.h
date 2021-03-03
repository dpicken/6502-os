#ifndef log_log_h
#define log_log_h

#include <stdio.h>

/** Log a message; message must be a static string literal. */
void log(const char* const message);

/** Dump the log. */
void log_dump(FILE* out);

#endif // ifndef log_log_h
