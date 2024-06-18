  .org $8000
_start:
  lda #$0
  sei

  .org $fffc
  .word _start
