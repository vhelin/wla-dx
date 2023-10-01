
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

        ; @BT linked.rom

        /////////////////////////////////////////////////////////////////////////////
        // TEST 3
        /////////////////////////////////////////////////////////////////////////////

        .org $200

        .section "Bear1" SEMIFREE KEEP
        .db "03>"               ; @BT TEST-03 03 START
        .db 0, 1                ; @BT 00 01 02 03 04 05 AA BB CC DD 06 07
        .ends                   ; @BT END

        .section "BearB" AFTER "BearA" PRIORITY 5 KEEP
        .db $cc, $dd
        .ends

        .section "BearA" APPENDTO "Bear3" PRIORITY 6
        .db $aa, $bb
        .ends
        
        .section "Bear3" OFFSET 2 AFTER "Bear1" PRIORITY 8 KEEP
        .db 4, 5
        .ends

        .section "Bear2" AFTER "Bear1" PRIORITY 9 KEEP
        .db 2, 3
        .ends

        .section "Bear4" OFFSET 8 AFTER "Bear1" PRIORITY 7 KEEP
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
        .db "04>"               ; @BT TEST-04 04 START
        .db $ee                 ; @BT EE 00 00 00 00 FF
        .ends                   ; @BT END
        
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
        .DB "05>"               ; @BT TEST-05 05 START
label_1X:.DB 0, 1               ; @BT 00 01 02 03 04 05 00 06 07
        .ENDS                   ; @BT END

        .bank 3 slot 3
        .org $400
        
        .SECTION "Section3X" FREE APPENDTO "Section2X" PRIORITY 27
label_3X:.DB 4, 5
        .ENDS
        

