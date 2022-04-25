#include "app.h"

#include "sd.h"

#include "controller/controller.h"
#include "display/display.h"
#include "io/io.h"
#include "util/hex.h"

void sd_app_enter(void) {
  if (!sd_is_initialized()) {
    fprintf(stderr, "sd: not initialized\n");
    return;
  }

  controller_button_set_demuxed_depressed_handler(sd_app_card_status, controller_button_a);
  controller_button_set_demuxed_depressed_handler(sd_app_card_open, controller_button_b);
  controller_button_set_demuxed_depressed_handler(sd_app_card_close, controller_button_c);

  sd_app_card_status();
}

void sd_app_card_status(void) {
  fprintf(vdu, "Card detected: %s\n", sd_card_detected() ? "yes" : "no");
}

void sd_app_card_open(void) {
  fprintf(vdu, "Card open: %s\n", sd_card_open() ? "OK" : "Fail");
}

void sd_app_card_close(void) {
  sd_card_close();
  fprintf(vdu, "Card closed\n");
}
