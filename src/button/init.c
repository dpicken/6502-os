#include "init.h"

#include "event.h"

#include "hw/map.h"
#include "timer/timer.h"

void button_init(void) {
  hw_button_direction_set_read();
  timer_add_fixed_rate(&button_event_poll, 50);
}
