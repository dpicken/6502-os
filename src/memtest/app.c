#include "app.h"

#include "button/event.h"
#include "kernel/memory.h"
#include "lcd/init.h"

#include <stdio.h>
#include <zlib.h>

void memtest_print_rom_checksum(void) {
  printf(" ROM: %08lx\n", memory_get_rom_checksum());
  printf("CALC: %08lx\n", crc32(0, memory_get_rom_base_address(), memory_get_rom_size()));
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
