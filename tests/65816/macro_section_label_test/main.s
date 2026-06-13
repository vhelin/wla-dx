; NOTE: This test was created by Claude Fable 5.

//////////////////////////////////////////////////////////////////////
// Regression test: a label inside a FORCE/OVERWRITE .SECTION, passed to
// a .MACRO and used in an assembly-time calculation, must resolve to the
// label's own address -- not the section's start address.
//
// With the bug (stack.c _resolve_string), every such label resolved to
// g_slots[...].address + section->address, so two labels in the same
// FORCE section both collapsed to the section start and their difference
// computed to 0 instead of their real distance.
//////////////////////////////////////////////////////////////////////

.memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
.endme

.lorom

.rombanksize $8000
.rombanks 1

.bank 0 slot 1

; @BT result.rom

.MACRO EmitDiff ARGS a, b
  .db b - a
.ENDM

.section "ForcedLabels" FORCE
        .db "AB>"                  ; @BT TEST-01 AB START
first:
        .db $00 $00 $00 $00 $00    ; @BT 00 00 00 00 00
second:
        EmitDiff first, second     ; @BT 05
        .db "<AB"                  ; @BT END
.ends
