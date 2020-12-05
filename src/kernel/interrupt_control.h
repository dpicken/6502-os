#ifndef kernel_interrupt_control_h
#define kernel_interrupt_control_h

#define interrupt_control_irq_enable()  __asm__("cli")
#define interrupt_control_irq_disable() __asm__("sei")

#endif // ifndef kernel_interrupt_control_h
