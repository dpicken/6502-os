#ifndef lcd_drivers_us2066_us2066_h
#define lcd_drivers_us2066_us2066_h

#include "lcd/driver.h"

extern const lcd_driver lcd_us2066;

void us2066_init(void);
void us2066_init_write_byte(unsigned char instruction);
void us2066_set_resolution(unsigned char x, unsigned char y);
unsigned char us2066_get_resolution_x(void);
unsigned char us2066_get_resolution_y(void);
unsigned char us2066_get_resolution_cell_count(void);
void us2066_clear(void);
void us2066_set_pos_home(void);
void us2066_set_pos(unsigned char pos);
unsigned char us2066_get_pos(void);
void us2066_set_ddram_address(unsigned char address);
void us2066_display_shift_left(void);
void us2066_display_shift_right(void);
void us2066_display_off(void);
void us2066_display_on(void);
void us2066_contrast_decrement(void);
void us2066_contrast_increment(void);
void us2066_contrast_set(void);
void us2066_orientation_default(void);
void us2066_orientation_rotated(void);
void us2066_putchar(char c);
void us2066_busy_wait(void);

#endif // ifndef lcd_drivers_us2066_us2066_h
