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
#include "sd/sd.h"
#include "switcher/app.h"
#include "timer/timer.h"
#include "via/via.h"

#include <stdio.h>

static unsigned char shutdown;

void main(void) {
  log(memory_get_rom_version());

  via_init();
  log("[via_init] done");

  controller_init(HW_CONTROLLER_VIA_PORT);
  log("[ctl_init] done");

  controller_led_on();

  irq_handler_init();
  irq_enable();
  log("[irq_init] done");

  if (controller_buttons_depressed(CONTROLLER_BUTTON_TO_BIT(controller_button_b))) {
    HW_DISPLAY_DRIVER_INIT_ALT(HW_DISPLAY_VIA_PORT);
  } else {
    HW_DISPLAY_DRIVER_INIT(HW_DISPLAY_VIA_PORT);
  }
  log("[dis_init] done");

#if HW_FT245R_SUPPORT
  ft245r_init(HW_FT245R_CONTROL_VIA_PORT, HW_FT245R_DATA_VIA_PORT);
  log("[ftr_init] done");
#endif

  if (!ft245r_is_initialized()) {
    io_init(io_closed_reader, io_display_write, io_display_write);
  } else {
    if (controller_buttons_depressed(CONTROLLER_BUTTON_TO_BIT(controller_button_c))) {
      io_init(io_ft245r_read, io_ft245r_write, io_ft245r_write);
    } else {
      io_init(io_ft245r_read, io_display_write, io_display_write);
    }
  }
  log("[cio_init] done");

#if HW_SD_CARD_SUPPORT
  sd_init(HW_SD_CARD_VIA);
  log("[sdc_init] done");
#endif

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
