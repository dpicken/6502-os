#include "control.h"

#include "busy_wait.h"
#include "io.h"

#include "kernel/delay.h"

#define LCD_INSTRUCTION_SET_DDRAM_ADDRESS         0x80
#define LCD_INSTRUCTION_SET_CGRAM_ADDRESS         0x40
#define LCD_INSTRUCTION_FUNCTION_SET              0x20
#define LCD_INSTRUCTION_CURSOR_DISPLAY            0x10
#define LCD_INSTRUCTION_DISPLAY_CONTROL           0x08
#define LCD_INSTRUCTION_ENTRY_MODE_SET            0x04
#define LCD_INSTRUCTION_RETURN_HOME               0x02
#define LCD_INSTRUCTION_CLEAR_DISPLAY             0x01

#define LCD_FUNCTION_SET_DATA_LENGTH_8BIT         0x10
#define LCD_FUNCTION_SET_DATA_LENGTH_4BIT         0x00
#define LCD_FUNCTION_SET_DISPLAY_LINES_2          0x08
#define LCD_FUNCTION_SET_DISPLAY_LINES_1          0x00
#define LCD_FUNCTION_SET_DISPLAY_LINE_1_FONT_5_10 0x04
#define LCD_FUNCTION_SET_DISPLAY_LINE_1_FONT_5_8  0x00

#define LCD_CURSOR_DISPLAY_DISPLAY_SHIFT          0x08
#define LCD_CURSOR_DISPLAY_CURSOR_MOVE            0x00
#define LCD_CURSOR_DISPLAY_RIGHT                  0x04
#define LCD_CURSOR_DISPLAY_LEFT                   0x00

#define LCD_DISPLAY_CONTROL_ON                    0x04
#define LCD_DISPLAY_CONTROL_OFF                   0x00
#define LCD_DISPLAY_CONTROL_CURSOR_ON             0x02
#define LCD_DISPLAY_CONTROL_CURSOR_OFF            0x00
#define LCD_DISPLAY_CONTROL_CURSOR_BLINK_ON       0x01
#define LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF      0x00

#define LCD_ENTRY_MODE_SET_INCREMENT              0x02
#define LCD_ENTRY_MODE_SET_DECREMENT              0x00
#define LCD_ENTRY_MODE_SET_SHIFT_DISPLAY_ON       0x01
#define LCD_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF      0x00

void lcd_write_init_instruction_8bit(unsigned char instruction) {
  hw_lcd_write_8bit(instruction, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  delay_ms(5);
}

void lcd_write_init_instruction_4bit(unsigned char instruction) {
  hw_lcd_write_4bit(instruction, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  delay_ms(1);
}

void lcd_init(void) {
  hw_lcd_control_direction_set_write();
  hw_lcd_data_direction_set_write();

  delay_ms(15);

  lcd_write_init_instruction_8bit(LCD_INSTRUCTION_FUNCTION_SET
      | LCD_FUNCTION_SET_DATA_LENGTH_8BIT);
  lcd_write_init_instruction_8bit(LCD_INSTRUCTION_FUNCTION_SET
      | LCD_FUNCTION_SET_DATA_LENGTH_8BIT);
  lcd_write_init_instruction_8bit(LCD_INSTRUCTION_FUNCTION_SET
      | LCD_FUNCTION_SET_DATA_LENGTH_8BIT);

  lcd_write_init_instruction_8bit(LCD_INSTRUCTION_FUNCTION_SET
      | LCD_FUNCTION_SET_DATA_LENGTH_4BIT);
  lcd_write_init_instruction_4bit(LCD_INSTRUCTION_FUNCTION_SET
      | LCD_FUNCTION_SET_DATA_LENGTH_4BIT
      | LCD_FUNCTION_SET_DISPLAY_LINES_2);
  lcd_write_init_instruction_4bit(LCD_INSTRUCTION_DISPLAY_CONTROL
      | LCD_DISPLAY_CONTROL_ON
      | LCD_DISPLAY_CONTROL_CURSOR_OFF
      | LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF);
  lcd_write_init_instruction_4bit(LCD_INSTRUCTION_ENTRY_MODE_SET
      | LCD_ENTRY_MODE_SET_INCREMENT
      | LCD_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF);

  lcd_clear();
}

void lcd_clear(void) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_CLEAR_DISPLAY, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  lcd_reset_cached_pos();
}

void lcd_home(void) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_RETURN_HOME, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  lcd_reset_cached_pos();
}

void lcd_display_shift_left(void) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_CURSOR_DISPLAY | LCD_CURSOR_DISPLAY_DISPLAY_SHIFT | LCD_CURSOR_DISPLAY_LEFT, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
}

void lcd_display_shift_right(void) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_CURSOR_DISPLAY | LCD_CURSOR_DISPLAY_DISPLAY_SHIFT | LCD_CURSOR_DISPLAY_RIGHT, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
}

void lcd_display_off(void) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_DISPLAY_CONTROL | LCD_DISPLAY_CONTROL_OFF, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
}

void lcd_display_on(void) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_DISPLAY_CONTROL | LCD_DISPLAY_CONTROL_ON, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
}

void lcd_set_ddram_address(unsigned char address) {
  lcd_busy_wait();
  hw_lcd_write_4bit(LCD_INSTRUCTION_SET_DDRAM_ADDRESS | address, 0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
}
