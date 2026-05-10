;
; Neo Geo 68k crt0 — minimal startup for homebrew cartridges
;
; Usage: in your main source file, after including macros.inc,
; 68k.inc and bios.inc, include this file, then define a
; label named `main:` that crt0 will `jsr` into. The .NGHEADER
; USERENTRY field should point at `_ng_start` (or at a wrapper that
; eventually falls through to it).
;
; The crt0 assumes the standard Neo Geo cartridge memory map as
; defined in `memorymaps/neogeo68k.i` and is placed manually at
; $0000 by the including file. crt0 emits:
;   * the 68k exception table at $000000-$0000FF
;   * the game callback wrappers referenced by .NGHEADER
;   * the startup body at $000200
;
;   .INCDIR "../../../include/neogeo"
;   .INCLUDE "../../../memorymaps/neogeo68k.i"
;   .INCLUDE "macros.inc"
;   .INCLUDE "68k.inc"
;   .INCLUDE "bios.inc"
;
;   .ROMBANKMAP
;     BANKSTOTAL 1
;     BANKSIZE $100000
;     BANKS 1
;   .ENDRO
;
;   .NGHEADER
;     NGH $0042
;     USERENTRY _ng_userentry
;     PLAYERSTART _ng_playerstart
;     DEMOEND _ng_demoend
;     COINSOUND _ng_coinsound
;   .ENDNG
;
;   .BANK 0 SLOT 0
;   .ORGA $0000
;   .INCLUDE "crt0.s"
;
;   main:
;     ; your code here
;     rts
;
; crt0 performs the following:
;   1. Disables interrupts (IPL = 7, supervisor)
;   2. Sets the supervisor stack pointer to the top of work RAM
;   3. Initializes LSPC (VRAMMOD = 1, auto-anim off)
;   4. Hides all 381 hardware sprites
;   5. Clears the fix layer via the BIOS
;   6. Initializes the controller subsystem via the BIOS
;   7. Selects palette bank 0
;   8. Sends a RESET command to the Z80 sound CPU
;   9. Leaves interrupts masked, falls through to `main`
;  10. On return from `main`, hangs in a tight VBLANK-ack loop so the
;      watchdog stays happy.

.dd $0010F300
.dd NG_BIOS_INIT_HARDWARE
.dd NG_BIOS_EXC_BUS_ERROR
.dd NG_BIOS_EXC_ADDR_ERROR
.dd NG_BIOS_EXC_ILLEGAL_OP
.dd NG_BIOS_EXC_INVALID_OP
.dd NG_BIOS_EXC_INVALID_OP
.dd NG_BIOS_EXC_INVALID_OP
.dd NG_BIOS_EXC_INVALID_OP
.dd NG_BIOS_EXC_TRACE
.dd NG_BIOS_EXC_FPU_EMU
.dd NG_BIOS_EXC_FPU_EMU
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd NG_BIOS_UNINITIALIZED_INT
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd NG_BIOS_SPURIOUS_INT
.dd _ng_vblank_handler
.dd _ng_timer_handler
.dd $00000000
.dd $00000000
.dd $00000000
.dd $00000000
.dd $00000000
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF
.dd $FFFFFFFF

.ORGA $0200

_ng_userentry:
  moveq  #0, d0
  move.b $10FDAE, d0
  cmpi.b #3, d0
  bls.w  _ng_start
  rts

_ng_playerstart:
  rts

_ng_demoend:
  rts

_ng_coinsound:
  rts

_ng_start:
  move.w #$2700, sr           ; IPL = 7, supervisor mode
  lea    $10F300, a7          ; supervisor stack at top of work RAM

  ; --- LSPC init --------------------------------------------------
  NG_VRAM_MOD 1               ; linear post-increment
  NG_DISABLE_AUTOANIM          ; auto-anim off, raster int off

  ; --- hide sprites + clear fix layer ----------------------------
  NG_CLEAR_SPRITES
  jsr    NG_BIOS_FIX_CLEAR
  jsr    NG_BIOS_CONTROLLER_SETUP

  ; --- palette bank 0 --------------------------------------------
  move.b #1, REG_PALBANK0     ; $3A001F selects palette bank 0
  move.b #1, REG_CRTFIX       ; $3A001B selects cartridge fix font

  ; --- zero out palette RAM bank 0 ($400000-$401FFF) -------------
  lea    $400000, a0
  move.w #($2000/4)-1, d0
_ng_start_clear_pal:
  move.l #0, (a0)+
  dbra   d0, _ng_start_clear_pal

  ; --- default palette baseline ----------------------------------
  ; Keep palette 0 minimal so BIOS/background state stays dark.
  move.w #$8000, $400000      ; palette 0, color 0: transparent black
  move.w #$7FFF, $400002      ; palette 0, color 1: white

  ; --- dedicated text palette (palette 15) -----------------------
  ; The hello example prints with palette 15 to avoid clashes with
  ; palette 0 entries the BIOS may also be using.
  move.w #$8000, $4001E0
  lea    $4001E2, a0
  moveq  #14, d0
_ng_start_init_text_palette:
  move.w #$7FFF, (a0)+
  dbra   d0, _ng_start_init_text_palette

  ; --- reset Z80 sound driver ------------------------------------
  NG_SEND_SOUND_CMD NG_SOUND_CMD_RESET

  ; --- leave IRQs masked unless the program installs handlers ----
  NG_ACK_VBLANK

  ; --- hand off to user `main` ----------------------------------
  jsr    main

  ; --- if main ever returns, fall into a safe idle loop ---------
_ng_start_idle:
  NG_ACK_VBLANK
  NG_KICK_WATCHDOG
  bra.b  _ng_start_idle

_ng_vblank_handler:
  jmp    NG_BIOS_SYSTEM_INT1

_ng_timer_handler:
  jmp    NG_BIOS_SYSTEM_INT2
