#ifndef lcd_init_h
#define lcd_init_h

#include "hw/map.h"

/** Initialize the LCD. */
void lcd_init(void);

/** Clear the LCD. */
void lcd_clear(void);

/** Reset the cursor's position. */
void lcd_home(void);

void lcd_set_pos_raw(unsigned int pos);

#endif // ifndef lcd_lcd_init_h
