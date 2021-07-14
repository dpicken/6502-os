#include "lcd.h"

#include "drivers/hd44780/hd44780.h"
#include "drivers/us2066/us2066.h"

const lcd_driver* const lcd_driver_hd44780 = &lcd_hd44780;
const lcd_driver* const lcd_driver_us2066 = &lcd_us2066;

const lcd_driver* const lcd = &lcd_us2066;

void lcd_write(const char* buf, unsigned int count) {
  int i;

  for (i = 0; i != count; ++i) {
    lcd->putchar(buf[i]);
  }
}

void lcd_write_str(const char* str) {
  while (*str != '\0') {
    lcd->putchar(*str);
    ++str;
  }
}
