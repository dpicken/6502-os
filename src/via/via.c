#include "via.h"

#include "hw/configuration.h"

#define VIA(address) ((via* const) address)

via* const via1 = VIA(HW_VIA1_ADDRESS);
#if HW_VIA_COUNT > 1
via* const via2 = VIA(HW_VIA2_ADDRESS);
#endif
#if HW_VIA_COUNT > 2
via* const via3 = VIA(HW_VIA3_ADDRESS);
#endif

#define VIA_IFR_T1 0x40

#define VIA_IER_SET  0x80
#define VIA_IER_T1   0x40

void via_init() {
  via1->ra = 0;
  via1->rb = 0;
#if HW_VIA_COUNT > 1
  via2->ra = 0;
  via2->rb = 0;
#endif
#if HW_VIA_COUNT > 2
  via3->ra = 0;
  via3->rb = 0;
#endif
}

void via_timer_t1_fixed_rate_start(via* via, unsigned int frequency) {
  via->acr = VIA_ACR_T1_CONTINUOUS_INTERRUPTS;
  via->ier = VIA_IER_SET | VIA_IER_T1;
  via->t1c_l = frequency;
  via->t1c_h = (frequency >> 8);
}

unsigned char via_timer_t1_interrupt_reset(via* via) {
  unsigned char reset = via->ifr & VIA_IFR_T1;
  via->ifr |= VIA_IFR_T1;
  return reset;
}
