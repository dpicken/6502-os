#include "log.h"

#include "cc65/write.h"
#include "kernel/system_time.h"

#include <stdio.h>

#define LOG_MAX_ENTRY_COUNT 50

typedef struct {
  unsigned long system_time_ms;
  const char* message;
} log_entry;

static log_entry log_entries[LOG_MAX_ENTRY_COUNT];
static unsigned char log_pos;

void log(const char* const message) {
  log_entries[log_pos].message = message;
  log_entries[log_pos].system_time_ms = system_time_get_ms();
  if (++log_pos == LOG_MAX_ENTRY_COUNT) {
    log_pos = 0;
  }
}

void log_dump(FILE* out) {
  unsigned char i;

  for (i = log_pos; i != LOG_MAX_ENTRY_COUNT; ++i) {
    if (log_entries[i].message != 0) {
      fprintf(out, "%s\n", log_entries[i].message);
    }
  }

  for (i = 0; i != log_pos; ++i) {
    if (log_entries[i].message != 0) {
      fprintf(out, "%s\n", log_entries[i].message);
    }
  }
}
