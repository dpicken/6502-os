#ifndef ft245r_ft245r_h
#define ft245r_ft245r_h

#include "hw/register.h"

/** Initialize the FT245R driver. */
void ft245r_init(hw_register via_data_port, hw_register via_data_ddr, hw_register via_control_port, hw_register via_control_ddr);

/** Returns true if the driver has been initialized. */
unsigned char ft245r_is_initialized(void);

/** Returns true if the rx buffer is not empty. */
unsigned char ft245r_rx_ready(void);

/** Reads a byte; invalid to call this if !ft245r_tx_ready(). */
unsigned char ft245r_rx(void);

/** Returns true if the tx buffer is not full. */
unsigned char ft245r_tx_ready(void);

/** Writes a byte; invalid to call this if !ft245r_tx_ready().  */
void ft245r_tx(unsigned char c);

#endif // ifndef ft245r_ft245r_h
