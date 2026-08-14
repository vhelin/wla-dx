;
; Intellivision 8KB cartridge window (CPU $5000-$5FFF)
;
; WLA uses byte addresses. Each CP1610 word is two bytes, so a cartridge
; that the CPU sees at $5000 is assembled at $A000. The linked image is
; 8KB of big-endian 16-bit words for a jzintv BIN+CFG pair.
;

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $A000 SIZE $2000 NAME "CART"
.ENDME
