
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small test for wla-6809
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0       $0000
   SLOT            1       $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $1000
   SLOT            3       $D000
   SLOT            4       $E000
   SLOT            5       $F000
.ENDME

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

.EMPTYFILL $AA

.BANK 0 SLOT 0
.ORG 0

.SECTION "MAIN" FORCE
.DW LibraryMain

Main:	

.ENDS

.ORG $1000
.SECTION "EXTRAS" FORCE
.DB "AA>"
ExtrasStart:
	/*
	BLE ExtrasStart+1
	BLS ExtrasStart+1
	BLT ExtrasEnd+1
	BMI ExtrasEnd+1
	*/
ExtrasEnd:
.DB "<AA"
.DB " "
.DB "BB>"
	/*
	JMP ExtrasStart.w
	JMP.W ExtrasStart
	*/
.DB "<BB"
.ENDS
