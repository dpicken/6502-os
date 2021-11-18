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

#define VIA_DATA_DIRECTION_SET_WRITE_MASK(ddr, write_mask) (*ddr) = (write_mask)
#define VIA_DATA_DIRECTION_SET_WRITE(ddr) VIA_DATA_DIRECTION_SET_WRITE_MASK(ddr, 0xFF)
#define VIA_DATA_DIRECTION_SET_READ(ddr)  VIA_DATA_DIRECTION_SET_WRITE_MASK(ddr, 0x00)

/** Sets a VIA's T1 timer to interrupt at a fixed rate. */
void via_timer_t1_fixed_rate_start(via* via, unsigned int frequency);

/** Resets a VIA's T1 timer interrupt flag, returns true if the flag was previously set. */
unsigned char via_timer_t1_interrupt_reset(via* via);

#endif // ifndef via_via_h
