  .org $8000

  .macro load_addr, address
  lda #<\address
  sta $10
  lda #>\address
  sta $11
  .endmacro

putc:
  sta $6001

  lda #$1
  sta $6000

  lda #$0
  sta $6000
  rts

puts:
  ldy #0

.loop:
  lda ($10),Y
  beq .zero
  iny
  pha
  jsr putc
  pla
  jmp .loop

.zero:
  lda #$A
  jsr putc

  rts

line00:
  .asciiz "Hello, world!"

line01:
  .asciiz "This second line should also appear!"

_start:
  lda #%00000001 ; only need one output
  sta $6002 ; ddrb

  lda #%11111111 ; all outputs
  sta $6003 ; ddra

  load_addr line00
  jsr puts

  load_addr line01
  jsr puts

  lda #$0
  brk

  .org $fffc
  .word _start
