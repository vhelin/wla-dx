/*

	*/
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example showing the wla z80 syntax
; written by ville helin <vhelin@cc.hut.fi> in 1998-2002
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INCLUDE defines.i

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a dummy macro
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.macro more
.db \1+1
.endm

.macro forward
  more \1+1
.endm

.MACRO JESUS
\._\@:
.DB \1, " ", \2, 0
.ENDM

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; structs with long names
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.STRUCT demo
    ABCDEFGHIJKLMNO  DB
    ABCDEFGHIJKLMNOP DB
.ENDST

.STRUCT ABCDEFGHIJKLMN
    abcdefghijklmnopqrstuvwxyzabcdef INSTANCEOF demo
.ENDST

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.DEFINE SKELETOR $10*2


.BANK 0 SLOT 0
.ORG 0

.section "kukkuu" force
lab1:
.db $ff, $ff, $ff, $fa
.ends

.section "hallo" force
lab2:
.db $ff, $ff, $ff, $fb
.ends

.section "halls" force returnorg
lab3:
.db $ff, $ff, $ff, $fc
.ends
/*
  forward 1+1+_camilla
_camilla
  JESUS "evil", "emperor"
*/

.section "structTest"
	ld (iy + ABCDEFGHIJKLMN.abcdefghijklmnopqrstuvwxyzabcdef.ABCDEFGHIJKLMNO), $00
	ld (iy + ABCDEFGHIJKLMN.abcdefghijklmnopqrstuvwxyzabcdef.ABCDEFGHIJKLMNOP), $00

.STRUCT MyStruct
    ByteVal DB
    WordVal DW	
.ENDST
	
	.db "01>"
	
.DSTRUCT defStruct INSTANCEOF MyStruct VALUES
    ByteVal .DB -1
    WordVal .DW -1	
.ENDST

	.db "<01"
	
.ends

.section "HELLO"
.db "START >"
	BIT	0, A
	IM	0
.db "< END"
.ends


.SECTION "Action" SEMIFREE
.DB	"AS"

MORE_ACTION:
	LD	A, 'c'
	EXX			; ?
	NOP			/* just testing */
	DEC	A
	JR	NC, MORE_ACTION
	LD	(1234), ix
	JP	MORE_ACTION
	RST
	RST	$10
	RST	SKELETOR
	ld	(MORE_ACTION), iy
	LD	BC, ((((TILESEND-TILES)/8)+1) << 8) | $BE
	LD	IX,MAIN + 12 * 2
	ld	($d980), ix
	ld	(SKELETOR + 100 - MORE_ACTION), iy
	sub	(iy +   SKELETOR+ 10)
	sub	(ix  +SKELETOR+10)
	ld	($c230), ix
        RET
.DB	"AE"
.ENDS


.SECTION "Beginning" FORCE
.DB	"BS"

MAIN:	NOP
	LD	(MAIN+3), HL
	LD	HL, MAIN+3
	LD	(MAIN+3), DE
	LD	HL, MAIN+3
	JP	MORE_ACTION
	RST

.DB	"BE"
.ENDS


.SECTION "Copier" SIZE 1000
.DB	"CS"

grid_put_sprite:
	PUSH	HL
	POP	IX
	SRL	D
TILES:	
	RRA
	AND	$80
	OR	E
	LD	E, A
	LD	HL, $FC00
TILESEND:	
	ADD	HL, DE
	LD	B, 8
	LD	DE, 16
_loop:	
/*	LD	A, (IX + 0) */
	LD	(HL), A
	INC	IX
	ADD	HL, DE
	DJNZ	_loop
	RET

.DB	"CE"
.ENDS


.section "BANKHEADER_NOT_REALLY" SIZE $20
.db	"HS"

.dw MAIN, MORE_ACTION, grid_put_sprite
.dw $FE00

	LD	(MAIN+3), HL
	LD	HL, MAIN+3
	LD	(MAIN+3), DE
	LD	HL, MAIN+3
	RST
	RST	$00
	LD	H, res 5, (   ix     +   100)

	JESUS "accept", "rules ok"
	JESUS "deny", "disobey"
	JESUS "idle", "whatever"
----
	jp	----
.db	"HE" , 1    ,     2,3,4,5,    9
.ends


.org $1000
.section "LAYER 0" overwrite

.db "START >"
.db $00
.dw $00
.db "< END"

.ends

.org $1000
.section "LAYER 1" overwrite

.db "START >"
.db $01
.dw $01
.db "< END"

.ends

.org $1000
.section "LAYER 2" overwrite

.db "START >"
.db $02
.dw $02
.db "< END"

.ends

.org $1200
.section "table1" force

.db "<TABLE1>"
.table byte, word, byte, word, byte, BYTE
.row $01, $0302, $04, $0605, $07, $08
.row "a", "bc", "d", "ef", "g", "h"
.db "</TABLE1>"

.ends

.org $1300
.section "table2" force

.TABLE DSB 16
.db "<TABLE2>"
@tile: .ROW $10 $14 $14 $18 $35 $34 $2C $39 $21 $20 $1E $09 $04 $1E $10 $3F
@sprite:.ROW $00 $20 $35 $2E $29 $3A $00 $3F $14 $29 $3A $14 $3E $3A $19 $25
.db "</TABLE2>"

.ends

.define DEF_0 4
.define DEF_1 4 EXPORT
.define DEF_2 DEF_1+4 EXPORT
.define DEF_3 = DEF_2+4 EXPORT

.print "Numbers from 0 to 2: ", dec 0, 1, dec 2, "\n"
.print "Nothing special here\n"
.print "$" hex $100 " $" hex $200 "\n"
.PRINT "Numbers 1 and 2: ", DEC 1, " ", DEC 2, "\n"
.print "Bye\n"

.printt "OLD .PRINTT "
.printv 100
.printt "\n"

.ENUM $0000 DESC EXPORT
ENUM_OK_123 dw
.IFDEF DEBUG
ENUM_NOT_OK_123 dw
.ENDIF
.ENDE
	
.ENUMID 0
.ENUMID SONG_1
.ENUMID SONG_2
.ENUMID SONG_3
.ENUMID 0 EXPORT
.ENUMID ID_1
.ENUMID ID_2
.ENUMID ID_3
.ENUMID 4 STEP 2 EXPORT
.ENUMID MONSTER_ID_1
.ENUMID MONSTER_ID_2
.ENUMID MONSTER_ID_3
