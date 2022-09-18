
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
           SLOT 1 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO


        .BANK 0 SLOT 0
        .ORG $1F00
        
        ; @BT linked.rom

        // the point of this file is to test linking a file without labels and pending calculations
        
        .db "90>"                                       ; @BT TEST-90 90 START
        .db 5                                           ; @BT 05
        .db 6                                           ; @BT 06
        .db "<90"                                       ; @BT END
