#ifndef io_io_writer_h
#define io_io_writer_h

#include <limits.h>

typedef int (*io_writer)(const char*, unsigned int);

#define IO_DECLARE_WRITER(fn_name) int fn_name(const char* buffer, unsigned int count)

#define IO_DEFINE_WRITER(fn_name, write_byte_fn) int fn_name(const char* buffer, unsigned int count) {\
  const char* it = buffer;\
  const char* end = buffer + (count & INT_MAX);\
\
  while (it != end) {\
    write_byte_fn(*it++);\
  }\
\
  return it - buffer;\
}\
unsigned char fn_name##_guard

#define IO_DEFINE_WRITER_NON_BLOCKING(fn_name, write_byte_ready_fn, write_byte_fn) int fn_name(const char* buffer, unsigned int count) {\
  const char* it = buffer;\
  const char* end = buffer + (count & INT_MAX);\
\
  while (write_byte_ready_fn() && it != end) {\
    write_byte_fn(*it++);\
  }\
\
  if (it == buffer) {\
    return io_fail(EAGAIN);\
  }\
\
  return it - buffer;\
}\
unsigned char fn_name##_guard

#endif // ifndef io_io_writer_h
