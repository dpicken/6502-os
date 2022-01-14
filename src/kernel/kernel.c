#include "kernel.h"

#include "delay.h"
#include "interrupt_wait.h"
#include "irq_control.h"
#include "irq_handler.h"
#include "log/log.h"
#include "memory.h"
#include "system_time.h"

#include "controller/controller.h"
#include "display/display.h"
#include "ft245r/ft245r.h"
#include "hw/configuration.h"
#include "io/io.h"
#include "switcher/app.h"
#include "timer/timer.h"
#include "via/via.h"

#include <stdio.h>

static unsigned char shutdown;

void main(void) {
  log(memory_get_rom_version());

  controller_init(HW_CONTROLLER_VIA1_PORT);
  log("[ctl_init] done");

  controller_led_on();

  irq_handler_init();
  irq_enable();
  log("[irq_init] done");

  HW_DISPLAY_DRIVER_INIT(HW_DISPLAY_VIA1_PORT);
  log("[dis_init] done");

#if HW_VIA_COUNT > 1
  ft245r_init(HW_FT245R_DATA_VIA2_PORT, HW_FT245R_CONTROL_VIA2_PORT);
  log("[ftr_init] done");
#endif

  if (!ft245r_is_initialized()) {
    io_init(io_closed_reader, io_display_write, io_display_write);
  } else {
    if (controller_buttons_depressed(CONTROLLER_BUTTON_TO_BIT(controller_button_down))) {
      io_init(io_ft245r_read, io_ft245r_write, io_ft245r_write);
    } else {
      io_init(io_ft245r_read, io_display_write, io_display_write);
    }
  }
  log("[cio_init] done");

  timer_add_one_shot(switcher_app_enter, 10);
  log("[swt_entr] done");

  log("[pol_loop] ...");
  kernel_event_poll_loop();
  log("[pol_loop] done");

  irq_disable();
  log("[irq_shut] done");

  log("[krl_exit] ...");
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
  log("Resetting...");
  __asm__("jmp ($FFFC)");
}

void kernel_shutdown(void) {
  log("Shutting down...");
  shutdown = 1;
}
