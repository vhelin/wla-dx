
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing wla-z80's sms functionality
; written by ville helin <vhelin@cc.hut.fi> in 2015
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.define MINUS_THREE -3 export
.define MINUS_ONE = -1 EXPORT

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

.RAMSECTION "Variables-1" SLOT 3
TEST_RESULT_1                     DB
.ENDS

.RAMSECTION "Variables-2" SLOT 3
TEST_RESULT_2                     DB
.ENDS

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

.EMPTYFILL $AA

.RAMSECTION "RamSection 0 bank $7f slot 5 YES REF" BANK $7f SLOT 5
rs0b127_a	dw
rs0b127_b	dw
rs0b127_c	dw
.ENDS

.RAMSECTION "RamSection 1 bank 0 slot 3 NO REF" BANK 0 SLOT 3 ORGA $C002 FORCE
rs1b0_a	dw
rs1b0_b	db
.ENDS

.RAMSECTION "RamSection 2 bank 2 slot 3 NO REF" BANK 2 SLOT 3 ALIGN 8
rs2b2_a	dw
rs2b2_b	db
.ENDS
	
.RAMSECTION "RamSection 3 bank 0 slot 3 YES REF" BANK 0 SLOT 3 ORGA $C000 ALIGN 8
rs3b0_all:	.dsb 1+2+1
rs3b0_a	db
rs3b0_b	dw
rs3b0_c	db
rs3b0_next .dsw 32
rs3b0_d dsw 16
rs3b0_e dsw 16
.ENDS

.RAMSECTION "RamSection 4 bank $7f slot 5 YES REF" BANK $7f SLOT 5 ALIGN 16
rs4b127_a	db
.		db
.		dw
rs4b127_c	db
rs4b127_d dsb 16-1-2-1
.ENDS

.RAMSECTION "RamSection 5 bank 0 slot 5 NO REF" BANK 0 SLOT 5 ALIGN 8
rs5b0_a	db
rs5b0_b	db
.ENDS

.RAMSECTION "RamSection 6 bank 0 slot 5 NO REF" BANK 0 SLOT 5 ALIGN 16
rs6b0_a	db
rs6b0_b	db
.ENDS

.RAMSECTION "RamSection 7 bank $7f slot 5 YES REF" BANK $7f SLOT 5 ALIGN 16
rs7b127_a	db
rs7b127_b	dw
rs7b127_c	db
.ENDS

.RAMSECTION "RamSection 8 bank 1 slot 5 NO REF" BANK 1 SLOT 5 ALIGN 8
rs8b1_a	db
rs8b1_b	db
.ENDS

.RAMSECTION "RamSection 9 bank 1 slot 5 NO REF" BANK 1 SLOT 5 ALIGN 16
rs9b1_a	db
rs9b1_b	db
.ENDS


.enum $100
enum_all: .dsb 57
enum_1  db
enum_2  .db
enum_3  .dw
enum_4  .word
enum_5  .addr
enum_6  .byt
enum_7  .byte
enum_8  .ds 5
enum_9  .dsb 10
enum_a  .dsw 16
enum_b:	dw
enum_c  db
.ende

.export enum_all, enum_1, enum_2, enum_3, enum_4, enum_5, enum_6, enum_7, enum_8, enum_9, enum_a, enum_b, enum_c


.struct point
x:	db
y:	db
.endst

.ramsection "Vars1" slot 0 priority 1
points1   instanceof point 1
otherVar1 db
.ends

.ramsection "Vars2" slot 0
points2   instanceof point
otherVar2 db
.ends

.ramsection "Vars3" slot 0
points3   instanceof point 2
otherVar3 db
.ends

.enum $0 export
vertices1 instanceof point 1
.ende

.enum $0 export
vertices2 instanceof point
.ende

.enum $0 export
vertices3 instanceof point 2
.ende


.MACRO TryCrashWLA
.DEFINE Index \@ export
.ENDM
        TryCrashWLA


.BLOCK "Hello"
.db "01>"
.db 1, 2, MINUS_THREE * MINUS_ONE, 4, 5, MINUS_ONE, MINUS_THREE
.db "<01"
.BLOCK "Hi!"
.db 6, 7, 8
.dw otherVar1, points2, otherVar3, library_hook, rs3b0_a, rs4b127_c, rs7b127_b, rs0b127_c, TEST_RESULT_2
.endb
.endb

.ENUM $FFF0 EXPORT
scroll_x  DB
scroll_y  DB
player_x: DW
player_y: DW
.ENDE
