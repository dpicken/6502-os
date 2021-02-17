#include "busy_wait.h"

#include "hw/map.h"

#define LCD_DR_BUSY 0x80

void lcd_busy_wait(void) {
  char data;

  hw_lcd_data_direction_set_read();
  do {
    data = hw_lcd_read_4bit(0, HW_LCD_CONTROL_E1 | HW_LCD_CONTROL_E2);
  } while (data & LCD_DR_BUSY);
  hw_lcd_data_direction_set_write();
}
