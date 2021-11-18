#include "kernel.h"

#include "delay.h"
#include "interrupt_wait.h"
#include "irq_control.h"
#include "irq_handler.h"
#include "log.h"
#include "memory.h"
#include "system_time.h"

#include "cc65/write.h"
#include "console/console.h"
#include "controller/controller.h"
#include "hw/configuration.h"
#include "lcd/lcd.h"
#include "switcher/app.h"
#include "timer/timer.h"
#include "via/via.h"

#include <stdio.h>

static unsigned char shutdown;

void main(void) {
  kernel_log_early(memory_get_rom_version());

  controller_init(HW_CONTROLLER_VIA_PORT);
  kernel_log_early("[ctl_init] done");

  controller_led_on();

  irq_handler_init();
  irq_enable();
  kernel_log_early("[irq_init] done");

#if HW_VIA_COUNT > 1
  if (controller_buttons_depressed(CONTROLLER_BUTTON_TO_BIT(controller_button_a))) {
    HW_LCD_DRIVER_INIT(HW_LCD_VIA_PORT_ALT1);
  } else if (controller_buttons_depressed(CONTROLLER_BUTTON_TO_BIT(controller_button_b))) {
    HW_LCD_DRIVER_INIT(HW_LCD_VIA_PORT_ALT2);
  } else {
    HW_LCD_DRIVER_INIT(HW_LCD_VIA_PORT_DEFAULT);
  }
#else
  HW_LCD_DRIVER_INIT(HW_LCD_VIA_PORT);
#endif
  kernel_log_early("[lcd_init] done");

  console_set_resolution(20, 4);
  kernel_log_early("[con_reso] done");

  vidiprinter = fopen(VIDIPRINTER_PATH, "a");
  kernel_log("[vid_init] done");

  timer_add_one_shot(switcher_app_enter, 10);
  kernel_log("[swt_entr] done");

  kernel_log("[pol_loop] ...");
  kernel_event_poll_loop();
  kernel_log("[pol_loop] done");

  irq_disable();
  kernel_log("[irq_shut] done");

  kernel_log("[krl_exit] ...");
  controller_led_off();
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
