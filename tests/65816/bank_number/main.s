
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.lorom

.rombanksize $8000
.rombanks 3

.define CALCULATION_1 :test_data << 12
.define CALCULATION_2 (test_data - $8000) >> 3
.define CALCULATION_3 test_data+1
.define ONE 1

.bank 0 slot 1
.section "Bank0" force returnorg
main:
	.db "01>"
	.dw :test_data << 12
	.dw (test_data - $8000) >> 3
	.dw (:test_data << 12) + ((test_data - $8000) >> 3)
	.dw ((test_data - $8000) >> 3) + (:test_data << 12)
	.db "<01"
.ends

.bank 2 slot 1
.section "Bank2" force returnorg
	.dsb $818, $00
test_data:
	.db $FF

	.db "02>"
	.dw CALCULATION_1 + CALCULATION_2
	.dw CALCULATION_2 + CALCULATION_1
	.db "<02"

	.db "03>"
	.db DIVME/HERE+ONE
	.db "<03"

	.db "04>"
	.db :CALCULATION_3
	.dw CALCULATION_3+1
	.db "<04"
.ends

.base $20	

.ramsection "LowRAM" bank $7E slot 0 returnorg
    test_var db
.ends

.bank 1 slot 1
.section "Bank1" force returnorg
more_data:
	.db "05>"
	.db (:more_data)+1
	.db 1+(:(more_data+1))
	.db ((:more_data)+1) + (1+(:(more_data+1))) + (more_data - $8000)
	.db "<05"
	
	.db "06>"
	.db :test_var
	.db :(test_var + $8000)
	.db :test_var + (more_data - $8000)
	.db "<06"
.ends
