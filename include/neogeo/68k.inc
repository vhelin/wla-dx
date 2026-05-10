;
; Neo Geo MC68000 register definitions and helper macros
;

.DEFINE REG_P1CNT         $300000
.DEFINE REG_DIPSW         $300001
.DEFINE REG_SYSTYPE       $300081

.DEFINE REG_SOUND         $320000
.DEFINE REG_STATUS_A      $320001

.DEFINE REG_P2CNT         $340000

.DEFINE REG_STATUS_B      $380000
.DEFINE REG_POUTPUT       $380001
.DEFINE REG_CRDBANK       $380011
.DEFINE REG_SLOT          $380021
.DEFINE REG_LEDLATCHES    $380031
.DEFINE REG_LEDDATA       $380041
.DEFINE REG_RTCCTRL       $380051
.DEFINE REG_RESETCC1      $380061
.DEFINE REG_RESETCC2      $380063
.DEFINE REG_RESETCL1      $380065
.DEFINE REG_RESETCL2      $380067
.DEFINE REG_SETCC1        $3800E1
.DEFINE REG_SETCC2        $3800E3
.DEFINE REG_SETCL1        $3800E5
.DEFINE REG_SETCL2        $3800E7

.DEFINE REG_NOSHADOW      $3A0001
.DEFINE REG_SHADOW        $3A0011
.DEFINE REG_SWPBIOS       $3A0003
.DEFINE REG_SWPROM        $3A0013
.DEFINE REG_CRDUNLOCK1    $3A0005
.DEFINE REG_CRDLOCK1      $3A0015
.DEFINE REG_CRDLOCK2      $3A0007
.DEFINE REG_CRDUNLOCK2    $3A0017
.DEFINE REG_CRDREGSEL     $3A0009
.DEFINE REG_CRDNORMAL     $3A0019
.DEFINE REG_BRDFIX        $3A000B
.DEFINE REG_CRTFIX        $3A001B
.DEFINE REG_SRAMLOCK      $3A000D
.DEFINE REG_SRAMUNLOCK    $3A001D
.DEFINE REG_PALBANK1      $3A000F
.DEFINE REG_PALBANK0      $3A001F

.DEFINE REG_VRAMADDR      $3C0000
.DEFINE REG_VRAMRW        $3C0002
.DEFINE REG_VRAMMOD       $3C0004
.DEFINE REG_LSPCMODE      $3C0006
.DEFINE REG_TIMERHIGH     $3C0008
.DEFINE REG_TIMERLOW      $3C000A
.DEFINE REG_IRQACK        $3C000C
.DEFINE REG_TIMERSTOP     $3C000E

.MACRO NG_SEND_SOUND_CMD
  move.b #\1, REG_SOUND
.ENDM

.MACRO NG_KICK_WATCHDOG
  move.b #0, REG_DIPSW
.ENDM

.MACRO NG_ACK_IRQ
  move.w #\1, REG_IRQACK
.ENDM

; --- P-ROM bank switching --------------------------------------------
;
; On bank-switched Neo Geo cartridges, the P-ROM area $200000-$2FFFFF
; (1 MiB) is a single window backed by multiple 1 MiB physical banks.
; The active physical bank is selected by writing to an address in the
; range $2FFFF0-$2FFFFE. The bank number is encoded in the low byte of
; the address: $2FFFF0 + (bank_number << 1). The value written is not
; used by the simple/unprotected bank mapper.
;
; This matches the scheme used by ngdevkit's P-ROM bankswitch example
; and by unprotected homebrew cartridges. Protected mappers (SMA,
; KOF2K2, etc.) use different ports and are out of scope.
;
; NG_SWITCH_P_BANK <bank_number>
;   Selects physical bank <bank_number> to be visible at $200000.

.MACRO NG_SWITCH_P_BANK
  move.w #0, ($2FFFF0 + (\1 * 2))
.ENDM

; --- Protected mapper note ------------------------------------------
;
; Boards such as SMA, PVC and KOF2002/KOF2003-style protected boards
; combine bank switching with board-specific address/data scrambling.
; WLA DX does not perform that scrambling. Keep source code in normal
; CPU-visible order, then run the appropriate post-link scrambler for
; the target board. The plain NG_SWITCH_P_BANK macro above is only for
; simple/unprotected homebrew mapping.

.DEFINE NG_MAPPER_UNPROTECTED 0
.DEFINE NG_MAPPER_SMA         1
.DEFINE NG_MAPPER_PVC         2
.DEFINE NG_MAPPER_KOF2002     3
.DEFINE NG_MAPPER_KOF2003     4

; --- LSPC / VRAM helpers ---------------------------------------------
;
; The LSPC video processor is accessed through four I/O registers in
; the VIDEO slot ($3C0000-$3C000F):
;
;   REG_VRAMADDR   ($3C0000)  Word. Current VRAM address.
;   REG_VRAMRW     ($3C0002)  Word. Reads/writes VRAM at VRAMADDR;
;                             VRAMADDR is post-incremented by VRAMMOD
;                             after each access.
;   REG_VRAMMOD    ($3C0004)  Word. Signed modulo added to VRAMADDR
;                             after each VRAMRW access.
;   REG_LSPCMODE   ($3C0006)  Word. On write, bits 8..15 set the
;                             auto-animation speed, bit 3 disables
;                             auto-animation, and bits 4..7 control
;                             raster interrupts. On read, bits 15..7
;                             hold the raster counter and bits 0..2
;                             hold the auto-animation counter.
;
; Fix-layer map lives at VRAM $7000-$74FF and is addressed column-
; major: tile at (col, row) is at $7000 + (col<<5) + row. Each entry
; is one word: (palette_bank << 12) | tile_code.
;
; Sprite control tables (used by NG_CLEAR_SPRITES):
;   SCB2 shrink/link  VRAM $8000-$81FF  (1 word per sprite)
;   SCB3 y/sticky     VRAM $8200-$83FF  (1 word per sprite)
;   SCB4 x            VRAM $8400-$85FF  (1 word per sprite)

.MACRO NG_VRAM_ADDR
  move.w #\1, REG_VRAMADDR
.ENDM

.MACRO NG_VRAM_WRITE
  move.w #\1, REG_VRAMRW
.ENDM

.MACRO NG_VRAM_MOD
  move.w #\1, REG_VRAMMOD
.ENDM

; NG_SET_AUTOANIM <speed>
;   Sets the auto-animation speed in frames (1..8). Tiles with the
;   auto-animation attribute advance once every <speed> frames.
.MACRO NG_SET_AUTOANIM
  move.w #((\1 & $FF) << 8), REG_LSPCMODE
.ENDM

; NG_DISABLE_AUTOANIM
;   Sets bit 3 of LSPCMODE to freeze auto-animated tiles.
.MACRO NG_DISABLE_AUTOANIM
  move.w #$0008, REG_LSPCMODE
.ENDM

; NG_ACK_VBLANK
;   Acknowledges the VBLANK interrupt. On cartridge systems VBLANK is
;   68k interrupt level 1 and is acknowledged with bit 2 of IRQACK.
.MACRO NG_ACK_VBLANK
  move.w #$0004, REG_IRQACK
.ENDM

; NG_WAIT_VBLANK
;   Busy-polls the LSPCMODE raster-position field until the display is
;   outside of the active display area. The raster counter is read from
;   bits 15..7 of REG_LSPCMODE; active display is $110..$1EF on NTSC
;   Neo Geo.
;
;   This macro uses d0 as a scratch register.
.MACRO NG_WAIT_VBLANK
\@_wait_vblank_start:
  move.w REG_LSPCMODE, d0
  lsr.w #7, d0
  cmpi.w #$0110, d0
  bcs.b \@_wait_vblank_done
  cmpi.w #$01F0, d0
  bcs.b \@_wait_vblank_start
\@_wait_vblank_done:
.ENDM

; NG_CLEAR_SPRITES
;   Hides all 381 hardware sprites by writing 0 to every entry of the
;   SCB3 (y / sticky) table at VRAM $8200. Trashes d0/d1.
.MACRO NG_CLEAR_SPRITES
  NG_VRAM_ADDR $8200
  NG_VRAM_MOD 1
  move.w #380, d0            ; 381 sprites, dbra loops count+1 times
\@_clear_sprites_loop:
  move.w #0, REG_VRAMRW
  dbra d0, \@_clear_sprites_loop
.ENDM

; --- NGH homebrew registry -------------------------------------------
;
; NGH codes (the 16-bit game identifier emitted by `.NGHEADER NGH`) are
; registered per title. Commercial SNK releases use values in the range
; $0001-$0FFF. Community homebrew projects SHOULD use values in
; $0000-$00FF (traditionally reserved for homebrew / test ROMs by the
; Neo Geo homebrew community) to avoid collisions with commercial
; releases and with each other. The authoritative list lives in the
; NGH registry maintained by the Neo Geo homebrew community. Please
; check the registry before picking an NGH value for a public release.