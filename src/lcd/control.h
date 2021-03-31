#ifndef lcd_control_h
#define lcd_control_h

#include "hw/map.h"

/** Initialize the LCD. */
void lcd_init(void);

/** Clear the LCD. */
void lcd_clear(void);

/** Reset the cursor's position. */
void lcd_home(void);

/** Shift the display left. */
void lcd_display_shift_left(void);

/** Shift the display right. */
void lcd_display_shift_right(void);

/** Turn the display off. */
void lcd_display_off(void);

/** Turn the display on. */
void lcd_display_on(void);

void lcd_set_ddram_address(unsigned char address);

#endif // ifndef lcd_lcd_control_h
