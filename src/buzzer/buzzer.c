#include "buzzer.h"

#include "hw/map.h"
#include "kernel/delay.h"
#include "timer/timer.h"

void buzzer_on(void) {
  hw_buzzer_set();
}

void buzzer_off(void) {
  hw_buzzer_reset();
}

void buzzer_short_buzz(void) {
  hw_buzzer_set();
  timer_add_one_shot_special(&hw_buzzer_reset, 100);
}

void buzzer_long_buzz(void) {
  hw_buzzer_set();
  timer_add_one_shot_special(&hw_buzzer_reset, 1000);
}
