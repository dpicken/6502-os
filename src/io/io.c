#include "io.h"

#include "display/display.h"
#include "ft245r/ft245r.h"
#include "sd/sd.h"
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
static io_closer io_closers[ARRAY_SIZE(io_readers)];
static long      io_cursors[ARRAY_SIZE(io_readers)];

void io_init(io_reader c_stdin, io_writer c_stdout, io_writer c_stderr) {
  unsigned char i;

  for (i = 0; i != ARRAY_SIZE(io_readers); ++i) {
    io_readers[i] = io_closed_reader;
    io_writers[i] = io_closed_writer;
    io_closers[i] = io_closed_closer;
  }

  io_readers[STDIN_FILENO] = c_stdin;
  io_closers[STDIN_FILENO] = io_null_closer;

  io_writers[STDOUT_FILENO] = c_stdout;
  io_closers[STDOUT_FILENO] = io_null_closer;

  io_writers[STDERR_FILENO] = c_stderr;
  io_closers[STDERR_FILENO] = io_null_closer;

  serial = fopen(DEV_FT245R, "a+");
  vdu = fopen(DEV_DISPLAY, "a");
  vidiprinter = fopen(DEV_DISPLAY_VIDIPRINTER, "a");
}

unsigned char io_is_initialized(void) {
  return io_closers[STDERR_FILENO] != NULL;
}

int io_closed_reader(long, char*, unsigned int) {
  return io_fail(EBADF);
}

int io_closed_writer(long, const char*, unsigned int) {
  return io_fail(EBADF);
}

int io_closed_closer(void) {
  return io_fail(EBADF);
}

int io_null_closer(void) {
  return 0;
}

int io_fail(int error_no) {
  errno = error_no;
  return -1;
}

int open(const char* name, int flags, ...) {
  unsigned char fd;

  for (fd = 0; fd != ARRAY_SIZE(io_readers); ++fd) {
    if (io_closers[fd] == io_closed_closer) {
      if (strcmp(name, DEV_DISPLAY) == 0) {
        return io_open_display(fd, flags);
      } else if (strcmp(name, DEV_DISPLAY_VIDIPRINTER) == 0) {
        return io_open_display_vidiprinter(fd, flags);
      } else if (strcmp(name, DEV_FT245R) == 0) {
        return io_open_ft245r(fd, flags);
      } else if (strcmp(name, DEV_SD_CARD) == 0) {
        return io_open_sd_card(fd, flags);
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
      io_closers[fd] = io_null_closer;
      return fd;
  }

  return io_fail(EINVAL);
}

int io_open_display_vidiprinter(int fd, int flags) {
  switch (flags & O_ACCESS_MODE_BITMASK) {
    case O_WRONLY:
      io_writers[fd] = io_display_vidiprinter_write;
      io_closers[fd] = io_null_closer;
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
      io_closers[fd] = io_null_closer;
      return fd;
    case O_WRONLY:
      io_writers[fd] = io_ft245r_write;
      io_closers[fd] = io_null_closer;
      return fd;
    case O_RDWR:
      io_readers[fd] = io_ft245r_read;
      io_writers[fd] = io_ft245r_write;
      io_closers[fd] = io_null_closer;
      return fd;
  }

  return io_fail(EINVAL);
}

int io_open_sd_card(int fd, int flags) {
  if (!sd_is_initialized()) {
    return io_fail(ENOENT);
  }

  switch (flags & O_ACCESS_MODE_BITMASK) {
    case O_RDONLY:
      if (!sd_card_open()) {
        return io_fail(EIO);
      }
      io_readers[fd] = io_sd_card_read;
      io_closers[fd] = io_sd_card_close;
      return fd;
    case O_WRONLY:
      if (!sd_card_open()) {
        return io_fail(EIO);
      }
      io_closers[fd] = io_sd_card_close;
      return fd;
    case O_RDWR:
      if (!sd_card_open()) {
        return io_fail(EIO);
      }
      io_readers[fd] = io_sd_card_read;
      io_closers[fd] = io_sd_card_close;
      return fd;
  }

  return io_fail(EINVAL);
}

int __fastcall__ close(int fd) {
  int status;

  if (fd >= ARRAY_SIZE(io_readers)) {
    return io_fail(EBADF);
  }

  status = io_closers[fd]();

  io_readers[fd] = io_closed_reader;
  io_writers[fd] = io_closed_writer;
  io_closers[fd] = io_closed_closer;
  io_cursors[fd] = 0;

  return status;
}

int __fastcall__ read(int fd, void* buf, unsigned int count) {
  int status;

  if (fd >= ARRAY_SIZE(io_readers)) {
    return io_fail(EBADF);
  }

  status = io_readers[fd](io_cursors[fd], (char*)buf, count);
  if (status == -1) {
    return io_fail(EIO);
  }

  io_cursors[fd] += status;
  return status;
}

int __fastcall__ write(int fd, const void* buf, unsigned int count) {
  int status;

  if (fd >= ARRAY_SIZE(io_writers)) {
    return io_fail(EBADF);
  }

  status = io_writers[fd](io_cursors[fd], buf, count);
  if (status == -1) {
    return io_fail(EIO);
  }

  io_cursors[fd] += status;
  return status;
}

long __fastcall__ lseek(int fd, long offset, int whence) {
  if (fd >= ARRAY_SIZE(io_writers)) {
    return io_fail(EBADF);
  }

  if (io_closers[fd] == io_closed_closer) {
    return io_fail(EBADF);
  }

  if (whence == SEEK_SET) {
    io_cursors[fd] = offset;
    return io_cursors[fd];
  } else if (whence == SEEK_CUR) {
    io_cursors[fd] += offset;
    return io_cursors[fd];
  } else if (whence == SEEK_END) {
    return io_fail(EINVAL);
  } else {
    return io_fail(EINVAL);
  }
}

IO_DEFINE_BYTE_WRITER(io_display_write, display_putc);
IO_DEFINE_BYTE_WRITER(io_display_vidiprinter_write, display_vidiprinter_putc);

IO_DEFINE_BYTE_READER_NON_BLOCKING(io_ft245r_read, ft245r_rx_ready, ft245r_rx);
IO_DEFINE_BYTE_WRITER_NON_BLOCKING(io_ft245r_write, ft245r_tx_ready, ft245r_tx);

IO_DEFINE_BLOCK_READER(io_sd_card_read, sd_card_read, SD_BLOCK_BYTE_COUNT_LOG2);
IO_DEFINE_CLOSER(io_sd_card_close, sd_card_close);
