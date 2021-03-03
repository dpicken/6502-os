#include "app.h"

#include "button/event.h"
#include "console/console.h"
#include "kernel/memory.h"
#include "cc65/write.h"

#include <stdio.h>
#include <zlib.h>

void memtest_app_enter(void) {
  button_released_set_handler(memtest_rom, button_code_fire1);

  memtest_rom();
}

void memtest_rom(void) {
  unsigned long checksum = memory_get_rom_checksum();
  unsigned long calculated_checksum = crc32(0, memory_get_rom_base_address(), memory_get_rom_size());
  unsigned char ok = (checksum == calculated_checksum);

  console_clear();

  fprintf(vidiprinter, "ROM: %s\n", ok ? "OK" : "FAIL");
  fprintf(vidiprinter, "csum: %08lx\n", checksum);
  fprintf(vidiprinter, "calc: %08lx", calculated_checksum);
}
