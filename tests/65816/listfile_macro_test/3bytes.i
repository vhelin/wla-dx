
.macro THREEBYTES
  .db $33
  .rept 2
  FOURBYTES
  .endr
  .db $44
  .db $55
.endm
