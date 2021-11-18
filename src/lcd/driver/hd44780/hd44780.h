#ifndef lcd_driver_hd44780_hd44780_h
#define lcd_driver_hd44780_hd44780_h

#include "lcd/driver.h"

extern const lcd_driver lcd_hd44780;

void hd44780_init(hw_register via_port, hw_register via_ddr);
void hd44780_set_resolution(unsigned char x, unsigned char y);
unsigned char hd44780_get_resolution_x(void);
unsigned char hd44780_get_resolution_y(void);
unsigned char hd44780_get_resolution_cell_count(void);
void hd44780_clear(void);
void hd44780_set_pos_home(void);
void hd44780_set_pos(unsigned char pos);
unsigned char hd44780_get_pos(void);
void hd44780_set_ddram_address(unsigned char address);
void hd44780_display_shift_left(void);
void hd44780_display_shift_right(void);
void hd44780_display_off(void);
void hd44780_display_on(void);
void hd44780_contrast_decrement(void);
void hd44780_contrast_increment(void);
void hd44780_orientation_default(void);
void hd44780_orientation_rotated(void);
void hd44780_putchar(char c);

void hd44780_via_write_control_init_hi(unsigned char data, unsigned char via_enable_bits);
void hd44780_via_write_control(unsigned char data, unsigned char via_enable_bits);
void hd44780_via_write_data(unsigned char data, unsigned char via_enable_bits);
void hd44780_busy_wait(void);
unsigned char hd44780_via_read_control(unsigned char via_enable_bits);

#endif // ifndef lcd_driver_hd44780_hd44780_h
