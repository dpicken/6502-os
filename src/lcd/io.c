#include "busy_wait.h"

#include "hw/map.h"

#include <string.h>

void lcd_putc(char c) {
  lcd_busy_wait();
  hw_lcd_data_write(c);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_RS);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_RS | HW_LCD_CONTROL_BIT_E);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_RS);
}

void lcd_puts(const char* str) {
  unsigned char str_len = strlen(str);
  unsigned char pad_len = str_len < 40 ? 40 - str_len : 0;

  while (*str != '\0') {
    lcd_putc(*str++);
  }

  while (pad_len--) {
    lcd_putc(' ');
  }
}
