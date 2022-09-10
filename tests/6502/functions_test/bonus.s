
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO


        .BANK 0 SLOT 0
        .ORG $1000
        
        ; @BT linked.rom
        
        .db "50>"                                       ; @BT TEST-50 50 START
        .db NUM_THREE * NUM_TWO                         ; @BT 06
        .db NUM_THREE                                   ; @BT 03
        .db "<50"                                       ; @BT END
