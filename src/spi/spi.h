#ifndef spi_spi_h
#define spi_spi_h

#include "via/via.h"

#define SPI_VIA_CS5 0x20
#define SPI_VIA_CS4 0x10
#define SPI_VIA_CS3 0x08
#define SPI_VIA_CS2 0x04
#define SPI_VIA_CS1 0x02
#define SPI_VIA_CS0 0x01

/**
 * Initialize a VIA for SPI.
 *
 * Uses port B, its control lines and the shift register.
 *
 * Supports up to 6 chip select lines [PB0, PB5].
 */
void spi_init(via* via);

/** Select the specified CS line(s). */
void spi_select(via* via, unsigned char cs_bitset);

/** Concurrently read and write a byte */
unsigned char spi_read_write_byte(via* via, unsigned char c);

#endif // ifndef spi_spi_h
