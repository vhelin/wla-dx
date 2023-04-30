
	.MEMORYMAP
	DEFAULTSLOT 1
	SLOT 0 $0000 $2000
	SLOT 1 STArT $2000 sIzE $6000
	.ENDME

	.ROMBANKMAP
	BANKSTOTAL 2
	BANKSIZE $2000
	BANKS 1
	BANKSIZE $6000
	BANKS 1
	.ENDRO

        .EMPTYFILL $FF

	.BANK 0 SLOT 0
	.ORGA 0

	; @BT linked.gb

        ; --------------------------------------------------------------------------------
	; 01
        ; --------------------------------------------------------------------------------
	
        .db "01>"               ; @BT TEST-01 01 START
                                ; @BT 00 01 02 03 04
        .ORGA 8+3               ; @BT END

        .db "02>"

        .ORGA 8+3+3+8
        
        .dsb $2000-11-3-8, $AA

	.SECTION "FREE-01" FREE KEEP ; @BT TEST-02 02 START
        .db 5, 6, 7, 8, 9            ; @BT 05 06 07 08 09
        .db "<02"                    ; @BT END
	.ENDS

        .SECTION "OVERWRITE-01" OVERWRITE ORGA $0003
        .db 0, 1, 2, 3, 4
        .db "<01"
        .ENDS

        .BANK 1 SLOT 1
        .ORG 0
        
        .SECTION "FORCE-01" FORCE
        .db "03>"               ; @BT TEST-03 03 START
        .db 0, 1, 2, 3          ; @BT 00 01 02 03
        .db "<03"               ; @BT END
        .ENDS

        .SECTION "SEMISUPERFREE-01" SEMISUPERFREE BANKS 1
        .db "04>"               ; @BT TEST-04 04 START
        .db 0, 1, 2, 3          ; @BT 00 01 02 03
        .db "<04"               ; @BT END
        .ENDS

        .ORG $0100
        
        .SECTION "SEMISUBFREE-01" SEMISUBFREE
        .db "05>"               ; @BT TEST-05 05 START
        .db 0, 1, 2, 3          ; @BT 00 01 02 03
        .db "<05"               ; @BT END
        .ENDS

        .SECTION "SEMIFREE-01" SEMIFREE
        .db "06>"               ; @BT TEST-06 06 START
        .db 0, 1, 2, 3          ; @BT 00 01 02 03
        .db "<06"               ; @BT END
        .ENDS

        .SECTION "SUPERFREE-01" SUPERFREE
        .db "07>"               ; @BT TEST-07 07 START
        .db 0, 1, 2, 3          ; @BT 00 01 02 03
        .db "<07"               ; @BT END
        .ENDS
        
