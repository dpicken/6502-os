#include "app.h"

#include "button/event.h"
#include "kernel/memory.h"
#include "lcd/init.h"

#include <stdio.h>
#include <zlib.h>

void memtest_print_rom_checksum(void) {
  lcd_clear();
  printf(" ROM: %08lx\n", memory_get_rom_checksum());
  printf("CALC: %08lx\n", crc32(0, memory_get_rom_base_address(), memory_get_rom_size()));
}

void memtest_app_enter(void) {
  memtest_print_rom_checksum();
  button_released_set_handler(memtest_print_rom_checksum, button_code_fire1);
}
