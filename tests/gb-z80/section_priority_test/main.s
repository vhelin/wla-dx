
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

        .SECTION "OVERWRITE-01" OVERWRITE ORGA $0003 PRIORITY 100
        .db 0, 1, 2, 3, 4
        .db "<01"
        .ENDS
