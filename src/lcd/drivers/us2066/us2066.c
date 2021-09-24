#include "us2066.h"

#include "us2066_constants.h"

#include "kernel/delay.h"
#include "lcd/drivers/hd44780/hd44780_constants.h"
#include "hw/map.h"

#define US2066_XSIZE_MAX 20
#define US2066_LINE_DDRAM_ADDRESS_OFFSET 0x20

#define LCD_CONTRAST_MIN 0
#define LCD_CONTRAST_MAX 255
#define LCD_CONTRAST_STEP 32

static unsigned char lcd_config_function_set_display_lines = US2066_FUNCTION_SET_DISPLAY_LINES_EVEN;;
static unsigned char lcd_config_contrast = 0x7F;

static unsigned char lcd_xsize = US2066_XSIZE_MAX;
static unsigned char lcd_pos_end = 80;
static unsigned char lcd_pos;

const lcd_driver lcd_us2066 = {
  us2066_init,
  us2066_set_resolution,
  us2066_get_resolution_x,
  us2066_get_resolution_y,
  us2066_get_resolution_cell_count,
  us2066_clear,
  us2066_set_pos_home,
  us2066_set_pos,
  us2066_get_pos,
  us2066_display_shift_left,
  us2066_display_shift_right,
  us2066_display_off,
  us2066_display_on,
  us2066_contrast_decrement,
  us2066_contrast_increment,
  us2066_orientation_default,
  us2066_orientation_rotated,

  us2066_putchar
};

void us2066_init(void) {
  hw_lcd_hd44780_data_direction_set_write();

  us2066_init_write_byte(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | HD44780_ENTRY_MODE_SET_INCREMENT
      | HD44780_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF);

  us2066_orientation_default();
  us2066_contrast_set();
  us2066_clear();

  us2066_init_write_byte(HD44780_INSTRUCTION_DISPLAY_CONTROL
      | HD44780_DISPLAY_CONTROL_ON
      | HD44780_DISPLAY_CONTROL_CURSOR_OFF
      | HD44780_DISPLAY_CONTROL_CURSOR_BLINK_OFF);
}

void us2066_init_write_byte(unsigned char instruction) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(instruction, 0, HW_LCD_HD44780_CONTROL_E1);
}

void us2066_set_resolution(unsigned char x, unsigned char y) {
  lcd_xsize = x;
  lcd_pos_end = x * y;
  lcd_pos = 0;
}

unsigned char us2066_get_resolution_x(void) {
  return lcd_xsize;
}

unsigned char us2066_get_resolution_y(void) {
  return lcd_pos_end / lcd_xsize;
}

unsigned char us2066_get_resolution_cell_count(void) {
  return lcd_pos_end;
}

void us2066_clear(void) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_CLEAR_DISPLAY, 0, HW_LCD_HD44780_CONTROL_E1);
  lcd_pos = 0;
}

void us2066_set_pos_home(void) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_RETURN_HOME, 0, HW_LCD_HD44780_CONTROL_E1);
  lcd_pos = 0;
}

void us2066_set_pos(unsigned char pos) {
  unsigned char x = pos % lcd_xsize;
  unsigned char y = pos / lcd_xsize;

  us2066_set_ddram_address((US2066_LINE_DDRAM_ADDRESS_OFFSET * y) + x);
  lcd_pos = pos;
}

unsigned char us2066_get_pos(void) {
  return lcd_pos;
}

void us2066_set_ddram_address(unsigned char address) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_SET_DDRAM_ADDRESS | address, 0, HW_LCD_HD44780_CONTROL_E1);
}

void us2066_display_shift_left(void) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_LEFT, 0, HW_LCD_HD44780_CONTROL_E1);
}

void us2066_display_shift_right(void) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_RIGHT, 0, HW_LCD_HD44780_CONTROL_E1);
}

void us2066_display_off(void) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_OFF, 0, HW_LCD_HD44780_CONTROL_E1);
}

void us2066_display_on(void) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_ON, 0, HW_LCD_HD44780_CONTROL_E1);
}

void us2066_contrast_decrement(void) {
  if (lcd_config_contrast > LCD_CONTRAST_STEP) {
    lcd_config_contrast -= LCD_CONTRAST_STEP;
  } else {
    lcd_config_contrast = LCD_CONTRAST_MIN;
  }
  us2066_contrast_set();
}

void us2066_contrast_increment(void) {
  if (lcd_config_contrast < LCD_CONTRAST_MAX - LCD_CONTRAST_STEP) {
    lcd_config_contrast += LCD_CONTRAST_STEP;
  } else {
    lcd_config_contrast = LCD_CONTRAST_MAX;
  }
  us2066_contrast_set();
}

void us2066_contrast_set(void) {
  us2066_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | lcd_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_SET);
  us2066_init_write_byte(US2066_INSTRUCTION_RE_OLED_CHARACTERIZATION
      | US2066_RE_OLED_CHARACTERIZATION_SD_SET);

  us2066_init_write_byte(US2066_INSTRUCTION_RE_SD_SET_CONTRAST_1);
  us2066_init_write_byte(US2066_INSTRUCTION_RE_SD_SET_CONTRAST_2
      | lcd_config_contrast);

  us2066_init_write_byte(US2066_INSTRUCTION_RE_OLED_CHARACTERIZATION
      | US2066_RE_OLED_CHARACTERIZATION_SD_RESET);
  us2066_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | lcd_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_RESET);
}

void us2066_orientation_default(void) {
  us2066_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | lcd_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_SET);
  us2066_init_write_byte(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_COMMON_SET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_SEGMENT_RESET);
  us2066_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | lcd_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_RESET);
}

void us2066_orientation_rotated(void) {
  us2066_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | lcd_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_SET);
  us2066_init_write_byte(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_COMMON_RESET
      | US2066_RE_ENTRY_MODE_SET_BI_DIRECTION_SEGMENT_SET);
  us2066_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | lcd_config_function_set_display_lines
      | US2066_FUNCTION_SET_EXTENSION_RE_RESET);
}

void us2066_putchar(char c) {
  us2066_busy_wait();
  hw_lcd_hd44780_write_byte(c, HW_LCD_HD44780_CONTROL_RS, HW_LCD_HD44780_CONTROL_E1);

  ++lcd_pos;
  if (lcd_pos == lcd_pos_end) {
    lcd_pos = 0;
  }

  if (lcd_pos % lcd_xsize == 0) {
    us2066_set_pos(lcd_pos);
  }
}

void us2066_busy_wait(void) {
  char data;

  hw_lcd_hd44780_data_direction_set_read();
  do {
    data = hw_lcd_hd44780_read_byte(0, HW_LCD_HD44780_CONTROL_E1);
  } while (data & HD44780_DR_BUSY);
  hw_lcd_hd44780_data_direction_set_write();
}
