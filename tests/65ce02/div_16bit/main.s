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
        LSR $2A     ; @BT 46 2A
        ADC $2A     ; @BT 65 2A
        LSR.W $2A   ; @BT 4E 2A 00
        ADC.W $2A   ; @BT 6D 2A 00
        LSR $2A.W   ; @BT 4E 2A 00
        ADC $2A.W   ; @BT 6D 2A 00

        sta.w $50               ; @BT 8D 50 00
        sta $50                 ; @BT 85 50
        sta $50.b               ; @BT 85 50
        sta.b $50               ; @BT 85 50
        sta $5432               ; @BT 8D 32 54
        sta $5432 + 2           ; @BT 8D 34 54
        sta $5432.w             ; @BT 8D 32 54
        sta $50.w               ; @BT 8D 50 00
        sta $50                 ; @BT 85 50

        .16BIT

        LSR $2A     ; @BT 4E 2A 00
        ADC $2A     ; @BT 6D 2A 00

        sta $50                 ; @BT 8D 50 00
        sta $50.b               ; @BT 85 50
        sta.b $50               ; @BT 85 50
        sta $50                 ; @BT 8D 50 00
        sta.w $50               ; @BT 8D 50 00
        sta $5432               ; @BT 8D 32 54
        sta $5432 + 1           ; @BT 8D 33 54
        sta $5432.w             ; @BT 8D 32 54
        sta $50.w               ; @BT 8D 50 00

        .db "<01"   ; @BT END
