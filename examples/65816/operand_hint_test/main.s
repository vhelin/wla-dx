
.HIROM

.snesheader
id    "BABU"
name  "Testing"
hirom
slowrom
cartridgetype $13
romsize $0c
sramsize $03
country $01
licenseecode $12
version 02
.endsnes

.MEMORYMAP
SLOTSIZE $8000
DEFAULTSLOT 0
SLOT 0 $8000
.ENDME

.ROMBANKSIZE $8000
.ROMBANKS 8

.NAME "Testing"

.SNESNATIVEVECTOR
COP    $0000
BRK    $1234
ABORT  $5678
NMI    hotto
UNUSED $0000
IRQ    $9abc
.ENDNATIVEVECTOR

.snesemuvector
cop    hotto
unused hotto
abort  hotto
nmi    hotto
reset  hotto
irqbrk hotto
.endemuvector

.STRUCT instrument
      SampleAddr                      dw
      SampleAddrExAndFineTune         db
      SampleVolume                    db
      SampleLength                    dw
      SampleLoopStart                 dw
.ENDST

.define isrAddr 0
.define GPU_ScanlineStatus 1
.define GPU_hBlankStatus 2


.MACRO LoadBlockToVRAM
	.db :\1
.ENDM

.MACRO LoadBlockToXRAM
	.db \1
.ENDM
	

.base $20
.BANK 0 SLOT 0
.ORG $0000

	.EQU Intro_greyText2nd = Intro_greyText+$8c00
	.db "01>"
	LoadBlockToVRAM Intro_greyText+$8c00,$4e00,$1c00
	.db "<01"
	.db "02>"
	LoadBlockToVRAM Intro_greyText2nd,$100,$200,$300
	.db "<02"
	.db "03>"
	LoadBlockToXRAM :(Intro_greyText2nd)+1
	.db "<03"
	.db "04>"
	LoadBlockToXRAM (Intro_greyText2nd >> 16)
	.db "<04"
Intro_greyText:	
	lda.b	#($80 | (>40))

	lda	(instrument.SampleLength+1).w,X
	lda	instrument.SampleVolume.w,X

        lda.b   #<isrSplitScreen
        sta     isrAddr
        lda.b   #>isrSplitScreen
        sta     isrAddr+1
                                                                                                           
        lda.b   #<40
        sta     GPU_ScanlineStatus
        lda.b   #($80 | (>40))
        sta     GPU_hBlankStatus

isrSplitScreen
  and #10
  .accu 16
  .index 8
  .8bit
  and #10
  .db $ff, $ff
  cpx #11
  .index 16
  cpx #11
  .db $ff, 255

.incbin "makefile" skip 11 read 6 swap fsize MSIZE
.printt "incbinned 6 of "
.printv dec MSIZE
.printt " bytes.\n"
.undefine MSIZE

.DEFINE ADDR 20
.define HDMA2122 $85d2

.SECTION "Test" FORCE

	mvn 1, 2
	mvp 3, 4
	
hotto	lda HDMA2122,x
 	lda HDMA2122.w,x

	and 10.b
	and 10.w
	and ADDR.B
	and ADDR.W
	and ADDR           - 4.b
	and ADDR  +  4.w
.ENDS

.dbsin 0.2, 10, 3.2, 120, 1.3


.org $200
.section "test 1" force
_tst1:	.db $de, $ad
.ends

.org $202
.section "test 2" force
_tst1:	.db $be, $ef
.ends


.org $200
.section "test 3" overwrite
_tst1:	.db $de, $ad
.ends

.define mase 2+1
.define koso 100+mase
.define moon koso+1
.export moon, koso, mase

.orga $85d2
kose:	lda kose.w,x

.struct monsta
x     db
name  ds 10
y     db
id    dw
trio1 dl
trio2 dsl 2
trio3 dl
.endst

.db "DATA START >>> "
	mvn $1, $2
	mvp $3, $4
.db " <<< DATA END"

.section "table" force
table:
.db "<TABLE>" 			                                                       ;  7 bytes
.table byte, word, BYTE, dw, dsb 2, long, byt
.data $01, $0302, $04, $0605, $07, $08, $0b0a09, $0c                                   ; 12 bytes
.row "a", "bc", "d", "ef", "g", "h", "ijk", "l"                                        ; 12 bytes
.row tableX+1, tableX+2, tableX+3, tableX+4, tableX+5, tableX+6, tableX+7, tableX+8    ; 12 bytes
.db "</TABLE>"                                                                         ;  8 bytes -> 51 bytes
.ends

.section "longs"
.db "<LONG>"
.dl $010203, $040506
.long $010203, $040506
.dsl 3 $aabbcc
.db "</LONG>"
.ends

.export monsta.x, monsta.y, monsta.id, monsta.name
