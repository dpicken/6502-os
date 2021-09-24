#include "lcd.h"

#include "drivers/hd44780/hd44780.h"
#include "drivers/us2066/us2066.h"

const lcd_driver* lcd = &lcd_us2066;

void lcd_init_hd44780(void) {
  lcd_hd44780.init();
  lcd = &lcd_hd44780;
}

void lcd_init_us2066(void) {
  lcd_us2066.init();
  lcd = &lcd_us2066;
}

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
