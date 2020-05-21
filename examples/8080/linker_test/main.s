
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
        MACRO_INCLUDE 11 \1
	.print "OUTMOST:::OUT\n"
.ENDM

.MACRO MACRO_INCLUDE
	.print "INSIDE:::IN\n"
	.include include.s
	.db \2
	.print "INSIDE:::OUT\n"
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

	.include "namespace.s" namespace "oops"
	.dw hello.oops.NamespaceMain, oops.NamespaceBonus
	
.DB "DD>"
 TEST 3
.DB "<DD"

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

.section "!DUMMY4" FREE
	DEFINE_BYTES "hello"
.ends
