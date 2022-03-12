
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example testing WINDOW & BITWINDOW in .SECTIONs
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .MEMORYMAP
        DEFAULTSLOT 0
        SLOTSIZE $2000
        SLOT 0 $0000
        SLOT 1 $2000
        SLOT 2 $4000
        SLOT 3 $6000
        .ENDME

        .ROMBANKMAP
        BANKSTOTAL 2
        BANKSIZE $2000
        BANKS 2
        .ENDRO

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; tests
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .BANK 0 SLOT 0
        .ORG $0000

        .SECTION "WindowTest2" FREE WINDOW $100 $200 BITWINDOW 3 PRIORITY 90
a_0x118:.dsb 8, 8
        .ENDS

        .SECTION "WindowTest3" FREE BITWINDOW 8 PRIORITY 80
a_0x200:.dsb $100, $ff
        .ENDS

        .SECTION "WindowTest1" FREE // WINDOW $100 $200 PRIORITY 100
a_0x109:.dsb 8, 8
        .ENDS

        .ORG $0000
        .SECTION "Blocker1" FORCE
        .dsb 1, 1
        .ENDS

        .ORG $0100
        .SECTION "Blocker2" FORCE
        .dsb 9, 9
        .ENDS
