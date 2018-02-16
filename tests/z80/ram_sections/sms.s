
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
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

.EMPTYFILL $AA
	
.RAMSECTION "RamSection 1 bank 0" BANK 0 SLOT 3 ALIGN 8
rs1b0_a	dw
rs1b0_b	db
.ENDS

.RAMSECTION "RamSection 1 bank 2" BANK 2 SLOT 3 ALIGN 8
rs1b2_a	dw
rs1b2_b	db
.ENDS
	
.RAMSECTION "RamSection 2 bank 0" BANK 0 SLOT 3 ALIGN 8
rs2b0_a	db
rs2b0_b	dw
rs2b0_c	db
.ENDS

.RAMSECTION "RamSection 3 bank 0" BANK 0 SLOT 5 ALIGN 16
rs3b0_a	db
rs3b0_b	dw
rs3b0_c	db
.ENDS

.RAMSECTION "RamSection 4 bank 0" BANK 0 SLOT 5 ALIGN 8
rs4b0_a	db
rs4b0_b	db
.ENDS

.RAMSECTION "RamSection 5 bank 0" BANK 0 SLOT 5 ALIGN 16
rs5b0_a	db
rs5b0_b	db
.ENDS

.RAMSECTION "RamSection 3 bank 1" BANK 1 SLOT 5 ALIGN 16
rs3b1_a	db
rs3b1_b	dw
rs3b1_c	db
.ENDS

.RAMSECTION "RamSection 4 bank 1" BANK 1 SLOT 5 ALIGN 8
rs4b1_a	db
rs4b1_b	db
.ENDS

.RAMSECTION "RamSection 5 bank 1" BANK 1 SLOT 5 ALIGN 16
rs5b1_a	db
rs5b1_b	db
.ENDS



.BLOCK "Hello"
.db 1, 2, 3, 4, 5
.BLOCK "Hi!"
.db 6, 7, 8
.endb
.endb
