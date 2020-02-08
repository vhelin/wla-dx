
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing wla-z80's smsheader functionality
; written by ville helin <vhelin@cc.hut.fi> in 2017
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
.ENDME

.RAMSECTION "Variables1" SLOT 3
RESULT_1        DB
NOTHING_1	DB	
NOTHING_2       DB
.ENDS

.RAMSECTION "Variables2" SLOT 3
RESULT_2        DB
NOTHING_4	DB	
NOTHING_3       DB
.ENDS

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

.EMPTYFILL $C9

.SMSHEADER
	VERSION 9
	REGIONCODE 6
	PRODUCTCODE 2, 3, 4
	RESERVEDSPACE $ff, $ff
.ENDSMS

.define TEST1
.printt "TEST1 - (default value) "
.printv dec TEST1
.printt "\n"

/* arithm tests */
.define TESM 1.21*4.9999
.define MOOM 1.234
.printt "\n"
.printv dec TESM
.printt " (should be 6)\n"
.printv dec 9.91/3.3*10
.printt " (should be 30)\n"
.printv dec 10/3*10
.printt " (should be 33)\n"
.printv dec 4^3
.printt " (should be 64)\n\n"
.undef TESM


.macro ldbc ; load b and c in one (optimised) operation, but allow b and c to be separated in the code
  ld bc,(\1<<8)|(\2)
.endm

.macro TileAddress ; returns VRAM address of tile n
  ld de, ($4000 + (\1*32))
.endm

.bank 0 slot 0
.org 0

.section "whatever" overwrite
-: jr -

_start:
	.define i 0
	.rept 10
	.redefine i i+1
	.db _start+i
	.endr
	.undefine i

	LD A, (IX+-1)
	ld a, (ix+1.01)
	ld a, (ix-1)
	ld a, (iy+127)
	ld a, (iy-128)
	ld (ix+1), 0
	ld (iy-1), 0

-  .dw caddr, CADDR
  .db $ff, $ff, :caddr, :CADDR, $ff, $ff
  .dw $aaaa, $bbbb, $cccc
  ldbc _dataend-_data,$be
  .dw $ffff
  .dw _data+1
  .dw $ffff
_data:
  .db 1,2,3,4,5
_dataend:
.ends

  TileAddress 1

.section "moo1"
_nams:	.dw _nams+1
.ends

.section "moo2"
_nams:	.dw _nams+1
.ends

.bank 0
.org $500
.section "LocalLabelsTest" semifree

BOOM1:
.db "HELLOMOTO"

	.db _f & $ff
	.db _sizeof_BOOM1
	.dw RESULT_2
OOPS1:
.dw _F
OOPS2:
OOPS3
__				; here it is! :D
OOPS4:
OOPS5
OOPS6
.dw _b
.dw _B
OOPS7:
.dw _b & $ff

.db "BYEMOTO"

BOOM2:
.ends


;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; standard stuff?
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.define issue11 10
.bank issue11-10 slot 6+issue11-4-2-10
.org $300

done:
	jr done
	jr bone
	jr bone+1-1
	jp bone
.dw $ffff
bone:

.BANK 1 SLOT 0
.ORGA $1000

.dw _f+1-1
.db "hello!"

	.db $ff
	.repeat 9 index counter
	.db counter + 1
	.endr
	.db $ff
	.db "MARKER1"
	sub	ixh
	sub	ixl
	.db "MARKER2"

	.repeat 65536*10 index counter
	.endr

__

.BANK 0 SLOT 1
.ORGA $5000

.dw BOOM1
.dw caddr, CADDR+1, caddr+1, CADDR+1, 1+CADDR+2-2
.db $ff, $ff, $ff, $ff

	JP	MAIN
-	jr	-

Notes: .db "I can put multiple strings here to make", " my notes as long as I want.", 0

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

MAIN:	RETN
	LDIR
	LD	A, (MAIN+1-MAIN)
	LD	A, (hello_kitty+1 +2   -    hello_kitty)
	OTIR
	LD	(MAIN    +      3), HL
	LD	(MAIN), HL
	LD	(MAIN), BC
	NOP
__	CALL	_b
--	CALL	--

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; additional garbage
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.ORG $100

hello_kitty:
	NOP
	PUSH	IY
	PUSH	IX
	POP	IX
	POP	IY
	JP	(IX)
	JP	(IY)

	.dw dragon

	ld hl, dragon
	call dragon

.STRUCT mon                ; check out the documentation on
name ds 2                  ; .STRUCT
age  db
.ENDST

.ENUM $2000 EXPORT
_scroll_x DB               ; db  - define byte (byt and byte work also)
_scroll_y DB
player_x: DW               ; dw  - define word (word works also)
player_y: DW
map_01:   DS  16           ; ds  - define size (bytes)
map_02    DSB 16           ; dsb - define size (bytes)
map_03    DSW  8           ; dsw - define size (words)
monster   INSTANCEOF mon 300 ; 300 instances of structure mon
dragon    INSTANCEOF mon   ; one mon
.ENDE

	ld hl, dragon
	call dragon

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; .base test
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.bank 2 slot 0
.base 1

.section "BaseTest" force

	.db "DATA>"
	.db :CADDR
	.db 1 + :CADDR - 1
	.db "<DATA"

.ends

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; .incbin test
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.macro macroOne 		; the input byte is \1, the output byte is "_out"
.redefine _out \1+1
.endm
	
data1a:	.incbin "data1/data.txt" skip 1 filter macroOne
data2a:	.incbin  data2/data.txt  skip 1 filter macroOne
	.incdir "data1"
data1b:	.incbin "data.txt" skip 1 filter macroOne
	.incbin "data3/data.txt"
	.incdir ""
	.incdir  data2
data2b:	.incbin "data.txt" skip 1 filter macroOne

	.fopen "data1/data.txt" fp
	.fread fp d
	.db d
	.undefine d

	.fopen data2/data.txt fp
	.fread fp d
	.db d
	.undefine d
