
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.lorom

.rombanksize $8000
.rombanks 3

.DEF prev_term $0000

.MACRO .term ARGS str
__\._\@:
        .WORD  prev_term
        .REDEF prev_term CADDR - 2
        .BYTE  str.length, str, 0
.ENDM

.DEF prev_test $0000
	
.MACRO .test ARGS str
__\._\@+1:
	.PRINT __\._\@+1, "\n"
        .WORD  prev_test
        .REDEF prev_test __\._\@+1
        .BYTE  str.length, str, 0
.ENDM

.MACRO .storager
        .DB \@, \@+1, 1+\@+1, \@-1
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

	.db "04>"
        .test "JA"
	.db "<04"

	.db "05>"
	.test "NEIN"
	.db "<05"
	
	.db "06>"
	.test "HALLO"
	.db "<06"

	.db "07>"
	.storager
	.db "<07"
.ends
