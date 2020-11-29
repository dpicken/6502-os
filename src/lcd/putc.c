#ifndef lcd_putc_h
#define lcd_putc_h

#include "busy_wait.h"

#include "hw/map.h"

void lcd_putc(char c) {
  lcd_busy_wait();
  hw_lcd_data_write(c);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_RS);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_RS | HW_LCD_CONTROL_BIT_E);
  hw_lcd_control_set(HW_LCD_CONTROL_BIT_RS);
}

#endif // ifndef lcd_putc_h
