;
; PC Engine / TurboGrafx-16 HuC6280 vector definitions
;

.DEFINE PCE_VECTOR_IRQ2             $FFF6
.DEFINE PCE_VECTOR_IRQ1             $FFF8
.DEFINE PCE_VECTOR_TIMER            $FFFA
.DEFINE PCE_VECTOR_NMI              $FFFC
.DEFINE PCE_VECTOR_RESET            $FFFE

.MACRO PCE_VECTOR_TABLE
  .DW \1
  .DW \2
  .DW \3
  .DW \4
  .DW \5
.ENDM