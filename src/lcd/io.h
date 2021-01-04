#ifndef lcd_io_h
#define lcd_io_h

void lcd_reset_xpos(void);

void lcd_putc(char c);

void lcd_newline(void);

void lcd_write(const char* buf, unsigned int count);

void lcd_puts(const char* str);

#endif // ifndef lcd_io_h
