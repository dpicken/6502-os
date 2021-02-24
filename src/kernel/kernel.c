#include "kernel.h"

#include "interrupt_wait.h"
#include "irq_control.h"
#include "irq_handler.h"
#include "log.h"
#include "system_time.h"

#include "button/event.h"
#include "button/init.h"
#include "console/console.h"
#include "lcd/init.h"
#include "switcher/app.h"
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

  console_init_40x4();
  kernel_log_early("[con_init] done");

  button_init();
  kernel_log("[btn_init] done");

  button_released_set_handler(switcher_app_enter, button_code_special_left_and_right);
  kernel_log("[swt_hkey] done");

  timer_add_one_shot(switcher_app_enter, 1000);
  kernel_log("[swt_entr] done");

  kernel_log("[pol_loop] ...");
  kernel_event_poll_loop();
}
