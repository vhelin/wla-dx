
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

        ; @BT linked.rom

        /////////////////////////////////////////////////////////////////////////////
        // TEST 1
        /////////////////////////////////////////////////////////////////////////////
        
        .DB "01>"               ; @BT TEST-01 01 START
                                ; @BT 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D
                                ; @BT END        
        .SECTION "SectionB" FREE APPENDTO "SectionA" PRIORITY 8
label_B:.DB 3, 4, 5, 6
        .ENDS

        .SECTION "SectionD" FREE APPENDTO "SectionA" PRIORITY 6
label_D:.DB 9, VALUE_10, VALUE_10 + 1, $C, $D
        .ENDS
        
        .SECTION "SectionA" FREE PRIORITY 9
label_A:.DB 0, 1, 2
        .ENDS

        .SECTION "SectionC" FREE APPENDTO "SectionA" PRIORITY 7
label_C .DB 7, 8
        .ENDS

        .DB "<01"

        /////////////////////////////////////////////////////////////////////////////
        // TEST 2
        /////////////////////////////////////////////////////////////////////////////

        .org $100
        
        .section "S02.01" FORCE PRIORITY 100
        .db "02>"               ; @BT TEST-02 02 START
        .db 0, 1                ; @BT 00 01 02 03 04 05 06 07 08 09
        .ends                   ; @BT END

        .section "S02.03" FREE APPENDTO "S02.01" PRIORITY 80
        .db 4, 5
        .ends

        /////////////////////////////////////////////////////////////////////////////
        // TEST 3
        /////////////////////////////////////////////////////////////////////////////

        .org $200
                                ; @BT TEST-03 03 START
                                ; @BT 09 08 00 07 06 05 04 03 02 00 01 00
                                ; @BT END

        .section "S03.05" APPENDTO "S03.04"
        .db VALUE_10-9, VALUE_10-10
        .db "<03"
        .ends

        .section "S03.03" APPENDTO "S03.02"
        .db VALUE_10-5, VALUE_10-6
        .ends

        .section "S03.01" KEEP
        .db "03>"
        .db VALUE_10-1, VALUE_10-2, 0
        .ends

        .section "S03.04" APPENDTO "S03.03"
        .db VALUE_10-7, VALUE_10-8, 0
        .ends

        .section "S03.02" APPENDTO "S03.01"
        .db VALUE_10-3, VALUE_10-4
        .ends
