#ifndef sd_sd_h
#define sd_sd_h

#include "via/via.h"

#define SD_BLOCK_BYTE_COUNT_LOG2  9
#define SD_BLOCK_BYTE_COUNT       (1U << SD_BLOCK_BYTE_COUNT_LOG2)

/** Initialize the SD driver. */
void sd_init(via* via);

/** Returns true if the driver has been initialized. */
unsigned char sd_is_initialized(void);

/** Returns true if an SD card is detected. */
unsigned char sd_card_detected(void);

/** Open the SD card; returns true on success. */
unsigned char sd_card_open(void);

/** Close the SD card. */
void sd_card_close(void);

/** Read the SD card. */
int sd_card_read(long block_address, unsigned int skip_byte_count, char* buffer, unsigned int byte_count);

void sd_card_power_on(void);
void sd_card_power_off(void);

void sd_spi_train(void);

unsigned char sd_spi_card_init(void);
unsigned int sd_spi_card_configure(void);

unsigned char sd_spi_cmd0_go_idle_state(void);
unsigned char sd_spi_cmd8_send_if_cond(void);
unsigned char sd_spi_cmd16_set_blocklen(unsigned long byte_count);
unsigned char sd_spi_cmd17_read_single_block(unsigned long block_address);
unsigned char sd_spi_cmd55_app_cmd(void);
unsigned char sd_spi_cmd58_read_ocr(void);
unsigned char sd_spi_acmd41_sd_send_op_cmd(void);

unsigned char sd_spi_read_transfer(unsigned long skip_byte_count, char* buffer, unsigned int byte_count);

unsigned char sd_spi_get_response(void);

void sd_spi_select(void);
void sd_spi_deselect(void);

void sd_spi_8_clock_cycles(void);

unsigned char sd_spi_read(void);
void sd_spi_write(unsigned char c);

void sd_log(const char* const cmd, unsigned char response);

#endif // ifndef sd_sd_h
