#ifndef lcd_io_h
#define lcd_io_h

/** Set the resolution. */
void lcd_set_resolution(unsigned char x, unsigned char y);

/** Get the (configured) size. */
unsigned char lcd_get_char_count(void);

/** Get the number of (configured) lines. */
unsigned char lcd_get_line_count(void);

/** Write a char to the LCD. */
void lcd_putc(char c);

/** Write a sequence of chars to the LCD. */
int lcd_write(const char* buf, unsigned int count);

/** Write a sequence of chars to the LCD, truncating to fit the current line. */
int lcd_write_line_no_wrap(const char* buf, unsigned int count);

/** Get the position of the cursor. */
unsigned char lcd_get_pos(void);

/** Set the position of the cursor. */
void lcd_set_pos(unsigned char pos);

void lcd_reset_cached_pos(void);

#endif // ifndef lcd_io_h
