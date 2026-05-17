;
; Sega 32X MC68000 Memory Map
;
; Slot 0 is the raw cartridge image as the console sees it before the 32X
; adapter bootstrap switches to Mars mode. Slot 1 is the common Mars 68000
; hot-start mirror used by standard 32X startup code at $880800.
;

.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 START $000000 SIZE $400000 NAME "CART"      ; 4MB cartridge image
SLOT 1 START $880000 SIZE $180000 NAME "MARSROM"   ; 68000 Mars hot-start mirror
SLOT 2 START $840000 SIZE $020000 NAME "FRAME0"    ; 32X framebuffer
SLOT 3 START $860000 SIZE $020000 NAME "FRAME1"    ; 32X framebuffer overwrite/image window
SLOT 4 START $A00000 SIZE   $2000 NAME "ZRAM"      ; 8KB Z80 RAM
SLOT 5 START $A15100 SIZE    $100 NAME "MARSREGS"  ; 32X adapter registers
SLOT 6 START $A15200 SIZE    $200 NAME "MARSPAL"   ; 32X palette RAM
SLOT 7 START $C00000 SIZE     $20 NAME "MDVDP"     ; Mega Drive VDP ports
SLOT 8 START $FF0000 SIZE  $10000 NAME "WRAM"      ; 64KB Work RAM
.ENDME