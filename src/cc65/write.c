#include "write.h"

#include "console/console.h"

#include <stdio.h>

typedef int (*writer)(const char*, unsigned int);

static const writer open_writers[FOPEN_MAX] = {
  0,              // STDIN_FILENO
  &console_write, // STDOUT_FILENO
  &console_write, // STDERR_FILENO
};

int __fastcall__ write(int fd, const void* buf, unsigned int count) {
  return open_writers[fd]((const char*) buf, count);
}
