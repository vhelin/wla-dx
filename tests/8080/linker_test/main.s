
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

        ; @BT linked.rom
        
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
.DB "AA>"                       ; @BT TEST-AA AA START
ExtrasStart:
	jz ExtrasStart          ; @BT CA 03 10
	jm ExtrasStart          ; @BT FA 03 10
	cz ExtrasEnd            ; @BT CC 0F 10
	cm ExtrasEnd            ; @BT FC 0F 10
ExtrasEnd:
.DB "<AA"                       ; @BT END
.DB " "
.DB "BB>"                       ; @BT TEST-BB BB START
	call ExtrasStart        ; @BT CD 03 10
	cpe ExtrasStart         ; @BT EC 03 10
.DB "<BB"                       ; @BT END
.DB "GG>"                       ; @BT TEST-GG GG START
---     nop                     ; @BT 00
 --     .db -1-1                ; @BT FE
 -:	.db -1-1                ; @BT FE
	jnz -                   ; @BT C2 24 10
	jnz --                  ; @BT C2 23 10
	jnz ---                 ; @BT C2 22 10
.DB "<GG"                       ; @BT END
.DB "HH>"                       ; @BT TEST-HH HH START
	.dw _f                  ; @BT 36 10
 __	.db 0                   ; @BT 00
	.dw _b                  ; @BT 36 10
.DB "<HH"                       ; @BT END
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

.DB "CC>"                       ; @BT TEST-CC CC START
JUMP_MACRO $f                   ; @BT 00 01 02 03 04 05 06 07 08 09 0A 0B 0F
JUMP_MACRO $f                   ; @BT 00 01 02 03 04 05 06 07 08 09 0A 0B 00 01 02 03 04 05 06 07 08 09 0A 0B 0F
.DB "<CC"                       ; @BT END

LABEL	.print "THE END\n"

.ENDS

	.define STR_SPACE "space"

	.include "name", STR_SPACE ".s" once namespace "oops"
	.dw oops.NamespaceMain, oops.NamespaceBonus

	.define INCLUDE "include"
	.define LU "lu"
	
	.DB "10>"                   ; @BT TEST-10 10 START
	.include "{INCLUDE}3.s"     ; @BT 08
	.include { "{INCLUDE}3.s" } ; @BT 07
	.include { "inc{LU}de3.s" } ; @BT 07
	.db "<10"                   ; @BT END
	
.DB "DD>"                       ; @BT TEST-DD DD START
 TEST 3                         ; @BT 01 03
.DB "<DD"                       ; @BT END

.DB "EE>"                       ; @BT TEST-EE EE START
	MACRO_INCLUDE_2         ; @BT 07 08
.DB "<EE"                       ; @BT END

.DB "FF>"                       ; @BT TEST-FF FF START
	MACRO_INCLUDE_3         ; @BT 01 02 03
	MACRO_INCLUDE_3         ; @BT 01 02 03
.DB "<FF"                       ; @BT END
	
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

        .print "LENGTH\n"
        .REPEAT 6-\1.length
          .DB "2"
        .ENDR

        .print "JOO!\n"
        .define joo = 2
        .while joo+\1.length > 0
          .db "1"
          .redefine joo = joo-1
        .endr
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
