#ifndef console_console_h
#define console_console_h

void console_init_16x2(void);

void console_init_40x4(void);

unsigned char console_get_line_count(void);

void console_clear(void);

void console_render(void);

void console_new_line(void);

void console_putc(char c);

int console_write(const char* buf, unsigned int count);

#endif // ifndef console_console_h
