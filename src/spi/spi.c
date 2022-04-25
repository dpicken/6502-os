#include "spi.h"

#include "kernel/delay.h"

#define SPI_VIA_SCLK_BIT    0x80
#define SPI_VIA_MOSI_BIT    0x40
//#define SPI_VIA_CS_MASK     (SPI_VIA_CS5 | SPI_VIA_CS4 | SPI_VIA_CS3 | SPI_VIA_CS2 | SPI_VIA_CS1 | SPI_VIA_CS0)
#define SPI_VIA_CS_MASK     SPI_VIA_CS5
#define SPI_VIA_WRITE_MASK  (SPI_VIA_SCLK_BIT | SPI_VIA_MOSI_BIT | SPI_VIA_CS_MASK)

void spi_init(via* via) {
  via->acr |= VIA_ACR_SHIFT_IN_CB1_CLOCKED;

  via->rb = (via->rb & ~SPI_VIA_WRITE_MASK) | SPI_VIA_CS_MASK;
  via->ddrb |= SPI_VIA_WRITE_MASK;
}

void spi_select(via* via, unsigned char cs_bitset) {
  unsigned char cs_active = (~cs_bitset) & SPI_VIA_CS_MASK;

  via->rb = (via->rb & ~SPI_VIA_WRITE_MASK) | cs_active;
}

unsigned char spi_read_write_byte(via* via, unsigned char c) {
  unsigned char rb_base = via->rb & ~(SPI_VIA_MOSI_BIT | SPI_VIA_SCLK_BIT);

  unsigned char bits_transferred;
  unsigned char mosi_bit;

  for (bits_transferred = 0; bits_transferred != 8; ++bits_transferred, (c <<= 1)) {
    mosi_bit = (c & 0x80) ? SPI_VIA_MOSI_BIT : 0;
    via->rb = rb_base | mosi_bit;
    via->rb = rb_base | mosi_bit | SPI_VIA_SCLK_BIT;
    via->rb = rb_base | mosi_bit;
  }

  via->rb = rb_base;

  return via->sr;
}
