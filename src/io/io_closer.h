#ifndef io_io_closer_h
#define io_io_closer_h

typedef int (*io_closer)(void);

#define IO_DECLARE_CLOSER(fn_name) int fn_name(void)

#define IO_DEFINE_CLOSER(fn_name, close_fn) int fn_name(void) {\
  close_fn();\
  return 0;\
}\
unsigned char fn_name##_guard

#endif // ifndef io_io_closer_h
