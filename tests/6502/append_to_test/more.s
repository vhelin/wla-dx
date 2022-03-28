
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0

        /////////////////////////////////////////////////////////////////////////////
        // TEST 2
        /////////////////////////////////////////////////////////////////////////////

        .section "S02.04" FREE APPENDTO "S02.01" PRIORITY 70
        .db 6, 7
        .ends

        .section "S02.02" FREE APPENDTO "S02.01" PRIORITY 90
        .db 2, 3
        .ends
        
        /////////////////////////////////////////////////////////////////////////////
        // TEST 4
        /////////////////////////////////////////////////////////////////////////////

        .section "KEEPME!" FREE KEEP
        .db "04>"
keepme_data:
        .db 3, 2, 1
        .db "<04"
        .ends
        
