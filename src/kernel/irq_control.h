#ifndef kernel_irq_control_h
#define kernel_irq_control_h

#define irq_enable()  __asm__("cli")
#define irq_disable() __asm__("sei")

#endif // ifndef kernel_irq_control_h
