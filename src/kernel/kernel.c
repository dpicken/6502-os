#include "kernel.h"

#include "delay.h"
#include "interrupt_wait.h"
#include "irq_control.h"
#include "irq_handler.h"
#include "log.h"
#include "memory.h"
#include "system_time.h"

#include "button/event.h"
#include "button/init.h"
#include "led/init.h"
#include "led/led.h"
#include "cc65/write.h"
#include "console/console.h"
#include "lcd/lcd.h"
#include "switcher/app.h"
#include "timer/timer.h"

#include <stdio.h>

static unsigned char shutdown;

void main(void) {
  kernel_log_early(memory_get_rom_version());

  led_init();
  kernel_log_early("[led_init] done");

  // Enable the LED early.
  led_on();

  irq_handler_init();
  irq_enable();
  kernel_log_early("[irq_init] done");

  system_time_init();
  kernel_log_early("[sys_time] done");

  lcd->init();
  kernel_log_early("[lcd_init] done");

  console_set_resolution(20, 4);
  kernel_log_early("[con_reso] done");

  // Disable the LED after early initialization is complete.
  led_off();

  vidiprinter = fopen(VIDIPRINTER_PATH, "a");
  kernel_log("[vid_init] done");

  button_init();
  kernel_log("[btn_init] done");

  button_released_set_handler(switcher_app_enter, button_code_special_left_and_right);
  kernel_log("[swt_hkey] done");

  timer_add_one_shot(switcher_app_enter, 10);
  kernel_log("[swt_entr] done");

  // Flash the LED after initialization is complete.
  led_flash_short();

  kernel_log("[pol_loop] ...");
  kernel_event_poll_loop();
  kernel_log("[pol_loop] done");

  irq_disable();
  kernel_log("[irq_shut] done");

  kernel_log("[krl_exit] ...");
}

void kernel_event_poll_loop(void) {
  while (!shutdown) {
    if (system_time_reset_ticked_event()) {
      timer_on_system_time_ticked();
    }
    interrupt_wait();
  }
}

void kernel_shutdown(void) {
  shutdown = 1;
}
