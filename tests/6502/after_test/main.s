
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

        /////////////////////////////////////////////////////////////////////////////
        // TEST 1
        /////////////////////////////////////////////////////////////////////////////
        
        .SECTION "SectionB" FREE APPENDTO "SectionA" PRIORITY 18
label_B:.DB 2, 3
        .ENDS

        .SECTION "SectionD" FREE APPENDTO "SectionC" PRIORITY 16
label_D:.DB 6, 7
        .DB "<01"
        .ENDS
        
        .SECTION "SectionA" FREE PRIORITY 19
        .DB "01>"
label_A:.DB 0, 1
        .ENDS

        .SECTION "SectionC" FREE APPENDTO "SectionB" PRIORITY 17
label_C .DB 4, 5
        .ENDS

        /////////////////////////////////////////////////////////////////////////////
        // TEST 2
        /////////////////////////////////////////////////////////////////////////////

        .org $100
        
        .SECTION "Section2" SEMIFREE
label_2:.DB 2, 3
        .ENDS

        .SECTION "Section4" FREE AFTER "Section3" PRIORITY 26
label_4:.DB 6, 7
        .DB "<02"
        .ENDS
        
        .SECTION "Section1" FREE PRIORITY 29
        .DB "02>"
label_1:.DB 0, 1
        .ENDS

        .SECTION "Section3" FREE APPENDTO "Section2" PRIORITY 27
label_3 .DB 4, 5
        .ENDS
