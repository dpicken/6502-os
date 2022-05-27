#include "sd.h"

#include "spi/spi.h"

#include "io/io.h"
#include "kernel/delay.h"
#include "util/algorithm.h"
#include "util/crc.h"
#include "util/hex.h"

#include <stdio.h>

#define SD_VIA_DETECT   0x10
#define SD_VIA_CS       SPI_VIA_CS5
#define SD_VIA_CS_NONE  0x00

#define SD_SPI_TRAIN_BYTE_COUNT 10
#define SD_SPI_COMMAND_ATTEMPT_COUNT 100
#define SD_SPI_RESPONSE_ATTEMPT_COUNT 100

#define SD_SPI_RESPONSE_IS_IN_IDLE_STATE(response)            (((response) & 0x81) == 0x01)
#define SD_SPI_RESPONSE_IS_ERASE_RESET(response)              (((response) & 0x82) == 0x02)
#define SD_SPI_RESPONSE_IS_ILLEGAL_COMMAND(response)          (((response) & 0x84) == 0x04)
#define SD_SPI_RESPONSE_IS_COMMUNICATION_CRC_ERROR(response)  (((response) & 0x88) == 0x08)
#define SD_SPI_RESPONSE_IS_ERASE_SEQUENCE_ERROR(response)     (((response) & 0x90) == 0x10)
#define SD_SPI_RESPONSE_IS_ADDRESS_ERROR(response)            (((response) & 0xA0) == 0x20)
#define SD_SPI_RESPONSE_IS_PARAMETER_ERROR(response)          (((response) & 0xC0) == 0x40)

#define SD_SPI_RESPONSE_IS_IN_IDLE_STATE_ONLY(response)       (((response) & 0xFF) == 0x01)
#define SD_SPI_RESPONSE_IS_SUCCESS(response)                  ((response) == 0x00)
#define SD_SPI_RESPONSE_UNKNOWN_ERROR                         0xFF

#define SD_SPI_TOKEN_IS_START_BLOCK(token)  ((token) == 0xFE)
#define SD_SPI_TOKEN_IS_DATA_ERROR(token)   (((token) & 0xF0) == 0x00)
#define SD_SPI_TOKEN_CRC_ERROR              0x00

#define SD_OCR_CARD_CARD_POWER_UP_STATUS  0x80000000
#define SD_OCR_CARD_CCS                   0x40000000
#define SD_OCR_3_3V                       0x00300000

#define SD_CARD_IS_BYTE_ADDRESSED(sd_card)  ((sd_card.ocr & SD_OCR_CARD_CCS) == 0)

static via* sd_via;

struct {
  unsigned long ocr;
} sd_card;


void sd_init(via* via) {
  spi_init(via);
  sd_via = via;
}

unsigned char sd_is_initialized(void) {
  return sd_via != 0;
}

unsigned char sd_card_detected(void) {
  return (sd_via->rb & SD_VIA_DETECT) == 0;
}

unsigned char sd_card_open(void) {
  sd_card_power_on();

  sd_spi_train();

  if (!sd_spi_card_init()) {
    sd_card_power_off();
    return 0;
  }

  if (!sd_spi_card_configure()) {
    sd_card_power_off();
    return 0;
  }

  return 1;
}

void sd_card_close(void) {
  sd_card_power_off();
}

int sd_card_read(long block_address, unsigned int skip_byte_count, char* buffer, unsigned int byte_count) {
  unsigned char response;
  unsigned char token;

  byte_count = u_min(byte_count, SD_BLOCK_BYTE_COUNT - skip_byte_count);

  if (SD_CARD_IS_BYTE_ADDRESSED(sd_card)) {
    block_address <<= SD_BLOCK_BYTE_COUNT_LOG2;
  }

  response = sd_spi_cmd17_read_single_block(block_address);
  if (!SD_SPI_RESPONSE_IS_SUCCESS(response)) {
    sd_log("cmd17", response);
    return -1;
  }

  token = sd_spi_read_transfer(skip_byte_count, buffer, byte_count);
  if (!SD_SPI_TOKEN_IS_START_BLOCK(token)) {
    sd_log("token", token);
    return -1;
  }

  return byte_count;
}

void sd_card_power_on(void) {
  sd_via->pcr = VIA_PCR_CA2_CONTROL_OUTPUT_HIGH;
}

void sd_card_power_off(void) {
  sd_via->pcr = VIA_PCR_CA2_CONTROL_OUTPUT_LOW;
}

void sd_spi_train(void) {
  unsigned char i;

  for (i = 0; i != SD_SPI_TRAIN_BYTE_COUNT; ++i) {
    sd_spi_8_clock_cycles();
  }
}

unsigned char sd_spi_card_init(void) {
  unsigned char response;
  unsigned char i;

  response = sd_spi_cmd0_go_idle_state();
  if (!SD_SPI_RESPONSE_IS_IN_IDLE_STATE_ONLY(response)) {
    sd_log("cmd0", response);
    return 0;
  }

  response = sd_spi_cmd8_send_if_cond();
  if (!SD_SPI_RESPONSE_IS_IN_IDLE_STATE_ONLY(response) && !SD_SPI_RESPONSE_IS_ILLEGAL_COMMAND(response)) {
    sd_log("cmd8", response);
    return 0;
  }

  response = sd_spi_cmd58_read_ocr();
  if (!SD_SPI_RESPONSE_IS_IN_IDLE_STATE_ONLY(response)) {
    sd_log("cmd58", response);
    return 0;
  }

  if ((sd_card.ocr & SD_OCR_3_3V) != SD_OCR_3_3V) {
    fprintf(stderr, "card does not support 3.3v\n");
    return 0;
  }

  for (i = 0; i != SD_SPI_COMMAND_ATTEMPT_COUNT; ++i) {
    response = sd_spi_cmd55_app_cmd();
    if (!SD_SPI_RESPONSE_IS_IN_IDLE_STATE_ONLY(response)) {
      sd_log("cmd55", response);
      return 0;
    }

    response = sd_spi_acmd41_sd_send_op_cmd();
    if (SD_SPI_RESPONSE_IS_SUCCESS(response)) {
      break;
    } else if (!SD_SPI_RESPONSE_IS_IN_IDLE_STATE_ONLY(response)) {
      sd_log("acmd41", response);
      return 0;
    }
    delay_ms(10);
  }

  if (!SD_SPI_RESPONSE_IS_SUCCESS(response)) {
    return 0;
  }

  response = sd_spi_cmd58_read_ocr();
  if (!SD_SPI_RESPONSE_IS_SUCCESS(response)) {
    sd_log("cmd58", response);
    return 0;
  }

  if ((sd_card.ocr & SD_OCR_CARD_CARD_POWER_UP_STATUS) == 0) {
    fprintf(stderr, "card power-up not done\n");
    return 0;
  }

  return 1;
}

unsigned int sd_spi_card_configure(void) {
  unsigned char response;

  response = sd_spi_cmd16_set_blocklen(SD_BLOCK_BYTE_COUNT);
  if (!SD_SPI_RESPONSE_IS_SUCCESS(response)) {
    sd_log("cmd16", response);
    return 0;
  }

  return 1;
}

unsigned char sd_spi_cmd0_go_idle_state(void) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 0);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x95);

  response = sd_spi_get_response();

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_cmd8_send_if_cond(void) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 8);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x01);
  sd_spi_write(0xAA);
  sd_spi_write(0x87);

  response = sd_spi_get_response();

  if (sd_spi_read() != 0x00 && !SD_SPI_RESPONSE_IS_ILLEGAL_COMMAND(response)) {
    response = SD_SPI_RESPONSE_UNKNOWN_ERROR;
  }
  if (sd_spi_read() != 0x00 && !SD_SPI_RESPONSE_IS_ILLEGAL_COMMAND(response)) {
    response = SD_SPI_RESPONSE_UNKNOWN_ERROR;
  }
  if (sd_spi_read() != 0x01 && !SD_SPI_RESPONSE_IS_ILLEGAL_COMMAND(response)) {
    response = SD_SPI_RESPONSE_UNKNOWN_ERROR;
  }
  if (sd_spi_read() != 0xAA && !SD_SPI_RESPONSE_IS_ILLEGAL_COMMAND(response)) {
    response = SD_SPI_RESPONSE_UNKNOWN_ERROR;
  }

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_cmd16_set_blocklen(unsigned long byte_count) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 16);
  sd_spi_write(byte_count >> 24);
  sd_spi_write(byte_count >> 16);
  sd_spi_write(byte_count >> 8);
  sd_spi_write(byte_count);
  sd_spi_write(0xFF);

  response = sd_spi_get_response();

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_cmd17_read_single_block(unsigned long block_address) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 17);
  sd_spi_write(block_address >> 24);
  sd_spi_write(block_address >> 16);
  sd_spi_write(block_address >> 8);
  sd_spi_write(block_address);
  sd_spi_write(0xFF);

  response = sd_spi_get_response();

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_cmd55_app_cmd(void) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 55);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0xFF);

  response = sd_spi_get_response();

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_cmd58_read_ocr(void) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 58);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0xFF);

  response = sd_spi_get_response();

  sd_card.ocr |= sd_spi_read();
  sd_card.ocr <<= 8;
  sd_card.ocr |= sd_spi_read();
  sd_card.ocr <<= 8;
  sd_card.ocr | sd_spi_read();
  sd_card.ocr <<= 8;
  sd_card.ocr |= sd_spi_read();

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_acmd41_sd_send_op_cmd(void) {
  unsigned char response;

  sd_spi_select();

  sd_spi_write(0x40 | 41);
  sd_spi_write(0x40); // HCS set: SDHC/SDXC support; TODO: Shouldn't set this if cmd8 was not a valid command
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0x00);
  sd_spi_write(0xFF);

  response = sd_spi_get_response();

  sd_spi_deselect();

  return response;
}

unsigned char sd_spi_read_transfer(unsigned long skip_byte_count, char* buffer, unsigned int byte_count) {
  unsigned int crc16_calculated = 0;

  unsigned char token;
  unsigned char c;
  unsigned int crc16_received;
  unsigned int i;

  sd_spi_select();

  token = sd_spi_get_response();
  if (SD_SPI_TOKEN_IS_START_BLOCK(token)) {
    for (i = 0; i != SD_BLOCK_BYTE_COUNT; ++i) {
      c = sd_spi_read();
      crc16_calculated = crc16(crc16_calculated, c);

      if (skip_byte_count != 0) {
        --skip_byte_count;
        continue;
      }

      if (byte_count != 0) {
        *buffer = c;
        ++buffer;
        --byte_count;
      }
    }

    crc16_received = sd_spi_read();
    crc16_received <<= 8;
    crc16_received |= sd_spi_read();

    if (crc16_received != crc16_calculated) {
      fprintf(stderr, "crc mm: %04X (%04X)\n", crc16_received, crc16_calculated);
      token = SD_SPI_TOKEN_CRC_ERROR;
    }
  }

  sd_spi_deselect();

  return token;
}

unsigned char sd_spi_get_response(void) {
  unsigned char response;
  unsigned char i;

  for (i = 0; i != SD_SPI_RESPONSE_ATTEMPT_COUNT; ++i) {
    response = sd_spi_read();
    if (response != 0xFF) {
      break;
    }
    delay_ms(100);
  }

  return response;
}

void sd_spi_select(void) {
  sd_spi_8_clock_cycles();
  spi_select(sd_via, SD_VIA_CS);
}

void sd_spi_deselect(void) {
  spi_select(sd_via, SD_VIA_CS_NONE);
  sd_spi_8_clock_cycles();
}

void sd_spi_8_clock_cycles(void) {
  spi_read_write_byte(sd_via, 0xFF);
}

unsigned char sd_spi_read(void) {
  return spi_read_write_byte(sd_via, 0xFF);
}

void sd_spi_write(unsigned char c) {
  spi_read_write_byte(sd_via, c);
}

void sd_log(const char* const cmd, unsigned char response) {
    fprintf(stderr, "%s: %c%c\n", cmd, hex_get_hi_char(response), hex_get_lo_char(response));
}
