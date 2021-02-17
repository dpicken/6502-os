#include "busy_wait.h"

#include "hw/map.h"

#include <string.h>

#define LCD_XPOS_MAX 40

unsigned char lcd_xpos;

void lcd_reset_xpos(void) {
  lcd_xpos = 0;
}

void lcd_putc(char c) {
  lcd_busy_wait();

  hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);

  if (++lcd_xpos == LCD_XPOS_MAX) {
    lcd_reset_xpos();
  }
}

void lcd_newline(void) {
  unsigned char pad_len = LCD_XPOS_MAX - lcd_xpos;
  while (pad_len--) {
    lcd_putc(' ');
  }
}

void lcd_write(const char* buf, unsigned int count) {
  int i;

  for (i = 0; i != count; ++i) {
    if (buf[i] == '\n') {
      lcd_newline();
    } else {
      lcd_putc(buf[i]);
    }
  }
}

void lcd_puts(const char* str) {
  lcd_write(str, strlen(str));
  lcd_newline();
}
