
.include "cgb_hardware.i"

.MEMORYMAP
SLOTSIZE $4000
DEFAULTSLOT 1
SLOT 0 $0000
SLOT 1 $4000
SLOTSIZE $2000
SLOT 2 $c000
.ENDME

.ROMBANKSIZE $4000
.ROMBANKS 2

.emptyfill $00

.ORGA $150

Start:
	.db "01>"
	.db $01 :Start $02 :Start+3
	.db $01, :Start, $02, :Start+3
	.db $01 | (:Start)
	.db $01 | :Start
	.db $00 + :(Start + $4000)
	.db "<01"

