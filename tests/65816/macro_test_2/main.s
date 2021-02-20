
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

.macro FILTER_MACRO_B
.redefine _out \1 & $FF
.endm

.macro FILTER_MACRO_W
.redefine _out = \1 & $FFFF
.endm

.macro FILTER_MACRO_L
.redefine _out \1 & $FFFFFF
.endm

.macro FILTER_MACRO_2
.redefine _out \2
.endm

.bank 0 slot 1

.section "Bank0" force
main:
	.db "01>"
        .dbm FILTER_MACRO_B, $123456, $ABCDEF, "0123456789"
	.db "<01"

	.db "02>"
        .dwm FILTER_MACRO_W, $123456, $ABCDEF, "0123456789"
	.db "<02"
	
	.db "03>"
        .dlm FILTER_MACRO_L, $123456, $ABCDEF, "0123456789"
	.db "<03"

        .db "04>"
        .dbm FILTER_MACRO_2, $123456, $ABCDEF, "01234"
	.db "<04"

        .db "05>"
        .dwm FILTER_MACRO_2, $123456, $ABCDEF, "01234"
	.db "<05"

        .db "06>"
        .dlm FILTER_MACRO_2, $123456, $ABCDEF, "01234"
	.db "<06"

.ends
