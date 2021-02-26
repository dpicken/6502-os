#include "app.h"

#include "buzzer.h":

#include "button/event.h"
#include "timer/timer.h"

#include <stdio.h>

void buzzer_app_enter(void) {
  puts("L:short        R:long");
  puts("    D:count down     ");
  puts("A:on/off  B:on  C:off");

  button_released_set_handler(buzzer_short_buzz, button_code_left);
  button_released_set_handler(buzzer_long_buzz, button_code_right);
  button_released_set_handler(buzzer_app_count_down, button_code_down);
  button_depressed_set_handler(buzzer_on, button_code_fire1);
  button_released_set_handler(buzzer_off, button_code_fire1);
  button_released_set_handler(buzzer_on, button_code_fire2);
  button_released_set_handler(buzzer_off, button_code_fire3);
}

void buzzer_app_count_down(void) {
  timer_add_fixed_rate(buzzer_short_buzz, 1000);
  timer_add_one_shot_special(timer_clear_all_non_special, 4000);
  timer_add_one_shot_special(buzzer_long_buzz, 5000);
}
