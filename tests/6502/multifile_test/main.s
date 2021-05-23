
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


        .SECTION "SectionA" FREE
label_A:.DB 2, 3
        .ENDS

        .INCLUDE "main_1.inc"
        .INCLUDE "main_2.inc"
