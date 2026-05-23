.INCDIR "../../../include/lynx"
.INCLUDE "../../../memorymaps/lynx.i"
.INCLUDE "hardware.inc"
.INCLUDE "vectors.inc"
.INCLUDE "macros.inc"

.ROMBANKSIZE $10000
.ROMBANKS 1

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0
Start:
  LYNX_DISABLE_INTERRUPTS
  LYNX_CLEAR_DECIMAL
  lda #<LYNX_SUZY_BASE
  lda #>LYNX_MIKEY_BASE
  rts

.ORG LYNX_VECTOR_NMI
  LYNX_VECTOR_TABLE Start, Start, Start