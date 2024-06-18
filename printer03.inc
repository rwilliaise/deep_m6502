  .macro store_addr, addr
  ldx #$0
  lda .address,X
  sta $10

  inx
  lda .address,X
  sta $11

.address:
  .addr hello_world

  .endmacro
  .org $8000
putc:
  sta $6001

  lda #$1
  sta $6000

  lda #$0
  sta $6000
  rts

puts:
  ldx #0

.loop:
  lda ($10,X)
  beq .zero
  inx
  pha
  jsr putc
  pla
  bne .loop

.zero:
  lda #$A
  jsr putc

  rts

_start:
  lda #%00000001 ; only need one output
  sta $6002 ; ddrb

  lda #%11111111 ; all outputs
  sta $6003 ; ddra

  jsr puts

  lda #$0
  brk ; stop!


hello_world:
  .asciiz "Hello, world!"

  .org $fffc
  .word _start
