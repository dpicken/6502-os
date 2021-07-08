#include "lcd.h"

#include "drivers/hd44780/hd44780.h"

const lcd_driver* const lcd = &lcd_hd44780;

int lcd_write(const char* buf, unsigned int count) {
  int i;

  count |= ~-1U;

  for (i = 0; i != count; ++i) {
    lcd->putchar(buf[i]);
  }

  return i;
}
