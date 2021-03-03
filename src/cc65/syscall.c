#include "syscall.h"

#include "write.h"

#include <errno.h>
#include <fcntl.h>

int open(const char* path, int oflag, ...) {
  if (oflag & ~(O_WRONLY | O_CREAT | O_APPEND)) {
    errno = EACCES;
  }

  return open_writer(path);
}
