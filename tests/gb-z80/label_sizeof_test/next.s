
.MEMORYMAP
DEFAULTSLOT 1.01
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $6000
.ENDME

.ROMBANKMAP
BANKSTOTAL 3
BANKSIZE $2000
BANKS 1
BANKSIZE $6000
BANKS 1
BANKSIZE $2000
BANKS 1
.ENDRO

        ; @BT linked.gb

        .bank 0 slot 0
        .org 0

        .section "More data" KEEP
label1: .ds 1024, 9
label2: .ds 1024, 10
label3: .ds 256, 11
        .ends

        .section "sizeofs" keep
        .db "03>"                ; @BT TEST-03 03 START
        .dw _sizeof_label1+0     ; @BT 00 04
        .dw _sizeof_label2       ; @BT 00 04
        .dw _sizeof_label3+2-1-1 ; @BT 00 01
        .db "<03"                ; @BT END
        .ends
        
