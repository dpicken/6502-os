#ifndef controller_controller_h
#define controller_controller_h

#include "hw/register.h"

typedef enum {
  controller_button_up,
  controller_button_down,
  controller_button_left,
  controller_button_right,
  controller_button_a,
  controller_button_b,
  controller_button_c
} controller_button;

#define CONTROLLER_BUTTON_COUNT (controller_button_c + 1)
#define CONTROLLER_BUTTON_TO_BIT(button) (1 << button)

typedef unsigned char controller_button_bitset;

/** Button event handler. */
typedef void (*controller_button_event_handler)(controller_button_bitset buttons);

/** Demuxed button event handler. */
typedef void (*controller_button_demuxed_event_handler)(void);

/** Initialize the controller. */
void controller_init(hw_register via_port, hw_register via_ddr);

/** Set the handler for button(s) depressed events. */
void controller_button_set_depressed_handler(controller_button_event_handler handler);

/** Set the handler for button(s) released events. */
void controller_button_set_released_handler(controller_button_event_handler handler);

/** Set the handler for button depressed events. */
void controller_button_set_demuxed_depressed_handler(controller_button_demuxed_event_handler handler, controller_button button);

/** Set the handler for button released events. */
void controller_button_set_demuxed_released_handler(controller_button_demuxed_event_handler handler, controller_button button);

/** Clear all button event handlers. */
void controller_button_clear_event_handlers(void);

/** Light the controller's LED. */
void controller_led_on(void);

/** Extinguish the controller's LED. */
void controller_led_off(void);

/** Test whether the specified buttons are depressed. */
unsigned char controller_buttons_depressed(controller_button_bitset buttons);

void controller_event_poll(void);

void controller_button_event_dispatch(controller_button_event_handler handler, controller_button_demuxed_event_handler* demuxed_handlers, controller_button_bitset buttons);

#endif // ifndef controller_controller_h
