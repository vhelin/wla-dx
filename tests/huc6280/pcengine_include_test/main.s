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
  PCE_MAP_IO_PAGE_TO_MPR0
  PCE_MAP_BANK_TO_MPR $12, 4
  PCE_MAP_LABEL_TO_MPR Start, 3
  PCE_PUSH_MPR 2
  PCE_POP_MPR 2
  rts

.ORG $0040
  PCE_VECTOR_TABLE Start, Start, Start, Start, Start