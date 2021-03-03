#ifndef kernel_memory_h
#define kernel_memory_h

const char* const __fastcall__ memory_get_rom_version(void);

/** Get ROM checksum. */
unsigned long __fastcall__ memory_get_rom_checksum(void);

/** Get ROM base address. */
const unsigned char* __fastcall__ memory_get_rom_base_address(void);

/** Get ROM size, in bytes. */
unsigned int __fastcall__ memory_get_rom_size(void);

#endif // ifndef kernel_memory_h
