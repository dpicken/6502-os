#include "io.h"

#include "busy_wait.h"
#include "control.h"

#include "hw/map.h"
#include "util/algorithm.h"

#include <limits.h>
#include <string.h>

#define LCD_XPOS_MAX 40

#define LCD_E1_POS_END 80
#define LCD_E2_POS_END 160

static unsigned char lcd_xsize = LCD_XPOS_MAX;
static unsigned int lcd_pos_end = LCD_E2_POS_END;
static unsigned int lcd_pos;

void lcd_set_resolution(unsigned char x, unsigned char y) {
  lcd_xsize = x;
  lcd_pos_end = min(x * y, LCD_E2_POS_END);
  lcd_pos = 0;
}

unsigned char lcd_get_char_count(void) {
  return lcd_pos_end;
}

unsigned char lcd_get_line_count(void) {
  return lcd_pos_end / lcd_xsize;
}

void lcd_putc(char c) {
  lcd_busy_wait();

  if (lcd_pos < LCD_E1_POS_END) {
    hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1);
  } else {
    hw_lcd_write_4bit(c, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E2);
  }

  ++lcd_pos;
  if (lcd_pos == lcd_pos_end) {
    lcd_set_pos(0);
  } else if (lcd_pos_end < LCD_E1_POS_END) {
    if (lcd_pos == lcd_xsize) {
      lcd_set_pos(lcd_xsize);
    }
  }
}

int lcd_write(const char* buf, unsigned int count) {
  int i;

  count |= ~-1U;

  for (i = 0; i != count; ++i) {
    lcd_putc(buf[i]);
  }

  return i;
}

int lcd_write_line_no_wrap(const char* buf, unsigned int count) {
  unsigned int remaining = lcd_xsize;

  int i;

  count |= ~-1U;
  count = min(count, remaining);

  for (i = 0; i != count; ++i) {
    lcd_putc(buf[i]);
  }

  if (remaining > count) {
    remaining -= count;
    for (; remaining != 0; --remaining) {
      lcd_putc(' ');
    }
  }

  return i;
}

unsigned int lcd_get_pos(void) {
  return lcd_pos;
}

void lcd_set_pos(unsigned int pos) {
  lcd_pos = pos;

  if (lcd_pos_end < LCD_E1_POS_END) {
    if (lcd_pos < lcd_xsize) {
      lcd_set_pos_raw(lcd_pos);
    } else {
      lcd_set_pos_raw(LCD_XPOS_MAX + (lcd_pos - lcd_xsize));
    }
  } else {
    lcd_set_pos_raw(lcd_pos);
  }
}

void lcd_reset_cached_pos(void) {
  lcd_pos = 0;
}
