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
#include "ft245r/ft245r.h"
#include "hw/configuration.h"
#include "lcd/lcd.h"
#include "switcher/app.h"
#include "timer/timer.h"
#include "via/via.h"

#include <stdio.h>

static unsigned char shutdown;

void main(void) {
  kernel_log_early(memory_get_rom_version());

  controller_init(HW_CONTROLLER_VIA1_PORT);
  kernel_log_early("[ctl_init] done");

  controller_led_on();

  irq_handler_init();
  irq_enable();
  kernel_log_early("[irq_init] done");

  HW_LCD_DRIVER_INIT(HW_LCD_VIA1_PORT);
  kernel_log_early("[lcd_init] done");

#if HW_VIA_COUNT > 1
  ft245r_init(HW_FT245R_DATA_VIA2_PORT, HW_FT245R_CONTROL_VIA2_PORT);
  kernel_log_early("[ftr_init] done");
#endif

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

void kernel_reset(void) {
  kernel_log("Resetting...");
  __asm__("jmp ($FFFC)");
}

void kernel_shutdown(void) {
  kernel_log("Shutting down...");
  shutdown = 1;
}
