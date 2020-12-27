#include "log.h"

#include "kernel/system_time.h"

#define LOG_MAX_ENTRY_COUNT 50

typedef struct {
  unsigned long system_time_ms;
  const char* message;
} log_entry;

log_entry log_entries[LOG_MAX_ENTRY_COUNT];
unsigned char log_index;

void log(const char* const message) {
  log_entries[log_index].message = message;
  log_entries[log_index].system_time_ms = system_time_get_ms();
  if (++log_index == LOG_MAX_ENTRY_COUNT) {
    log_index = 0;
  }
}

unsigned char log_dump(log_dumper dumper, unsigned char offset) {
  unsigned char index = (log_index - 1 - offset) % LOG_MAX_ENTRY_COUNT;

  if (index == log_index) {
    return 0;
  }

  return log_dump_entry(dumper, index);
}

unsigned char log_dump_entry(log_dumper dumper, unsigned char index) {
  if (log_entries[index].message == 0) {
    return 0;
  }

  dumper(log_entries[index].message);
  return 1;
}
