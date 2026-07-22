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

        .BANK 0 SLOT 0
        .ORG $0000

        .SECTION "Anchor" FORCE SPAN 0/1
        .DSB $21, $AA
        .ENDS

        .SECTION "Follower" FORCE SPAN 0/1 AFTER "Anchor"
        .DB $BB
        .ENDS