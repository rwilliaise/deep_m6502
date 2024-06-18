; setting interrupt disable using stack

  php
  pla
  ora #$4 ; interrupt disable
  pha
  lda #0 ; return 0
  plp
