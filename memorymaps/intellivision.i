;
; Intellivision CP1610 memory map (byte addresses)
;
; WLA uses byte addresses. Each CP1610 word is two bytes, so a CPU word
; address W is byte address W*2. A cartridge that the CPU sees at $5000
; is assembled at byte address $A000.
;

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $0000 SIZE $10000 NAME "CP1610"
.ENDME
