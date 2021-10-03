#ifndef lcd_drivers_hd44780_hd44780_constants_h
#define lcd_drivers_hd44780_hd44780_constants_h

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

#define HD44780_DR_BUSY                               0x80

#endif // ifndef lcd_drivers_hd44780_hd44780_constants_h