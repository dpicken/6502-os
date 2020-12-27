#include "log.h"

#include "lcd/io.h"
#include "log/log.h"

void kernel_log_early(const char* const str) {
  log(str);
}

void kernel_log(const char* const str) {
  kernel_log_early(str);
  lcd_puts(str);
}
