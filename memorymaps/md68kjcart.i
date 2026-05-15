;
; Sega Mega Drive/Genesis MC68000 Memory Map for J-Cart cartridges
;
; The extra controller I/O lives in the cartridge address range and is
; exposed through include/megadrive/jcart.inc. The ROM layout is otherwise
; the standard 4MB Mega Drive cartridge window.
;

.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 START $000000 SIZE $400000 NAME "ROM"   ; 4MB ROM / Cartridge
SLOT 1 START $A00000 SIZE   $2000 NAME "ZRAM"  ; 8KB Z80 RAM
SLOT 2 START $FF0000 SIZE  $10000 NAME "WRAM"  ; 64KB Work RAM
.ENDME
