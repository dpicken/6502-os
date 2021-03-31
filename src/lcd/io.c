#include "io.h"

#include "busy_wait.h"
#include "control.h"

#include "hw/map.h"
#include "util/algorithm.h"

#include <limits.h>
#include <string.h>

#define LCD_E1_POS_END 80
#define LCD_E2_POS_END 160

static unsigned char lcd_xsize = LCD_XSIZE_MAX;
static unsigned char lcd_pos_end = LCD_E2_POS_END;
static unsigned char lcd_pos;

void lcd_set_resolution(unsigned char x, unsigned char y) {
  lcd_xsize = x;
  lcd_pos_end = x * y;
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
    lcd_pos = 0;
  }

  if (lcd_pos % lcd_xsize == 0) {
    lcd_set_pos(lcd_pos);
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

unsigned char lcd_get_pos(void) {
  return lcd_pos;
}

void lcd_set_pos(unsigned char pos) {
  unsigned char x = pos % lcd_xsize;
  unsigned char y = pos / lcd_xsize;

  if (lcd_pos_end == LCD_E2_POS_END && y > 1) {
    // A 40x4 line LCD has two 40x2 controllers.
    y -= 2;
  }

  lcd_pos = pos;

  if (y == 0) {
    lcd_set_ddram_address(LCD_LINE_1_DDRAM_ADDRESS_BASE + x);
  } else if (y == 1) {
    lcd_set_ddram_address(LCD_LINE_2_DDRAM_ADDRESS_BASE + x);
  } else if (y == 2) {
    lcd_set_ddram_address(LCD_LINE_1_DDRAM_ADDRESS_BASE + lcd_xsize + x);
  } else {
    lcd_set_ddram_address(LCD_LINE_2_DDRAM_ADDRESS_BASE + lcd_xsize + x);
  }
}

void lcd_reset_cached_pos(void) {
  lcd_pos = 0;
}
