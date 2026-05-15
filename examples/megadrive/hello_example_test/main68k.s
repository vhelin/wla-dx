;
; HELLO WORLD - Mega Drive/Genesis homebrew example
;
; Displays centered white text on a black screen and asks the Z80 sound driver
; to play two beeps.
;

.INCDIR "../../../include/megadrive"

.INCLUDE "../../../memorymaps/md68k.i"
.INCLUDE "68k.inc"
.INCLUDE "vdp_regs.inc"
.INCLUDE "macros.inc"

.DEFINE MD_FONT_VRAM        $0020
.DEFINE MD_PLANE_A_VRAM     $C000
.DEFINE MD_TEXT_TILEMAP     $C69C

.ROMBANKMAP
  BANKSTOTAL 8
  BANKSIZE $4000
  BANKS 8
.ENDRO

.EMPTYFILL $00
.ROMFORMAT BIN

.MDVECTORS
  RESET _md_start
  DEFAULT _md_default_irq
  VBLANK VBlankInt
.ENDMDVECTORS

.SMDHEADER
  SYSTEMTYPE "SEGA MEGA DRIVE "
  COPYRIGHT "(C)WLA DX 2026"
  TITLEDOMESTIC "WLA DX MEGA DRIVE HELLO WORLD"
  TITLEOVERSEAS "WLA DX GENESIS HELLO WORLD"
  SERIALNUMBER "GM 00000001-00"
  DEVICESUPPORT "J"
  ROMADDRESSRANGE $000000,$01FFFF
  REGIONSUPPORT "JUE"
.ENDSMD

.BANK 0 SLOT 0
.ORGA $0200
.INCLUDE "crt0.s"

main:
  MD_DISABLE_INTERRUPTS
  bsr.w clear_vram
  bsr.w load_palette
  bsr.w load_font
  bsr.w draw_hello
  bsr.w delay_before_sound
  bsr.w load_z80_driver

MainLoop:
  bra.b MainLoop

VBlankInt:
  rte

load_z80_driver:
  MD_Z80_REQUEST_BUS
  MD_Z80_RELEASE_RESET
  bsr.w wait_z80_bus
  lea Z80Driver, a0
  lea REG_MD_Z80_RAM, a1
  move.w #(Z80DriverEnd-Z80Driver)-1, d7
LoadZ80Loop:
  move.b (a0)+, (a1)+
  dbra d7, LoadZ80Loop
  MD_Z80_HOLD_RESET
  MD_Z80_RELEASE_BUS
  MD_Z80_RELEASE_RESET
  rts

clear_vram:
  moveq #0, d0
  MD_VDP_SET_ADDR_VRAM_WRITE $0000
  move.w #$7FFF, d7
ClearVramLoop:
  move.w d0, REG_MD_VDP_DATA
  dbra d7, ClearVramLoop
  rts

load_palette:
  move.l #$C0000000, REG_MD_VDP_CTRL
  move.w #$0000, REG_MD_VDP_DATA
  move.w #$0EEE, REG_MD_VDP_DATA
  rts

load_font:
  MD_VDP_SET_ADDR_VRAM_WRITE MD_FONT_VRAM
  lea FontTiles, a0
  move.w #((FontTilesEnd-FontTiles)/2)-1, d7
LoadFontLoop:
  move.w (a0)+, REG_MD_VDP_DATA
  dbra d7, LoadFontLoop
  rts

draw_hello:
  MD_VDP_SET_ADDR_VRAM_WRITE MD_TEXT_TILEMAP
  lea HelloTileIds, a0
  move.w #10, d7
DrawHelloLoop:
  moveq #0, d0
  move.b (a0)+, d0
  move.w d0, REG_MD_VDP_DATA
  dbra d7, DrawHelloLoop
  rts

wait_z80_bus:
  btst #0, REG_MD_Z80_BUSREQ
  bne.b wait_z80_bus
  rts

delay_before_sound:
  move.l #$00040000, d1
DelayBeforeSoundLoop:
  subq.l #1, d1
  bne.b DelayBeforeSoundLoop
  rts

HelloTileIds:
  .db 1,2,3,3,4,0,5,4,6,3,7

.align 2
FontTiles:
  ; H
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $10,$00,$00,$10,$11,$11,$11,$10
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $10,$00,$00,$10,$00,$00,$00,$00

  ; E
  .db $11,$11,$11,$10,$10,$00,$00,$00
  .db $10,$00,$00,$00,$11,$11,$10,$00
  .db $10,$00,$00,$00,$10,$00,$00,$00
  .db $11,$11,$11,$10,$00,$00,$00,$00

  ; L
  .db $10,$00,$00,$00,$10,$00,$00,$00
  .db $10,$00,$00,$00,$10,$00,$00,$00
  .db $10,$00,$00,$00,$10,$00,$00,$00
  .db $11,$11,$11,$10,$00,$00,$00,$00

  ; O
  .db $01,$11,$11,$00,$10,$00,$00,$10
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $01,$11,$11,$00,$00,$00,$00,$00

  ; W
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $10,$01,$00,$10,$10,$01,$00,$10
  .db $10,$01,$00,$10,$10,$10,$10,$10
  .db $01,$00,$01,$00,$00,$00,$00,$00

  ; R
  .db $11,$11,$11,$00,$10,$00,$00,$10
  .db $10,$00,$00,$10,$11,$11,$11,$00
  .db $10,$01,$00,$00,$10,$00,$10,$00
  .db $10,$00,$01,$00,$00,$00,$00,$00

  ; D
  .db $11,$11,$11,$00,$10,$00,$00,$10
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $10,$00,$00,$10,$10,$00,$00,$10
  .db $11,$11,$11,$00,$00,$00,$00,$00
FontTilesEnd:

.align 2
Z80Driver:
  .INCBIN "hello-z80.bin"
Z80DriverEnd: