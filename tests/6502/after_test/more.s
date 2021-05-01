
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
        // TEST 3
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
        
