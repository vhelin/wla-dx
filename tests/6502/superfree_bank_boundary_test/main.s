        .MEMORYMAP
        DEFAULTSLOT 0
        SLOTSIZE $20
        SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
        BANKSTOTAL 2
        BANKSIZE $20
        BANKS 2
        .ENDRO

        .SECTION "ExactBankSuperFree" SUPERFREE
        .DSB $20, $66
exact_bank_superfree_end:
        .ENDS