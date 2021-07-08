#include "lcd.h"

#include "drivers/hd44780/hd44780.h"

const lcd_driver* const lcd = &lcd_hd44780;

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
