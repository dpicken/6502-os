#include "log.h"

#include "log/log.h"

#include <stdio.h>

void kernel_log_early(const char* const str) {
  log(str);
}

void kernel_log(const char* const str) {
  kernel_log_early(str);
  puts(str);
}
