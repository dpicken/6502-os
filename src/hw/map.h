#ifndef hw_map_h
#define hw_map_h

#define HW_BUTTON_0           0x01
#define HW_BUTTON_1           0x02
#define HW_BUTTON_2           0x04
#define HW_BUTTON_3           0x08
#define HW_BUTTON_4           0x10
#define HW_BUTTON_5           0x20
#define HW_BUTTON_6           0x40

#define HW_BUZZER             0x80

#define HW_LCD_HD44780_CONTROL_RS   0x01
#define HW_LCD_HD44780_CONTROL_R_WB 0x02
#define HW_LCD_HD44780_CONTROL_E1   0x04
#define HW_LCD_HD44780_CONTROL_E2   0x08

/** Get the CPU frequency. */
unsigned long hw_cpu_get_frequency(void);

/** Start the fixed rate timer. */
void hw_timer_fixed_rate_start(unsigned int frequency);

/** Reset the fixed rate timer interrupt; returns true if it was set. */
unsigned char hw_timer_fixed_rate_interrupt_reset(void);

/** Set the buzzer direction to write. */
void hw_buzzer_direction_set_write(void);

/** Set the buzzer. */
void hw_buzzer_set(void);

/** Reset the buzzer. */
void hw_buzzer_reset(void);

/** Set the button direction to read. */
void hw_button_direction_set_read(void);

/** Read the button bits. */
unsigned char hw_button_read(void);

/** Set the LCD data direction to write. */
void hw_lcd_hd44780_data_direction_set_write(void);

/** Set the LCD data direction to read. */
void hw_lcd_hd44780_data_direction_set_read(void);

/** Write 4 high order bits of data to the LCD; data direction must be set to write prior to calling this. */
void hw_lcd_hd44780_write_hi_nibble(unsigned char data, unsigned char rs, unsigned char e);

/** Write 1 byte of data to the LCD, in 4 bit mode; data direction must be set to write prior to calling this. */
void hw_lcd_hd44780_write_byte(unsigned char data, unsigned char rs, unsigned char e);

/** Read 1 byte of data from the LCD, in 4 bit mode; data direction must be set to read prior to calling this. */
unsigned char hw_lcd_hd44780_read_byte(unsigned char rs, unsigned char e);

#endif // ifndef hw_map_h
