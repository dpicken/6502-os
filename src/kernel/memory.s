.export _memory_get_rom_version
.export _memory_get_rom_checksum
.export _memory_get_rom_base_address
.export _memory_get_rom_size

.import __ROM_CSUM_START__
.import __ROM_IMAGE_VERSION_START__
.import __ROM_IMAGE_START__
.import __ROM_IMAGE_SIZE__
.importzp sreg

.segment "CODE"

_memory_get_rom_checksum:
  lda __ROM_CSUM_START__ + 3
  sta sreg + 1
  lda __ROM_CSUM_START__ + 2
  sta sreg
  ldx __ROM_CSUM_START__ + 1
  lda __ROM_CSUM_START__
  rts

_memory_get_rom_version:
  ldx #>(__ROM_IMAGE_VERSION_START__)
  lda #<(__ROM_IMAGE_VERSION_START__)
  rts

_memory_get_rom_base_address:
  ldx #>(__ROM_IMAGE_START__)
  lda #<(__ROM_IMAGE_START__)
  rts

_memory_get_rom_size:
  ldx #>(__ROM_IMAGE_SIZE__)
  lda #<(__ROM_IMAGE_SIZE__)
  rts
