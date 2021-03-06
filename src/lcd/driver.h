#ifndef lcd_driver_h
#define lcd_driver_h

typedef struct lcd_driver {
  /** Initialize display. */
  void (*init)(void);

  /** Set the resolution. */
  void (*set_resolution)(unsigned char x, unsigned char y);

  /** Get the horizontal resolution. */
  unsigned char (*get_resolution_x)(void);

  /** Get the vertical resolution. */
  unsigned char (*get_resolution_y)(void);

  /** Get the number of display cells. */
  unsigned char (*get_resolution_cell_count)(void);

  /** Clear the display. */
  void (*clear)(void);

  /** Reset the cursor's position. */
  void (*set_pos_home)(void);

  /** Set the cursor's position. */
  void (*set_pos)(unsigned char pos);

  /** Get the cursor's position. */
  unsigned char (*get_pos)(void);

  /** Shift the display left. */
  void (*display_shift_left)(void);

  /** Shift the display right. */
  void (*display_shift_right)(void);

  /** Turn the display off. */
  void (*display_off)(void);

  /** Turn the display on. */
  void (*display_on)(void);

  /** Decrement the constrast. */
  void (*contrast_decrement)(void);

  /** Increment the constrast. */
  void (*contrast_increment)(void);

  /** Write a character to the display. */
  void (*putchar)(char c);
} lcd_driver;

#endif // ifndef lcd_driver_h
