.export interrupt_irq
.export interrupt_nmi

.segment "CODE"

interrupt_irq:
  ; Push X and A
  phx
  pha
  ; Transfer stack pointer to X
  tsx
  ; Position X on the status register value stored on the stack
  inx
  inx
  ; Load status register value and check for the B bit
  lda $100,x
  and #$10
  bne break

irq:
  ; Pop A and X
  pla
  plx
  rti

break:
  jmp break

interrupt_nmi:
  rti
