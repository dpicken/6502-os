#include "event.h"

#include "hw/map.h"

button_event_handler button_depressed_handlers[button_count];
button_event_handler button_released_handlers[button_count];

unsigned char button_bits_history;

void button_depressed_set_handler(button_event_handler handler, button_code code) {
  button_depressed_handlers[code] = handler;
}

void button_released_set_handler(button_event_handler handler, button_code code) {
  button_released_handlers[code] = handler;
}

void button_clear_all_non_special(void) {
  button_code i;

  for (i = 0; i != button_count_non_special; ++i) {
    button_depressed_set_handler(0, i);
    button_released_set_handler(0, i);
  }
}

void button_event_dispatch(button_code code, unsigned char depressed) {
  button_event_handler handler = depressed
      ? button_depressed_handlers[code]
      : button_released_handlers[code];
  if (handler != 0) {
    handler();
  }
}

void button_event_poll(void) {
  unsigned char button_bits = hw_button_read();
  unsigned char button_bits_changed = button_bits ^ button_bits_history;
  button_bits_history = button_bits;

  if ((button_bits_changed & (HW_BUTTON_2 | HW_BUTTON_3)) == (HW_BUTTON_2 | HW_BUTTON_3)
      && (button_bits & HW_BUTTON_2) == (button_bits & HW_BUTTON_3)) {
    button_event_dispatch(button_code_special_left_and_right, button_bits & (HW_BUTTON_2 | HW_BUTTON_3));
    return;
  }

  if (button_bits_changed & HW_BUTTON_0) {
    button_event_dispatch(button_code_up, button_bits & HW_BUTTON_0);
  }
  if (button_bits_changed & HW_BUTTON_1) {
    button_event_dispatch(button_code_down, button_bits & HW_BUTTON_1);
  }
  if (button_bits_changed & HW_BUTTON_2) {
    button_event_dispatch(button_code_left, button_bits & HW_BUTTON_2);
  }
  if (button_bits_changed & HW_BUTTON_3) {
    button_event_dispatch(button_code_right, button_bits & HW_BUTTON_3);
  }
  if (button_bits_changed & HW_BUTTON_4) {
    button_event_dispatch(button_code_fire1, button_bits & HW_BUTTON_4);
  }
  if (button_bits_changed & HW_BUTTON_5) {
    button_event_dispatch(button_code_fire2, button_bits & HW_BUTTON_5);
  }
  if (button_bits_changed & HW_BUTTON_6) {
    button_event_dispatch(button_code_fire3, button_bits & HW_BUTTON_6);
  }

}
