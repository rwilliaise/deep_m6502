; testing subroutines

  lda #42
  jsr zero
  sei
zero:
  lda #0
  rts
