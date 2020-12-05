.import callirq
.import startup_init

.export doneirq
.export initirq
.export interrupt_dispatch_irq
.export interrupt_dispatch_nmi
.export interrupt_dispatch_reset

.segment "ONCE"

initirq:
  rts

.segment "CODE"

doneirq:
  rts

interrupt_dispatch_irq:
  pha
  phx
  phy
  jsr callirq
  ply
  plx
  pla
  rti

interrupt_dispatch_nmi:
  rti

interrupt_dispatch_reset:
  jmp startup_init
