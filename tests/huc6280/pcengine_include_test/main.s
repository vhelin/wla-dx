.INCDIR "../../../include/pcengine"
.INCLUDE "../../../memorymaps/pcengine.i"
.INCLUDE "hardware.inc"
.INCLUDE "vectors.inc"
.INCLUDE "macros.inc"

.ROMBANKSIZE $2000
.ROMBANKS 1

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0
Start:
  PCE_DISABLE_INTERRUPTS
  PCE_CLEAR_DECIMAL
  lda #<PCE_REG_VDC_SELECT
  lda #>PCE_REG_VCE_DATA_HI
  rts

.ORG $0010
  PCE_VECTOR_TABLE Start, Start, Start, Start, Start