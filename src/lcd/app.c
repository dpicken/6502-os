#include "app.h"

#include "control.h"
#include "io.h"

#include "button/event.h"
#include "timer/timer.h"
#include "util/algorithm.h"
#include "util/hex.h"

#include <stdint.h>
#include <stdio.h>

#define CHAR_SET_END (1 << 8)

static int char_set_pos;
static unsigned char ddram_address;

void lcd_app_enter(void) {
  button_released_set_handler(lcd_app_char_set_previous_page, button_code_up);
  button_released_set_handler(lcd_app_char_set_next_page, button_code_down);
  button_released_set_handler(lcd_display_shift_left, button_code_left);
  button_released_set_handler(lcd_display_shift_right, button_code_right);
  button_released_set_handler(lcd_app_ddram_fill, button_code_fire1);
  button_depressed_set_handler(lcd_display_off, button_code_fire2);
  button_released_set_handler(lcd_display_on, button_code_fire2);
  button_released_set_handler(lcd_app_ddram_visual_walk, button_code_fire3);

  lcd_app_char_set_home();
}

void lcd_app_char_set_home(void) {
  lcd_home();
  char_set_pos = 0;
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_next_page(void) {
  if (char_set_pos < CHAR_SET_END) {
    char_set_pos += lcd_get_char_count();
  }
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_previous_page(void) {
  if (char_set_pos > 0) {
    char_set_pos -= lcd_get_char_count();
  }
  lcd_app_char_set_dump_page();
}

void lcd_app_char_set_dump_page(void) {
  int char_set_page_end = char_set_pos + lcd_get_char_count();
  int char_set_end = min(char_set_page_end, CHAR_SET_END);

  int i;

  lcd_clear();
  for (i = char_set_pos; i < char_set_end; ++i) {
    lcd_putc(i);
  }
}

void lcd_app_ddram_fill(void) {
  unsigned char ddram_address;

  lcd_set_ddram_address(LCD_LINE_1_DDRAM_ADDRESS_BASE);
  for (ddram_address = LCD_LINE_1_DDRAM_ADDRESS_BASE; ddram_address != LCD_LINE_1_DDRAM_ADDRESS_END; ++ddram_address) {
    hw_lcd_write_4bit(0xFF, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  }

  lcd_set_ddram_address(LCD_LINE_2_DDRAM_ADDRESS_BASE);
  for (ddram_address = LCD_LINE_2_DDRAM_ADDRESS_BASE; ddram_address != LCD_LINE_2_DDRAM_ADDRESS_END; ++ddram_address) {
    hw_lcd_write_4bit(0xFF, HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  }
}

void lcd_app_ddram_visual_walk(void) {
  lcd_clear();

  lcd_set_ddram_address(ddram_address);
  hw_lcd_write_4bit(hex_get_hi_char(ddram_address), HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  hw_lcd_write_4bit(hex_get_lo_char(ddram_address), HW_LCD_CONTROL_RS, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);

  ++ddram_address;
  if (ddram_address == LCD_LINE_2_DDRAM_ADDRESS_END) {
    ddram_address = 0;
  } else {
    if (ddram_address == LCD_LINE_1_DDRAM_ADDRESS_END) {
      ddram_address = LCD_LINE_2_DDRAM_ADDRESS_BASE;
    }
    timer_add_one_shot(lcd_app_ddram_visual_walk, 500);
  }
}
