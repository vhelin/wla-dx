;
; Neo Geo fix-layer text printer
;
; Include after 68k.inc. Text is zero-terminated and emitted horizontally
; using the standard fix-layer column-major VRAM stride. Invoke
; NG_FIXPRINT_ROUTINE once in a code bank to instantiate the helper.
;

.IFNDEF NG_FIXPRINT_INC
.DEFINE NG_FIXPRINT_INC 1

.DEFINE NG_FIXPRINT_DEFAULT_PALETTE $F000

; NG_PRINT <tilemap_addr>, <text_label>
;   Prints a zero-terminated ASCII string to the fix layer. The tilemap
;   address is a VRAM address such as $70CD. Uses palette 15 by default.
;   Trashes d0/d1/a0.
.MACRO NG_PRINT
  move.w #\1, d0
  lea    \2, a0
  bsr.w  NG_FIX_PRINT
.ENDM

.MACRO NG_FIXPRINT_ROUTINE
NG_FIX_PRINT:
  move.w d0, REG_VRAMADDR
  NG_VRAM_MOD $20
_ng_fix_print_next:
  moveq  #0, d1
  move.b (a0)+, d1
  beq.b  _ng_fix_print_done
  ori.w  #NG_FIXPRINT_DEFAULT_PALETTE, d1
  move.w d1, REG_VRAMRW
  bra.b  _ng_fix_print_next
_ng_fix_print_done:
  NG_VRAM_MOD 1
  rts
.ENDM

.ENDIF