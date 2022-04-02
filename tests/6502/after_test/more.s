
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
           SLOT 1 $2000
           SLOT 2 $4000
           SLOT 3 $6000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 4
           BANKSIZE $2000
           BANKS 4
        .ENDRO

        .EMPTYFILL $00

        .BANK 0 SLOT 0
        .ORG 0

        /////////////////////////////////////////////////////////////////////////////
        // TEST 3
        /////////////////////////////////////////////////////////////////////////////

        .org $200

        .section "Bear1" SEMIFREE
        .db "03>"
        .db 0, 1
        .ends

        .section "BearB" AFTER "BearA" PRIORITY 5
        .db $cc, $dd
        .ends

        .section "BearA" APPENDTO "Bear3" PRIORITY 6
        .db $aa, $bb
        .ends
        
        .section "Bear3" OFFSET 2 AFTER "Bear1" PRIORITY 8
        .db 4, 5
        .ends

        .section "Bear2" AFTER "Bear1" PRIORITY 9
        .db 2, 3
        .ends

        .section "Bear4" OFFSET 8 AFTER "Bear1" PRIORITY 7
        .db 6, 7
        .db "<03"
        .ends
        
        /////////////////////////////////////////////////////////////////////////////
        // TEST 4
        /////////////////////////////////////////////////////////////////////////////

        .org $300

        .section "CONCRETE-2" OFFSET 4 AFTER "CONCRETE-1"
        .db $ff
        .db "<04"
        .ends

        .section "CONCRETE-1" FORCE
        .db "04>"
        .db $ee
        .ends
        
        /////////////////////////////////////////////////////////////////////////////
        // TEST 5
        /////////////////////////////////////////////////////////////////////////////

        .bank 0 slot 0
        .org $100

        .SECTION "Section2X" FREE AFTER "Section1X"
label_2X:.DB :label_2X, 3
        .ENDS

        .bank 1 slot 1
        .org $200
        
        .SECTION "Section4X" FREE AFTER "Section3X" PRIORITY 26 OFFSET 1
label_4X:.DB 6, 5+:label_4X
        .DB "<05"
        .ENDS

        .bank 2 slot 2
        .org $300
        
        .SECTION "Section1X" FORCE PRIORITY 29
        .DB "05>"
label_1X:.DB 0, 1
        .ENDS

        .bank 3 slot 3
        .org $400
        
        .SECTION "Section3X" FREE APPENDTO "Section2X" PRIORITY 27
label_3X:.DB 4, 5
        .ENDS
        

