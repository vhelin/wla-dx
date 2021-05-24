
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


        .section "SectionB" FREE
label_B:.db 0, 1
        .ends

        .INCLUDE "more_1.inc"
        .INCLUDE "more_2.inc"
