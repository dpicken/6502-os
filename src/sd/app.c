#include "app.h"

#include "sd.h"

#include "controller/controller.h"
#include "display/display.h"
#include "io/io.h"
#include "util/array.h"
#include "util/hex.h"

#include <errno.h>
#include <string.h>

static FILE* sd_card;

void sd_app_enter(void) {
  if (!sd_is_initialized()) {
    fprintf(stderr, "sd: not initialized\n");
    return;
  }

  controller_button_set_demuxed_depressed_handler(sd_app_card_status, controller_button_a);
  controller_button_set_demuxed_depressed_handler(sd_app_card_open, controller_button_b);
  controller_button_set_demuxed_depressed_handler(sd_app_card_close, controller_button_c);
  controller_button_set_demuxed_depressed_handler(sd_app_card_read, controller_button_down);

  sd_app_card_status();
}

void sd_app_card_status(void) {
  fprintf(vdu, "Card detected: %s\n", sd_card_detected() ? "yes" : "no");
}

void sd_app_card_open(void) {
  if (sd_card != NULL) {
    fprintf(vdu, "Card already open\n");
    return;
  }

  fprintf(vdu, "Card open: %s\n", (sd_card = fopen(DEV_SD_CARD, "r+")) ? "OK" : "Fail");
  if (sd_card == NULL) {
    fprintf(stderr, "fopen: %s\n", strerror(errno));
  }
}

void sd_app_card_close(void) {
  if (sd_card == NULL) {
    fprintf(vdu, "Card not open\n");
    return;
  }

  fclose(sd_card);
  sd_card = NULL;
  fprintf(vdu, "Card closed\n");
}

void sd_app_card_read(void) {
  unsigned char buffer[8];
  long address;
  size_t item_count;

  if (sd_card == NULL) {
    fprintf(vdu, "Card not open\n");
    return;
  }

  address = ftell(sd_card);
  if (address == -1) {
    fprintf(stderr, "ftell: %s\n", strerror(errno));
    return;
  }

  item_count = fread(buffer, sizeof(*buffer), ARRAY_SIZE(buffer), sd_card);
  if (ferror(sd_card) != 0) {
    fprintf(stderr, "fread: %s\n", strerror(errno));
    return;
  }

  if (item_count != ARRAY_SIZE(buffer)) {
    fprintf(stderr, "short read: %i\n", item_count);
    return;
  }

  fprintf(vdu, "%li:\n", address);
  fprintf(vdu, "%c%c %c%c %c%c %c%c\n",
      hex_get_hi_char(buffer[0]), hex_get_lo_char(buffer[0]),
      hex_get_hi_char(buffer[1]), hex_get_lo_char(buffer[1]),
      hex_get_hi_char(buffer[2]), hex_get_lo_char(buffer[2]),
      hex_get_hi_char(buffer[3]), hex_get_lo_char(buffer[3]));
  fprintf(vdu, "%c%c %c%c %c%c %c%c\n",
      hex_get_hi_char(buffer[4]), hex_get_lo_char(buffer[4]),
      hex_get_hi_char(buffer[5]), hex_get_lo_char(buffer[5]),
      hex_get_hi_char(buffer[6]), hex_get_lo_char(buffer[6]),
      hex_get_hi_char(buffer[7]), hex_get_lo_char(buffer[7]));
}
