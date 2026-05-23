;
; Atari Lynx 65C02 vector helpers
;

.MACRO LYNX_VECTOR_TABLE
  .DW \1
  .DW \2
  .DW \3
.ENDM