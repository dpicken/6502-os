#include "write.h"

#include "lcd/io.h"

int __fastcall__ write (int fd, const void* buf, unsigned count) {
  (void) fd; // FIXME: Assuming this is always stdout.
  lcd_write((const char*) buf, count);
  return count;
}
