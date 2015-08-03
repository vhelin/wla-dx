
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing wla-z80's sms functionality
; written by ville helin <vhelin@cc.hut.fi> in 2015
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0       $0000
   SLOT            1       $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $2000
   SLOT            3       $C000
   SLOT            4       $E000
   SLOT            5       $FF01
.ENDME

.RAMSECTION "Variables" SLOT 3
TEST_RESULT                     DB
.ENDS

.ROMBANKMAP
BANKSTOTAL 2
BANKSIZE $4000
BANKS 2
.ENDRO

.EMPTYFILL $AA
	
.RAMSECTION "RamSection 1" BANK 0 SLOT 3 ALIGN 8
rs1_a	dw
rs1_b	db
.ENDS

.RAMSECTION "RamSection 2" BANK 0 SLOT 3 ALIGN 8
rs2_a	db
rs2_b	dw
rs2_c	db
.ENDS

.RAMSECTION "RamSection 3" BANK 0 SLOT 5 ALIGN 16
rs3_a	db
rs3_b	dw
rs3_c	db
.ENDS

.RAMSECTION "RamSection 4" BANK 0 SLOT 5 ALIGN 8
rs4_a	db
rs4_b	db
.ENDS

.RAMSECTION "RamSection 5" BANK 0 SLOT 5 ALIGN 16
rs5_a	db
rs5_b	db
.ENDS
