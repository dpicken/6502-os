#ifndef lcd_io_h
#define lcd_io_h

/** Write a char to the LCD. */
void lcd_putc(char c);

/** Write a sequence of chars to the LCD. */
void lcd_write(const char* buf, unsigned int count);

void lcd_reset_cached_pos(void);
void lcd_set_cached_pos(unsigned int pos);

#endif // ifndef lcd_io_h
