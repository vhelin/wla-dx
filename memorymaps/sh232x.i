;
; Sega 32X SH-2 Memory Map
;
; MAME maps raw cartridge file offset 0 to SH-2 address $02000000 through the
; 32-bit big-endian gamecart_sh2 region. The $20000000/$24000000 windows are
; the uncached Mars register/framebuffer addresses commonly used by 32X homebrew. SH-2
; internal registers live at $FFFFFE00 and above, but WLA memory-map slot starts
; are signed-positive values, so those addresses are provided as constants in
; include/sega32x/sh2.inc instead of as a slot here.
;

.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 START $02000000 SIZE $00400000 NAME "CART"      ; 4MB cartridge ROM
SLOT 1 START $06000000 SIZE $00040000 NAME "SDRAM"     ; 256KB shared SDRAM
SLOT 2 START $20004000 SIZE     $400 NAME "MARSREGS"  ; 32X system/PWM/VDP regs
SLOT 3 START $20004200 SIZE     $200 NAME "MARSPAL"   ; 32X palette RAM
SLOT 4 START $24000000 SIZE $00020000 NAME "FRAME0"    ; framebuffer
SLOT 5 START $24020000 SIZE $00020000 NAME "FRAME1"    ; overwrite/image window
.ENDME