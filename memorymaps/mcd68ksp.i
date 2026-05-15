;
; Sega CD / Mega CD MC68000 Sub CPU boot program memory map (SP.BIN)
;

.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 START $000000 SIZE $080000 NAME "PRGRAM"   ; 512KB Sub CPU program RAM
SLOT 1 START $080000 SIZE $040000 NAME "WORDRAM"  ; Word RAM window
SLOT 2 START $FF0000 SIZE   $2000 NAME "PCM"      ; PCM sound registers / RAM window
SLOT 3 START $FF8000 SIZE    $200 NAME "GATE"     ; Sub CPU gate array registers
.ENDME
