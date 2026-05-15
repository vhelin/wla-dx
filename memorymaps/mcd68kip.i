;
; Sega CD / Mega CD MC68000 Main CPU boot program memory map (IP.BIN)
;

.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 START $000000 SIZE $080000 NAME "IP"       ; initial program image
SLOT 1 START $200000 SIZE $040000 NAME "WORDRAM"  ; 2M Word RAM window
SLOT 2 START $A12000 SIZE     $30 NAME "GATE"     ; Sega CD gate array registers
SLOT 3 START $FF0000 SIZE $010000 NAME "WRAM"     ; 64KB Work RAM
.ENDME
