#ifndef led_led_h
#define led_led_h

/** Turn the LED on. */
void led_on(void);

/** Turn the LED off. */
void led_off(void);

/** Flash the LED a short duration. */
void led_flash_short(void);

/** Flash the LED for a long(er) duration. */
void led_flash_long(void);

/** Flash the LED continuously. */
void led_flash_continuous(void);

/** Flash the LED continuously, with a non-clearable timer. */
void led_flash_continuous_special(void);

#endif // ifndef led_led_h
