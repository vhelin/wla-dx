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
LSR $2A     ; @BT 46 2A
ADC $2A     ; @BT 65 2A

.16BIT

LSR $2A     ; @BT 4E 2A 00
ADC $2A     ; @BT 6D 2A 00
.db "<01"   ; @BT END
