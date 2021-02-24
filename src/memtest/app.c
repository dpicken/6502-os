#include "app.h"

#include "button/event.h"
#include "kernel/memory.h"
#include "console/console.h"

#include <stdio.h>
#include <zlib.h>

void memtest_rom(void) {
  printf(" ROM: %08lx\n", memory_get_rom_checksum());
  printf("CALC: %08lx\n", crc32(0, memory_get_rom_base_address(), memory_get_rom_size()));
}

void memtest_app_enter(void) {
  button_released_set_handler(memtest_rom, button_code_fire1);

  memtest_rom();
}
