
	; testing section appending (order by size and by priorities)

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

	.BANK 0 SLOT 0
	.ORGA 0

	; @BT linked.gb

        ; --------------------------------------------------------------------------------
	; 01
        ; --------------------------------------------------------------------------------
	
	.SECTION "ROOT-01" KEEP
				; @BT TEST-01 01 START
	.db "01>"
	.db 0			; @BT 00 01 02 03 04 05 06 07
				; @BT END
	.ENDS

	.SECTION "ADD-01-2" APPENDTO "ROOT-01"
	.db 7
	.db "<01"
	.ENDS

	.SECTION "ADD-01-1" APPENDTO "ROOT-01"
	.db 1, 2, 3, 4, 5, 6
	.ENDS

        ; --------------------------------------------------------------------------------
        ; 02
        ; --------------------------------------------------------------------------------
        
	.SECTION "ROOT-02" KEEP
				; @BT TEST-02 02 START
	.db "02>"
	.db 0			; @BT 00 01 02 03 04 05 06 07
				; @BT END
	.ENDS

	.SECTION "ADD-02-1" APPENDTO "ROOT-02"
	.db 1, 2, 3, 4, 5, 6
	.ENDS

	.SECTION "ADD-02-2" APPENDTO "ROOT-02"
	.db 7
	.db "<02"
	.ENDS
	
        ; --------------------------------------------------------------------------------
        ; 03
        ; --------------------------------------------------------------------------------
	
	.SECTION "ROOT-03" KEEP
				; @BT TEST-03 03 START
	.db "03>"
	.db 0			; @BT 00 07 01 02 03 04 05 06
				; @BT END
	.ENDS

	.SECTION "ADD-03-1" APPENDTO "ROOT-03" PRIORITY 100
	.db 7
	.ENDS

	.SECTION "ADD-03-2" APPENDTO "ROOT-03" PRIORITY 1
	.db 1, 2, 3, 4, 5, 6
	.db "<03"
	.ENDS
	
        ; --------------------------------------------------------------------------------
        ; 04
        ; --------------------------------------------------------------------------------
	
	.SECTION "ROOT-04" KEEP
				; @BT TEST-04 04 START
	.db "04>"
	.db 0			; @BT 00 07 01 02 03 04 05 06
				; @BT END
	.ENDS

	.SECTION "ADD-04-2" APPENDTO "ROOT-04" PRIORITY 1
	.db 1, 2, 3, 4, 5, 6
	.db "<04"
	.ENDS

	.SECTION "ADD-04-1" APPENDTO "ROOT-04" PRIORITY 100
	.db 7
	.ENDS

        ; --------------------------------------------------------------------------------
	; 05
        ; --------------------------------------------------------------------------------
	
        .SECTION "Section1" RETURNORG KEEP
section1:
        .DB "05>"               ; @BT TEST-05 05 START
        .DB 0, 1                ; @BT 00 01 02 03 04 05 06 07 08 09
        .ENDS

        .SECTION "Section2" RETURNORG KEEP APPENDTO "Section1" AUTOPRIORITY
section2:
        .db 2
        .ENDS

        .SECTION "Section3" RETURNORG KEEP APPENDTO "Section1" AUTOPRIORITY
section3:
        .db 3, 4, 5
        .ENDS

        .SECTION "Section4" RETURNORG KEEP APPENDTO "Section1" AUTOPRIORITY
section4:
        .db 6, 7, 8, 9
        .db "<05"               ; @BT END
        .ENDS
