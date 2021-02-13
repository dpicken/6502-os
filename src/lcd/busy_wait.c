#include "busy_wait.h"

#include "hw/map.h"

#define LCD_DATA_BUSY_FLAG 0x80

void lcd_busy_wait(void) {
  char busy;

  hw_lcd_data_direction_set_read();
  do {
    hw_lcd_control_set(HW_LCD_CONTROL_BIT_RW);
    hw_lcd_control_set(HW_LCD_CONTROL_BIT_RW | HW_LCD_CONTROL_BIT_E);
    busy = hw_lcd_data_read() & LCD_DATA_BUSY_FLAG;

    hw_lcd_control_set(HW_LCD_CONTROL_BIT_RW);
    hw_lcd_control_set(HW_LCD_CONTROL_BIT_RW | HW_LCD_CONTROL_BIT_E);
    hw_lcd_data_read();
  } while (busy);
  hw_lcd_control_set(0);
  hw_lcd_data_direction_set_write();
}
