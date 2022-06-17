#include "app.h"

#include "display/display.h"
#include "io/io.h"
#include "kernel/memory.h"
#include "switcher/app.h"
#include "util/hex.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

static const ui_menu_item loader_menu_items[] = {
  UI_MENU_MAKE_ITEM("Load SD card", loader_app_load_sd),
  UI_MENU_MAKE_ITEM("Run", loader_app_run)
};

ui_menu loader_menu = UI_MENU_MAKE_SUB_MENU(&switcher_menu, loader_menu_items);

void loader_app_load_sd(void) {
  display_clear();
  loader_app_load_sd_impl();
  ui_menu_enter_current();
}

void loader_app_load_sd_impl(void) {
  FILE* sd_card = fopen(DEV_SD_CARD, "r+");
  unsigned char* it = memory_get_user_space_base_address();
  const unsigned char* end = it + memory_get_user_space_size();

  if (sd_card == NULL) {
    fprintf(stderr, "fopen: %s\n", strerror(errno));
    return;
  }

  while (it != end) {
    it += fread(it, sizeof(*it), end - it, sd_card);
    if (ferror(sd_card) != 0) {
      fprintf(stderr, "fread: %s\n", strerror(errno));
      break;
    }
    fprintf(stderr, "Loading: %-5td\r", end - it);
  }

  fclose(sd_card);
}

void loader_app_run(void) {
  const char* user_space = memory_get_user_space_base_address();

  fprintf(stderr, "%p:\n", user_space);
  fprintf(stderr, "%c%c %c%c %c%c %c%c\n",
      hex_get_hi_char(user_space[0]), hex_get_lo_char(user_space[0]),
      hex_get_hi_char(user_space[1]), hex_get_lo_char(user_space[1]),
      hex_get_hi_char(user_space[2]), hex_get_lo_char(user_space[2]),
      hex_get_hi_char(user_space[3]), hex_get_lo_char(user_space[3]));

  memory_call_user_space();
}
