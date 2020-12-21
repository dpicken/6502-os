#ifndef button_event_h
#define button_event_h

typedef enum {
  button_code_up,
  button_code_down,
  button_code_left,
  button_code_right,
  button_code_fire,

  button_count
} button_code;

typedef void (*button_event_handler)(void);

/** Callback the specified handler when the specified button is depressed. */
void button_depressed_set_handler(button_event_handler handler, button_code code);

/** Callback the specified handler when the specified button is released. */
void button_released_set_handler(button_event_handler handler, button_code code);

void button_event_dispatch(button_code code, unsigned char depressed);

void button_event_poll(void);

#endif // ifndef button_event_h
