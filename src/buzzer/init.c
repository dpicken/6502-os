#include "init.h"

#include "hw/map.h"

void buzzer_init(void) {
  hw_buzzer_direction_set_write();
}
