
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 2
           BANKSIZE $2000
           BANKS 2
        .ENDRO

        .BANK 1 SLOT 0
        .ORG 0

        // here we test things that have caused problems on Amiga before
        // (hopefully not any more)

        ; @BT linked.rom

        .db "01>"               ; @BT TEST-01 01 START
label_01:
        .db <$1234              ; @BT 34
        .db >$5678              ; @BT 56
        .db :label_01           ; @BT 01
        .db bank(label_01)      ; @BT 01
        .db "<01"               ; @BT END

        .db "02>"                                     ; @BT TEST-02 02 START
        .dw loword($12345678)                         ; @BT 78 56
        .dw hiword($9abcdef0)                         ; @BT BC 9A
        .db "<02"                                     ; @BT END
        
