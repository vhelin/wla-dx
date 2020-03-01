
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

.bank 0 slot 0
.section "Section1" force
	.DB "01>"
	AND #10
	AND 10,X
	AND 10
	.DB "<01"

	.WDC
	
	.DB "02>"
	AND #<10
	AND <10,X
	AND <10
	LSR <10
	LSR
	.DB "<02"

	.NOWDC
	
	.DB "03>"
	AND #<$1020
	AND <$1020,X
	AND <$1020
	LSR <$1020
	LSR
	.DB "<03"
.ends

