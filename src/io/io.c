#include "io.h"

#include "display/display.h"
#include "ft245r/ft245r.h"
#include "util/array.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define O_ACCESS_MODE_BITMASK (O_RDONLY | O_WRONLY | O_RDWR)

#define DEV_DISPLAY "/dev/display"
#define DEV_DISPLAY_VIDIPRINTER "/dev/display_vidiprinter"
#define DEV_FT245R "/dev/ft245r"

FILE* serial;
FILE* vdu;
FILE* vidiprinter;

static io_reader io_readers[FOPEN_MAX];
static io_writer io_writers[ARRAY_SIZE(io_readers)];

void io_init(io_reader c_stdin, io_writer c_stdout, io_writer c_stderr) {
  unsigned char i;

  for (i = 0; i != ARRAY_SIZE(io_readers); ++i) {
    io_readers[i] = io_closed_reader;
    io_writers[i] = io_closed_writer;
  }

  io_readers[STDIN_FILENO] = c_stdin;
  io_writers[STDOUT_FILENO] = c_stdout;
  io_writers[STDERR_FILENO] = c_stderr;

  serial = fopen(DEV_FT245R, "a+");
  vdu = fopen(DEV_DISPLAY, "a");
  vidiprinter = fopen(DEV_DISPLAY_VIDIPRINTER, "a");
}

unsigned char io_is_initialized(void) {
  return io_writers[STDERR_FILENO] != NULL;
}

int io_closed_reader(char*, unsigned int) {
  return io_fail(EBADF);
}

int io_closed_writer(const char*, unsigned int) {
  return io_fail(EBADF);
}

int io_fail(int error_no) {
  errno = error_no;
  return -1;
}

int open(const char* name, int flags, ...) {
  unsigned char fd;

  for (fd = 0; fd != ARRAY_SIZE(io_readers); ++fd) {
    if (io_readers[fd] == io_closed_reader && io_writers[fd] == io_closed_writer) {
      if (strcmp(name, DEV_DISPLAY) == 0) {
        return io_open_display(fd, flags);
      } else if (strcmp(name, DEV_DISPLAY_VIDIPRINTER) == 0) {
        return io_open_display_vidiprinter(fd, flags);
      } else if (strcmp(name, DEV_FT245R) == 0) {
        return io_open_ft245r(fd, flags);
      } else {
        return io_fail(ENOENT);
      }
    }
  }

  return io_fail(EMFILE);
}

int io_open_display(int fd, int flags) {
  switch (flags & O_ACCESS_MODE_BITMASK) {
    case O_WRONLY:
      io_writers[fd] = io_display_write;
      return fd;
  }

  return io_fail(EINVAL);
}

int io_open_display_vidiprinter(int fd, int flags) {
  switch (flags & O_ACCESS_MODE_BITMASK) {
    case O_WRONLY:
      io_writers[fd] = io_display_vidiprinter_write;
      return fd;
  }

  return io_fail(EINVAL);
}

int io_open_ft245r(int fd, int flags) {
  if (!ft245r_is_initialized()) {
    return io_fail(ENOENT);
  }

  switch (flags & O_ACCESS_MODE_BITMASK) {
    case O_RDONLY:
      io_readers[fd] = io_ft245r_read;
      return fd;
    case O_WRONLY:
      io_writers[fd] = io_ft245r_write;
      return fd;
    case O_RDWR:
      io_readers[fd] = io_ft245r_read;
      io_writers[fd] = io_ft245r_write;
      return fd;
  }

  return io_fail(EINVAL);
}

int __fastcall__ close(int fd) {
  if (fd >= ARRAY_SIZE(io_readers)) {
    return io_fail(EBADF);
  }

  io_readers[fd] = io_closed_reader;
  io_writers[fd] = io_closed_writer;
  return 0;
}

int __fastcall__ read(int fd, void* buf, unsigned count) {
  if (fd >= ARRAY_SIZE(io_readers)) {
    return io_fail(EBADF);
  }

  return io_readers[fd]((char*)buf, count);
}

int __fastcall__ write(int fd, const void* buf, unsigned count) {
  if (fd >= ARRAY_SIZE(io_writers)) {
    return io_fail(EBADF);
  }

  return io_writers[fd](buf, count);
}

IO_DEFINE_WRITER(io_display_write, display_putc);
IO_DEFINE_WRITER(io_display_vidiprinter_write, display_vidiprinter_putc);

IO_DEFINE_READER_NON_BLOCKING(io_ft245r_read, ft245r_rx_ready, ft245r_rx);
IO_DEFINE_WRITER_NON_BLOCKING(io_ft245r_write, ft245r_tx_ready, ft245r_tx);
