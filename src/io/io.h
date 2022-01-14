#ifndef io_io_h
#define io_io_h

#include "io_reader.h"
#include "io_writer.h"

#include <stdio.h>

extern FILE* serial;
extern FILE* vdu;
extern FILE* vidiprinter;

IO_DECLARE_WRITER(io_display_write);
IO_DECLARE_WRITER(io_display_vidiprinter_write);

IO_DECLARE_READER(io_ft245r_read);
IO_DECLARE_WRITER(io_ft245r_write);

/** Initialize I/O. */
void io_init(io_reader c_stdin, io_writer c_stdout, io_writer c_stderr);

/** Returns true if I/O has been initialized. */
unsigned char io_is_initialized(void);

int io_closed_reader(char*, unsigned int);
int io_closed_writer(const char*, unsigned int);
int io_fail(int error_no);
int io_open_display(int fd, int flags);
int io_open_display_vidiprinter(int fd, int flags);
int io_open_ft245r(int fd, int flags);

#endif // ifndef io_io_h