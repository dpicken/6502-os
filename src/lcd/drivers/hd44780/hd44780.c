#include "hd44780.h"

#include "kernel/delay.h"
#include "hw/map.h"

#define HD44780_INSTRUCTION_SET_DDRAM_ADDRESS         0x80
#define HD44780_INSTRUCTION_SET_CGRAM_ADDRESS         0x40
#define HD44780_INSTRUCTION_FUNCTION_SET              0x20
#define HD44780_INSTRUCTION_CURSOR_DISPLAY            0x10
#define HD44780_INSTRUCTION_DISPLAY_CONTROL           0x08
#define HD44780_INSTRUCTION_ENTRY_MODE_SET            0x04
#define HD44780_INSTRUCTION_RETURN_HOME               0x02
#define HD44780_INSTRUCTION_CLEAR_DISPLAY             0x01

#define HD44780_FUNCTION_SET_DATA_LENGTH_8BIT         0x10
#define HD44780_FUNCTION_SET_DATA_LENGTH_4BIT         0x00
#define HD44780_FUNCTION_SET_DISPLAY_LINES_2          0x08
#define HD44780_FUNCTION_SET_DISPLAY_LINES_1          0x00
#define HD44780_FUNCTION_SET_DISPLAY_LINE_1_FONT_5_10 0x04
#define HD44780_FUNCTION_SET_DISPLAY_LINE_1_FONT_5_8  0x00

#define HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT          0x08
#define HD44780_CURSOR_DISPLAY_CURSOR_MOVE            0x00
#define HD44780_CURSOR_DISPLAY_RIGHT                  0x04
#define HD44780_CURSOR_DISPLAY_LEFT                   0x00

#define HD44780_DISPLAY_CONTROL_ON                    0x04
#define HD44780_DISPLAY_CONTROL_OFF                   0x00
#define HD44780_DISPLAY_CONTROL_CURSOR_ON             0x02
#define HD44780_DISPLAY_CONTROL_CURSOR_OFF            0x00
#define HD44780_DISPLAY_CONTROL_CURSOR_BLINK_ON       0x01
#define HD44780_DISPLAY_CONTROL_CURSOR_BLINK_OFF      0x00

#define HD44780_ENTRY_MODE_SET_INCREMENT              0x02
#define HD44780_ENTRY_MODE_SET_DECREMENT              0x00
#define HD44780_ENTRY_MODE_SET_SHIFT_DISPLAY_ON       0x01
#define HD44780_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF      0x00

#define HD44780_DR_BUSY 0x80

#define HD44780_XSIZE_MAX 40
#define HD44780_E1_POS_END 80
#define HD44780_E2_POS_END 160

#define HD44780_LINE_1_DDRAM_ADDRESS_BASE 0x00
#define HD44780_LINE_2_DDRAM_ADDRESS_BASE 0x40

#define HD44780_LINE_1_DDRAM_ADDRESS_END (HD44780_LINE_1_DDRAM_ADDRESS_BASE + HD44780_XSIZE_MAX)
#define HD44780_LINE_2_DDRAM_ADDRESS_END (HD44780_LINE_2_DDRAM_ADDRESS_BASE + HD44780_XSIZE_MAX)

static unsigned char lcd_xsize = HD44780_XSIZE_MAX;
static unsigned char lcd_pos_end = HD44780_E2_POS_END;
static unsigned char lcd_pos;

const lcd_driver lcd_hd44780 = {
  hd44780_init,
  hd44780_set_resolution,
  hd44780_get_resolution_x,
  hd44780_get_resolution_y,
  hd44780_get_resolution_cell_count,
  hd44780_clear,
  hd44780_set_pos_home,
  hd44780_set_pos,
  hd44780_get_pos,
  hd44780_display_shift_left,
  hd44780_display_shift_right,
  hd44780_display_off,
  hd44780_display_on,
  hd44780_contrast_decrement,
  hd44780_contrast_increment,

  hd44780_putchar
};

void hd44780_init(void) {
  hw_lcd_hd44780_data_direction_set_write();

  delay_ms(15);

  hd44780_init_write_hi_nibble(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_8BIT);
  hd44780_init_write_hi_nibble(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_8BIT);
  hd44780_init_write_hi_nibble(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_8BIT);

  hd44780_init_write_hi_nibble(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_4BIT);
  hd44780_init_write_byte(HD44780_INSTRUCTION_FUNCTION_SET
      | HD44780_FUNCTION_SET_DATA_LENGTH_4BIT
      | HD44780_FUNCTION_SET_DISPLAY_LINES_2);
  hd44780_init_write_byte(HD44780_INSTRUCTION_DISPLAY_CONTROL
      | HD44780_DISPLAY_CONTROL_ON
      | HD44780_DISPLAY_CONTROL_CURSOR_OFF
      | HD44780_DISPLAY_CONTROL_CURSOR_BLINK_OFF);
  hd44780_init_write_byte(HD44780_INSTRUCTION_ENTRY_MODE_SET
      | HD44780_ENTRY_MODE_SET_INCREMENT
      | HD44780_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF);

  hd44780_clear();
}

void hd44780_init_write_hi_nibble(unsigned char instruction) {
  hw_lcd_hd44780_write_hi_nibble(instruction, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
  delay_ms(5);
}

void hd44780_init_write_byte(unsigned char instruction) {
  hw_lcd_hd44780_write_byte(instruction, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
  delay_ms(1);
}

void hd44780_busy_wait(void) {
  char data;

  hw_lcd_hd44780_data_direction_set_read();
  do {
    data = hw_lcd_hd44780_read_byte(0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
  } while (data & HD44780_DR_BUSY);
  hw_lcd_hd44780_data_direction_set_write();
}

void hd44780_set_resolution(unsigned char x, unsigned char y) {
  lcd_xsize = x;
  lcd_pos_end = x * y;
  lcd_pos = 0;
}

unsigned char hd44780_get_resolution_x(void) {
  return lcd_xsize;
}

unsigned char hd44780_get_resolution_y(void) {
  return lcd_pos_end / lcd_xsize;
}

unsigned char hd44780_get_resolution_cell_count(void) {
  return lcd_pos_end;
}

void hd44780_clear(void) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_CLEAR_DISPLAY, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
  lcd_pos = 0;
}

void hd44780_set_pos_home(void) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_RETURN_HOME, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
  lcd_pos = 0;
}

void hd44780_set_pos(unsigned char pos) {
  unsigned char x = pos % lcd_xsize;
  unsigned char y = pos / lcd_xsize;

  if (lcd_pos_end == HD44780_E2_POS_END && y > 1) {
    // A 40x4 line LCD has two 40x2 controllers.
    y -= 2;
  }

  if (y == 0) {
    hd44780_set_ddram_address(HD44780_LINE_1_DDRAM_ADDRESS_BASE + x);
  } else if (y == 1) {
    hd44780_set_ddram_address(HD44780_LINE_2_DDRAM_ADDRESS_BASE + x);
  } else if (y == 2) {
    hd44780_set_ddram_address(HD44780_LINE_1_DDRAM_ADDRESS_BASE + lcd_xsize + x);
  } else {
    hd44780_set_ddram_address(HD44780_LINE_2_DDRAM_ADDRESS_BASE + lcd_xsize + x);
  }

  lcd_pos = pos;
}

unsigned char hd44780_get_pos(void) {
  return lcd_pos;
}

void hd44780_set_ddram_address(unsigned char address) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_SET_DDRAM_ADDRESS | address, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
}

void hd44780_display_shift_left(void) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_LEFT, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
}

void hd44780_display_shift_right(void) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_CURSOR_DISPLAY | HD44780_CURSOR_DISPLAY_DISPLAY_SHIFT | HD44780_CURSOR_DISPLAY_RIGHT, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
}

void hd44780_display_off(void) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_OFF, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
}

void hd44780_display_on(void) {
  hd44780_busy_wait();
  hw_lcd_hd44780_write_byte(HD44780_INSTRUCTION_DISPLAY_CONTROL | HD44780_DISPLAY_CONTROL_ON, 0, HW_LCD_HD44780_CONTROL_E1 | HW_LCD_HD44780_CONTROL_E2);
}

void hd44780_contrast_decrement(void) {
}

void hd44780_contrast_increment(void) {
}

void hd44780_putchar(char c) {
  hd44780_busy_wait();
  if (lcd_pos < HD44780_E1_POS_END) {
    hw_lcd_hd44780_write_byte(c, HW_LCD_HD44780_CONTROL_RS, HW_LCD_HD44780_CONTROL_E1);
  } else {
    hw_lcd_hd44780_write_byte(c, HW_LCD_HD44780_CONTROL_RS, HW_LCD_HD44780_CONTROL_E2);
  }

  ++lcd_pos;
  if (lcd_pos == lcd_pos_end) {
    lcd_pos = 0;
  }

  if (lcd_pos % lcd_xsize == 0) {
    hd44780_set_pos(lcd_pos);
  }
}
