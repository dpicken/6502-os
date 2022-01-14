#ifndef io_io_reader_h
#define io_io_reader_h

#include <limits.h>

typedef int (*io_reader)(char*, unsigned int);

#define IO_DECLARE_READER(fn_name) int fn_name(char* buffer, unsigned int count)

#define IO_DEFINE_READER(fn_name, read_byte_fn) int fn_name(char* buffer, unsigned int count) {\
  char* it = buffer;\
  const char* end = buffer + (count & INT_MAX);\
\
  while (it != end) {\
    *it++ = read_byte_fn();\
  }\
\
  return it - buffer;\
}\
unsigned char fn_name##_guard

#define IO_DEFINE_READER_NON_BLOCKING(fn_name, read_byte_ready_fn, read_byte_fn) int fn_name(char* buffer, unsigned int count) {\
  char* it = buffer;\
  const char* end = buffer + (count & INT_MAX);\
\
  while (read_byte_ready_fn() && it != end) {\
    *it++ = read_byte_fn();\
  }\
\
  if (it == buffer) {\
    return io_fail(EAGAIN);\
  }\
\
  return it - buffer;\
}\
unsigned char fn_name##_guard

#endif // ifndef io_io_reader_h
