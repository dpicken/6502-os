#include "init.h"

#include "hw/map.h"

void led_init(void) {
  hw_led_direction_set_write();
}
