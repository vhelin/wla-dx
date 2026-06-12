; NOTE: This test was created by Claude Fable 5.
; @BT linked.rom

; Regression test for the MC68000 "SP" register alias.
;
; A label or definition named S0..S7 must NOT be parsed as address register
; A7 -- the SP alias must only match the exact token "SP". With the bug, the
; operand parser accepted "S" followed by a digit as An, so "move.w S3,d0"
; assembled as "move.w a7,d0" (30 0f) instead of an absolute reference to the
; label S3. SP / A7 themselves must still assemble to the A7 form.

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 $000000 $100000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.define S6 $0010

.BANK 0 SLOT 0
.ORG 0

        .db "AB>"               ; @BT TEST-01 AB START
S3:     move.w S3,d0            ; @BT $30 $39 $00 $00 $00 $03
        move.w S6,d1            ; @BT $32 $39 $00 $00 $00 $10
        move.w sp,d2            ; @BT $34 $0F
        move.w a7,d3            ; @BT $36 $0F
        .db "<AB"               ; @BT END
