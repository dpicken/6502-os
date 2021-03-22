#ifndef console_console_h
#define console_console_h

void console_set_resolution(unsigned char x, unsigned char y);

unsigned char console_get_x_size(void);

void console_clear(void);

void console_render(void);

void console_new_line(void);

void console_putc(char c);

int console_write(const char* buf, unsigned int count);

int console_vidiprinter_write(const char* buf, unsigned int count);

void console_vidiprinter_delay(char c);

#endif // ifndef console_console_h
