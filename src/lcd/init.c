#include "init.h"

#include "io.h"

#include "busy_wait.h"

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

void lcd_write_instruction(unsigned char instruction) {
  lcd_busy_wait();
  hw_lcd_data_write(instruction);
  hw_lcd_control_set(0);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_E);
  hw_lcd_control_set(0);
}

void lcd_init(void) {
  hw_lcd_control_direction_set_write();
  hw_lcd_data_direction_set_write();

  lcd_write_instruction(LCD_INSTRUCTION_FUNCTION_SET
      | LCD_FUNCTION_SET_DATA_LENGTH_8BIT
      | LCD_FUNCTION_SET_DISPLAY_LINES_2);

  lcd_write_instruction(LCD_INSTRUCTION_DISPLAY_CONTROL
      | LCD_DISPLAY_CONTROL_ON
      | LCD_DISPLAY_CONTROL_CURSOR_OFF
      | LCD_DISPLAY_CONTROL_CURSOR_BLINK_OFF);

  lcd_write_instruction(LCD_INSTRUCTION_ENTRY_MODE_SET
      | LCD_ENTRY_MODE_SET_INCREMENT
      | LCD_ENTRY_MODE_SET_SHIFT_DISPLAY_OFF);

  lcd_clear();
}

void lcd_home(void) {
  lcd_write_instruction(LCD_INSTRUCTION_RETURN_HOME);
  lcd_reset_xpos();
}

void lcd_clear(void) {
  lcd_write_instruction(LCD_INSTRUCTION_CLEAR_DISPLAY);
  lcd_reset_xpos();
}
