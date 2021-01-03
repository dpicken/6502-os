#include "app.h"

#include "button/event.h"
#include "kernel/memory.h"
#include "lcd/init.h"
#include "lcd/io.h"

#include <stdio.h>
#include <zlib.h>

void memtest_print_rom_checksum(void) {
  char checksum_str[15];
  snprintf(checksum_str, sizeof(checksum_str), " ROM: %08lx", memory_get_rom_checksum());
  lcd_puts(checksum_str);
  snprintf(checksum_str, sizeof(checksum_str), "CALC: %08lx", crc32(0, memory_get_rom_base_address(), memory_get_rom_size()));
  lcd_puts(checksum_str);
}

void memtest_app_button_left_released(void) {
  lcd_clear();
  memtest_print_rom_checksum();
}

void memtest_app_button_right_released(void) {
  lcd_clear();
}

void memtest_app_enter(void) {
  lcd_clear();
  button_clear_handlers();
  button_released_set_handler(memtest_app_button_left_released, button_code_left);
  button_released_set_handler(memtest_app_button_right_released, button_code_right);
}
