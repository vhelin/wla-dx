
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small test for wla-8080
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

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

.EMPTYFILL $AA

.BANK 0 SLOT 0
.ORG 0

.SECTION "FREEBIRD" ALIGN 256 OFFSET 32 FREE KEEP
FreeBirdMain:	nop
.ENDS

.SLOT 1
.SLOT $4000
.SLOT "SlotOne"
.SLOT 0

.SECTION "MAIN" FORCE
.DW $10
.DW LibraryMain
.DW ExtrasMain
.DB $11
Main:	nop
	jmp Main
.DB $10
.DW DummyMain, DummyMain2
.ENDS

.ORG $1000
.SECTION "EXTRAS" FORCE
.DB "AA>"
ExtrasStart:
	jz ExtrasStart
	jm ExtrasStart
	cz ExtrasEnd
	cm ExtrasEnd
ExtrasEnd:
.DB "<AA"
.DB " "
.DB "BB>"
	call ExtrasStart
	cpe ExtrasStart
.DB "<BB"
.DB "GG>"
---     nop
 --     .db -1-1
 -:	.db -1-1
	jnz -
	jnz --
	jnz ---
.DB "<GG"
.DB "HH>"
	.dw _f
 __	.db 0
	.dw _b
.DB "<HH"
.ENDS

.ORG $2000
.SECTION "DUMMY" FREE
DummyMain:
	nop
	nop
	nop
.ENDS

.MACRO JUMP_MACRO
	.print "OUTMOST:::IN\n"
        MACRO_INCLUDE_1 11 \1
	.print "OUTMOST:::OUT\n"
.ENDM

.MACRO MACRO_INCLUDE_1
	.print "INSIDE:::IN\n"
	.include include1.s
	.db \2
	.print "INSIDE:::OUT\n"
.ENDM

.MACRO MACRO_INCLUDE_2
	.INCLUDE "include3.s"
	.INCLUDE "include4.s"
.ENDM

.MACRO MACRO_INCLUDE_3
	.INCLUDE "include5.s" ONCE
.ENDM

.ORG $2000
.SECTION "DUMMY2" FREE
DummyMain2:
	nop
	nop
	nop

.include "include2.s"

.DB "CC>"
JUMP_MACRO $f
JUMP_MACRO $f
.DB "<CC"

LABEL	.print "THE END\n"

.ENDS	

	.include "name", "space" ".s" namespace "oops"
	.dw hello.NamespaceMain, oops.NamespaceBonus
	
.DB "DD>"
 TEST 3
.DB "<DD"

.DB "EE>"
	MACRO_INCLUDE_2
.DB "<EE"

.DB "FF>"
	MACRO_INCLUDE_3
	MACRO_INCLUDE_3
.DB "<FF"
	
.SECTION "DUMMY3" FREE
DummyMain3:
	nop
	nop
	nop
.ENDS

SHORT_STRING = "one"

.MACRO DEFINE_BYTES
	.DB \1
	.printt "This should be 3: "
	.printv dec SHORT_STRING.length
	.printt "\n"
	.printt "This should be 4: "
	.printv dec "moto".length
	.printt "\n"
	.printt "This should be 11: "
	.printv dec 16-\1.length
	.print "\n"
.REPEAT 16-\1.length
    .DB " "
.ENDR
.ENDM

.section "DUMMY4" FREE KEEP
	DEFINE_BYTES "hello"
.ends

.org $20
.ramsection "ramsection1" bank 1 slot "RAM 2" SEMIFREE KEEP
RAMVAR1	db
.ends

.org $20
.ramsection "ramsection2" bank 1 slot "RAM 2" FREE KEEP
 RAMVAR2 db
.ends

.org $20
.ramsection "ramsection3" bank 1 slot "RAM 2" SEMISUBFREE KEEP
 RAMVAR3: db

.ends

	;; STRUCT & RAMSECTION TEST
	
.STRUCT enemy
id      db
y       db
x       db
.ENDST
	
.macro makeRAMSECTIONs
.ramsection "enemy \@" align 256 offset 32 free priority 1000-\@ keep
enemies.\@ INSTANCEOF enemy
.ends
.endm

	.rept 10
	makeRAMSECTIONs
	.endr

	;; STRUCT TEST

.struct enemy2
    HP                      db
    posx                    db
    posy                    db
.endst

.ramsection "enemies" slot 3 keep
  enemies2 instanceof enemy2 3 startfrom 0
.ends
