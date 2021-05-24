
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

        
        .BANK 1 SLOT 1
        .ORG 0

        .section "SectionC" FREE
label_C:.db 0, 1
        .ends


        .BANK 15 SLOT 1
        .ORG 0
        
        .section "SectionD" FREE
label_D:.db 0, 1
        .ends

        
        .BANK 15 SLOT 2
        .ORG $1FF0
        
        .section "SectionE" FORCE
label_E:.db 0, 1
        .ends
        
