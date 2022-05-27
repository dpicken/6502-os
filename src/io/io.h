#ifndef io_io_h
#define io_io_h

#include "io_closer.h"
#include "io_reader.h"
#include "io_writer.h"

#include <stdio.h>

extern FILE* serial;
extern FILE* vdu;
extern FILE* vidiprinter;

#define DEV_SD_CARD "/dev/sd_card"

IO_DECLARE_WRITER(io_display_write);
IO_DECLARE_WRITER(io_display_vidiprinter_write);

IO_DECLARE_READER(io_ft245r_read);
IO_DECLARE_WRITER(io_ft245r_write);

IO_DECLARE_READER(io_sd_card_read);
IO_DECLARE_CLOSER(io_sd_card_close);

/** Initialize I/O. */
void io_init(io_reader c_stdin, io_writer c_stdout, io_writer c_stderr);

/** Returns true if I/O has been initialized. */
unsigned char io_is_initialized(void);

int io_closed_reader(long, char*, unsigned int);
int io_closed_writer(long, const char*, unsigned int);
int io_closed_closer(void);
int io_null_closer(void);
int io_fail(int error_no);
int io_open_display(int fd, int flags);
int io_open_display_vidiprinter(int fd, int flags);
int io_open_ft245r(int fd, int flags);
int io_open_sd_card(int fd, int flags);

#endif // ifndef io_io_h
