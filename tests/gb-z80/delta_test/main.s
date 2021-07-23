
; testing label delta calculation

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
	
start:	nop
	or a,a
	pop af
	ld sp,test1
startend:

.section "Section1" keep
	.db 0, 1, 2, 3
test1:
	.db test1 & $ff, 5, 6
test1end:
	.db "01>"		; @BT TEST-01 01 START
	.dsb test1end-test1, 0	; @BT 00 00 00
	.db "<01"		; @BT END
.ends

.section "Section2" keep
test3:	ld h,e
	ld h,100
	ld bc,test3
test3end:
	.db "02>"		; @BT TEST-02 02 START
	.dsb test1end-test1 + test3end-test3, 1 ; @BT 01 01 01 01 01 01 01 01 01
	.db "<02"		; @BT END
.ends

.section "Section3" keep
test2:
	.db 7, 8, 9, (test1 & $ff) + 6, 11, 12, 13, 14
test2end:
	.db "03>"		; @BT TEST-03 03 START
	.dsb test2end-test2 - (test3end-test3), 2 ; @BT 02 02
	.db "<03"		; @BT END
.ends

.section "Section4" keep
	.db "04>"		; @BT TEST-04 04 START
	.db test4end-test4, startend-start ; @BT 06 06
	.db "<04"		; @BT END
.ends
	
.section "Section5" keep
test4:	.dw test1, test2, test3
test4end:
.ends
	
	.db "05>"		; @BT TEST-05 05 START
	.dsb startend-start, 3	; @BT 03 03 03 03 03 03
	.db "<05"		; @BT END
