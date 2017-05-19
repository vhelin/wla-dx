
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

.BANK 0 SLOT 0
.ORGA 0

.section "MASTER"
	.db 0, 1, 2, 3
test1:
	.db test1 & $ff, 5, 6
.ends

.section "EXTRA"
test3:	.db $20, $21, $22
.ends

.section "SLAVE1" appendto "MASTER"
test2:
	.db 7, 8, 9, (test1 & $ff) + 6, 11, 12
.ends

.section "SLAVE2" appendto "MASTER"
	.db 13, 14, 15
	.db test2 + 2 - 3 + 1
.ends
	
