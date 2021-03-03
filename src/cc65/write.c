#include "write.h"

#include "console/console.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

FILE* vidiprinter;

typedef int (*writer)(const char*, unsigned int);

static const writer open_writers[FOPEN_MAX] = {
  0,              // STDIN_FILENO
  &console_write, // STDOUT_FILENO
  &console_write, // STDERR_FILENO
  &console_write_vidiprinter // VIDIPRINTER_FILENO
};

int open_writer(const char* path) {
  if (strcmp(path, VIDIPRINTER_PATH) == 0) {
    return VIDIPRINTER_FILENO;
  } else {
    errno = EACCES;
    return -1;
  }
}

int __fastcall__ write(int fd, const void* buf, unsigned int count) {
  return open_writers[fd]((const char*) buf, count);
}
