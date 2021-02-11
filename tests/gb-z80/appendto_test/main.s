
; testing section appending

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

.macro Times100 args guuggeli bayonetta
.printt "*** BEGIN *** "
.printv dec guuggeli*100
.printt " "
.printv dec \1*100
.printt " "
.printv dec bayonetta*0.1
.printt " "
.printv dec \2*0.1
.db guuggeli*100, \1*100, bayonetta*0.1, \2*0.1, "HELLO"
.printt " *** END ***\n"
.endm

.BANK 0 SLOT 0
.ORGA 0

.section "MacroTest" force
  Times100 0.1 100
.ends

.section "MASTER"
	.db 0, 1, 2, 3
test1:
	.db test1 & $ff, 5, 6
.ends

.section "EXTRA"
test3:	.db $20, $21, $22
test3end:
.ends

.section "SLAVE1" appendto "MASTER"
test2:
	.db 7, 8, 9, (test1 & $ff) + 6, 11, 12
test2end:
.ends

.section "SLAVE2" appendto "MASTER"
	.db 13, 14, 15
	.db test2 + 2 - 3 + 1
	.db $ff, $ff, $ff
	ld hl, _sizeof_label
	.db $ff, $ff, $ff
.ends
	
.section "SIZEOFs"
label:	.dw test1, test2, test3
label_END:	
.ends
	
