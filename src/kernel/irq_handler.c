#include "irq_handler.h"

#include "system_time.h"

#include "hw/map.h"

#include <6502.h>

static unsigned char IRQ_STACK[64];

void irq_handler_init(void) {
  set_irq(&irq_handler_impl, IRQ_STACK, sizeof(IRQ_STACK));
}

unsigned char irq_handler_impl(void) {
  if (hw_timer_fixed_rate_interrupt_reset()) {
    system_time_on_tick();
    return IRQ_HANDLED;
  }
  return IRQ_NOT_HANDLED;
}
