  .org $8000
putc:
  sta $6001

  lda #$1
  sta $6000

  lda #$0
  sta $6000
  rts

_start:
  lda #%00000001 ; only need one output
  sta $6002 ; ddrb

  lda #%11111111 ; all outputs
  sta $6003 ; ddra

  ldx #0

loop:
  lda hello_world,X
  beq zero
  inx
  pha
  jsr putc
  pla
  bne loop

zero:
  lda #$A
  jsr putc

  lda #$0
  sei ; stop!

hello_world:
  .asciiz "Hello, world!"

  .org $fffc
  .word _start
