#include "kernel.h"

#include "app.h"
#include "interrupt_wait.h"
#include "irq_control.h"
#include "irq_handler.h"
#include "log.h"
#include "system_time.h"

#include "button/init.h"
#include "lcd/init.h"
#include "timer/timer.h"

void kernel_event_poll_loop(void) {
  for (;;) {
    if (system_time_reset_ticked_event()) {
      timer_on_system_time_ticked();
    }
    interrupt_wait();
  }
}

void main(void) {
  irq_handler_init();
  irq_enable();
  kernel_log_early("[irq_init] done");

  system_time_init();
  kernel_log_early("[sys_time] done");

  lcd_init();
  kernel_log_early("[lcd_init] done");

  button_init();
  kernel_log("[btn_init] done");

  kernel_app_start();
  kernel_log("[krl_apst] done");

  kernel_log("[krl_poll] start");
  kernel_event_poll_loop();
}
