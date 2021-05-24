
        .EMPTYFILL 0
        
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
           SLOT 1 $A000
           SLOT 2 $E000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0


        .SECTION "SectionA" FREE
label_A:.DB 2, 3
        .ENDS

        .INCLUDE "main_1.inc"
        .INCLUDE "main_2.inc"
