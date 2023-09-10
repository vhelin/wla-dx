
; testing label value calculation in wla-gb (the assembler, not the linker)

.MEMORYMAP
DEFAULTSLOT 1
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $2000
SLOT 2 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 3
BANKSIZE $2000
BANKS 3
.ENDRO

.EMPTYFILL $ff
        
.BANK 1 SLOT 1
.ORG 0

; @BT linked.gb

start:  nop
        or a,a
        pop af
        ld sp,startend

        .define POSTPONED_TO_LINKER = startend+1
        .define POSTPONED_TO_LINKER2 = FORCE_start+1
        .define POSTPONED_TO_LINKER3 = POSTPONED_TO_LINKER2+$2000
        .define POSTPONED_TO_LINKER4 = POSTPONED_TO_LINKER
        .define POSTPONED_TO_LINKER5 = POSTPONED_TO_LINKER+1
	.define STARTEND_PLUS_1 POSTPONED_TO_LINKER
	.define STARTEND_PLUS_2 POSTPONED_TO_LINKER+1
	
startend:
        
        .db "00>"                ; @BT TEST-00 00 START
        .dw POSTPONED_TO_LINKER  ; @BT 07 20
        .db :POSTPONED_TO_LINKER ; @BT 01
        .db bank(POSTPONED_TO_LINKER) ; @BT 01
        .dw POSTPONED_TO_LINKER4  ; @BT 07 20
        .db :POSTPONED_TO_LINKER4 ; @BT 01
        .db bank(POSTPONED_TO_LINKER4) ; @BT 01
        .dw POSTPONED_TO_LINKER5  ; @BT 08 20
        .db :POSTPONED_TO_LINKER5 ; @BT 01
        .db bank(POSTPONED_TO_LINKER5) ; @BT 01
        .db "<00"                ; @BT END
        
        .db "01>"               ; @BT TEST-01 01 START
        .db :startend + 1       ; @BT 02
        .db "<01"               ; @BT END

        .assert startend == $2006
        .assert startend - 1 == $2005
        .assert startend - start == 6
        .assert startend - 1 - start == 5
	.assert STARTEND_PLUS_1 == $2007
	.assert STARTEND_PLUS_1 + 1 == $2008
	.assert STARTEND_PLUS_2 == $2008

	.define STARTEND_LATER = startend+1
	
        .section "FORCE" force slot 2 org $101
FORCE_start:
        .db "02>"               ; @BT TEST-02 02 START
	.db :STARTEND_LATER	; @BT 01
	.dw STARTEND_LATER	; @BT 07 20
	.db :STARTEND_PLUS_2	; @BT 01
	.db STARTEND_PLUS_2 & 8	; @BT 08
        .db :POSTPONED_TO_LINKER2 ; @BT 01
        .db :POSTPONED_TO_LINKER3 ; @BT 02
        .db POSTPONED_TO_LINKER2 & 0 ; @BT 00
        .db POSTPONED_TO_LINKER3 & 0 ; @BT 00
        .db "<02"                 ; @BT END
        .ends

        .assert FORCE_start == $8101

        .section "OVERWRITE" overwrite slot 1 org $200
OVERWRITE_start:
        .db 1
        .ends

        .assert OVERWRITE_start == $2200
        
        .FUNCTION SUM_AB2(var1,var2) var1*1+var2*1
        .function SUM_ABC4(varA,varB,varC) SUM_AB2(SUM_AB2(varA,varB),varC)
        
        .db "15>"               ; @BT TEST-15 15 START
        .db SUM_ABC4(5,6,7)     ; @BT 12
        .db "<15"               ; @BT END

        .db "16>"               ; @BT TEST-16 16 START
        .db SUM_ABC4(5+0,6+0,7+0)     ; @BT 12
        .db "<16"               ; @BT END
