#include "io.h"

#include "busy_wait.h"
#include "init.h"

#include "hw/map.h"

#include <string.h>

#define LCD_XPOS_MAX 40

#define LCD_E1_POS_END 80
#define LCD_E2_POS_END 160

static unsigned char lcd_xsize = LCD_XPOS_MAX;
static unsigned int lcd_pos_end = LCD_E2_POS_END;
static unsigned int lcd_pos;

void lcd_set_16x2(void) {
  lcd_xsize = 16;
  lcd_pos_end = 16 * 2;
  lcd_pos = 0;
}

void lcd_set_40x4(void) {
  lcd_xsize = 40;
  lcd_pos_end = LCD_E2_POS_END;
  lcd_pos = 0;
}

void lcd_putc(char c) {
  lcd_busy_wait();

  if (lcd_pos < LCD_E1_POS_END) {
    hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1);
  } else {
    hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E2);
  }

  ++lcd_pos;
  if (lcd_xsize < LCD_XPOS_MAX) {
    if (lcd_pos == lcd_xsize) {
      lcd_set_pos(LCD_XPOS_MAX);
    } else if (lcd_pos == lcd_pos_end) {
      lcd_set_pos(0);
    }
  } else {
    if (lcd_pos == lcd_pos_end) {
      lcd_set_pos(0);
    }
  }
}

void lcd_write(const char* buf, unsigned int count) {
  unsigned int i;

  for (i = 0; i != count; ++i) {
    lcd_putc(buf[i]);
  }
}

void lcd_set_pos(unsigned int pos) {
  lcd_pos = pos;
  if (lcd_xsize == LCD_XPOS_MAX) {
    lcd_set_pos_raw(lcd_pos);
  } else {
    if (lcd_pos < lcd_xsize) {
      lcd_set_pos_raw(lcd_pos);
    } else {
      lcd_set_pos_raw(LCD_XPOS_MAX + (lcd_pos - lcd_xsize));
    }
  }
}

void lcd_reset_cached_pos(void) {
  lcd_pos = 0;
}
