;
; Neo Geo BIOS entry-point addresses and call macros
;
; The Neo Geo BIOS exposes a fixed vector table near the top of the
; BIOS ROM region ($C00000-$C1FFFF). Addresses below match the MVS/AES
; BIOS as documented in the Neo Geo Programmer's Reference Guide and
; the unibios source. Each entry is a 6-byte `jmp` to the real code,
; so calling them via `jsr` returns cleanly to the caller.
;
; Parameters are generally passed in data/address registers. Check the
; per-function comment for the expected register convention.
;
; Neo Geo CD adds a second vector table at $C00500-$C006FF with media
; / CD-audio services. The `NG_BIOS_CD_*` macros below dispatch into
; that range.

; ===== MVS / AES (cartridge) BIOS vectors =============================

.DEFINE NG_BIOS_INIT_HARDWARE    $C00402  ; reset entry used by cart vectors
.DEFINE NG_BIOS_EXC_BUS_ERROR    $C00408
.DEFINE NG_BIOS_EXC_ADDR_ERROR   $C0040E
.DEFINE NG_BIOS_EXC_ILLEGAL_OP   $C00414
.DEFINE NG_BIOS_EXC_INVALID_OP   $C0041A  ; div-by-zero / CHK / TRAPV / privilege
.DEFINE NG_BIOS_EXC_TRACE        $C00420
.DEFINE NG_BIOS_EXC_FPU_EMU      $C00426
.DEFINE NG_BIOS_UNINITIALIZED_INT $C0042C
.DEFINE NG_BIOS_SPURIOUS_INT     $C00432

.DEFINE NG_BIOS_SYSTEM_INT1      $C00438  ; VBLANK handler entry
.DEFINE NG_BIOS_SYSTEM_INT2      $C0043E  ; timer handler entry
.DEFINE NG_BIOS_SYSTEM_RETURN    $C00444  ; return from BIOS callback
.DEFINE NG_BIOS_SYSTEM_IO        $C0044A  ; update controllers / credits
.DEFINE NG_BIOS_CREDIT_CHECK     $C00450  ; check credits, d0 = amount
.DEFINE NG_BIOS_CREDIT_DOWN      $C00456  ; subtract credits, d0 = amount
.DEFINE NG_BIOS_READ_CALENDAR    $C0045C  ; read RTC into BIOS RAM
.DEFINE NG_BIOS_SETUP_CALENDAR   $C00462
.DEFINE NG_BIOS_CARD             $C00468  ; memory-card dispatcher
.DEFINE NG_BIOS_CARD_ERROR       $C0046E  ; display card-error screen
.DEFINE NG_BIOS_HOW_TO_PLAY      $C00474  ; run "how to play" demo
.DEFINE NG_BIOS_CHECKSUM         $C0047A
.DEFINE NG_BIOS_FIX_CLEAR        $C004C2  ; clear fix layer
.DEFINE NG_BIOS_LSP_1ST          $C004C8  ; clear sprite control tables
.DEFINE NG_BIOS_MESS_OUT         $C004CE  ; print a string (see below)
.DEFINE NG_BIOS_CONTROLLER_SETUP $C004D4  ; initialize controllers

; NG_BIOS_MESS_OUT convention:
;   a0.l : pointer to null-terminated string (byte per char, ASCII)
;   d0.w : fix-layer starting address (column-major, as documented in
;          68k.inc; high byte is palette bank, low byte unused here)
;   d1.w : (MVS only) x/y offset or tile-attribute word; see BIOS
;          version notes
; Registers d0-d7/a0-a6 may be trashed by the BIOS.

; ===== Neo Geo CD BIOS vectors ========================================
;
; The CD BIOS exposes a command dispatcher at $C0056A that takes a
; command number in d0.w and parameters in d1..d3 / a0..a1. Common
; command codes:

.DEFINE NG_BIOS_CD_COMMAND       $C0056A

.DEFINE NG_CD_CMD_LOADFILES      $0002  ; load files listed at (a0)
.DEFINE NG_CD_CMD_CDDAPLAY       $0104  ; start CDDA track (d1 = track)
.DEFINE NG_CD_CMD_CDDAPAUSE      $0105  ; pause CDDA
.DEFINE NG_CD_CMD_CDDARESUME     $0106  ; resume CDDA
.DEFINE NG_CD_CMD_CDDASTOP       $0103  ; stop CDDA
.DEFINE NG_CD_CMD_UPLOAD         $0201  ; upload data (see BIOS docs)

; --- Cartridge BIOS call helpers --------------------------------------

; NG_BIOS_CALL <vector_address>
;   Shortcut for `jsr <vector_address>`. Use with any of the
;   NG_BIOS_* MVS/AES vectors above.
.MACRO NG_BIOS_CALL
  jsr \1
.ENDM

; NG_BIOS_CLEAR_FIX
;   Clears the entire fix layer by calling the BIOS.
.MACRO NG_BIOS_CLEAR_FIX
  jsr NG_BIOS_FIX_CLEAR
.ENDM

; NG_BIOS_CLEAR_SPRITES
;   Initializes sprite control tables via the BIOS (LSP_1ST).
.MACRO NG_BIOS_CLEAR_SPRITES
  jsr NG_BIOS_LSP_1ST
.ENDM

; NG_BIOS_RETURN_TO_MENU
;   Returns control to the BIOS (used for MVS game-over / timeout).
.MACRO NG_BIOS_RETURN_TO_MENU
  jmp NG_BIOS_SYSTEM_RETURN
.ENDM

; --- Neo Geo CD call helpers -----------------------------------------

; NG_BIOS_CD_CALL <command>
;   Loads d0.w with <command> and jumps to the CD BIOS dispatcher.
;   Caller is responsible for setting up d1..d3 / a0..a1 as required
;   by the specific command.
.MACRO NG_BIOS_CD_CALL
  move.w #\1, d0
  jsr NG_BIOS_CD_COMMAND
.ENDM

; NG_BIOS_CD_CDDAPLAY <track>
;   Starts CDDA playback of the given BCD-encoded track number.
.MACRO NG_BIOS_CD_CDDAPLAY
  move.w #\1, d1
  NG_BIOS_CD_CALL NG_CD_CMD_CDDAPLAY
.ENDM

; NG_BIOS_CD_CDDAPAUSE
.MACRO NG_BIOS_CD_CDDAPAUSE
  NG_BIOS_CD_CALL NG_CD_CMD_CDDAPAUSE
.ENDM

; NG_BIOS_CD_CDDARESUME
.MACRO NG_BIOS_CD_CDDARESUME
  NG_BIOS_CD_CALL NG_CD_CMD_CDDARESUME
.ENDM

; NG_BIOS_CD_CDDASTOP
.MACRO NG_BIOS_CD_CDDASTOP
  NG_BIOS_CD_CALL NG_CD_CMD_CDDASTOP
.ENDM

; NG_BIOS_CD_LOADFILES <list_addr>
;   Loads the file list pointed to by <list_addr> (passed in a0) from
;   the CD into main RAM / VRAM as described in the list structure.
.MACRO NG_BIOS_CD_LOADFILES
  lea \1, a0
  NG_BIOS_CD_CALL NG_CD_CMD_LOADFILES
.ENDM
