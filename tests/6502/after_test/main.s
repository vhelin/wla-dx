
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

        .DW label_A

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
        .DB "01>"               ; @BT TEST-01 01 START
label_A:.DB 0, 1                ; @BT 00 01 02 03 04 05 06 07
        .ENDS                   ; @BT END

        .SECTION "SectionC" FREE APPENDTO "SectionB" PRIORITY 17
label_C .DB 4, 5
        .ENDS

        /////////////////////////////////////////////////////////////////////////////
        // TEST 2
        /////////////////////////////////////////////////////////////////////////////

        .org $100

        .dw label_1, label_2, label_3, label_4, Empty1, Empty4
        
        .SECTION "Section2"
label_2:.DB 2, 3
        .ENDS

        .SECTION "Section4" FREE AFTER "Section3"
label_4:.DB 6, 7
        .DB "<02"
        .ENDS
        
        .SECTION "Section1" FREE
        .DB "02>"               ; @BT TEST-02 02 START
label_1:.DB 0, 1                ; @BT 00 01 02 03 04 05 06 07
        .ENDS                   ; @BT END

        .SECTION "Section3" FREE AFTER "Section2"
label_3 .DB 4, 5
        .ENDS

        /////////////////////////////////////////////////////////////////////////////
        // NO TEST
        /////////////////////////////////////////////////////////////////////////////

        .section "preSuite" free keep
   preSuite:
        .dw endSuite
        .ends

        .section "preSuite.end" appendto preSuite
   endSuite:
        .db 0
        .ends

        /////////////////////////////////////////////////////////////////////////////
        // TEST 6
        /////////////////////////////////////////////////////////////////////////////

        .SECTION "Empty1" SEMIFREE KEEP
   Empty1:
        .ENDS

        .SECTION "Empty2" FREE KEEP AFTER "Empty1"
        .DB "06>"               ; @BT TEST-06 06 START
        .ENDS                   ; @BT 01 02 03 04
                                ; @BT END

        .SECTION "Empty4" FREE KEEP AFTER "Empty3"
   Empty4:
        .ENDS
        
        .SECTION "Empty3" FREE KEEP AFTER "Empty2"
        .DB 1, 2, 3, 4
        .DB "<06"
        .ENDS
