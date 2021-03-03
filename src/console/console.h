#ifndef console_console_h
#define console_console_h

void console_set_resolution(unsigned char x, unsigned char y);

void console_clear(void);

void console_render(void);

void console_new_line(void);

void console_putc(char c);

int console_write(const char* buf, unsigned int count);

int console_write_vidiprinter(const char* buf, unsigned int count);

#endif // ifndef console_console_h
