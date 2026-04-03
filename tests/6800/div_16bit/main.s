.MEMORYMAP
    DEFAULTSLOT 0
    SLOTSIZE $1000
    SLOT 0 $1000
.ENDME

.ROMBANKSIZE $1000
.ROMBANKS 1

.BANK 0
.ORG 0

; @BT linked.rom

        .db "01>"   ; @BT TEST-01 01 START
        LDX $2A     ; @BT DE 2A
        LDX $2A     ; @BT DE 2A

        stab $40, X ; @BT E7 40
        stab $40    ; @BT D7 40
        stab.b $40, X ; @BT E7 40
        stab.b $40    ; @BT D7 40
        stab $40.b, X ; @BT E7 40
        stab $40.b    ; @BT D7 40
        stab.w $40    ; @BT F7 00 40
        stab $40.w    ; @BT F7 00 40
        
        .16BIT

        LDX $2A     ; @BT FE 00 2A
        LDX $2A     ; @BT FE 00 2A

        stab $40, X ; @BT E7 40
        stab $40    ; @BT F7 00 40
        stab $40.b, X ; @BT E7 40
        stab $40.b    ; @BT D7 40
        stab.b $40    ; @BT D7 40
        stab.b $40, X ; @BT E7 40
        stab.w $40    ; @BT F7 00 40
        stab $40.w    ; @BT F7 00 40

        .db "<01"   ; @BT END
