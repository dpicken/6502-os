.export __STARTUP__: absolute = 1

.export startup_reset
.export _exit

.import copydata
.import zerobss
.import initlib
.import donelib
.import _main

.import __RAM_START__, __RAM_SIZE__

.include "zeropage.inc"

.segment "STARTUP"
startup_reset:
  ; Initialize stack pointer
  ldx #$FF
  txs

  ; Initialize the cc65 argument stack pointer
  lda #<(__RAM_START__ + __RAM_SIZE__)
  sta sp
  lda #>(__RAM_START__ + __RAM_SIZE__)
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
  brk
