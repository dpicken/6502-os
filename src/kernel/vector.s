.import interrupt_irq
.import interrupt_nmi
.import startup_reset

.segment  "VECTORS"

.addr interrupt_nmi
.addr startup_reset
.addr interrupt_irq
