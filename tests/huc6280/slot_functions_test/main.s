
        .memorymap
        defaultslot 0
        slotsize $2000
        slot 0 $0000
        slot 1 $2000
        slot 2 $4000
        slot 3 $6000 NAME "MPR3"
        slot 4 $8000 NAME "MPR4"
        slot 5 $a000
        slot 6 $c000
        slot 7 $e000
        .endme

        .rombankmap
        bankstotal 4
        banksize $2000
        banks 4
        .endro

        .emptyfill $ff

        ; @BT result.rom

        .bank 0 slot 7
        .org 0

        .db "01>"                                      ; @BT TEST-01 01 START
        .dw slotbase(0), slotbase(3), slotbase(8-1)    ; @BT 00 00 00 60 00 E0
        .dw slotbase("MPR3")                           ; @BT 00 60
        .db slot(targetLabel)                          ; @BT 07
        .dw targetLabel                                ; @BT 23 E1
        .dw slotaddress(targetLabel, 1+1+1)            ; @BT 23 61
        .dw slotaddress(targetLabel, "MPR3")           ; @BT 23 61
        .dw slotaddress(targetLabel + 2, 4)            ; @BT 25 81
        .dw slotaddress(targetLabel + 2, "MPR4")       ; @BT 25 81
        .dw slotbase(slot(targetLabel))                ; @BT 00 E0
        .db "<01"                                      ; @BT END

        .bank 1 slot 7
        .org $0123

targetLabel:
        .db $aa, $bb, $cc