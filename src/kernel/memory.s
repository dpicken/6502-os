.export _memory_get_rom_checksum
.export _memory_get_rom_base_address
.export _memory_get_rom_size

.import __ROM_CSUM_START__
.import __ROM_START__
.import __ROM_SIZE__
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

_memory_get_rom_base_address:
  ldx #>(__ROM_START__)
  lda #<(__ROM_START__)
  rts

_memory_get_rom_size:
  ldx #>(__ROM_SIZE__)
  lda #<(__ROM_SIZE__)
  rts
