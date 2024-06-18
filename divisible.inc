; check if 42 is divisible by 6

; have to set carry flag??
  sec 
  lda #42
_start:
  sbc #6
  bne _start
  sei
