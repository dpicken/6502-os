#ifndef button_event_h
#define button_event_h

typedef enum {
  button_code_up,
  button_code_down,
  button_code_left,
  button_code_right,
  button_code_fire1,
  button_code_fire2,
  button_code_fire3,

  button_count_non_special,

  button_code_special_left_and_right = button_count_non_special,

  button_count
} button_code;

typedef void (*button_event_handler)(void);

/** Callback the specified handler when the specified button is depressed. */
void button_depressed_set_handler(button_event_handler handler, button_code code);

/** Callback the specified handler when the specified button is released. */
void button_released_set_handler(button_event_handler handler, button_code code);

/** Clear all non-special callbacks. */
void button_clear_all_non_special(void);

void button_event_dispatch(button_code code, unsigned char depressed);

void button_event_poll(void);

#endif // ifndef button_event_h
