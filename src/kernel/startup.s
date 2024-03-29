.export __STARTUP__: absolute = 1

.export startup_init
.export _exit

.import copydata
.import donelib
.import initlib
.import zerobss
.import _main

.import __RAM_OS_START__, __RAM_OS_SIZE__

.include "zeropage.inc"

.segment "STARTUP"

startup_init:
  ; Initialize stack pointer
  ldx #$FF
  txs

  ; Initialize the cc65 argument stack pointer
  lda #<(__RAM_OS_START__ + __RAM_OS_SIZE__)
  sta sp
  lda #>(__RAM_OS_START__ + __RAM_OS_SIZE__)
  sta sp+1

  ; Initialize global variables
  jsr zerobss
  jsr copydata

  ; Run constructors
  jsr initlib

  ; Call program
  jsr _main

_exit:
  ; Run destructors
  jsr donelib

halt:
  nop
  jmp halt
  brk
