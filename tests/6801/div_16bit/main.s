.MEMORYMAP
    DEFAULTSLOT 0
    SLOTSIZE $100
    SLOT 0 $100
.ENDME

.ROMBANKSIZE 100
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
        std $30, X    ; @BT ED 30
        std.b $30, X  ; @BT ED 30
        std $30.b, X  ; @BT ED 30
        std $30       ; @BT DD 30
        std.b $30     ; @BT DD 30
        std $30.b     ; @BT DD 30
        std.w $30     ; @BT FD 00 30
        std $30.w     ; @BT FD 00 30
        
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
        std $30, X    ; @BT ED 30
        std.b $30, X  ; @BT ED 30
        std $30.b, X  ; @BT ED 30
        std $30       ; @BT FD 00 30
        std.b $30     ; @BT DD 30
        std $30.b     ; @BT DD 30
        std.w $30     ; @BT FD 00 30
        std $30.w     ; @BT FD 00 30
        
        .db "<01"   ; @BT END
