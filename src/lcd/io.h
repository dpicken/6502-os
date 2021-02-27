#ifndef lcd_io_h
#define lcd_io_h

/** Set output to a 16x2 LCD. */
void lcd_set_16x2(void);

/** Set output to a 40x4 LCD. */
void lcd_set_40x4(void);

/** Write a char to the LCD. */
void lcd_putc(char c);

/** Write a sequence of chars to the LCD. */
void lcd_write(const char* buf, unsigned int count);

/** Set the position of the cursor. */
void lcd_set_pos(unsigned int pos);

void lcd_reset_cached_pos(void);

#endif // ifndef lcd_io_h
