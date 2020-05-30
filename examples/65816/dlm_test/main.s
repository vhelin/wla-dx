
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

.macro macro_increment
.redefine _out \1+1
.endm
	
.bank 0 slot 0
.section "Section1" force
	.DB "01>"
	.dbm macro_increment 1, 2, 3, FOUR
	.DB "<01"
	.DB "02>"
	.dwm macro_increment 1, 2, 3, FOUR
	.DB "<02"
	.DB "03>"
	.dlm macro_increment 1, 2, 3, FOUR
	.DB "<03"
.ends

