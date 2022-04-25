#ifndef via_via_h
#define via_via_h

#include "hw/register.h"

typedef struct {
  hw_register_value_type rb;
  hw_register_value_type ra;
  hw_register_value_type ddrb;
  hw_register_value_type ddra;
  hw_register_value_type t1c_l;
  hw_register_value_type t1c_h;
  hw_register_value_type t1l_l;
  hw_register_value_type t1l_h;
  hw_register_value_type t2c_l;
  hw_register_value_type t2c_h;
  hw_register_value_type sr;
  hw_register_value_type acr;
  hw_register_value_type pcr;
  hw_register_value_type ifr;
  hw_register_value_type ier;
  hw_register_value_type ra_no_handshake;
} via;

extern via* const via1;
extern via* const via2;
extern via* const via3;

#define VIA_DATA_DIRECTION_SET_WRITE_MASK(ddr, write_mask) (*ddr) = (write_mask)
#define VIA_DATA_DIRECTION_SET_WRITE(ddr) VIA_DATA_DIRECTION_SET_WRITE_MASK(ddr, 0xFF)
#define VIA_DATA_DIRECTION_SET_READ(ddr)  VIA_DATA_DIRECTION_SET_WRITE_MASK(ddr, 0x00)

#define VIA_ACR_T1_CONTINUOUS_INTERRUPTS 0x40
#define VIA_ACR_SHIFT_IN_CB1_CLOCKED     0x0C

#define VIA_PCR_CB2_CONTROL_OUTPUT_HIGH 0xE0
#define VIA_PCR_CB2_CONTROL_OUTPUT_LOW  0xC0
#define VIA_PCR_CB1_INTERRUPT_CONTROL_POSITIVE_ACTIVE_EDGE 0x10

#define VIA_PCR_CA2_CONTROL_OUTPUT_LOW  0x0C
#define VIA_PCR_CA2_CONTROL_OUTPUT_HIGH 0x0E
#define VIA_PCR_CA1_INTERRUPT_CONTROL_POSITIVE_ACTIVE_EDGE 0x01

/** Initializes each via's ORA and ORB to zero. */
void via_init();

/** Sets a VIA's T1 timer to interrupt at a fixed rate. */
void via_timer_t1_fixed_rate_start(via* via, unsigned int frequency);

/** Resets a VIA's T1 timer interrupt flag, returns true if the flag was previously set. */
unsigned char via_timer_t1_interrupt_reset(via* via);

#endif // ifndef via_via_h
