
.MEMORYMAP
    DEFAULTSLOT 0
    SLOTSIZE $1000
    SLOT 0 $0000
    SLOT 1 $1000
    SLOT 2 $2000
    SLOT 3 $3000
.ENDME

.ROMBANKSIZE $1000
.ROMBANKS 4

.BANK 0 SLOT 0
.ORG 33h

.section "LABEL 1" FORCE KEEP
label_33h:
        nop
.ends

.BANK 0 SLOT 1
.ORG 0

.section "LABEL 2" FORCE KEEP
label_1000h:
        nop
.ends

.ORG 1

        ; @BT linked.rom

        .db "01>"               ; @BT TEST-01 01 START
        adc label_33h           ; @BT 6D 33 00
        adc label_33h.b         ; @BT 65 33
        adc label_1000h         ; @BT 6D 00 10
        adc label_1000h.l       ; @BT 6F 00 10 00
        .db "<01"               ; @BT END

        .db "02>"               ; @BT TEST-02 02 START
        adc label_33h + 1       ; @BT 6D 34 00
        adc label_33h.b + 1     ; @BT 65 34
        adc label_1000h + 1     ; @BT 6D 01 10
        adc label_1000h.l + 1   ; @BT 6F 01 10 00
        .db "<02"               ; @BT END
