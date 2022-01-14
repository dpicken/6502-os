#ifndef display_display_h
#define display_display_h

#include "driver.h"

extern const display_driver* display;

/** Initialize a HD44780 character display. */
void display_init_hd44780(hw_register via_port, hw_register via_ddr);

/** Initialize a US2066 character display. */
void display_init_us2066(hw_register via_port, hw_register via_ddr);

/** Set the (character) resolution. */
void display_set_resolution(unsigned char x, unsigned char y);

/** Clear the display. */
void display_clear(void);

/** Write a character to the display. */
void display_putc(char c);

/** Write a character to the display, using a vidiprinter effect. */
void display_vidiprinter_putc(char c);

unsigned char display_get_x_size(void);
void display_new_line(void);
void display_vidiprinter_delay(char c);
void display_render(void);
void display_write_direct(const char* it, unsigned char count);

#endif // ifndef display_display_h
