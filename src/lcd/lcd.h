#ifndef lcd_lcd_h
#define lcd_lcd_h

#include "driver.h"

extern const lcd_driver* const lcd;

/** Write a character buffer to the LCD. */
void lcd_write(const char* buf, unsigned int count);

/** Write a null-terminated string to the LCD. */
void lcd_write_str(const char* str);

#endif // ifndef lcd_lcd_h
