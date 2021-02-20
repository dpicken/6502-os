#include "io.h"

#include "busy_wait.h"
#include "init.h"

#include "hw/map.h"

#include <string.h>

#define LCD_XPOS_MAX 40

#define LCD_LINE_1_POS_END 40
#define LCD_LINE_2_POS_END 80
#define LCD_LINE_3_POS_END 120
#define LCD_LINE_4_POS_END 160

#define LCD_E1_POS_END LCD_LINE_2_POS_END
#define LCD_E2_POS_END LCD_LINE_4_POS_END

unsigned int lcd_pos_end = LCD_LINE_2_POS_END;
unsigned int lcd_pos;

void lcd_reset_cached_pos(void) {
  lcd_pos = 0;
}

void lcd_set_cached_pos(unsigned int pos) {
  lcd_pos = pos;
}

void lcd_putc(char c) {
  lcd_busy_wait();

  if (lcd_pos < LCD_E1_POS_END) {
    hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1);
  } else {
    hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E2);
  }

  if (++lcd_pos == lcd_pos_end) {
    lcd_reset_cached_pos();
  }
}

void lcd_newline(void) {
  if (lcd_pos < LCD_LINE_1_POS_END) {
    lcd_set_pos(LCD_LINE_1_POS_END);
  } else if (lcd_pos_end < LCD_LINE_4_POS_END) {
    lcd_set_pos(0);
  } else if (lcd_pos > LCD_LINE_1_POS_END && lcd_pos < LCD_LINE_2_POS_END) {
    lcd_set_pos(LCD_LINE_2_POS_END);
  } else if (lcd_pos > LCD_LINE_2_POS_END && lcd_pos < LCD_LINE_3_POS_END) {
    lcd_set_pos(LCD_LINE_3_POS_END);
  } else {
    lcd_set_pos(0);
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
