#include "irq_handler.h"

#include "system_time.h"

#include "hw/configuration.h"
#include "via/via.h"

#include <6502.h>

static unsigned char IRQ_STACK[64];

void irq_handler_init(void) {
  set_irq(&irq_handler_impl, IRQ_STACK, sizeof(IRQ_STACK));

  via_timer_t1_fixed_rate_start(HW_SYSTEM_TIMER_VIA, system_time_get_tick_frequency());
}

unsigned char irq_handler_impl(void) {
  if (via_timer_t1_interrupt_reset(HW_SYSTEM_TIMER_VIA)) {
    system_time_on_tick();
    return IRQ_HANDLED;
  }
  return IRQ_NOT_HANDLED;
}
