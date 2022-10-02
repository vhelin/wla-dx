
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0       $0000      "SlotZero"
   SLOT            1       $4000      "SlotOne"
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $1000
   SLOT            3       $D000      "RAM 1"
   SLOT            4       $E000      "RAM 2"
   SLOT            5       $F000      "RAM 3"
.ENDME

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

	.BANK 0 SLOT 0
	.ORG 0
	
.SECTION "EXTRAS 1"
ExtrasMain:
	PCHL
	RAR
	HLT
.DW var2
.ENDS

.RAMSECTION "VARS"
var1 db
var2 db
var3 db
.ENDS
