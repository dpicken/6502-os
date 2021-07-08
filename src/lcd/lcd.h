#ifndef lcd_lcd_h
#define lcd_lcd_h

#include "driver.h"

extern const lcd_driver* const lcd;

int lcd_write(const char* buf, unsigned int count);

#endif // ifndef lcd_lcd_h
