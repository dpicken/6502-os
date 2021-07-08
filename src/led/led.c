#include "led.h"

#include "hw/map.h"
#include "kernel/delay.h"
#include "timer/timer.h"

void led_on(void) {
  hw_led_set();
}

void led_off(void) {
  hw_led_reset();
}

void led_flash_short(void) {
  hw_led_set();
  timer_add_one_shot_special(&hw_led_reset, 100);
}

void led_flash_long(void) {
  hw_led_set();
  timer_add_one_shot_special(&hw_led_reset, 1000);
}

void led_flash_continuous(void) {
  timer_add_fixed_rate(&hw_led_set, 1000);
  timer_add_fixed_rate(&hw_led_reset, 100);
}

void led_flash_continuous_special(void) {
  timer_add_fixed_rate_special(&hw_led_set, 1000);
  timer_add_fixed_rate_special(&hw_led_reset, 100);
}
