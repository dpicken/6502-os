#include "lcd.h"

#include "driver/hd44780/hd44780.h"
#include "driver/us2066/us2066.h"

const lcd_driver* lcd;

void lcd_init_hd44780(hw_register via_port, hw_register via_ddr) {
  hd44780_init(via_port, via_ddr);
  lcd = &lcd_hd44780;
}

void lcd_init_us2066(hw_register via_port, hw_register via_ddr) {
  us2066_init(via_port, via_ddr);
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
