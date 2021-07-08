#include "app.h"

#include "led.h"

#include "button/event.h"

#include <stdio.h>

void led_app_enter(void) {
  button_depressed_set_handler(led_flash_short, button_code_left);
  button_depressed_set_handler(led_flash_long, button_code_right);
  button_depressed_set_handler(led_flash_continuous_special, button_code_up);
  button_depressed_set_handler(led_flash_continuous, button_code_down);
  button_depressed_set_handler(led_on, button_code_fire1);
  button_released_set_handler(led_off, button_code_fire1);
  button_depressed_set_handler(led_on, button_code_fire2);
  button_depressed_set_handler(led_off, button_code_fire3);

  printf("  U:cs              \n");
  printf("L:short  R:long     \n");
  printf("  D:c  A:1/0 B:1 C:0\n");
}
