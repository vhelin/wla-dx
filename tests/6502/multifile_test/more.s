
        .EMPTYFILL 0
        
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


        .section "SectionB" FREE
label_B:.db 0, 1
        .ends

        .INCLUDE "more_1.inc"
        .INCLUDE "more_2.inc"
