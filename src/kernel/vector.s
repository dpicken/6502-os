.import interrupt_dispatch_irq
.import interrupt_dispatch_nmi
.import interrupt_dispatch_reset

.segment  "VECTORS"

.addr interrupt_dispatch_nmi
.addr interrupt_dispatch_reset
.addr interrupt_dispatch_irq
