  .org $8000
_start:
  lda #%00000001 ; only need one output
  sta $6002 ; ddrb

  lda #%11111111 ; all outputs
  sta $6003 ; ddra

  lda #'a'
  sta $6001

  lda #$1
  sta $6000

  lda #$0
  sta $6000

  lda #$A ; newline
  sta $6001

  lda #$1
  sta $6000

  lda #$0
  sei ; stop!

  .org $fffc
  .word _start
