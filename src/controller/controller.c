#include "controller.h"

#include "timer/timer.h"
#include "via/via.h"

#define CONTROLLER_VIA_BUTTON_MASK ((1 << CONTROLLER_BUTTON_COUNT) - 1)
#define CONTROLLER_VIA_LED_MASK    (~CONTROLLER_VIA_BUTTON_MASK)

static hw_register controller_via_port;

static controller_button_event_handler depressed_handler;
static controller_button_event_handler released_handler;
static controller_button_demuxed_event_handler demuxed_depressed_handlers[CONTROLLER_BUTTON_COUNT];
static controller_button_demuxed_event_handler demuxed_released_handlers[CONTROLLER_BUTTON_COUNT];

static controller_button_bitset button_bits_previous = CONTROLLER_VIA_BUTTON_MASK; // Ignore buttons held down at boot during the first poll

void controller_init(hw_register via_port, hw_register via_ddr) {
  controller_via_port = via_port;
  VIA_DATA_DIRECTION_SET_WRITE_MASK(via_ddr, CONTROLLER_VIA_LED_MASK);
  timer_add_fixed_rate_special(controller_event_poll, 20);
}

void controller_button_set_depressed_handler(controller_button_event_handler handler) {
  depressed_handler = handler;
}

void controller_button_set_released_handler(controller_button_event_handler handler) {
  released_handler = handler;
}

void controller_button_set_demuxed_depressed_handler(controller_button_demuxed_event_handler handler, controller_button button) {
  demuxed_depressed_handlers[button] = handler;
}

void controller_button_set_demuxed_released_handler(controller_button_demuxed_event_handler handler, controller_button button) {
  demuxed_released_handlers[button] = handler;
}

void controller_button_clear_event_handlers(void) {
  int i;

  depressed_handler = 0;
  released_handler = 0;

  for (i = 0; i != CONTROLLER_BUTTON_COUNT; ++i) {
    demuxed_depressed_handlers[i] = 0;
    demuxed_released_handlers[i] = 0;
  }
}

void controller_led_on(void) {
  *controller_via_port = CONTROLLER_VIA_LED_MASK;
}

void controller_led_off(void) {
  *controller_via_port = ~CONTROLLER_VIA_LED_MASK;
}

unsigned char controller_buttons_depressed(controller_button_bitset buttons) {
  controller_button_bitset button_bits_current = *controller_via_port & CONTROLLER_VIA_BUTTON_MASK;
  return button_bits_current & buttons;
}

void controller_event_poll(void) {
  controller_button_bitset button_bits_current = *controller_via_port & CONTROLLER_VIA_BUTTON_MASK;
  controller_button_bitset new_depressed_buttons = button_bits_current & ~button_bits_previous;
  controller_button_bitset new_released_buttons = ~button_bits_current & button_bits_previous;

  button_bits_previous = button_bits_current;

  controller_button_event_dispatch(depressed_handler, demuxed_depressed_handlers, new_depressed_buttons);
  controller_button_event_dispatch(released_handler, demuxed_released_handlers, new_released_buttons);
}

void controller_button_event_dispatch(controller_button_event_handler handler, controller_button_demuxed_event_handler* demuxed_handlers, controller_button_bitset buttons) {
  controller_button_demuxed_event_handler demuxed_handler;
  int i;

  if (buttons == 0) {
    return;
  }

  if (handler != 0) {
    handler(buttons);
  }

  for (i = 0; i != CONTROLLER_BUTTON_COUNT; ++i) {
    demuxed_handler = demuxed_handlers[i];
    if (demuxed_handler != 0 && (buttons & CONTROLLER_BUTTON_TO_BIT(i))) {
      demuxed_handler();
    }
  }
}
