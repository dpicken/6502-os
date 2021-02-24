#include "write.h"

#include "console/console.h"

int __fastcall__ write(int fd, const void* buf, unsigned count) {
  (void) fd; // FIXME: Assuming this is always stdout.
  console_write((const char*) buf, count);
  return count;
}
