#include "nmi_handler.h"

#include "delay.h"
#include "irq_control.h"

#include "display/display.h"

#include <stdio.h>

static unsigned char nmiCount;
static unsigned char nmiReentryDepth;
static unsigned char nmiMaxReentryDepth;

void nmi_handler(void) {
  ++nmiReentryDepth;
  if (nmiReentryDepth > nmiMaxReentryDepth) {
    nmiMaxReentryDepth = nmiReentryDepth;
  }

  ++nmiCount;

  display_clear();
  fprintf(stderr, "NMI: %i/%i (%i)\n", nmiCount, nmiReentryDepth, nmiMaxReentryDepth);

  irq_enable();
  delay_ms(1000);
  irq_disable();

  --nmiReentryDepth;
  fprintf(stderr, "NMI: %i/%i (%i)\n", nmiCount, nmiReentryDepth, nmiMaxReentryDepth);
}
