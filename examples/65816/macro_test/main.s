
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.lorom

.rombanksize $8000
.rombanks 3

.DEF    prev_term       $0000

.MACRO  .term   ARGS    str
__\._\@:
        .WORD   prev_term
        .REDEF  prev_term       CADDR - 2
        .BYTE   str.length, str, 0
.ENDM

.bank 0 slot 1
.section "Bank0" force returnorg
main:
	.db "01>"
        .term "YES"
	.db "<01"

	.db "02>"
	.term "NO"
	.db "<02"
	
	.db "03>"
	.term "HELLO"
	.db "<03"
.ends
