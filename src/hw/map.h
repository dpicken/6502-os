#ifndef hw_map_h
#define hw_map_h

#define HW_LCD_CONTROL_BIT_RS 0x20
#define HW_LCD_CONTROL_BIT_RW 0x40
#define HW_LCD_CONTROL_BIT_E  0x80

/** Set the LCD control direction to write. */
void hw_lcd_control_direction_set_write(void);

/** Set the LCD control bits; control direction must be set to write prior to calling this. */
void hw_lcd_control_set(unsigned char hw_lcd_control_bits);

/** Set the LCD data direction to write. */
void hw_lcd_data_direction_set_write(void);

/** Set the LCD data direction to read. */
void hw_lcd_data_direction_set_read(void);

/** Write data to the LCD; data direction must be set to write prior to calling this. */
void hw_lcd_data_write(unsigned char bits);

/** Read data from the LCD; data direction must be set to read prior to calling this. */
unsigned char hw_lcd_data_read(void);

#endif // ifndef hw_map_h
