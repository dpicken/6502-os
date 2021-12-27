#include "ft245r.h"

#include "via/via.h"

#define FT245R_VIA_CONTROL_TX_ENABLE    (0x08 | 0x04)
#define FT245R_VIA_CONTROL_XX_DISABLE   0x04
#define FT245R_VIA_CONTROL_RX_ENABLE    0x00

#define FT245R_VIA_CONTROL_TX_FULL      0x02
#define FT245R_VIA_CONTROL_RX_EMPTY     0x01

#define FT245R_VIA_CONTROL_WRITE_MASK   (~(FT245R_VIA_CONTROL_RX_EMPTY | FT245R_VIA_CONTROL_TX_FULL))

static hw_register ft245r_via_data_port;
static hw_register ft245r_via_data_ddr;
static hw_register ft245r_via_control_port;

void ft245r_init(hw_register via_data_port, hw_register via_data_ddr, hw_register via_control_port, hw_register via_control_ddr) {
  ft245r_via_data_port = via_data_port;
  ft245r_via_data_ddr = via_data_ddr;
  ft245r_via_control_port = via_control_port;
  VIA_DATA_DIRECTION_SET_WRITE_MASK(via_control_ddr, FT245R_VIA_CONTROL_WRITE_MASK);
}

unsigned char ft245r_is_initialized(void) {
  return ft245r_via_data_port != 0;
}

unsigned char ft245r_rx_ready(void) {
  return (*ft245r_via_control_port & FT245R_VIA_CONTROL_RX_EMPTY) == 0;
}

unsigned char ft245r_rx(void) {
  unsigned char c;

  VIA_DATA_DIRECTION_SET_READ(ft245r_via_data_ddr);
  *ft245r_via_control_port = FT245R_VIA_CONTROL_RX_ENABLE;
  c = *ft245r_via_data_port;
  *ft245r_via_control_port = FT245R_VIA_CONTROL_XX_DISABLE;

  return c;
}

unsigned char ft245r_tx_ready(void) {
  return (*ft245r_via_control_port & FT245R_VIA_CONTROL_TX_FULL) == 0;
}

void ft245r_tx(unsigned char c) {
  VIA_DATA_DIRECTION_SET_WRITE(ft245r_via_data_ddr);
  *ft245r_via_data_port = c;
  *ft245r_via_control_port = FT245R_VIA_CONTROL_TX_ENABLE;
  *ft245r_via_control_port = FT245R_VIA_CONTROL_XX_DISABLE;
}
