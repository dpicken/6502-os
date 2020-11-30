#include "init.h"

#include "hw/map.h"

void button_init(void) {
  hw_button_direction_set_read();
}
