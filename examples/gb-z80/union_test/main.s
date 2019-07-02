.include "cgb_hardware.i"

; Testing "union" feature. Check the .sym file for actual output.

.MEMORYMAP
SLOTSIZE $4000
DEFAULTSLOT 1
SLOT 0 $0000
SLOT 1 $4000
SLOTSIZE $2000
SLOT 2 $c000
.ENDME

.ROMBANKSIZE $4000
.ROMBANKS 2

.emptyfill $01

.ORGA $150

.STRUCT MyStruct1
	b0:	db
.ENDST

.STRUCT MyStruct2
	b0:	db
	w1:	dw
	b2:	db
	struct1: INSTANCEOF MyStruct1
.ENDST

.STRUCT MyStruct3
	string:	dsb 8
	b0:	db
	struct2: INSTANCEOF MyStruct2
	b1:	db
.ENDST

/* Legacy DSTRUCT syntax (doesn't support unions) */
.DSTRUCT data INSTANCEOF MyStruct3 DATA "hi", $01, $02, $0302, $04, $05, $06

.STRUCT MyStruct4
	startString: dsb 8
	b0:	db
	test.dots:	db

	.union
		word: dw
	.nextu
		word.l: db
		word.h: db
	.endu

	.union named
		thing: db
	.endu

	multiS1: INSTANCEOF MyStruct1 10

	MyStruct2: INSTANCEOF MyStruct2

	largedataAlias:	.dsb $100
	largedata:	dsb $100

	dotted:	.db
	endString: dsb 8
.ENDST

/* New DSTRUCT syntax */
.DSTRUCT data2 INSTANCEOF MyStruct4 VALUES
	startString:		.db "<BEGIN>"
	endString:		.db "<END>"
	dotted:			.db "<" ; Overlaps with endString

	; b0 not defined; defaults to emptyfill
	test.dots:		.db $f0
	word.l:			.db $f1
	word.h:			.db $f2
	word:			.dw $f2f1 ; Overlaps with word.l, word.h
	named:			.db $f3
	named.thing:		.db $f3 ; Overlap

	multiS1:		.db $f4
	multiS1.b0:		.db $f4 ; Overlap
	multiS1.1.b0:		.db $f4 ; Overlap
	multiS1.2.b0:		.db $f5
	multiS1.3.b0:		.db $f6
	; 4-9 will be emptyfill'd
	multiS1.10.b0:		.db $fa

	MyStruct2.b0:		.db $fb
	MyStruct2.w1: 		.dw $fdfc
	MyStruct2.b2: 		.db $fe
	MyStruct2.struct1.b0:
				.db $ff

	largedata:
		.db $00 $01 $02 $03 $04 $05 $06 $07
		.db $08 $09 $0a $0b $0c $0d $0e $0f
		.db $10 $11 $12 $13 $14 $15 $16 $17
		.db $18 $19 $1a $1b $1c $1d $1e $1f
		.db $20 $21 $22 $23 $24 $25 $26 $27
		.db $28 $29 $2a $2b $2c $2d $2e $2f
		.db $30 $31 $32 $33 $34 $35 $36 $37
		.db $38 $39 $3a $3b $3c $3d $3e $3f
		.db $40 $41 $42 $43 $44 $45 $46 $47
		.db $48 $49 $4a $4b $4c $4d $4e $4f
		.db $50 $51 $52 $53 $54 $55 $56 $57
		.db $58 $59 $5a $5b $5c $5d $5e $5f
		.db $60 $61 $62 $63 $64 $65 $66 $67
		.db $68 $69 $6a $6b $6c $6d $6e $6f
		.db $70 $71 $72 $73 $74 $75 $76 $77
		.db $78 $79 $7a $7b $7c $7d $7e $7f
		.db $80 $81 $82 $83 $84 $85 $86 $87
		.db $88 $89 $8a $8b $8c $8d $8e $8f
		.db $90 $91 $92 $93 $94 $95 $96 $97
		.db $98 $99 $9a $9b $9c $9d $9e $9f
		.db $a0 $a1 $a2 $a3 $a4 $a5 $a6 $a7
		.db $a8 $a9 $aa $ab $ac $ad $ae $af
		.db $b0 $b1 $b2 $b3 $b4 $b5 $b6 $b7
		.db $b8 $b9 $ba $bb $bc $bd $be $bf
		.db $c0 $c1 $c2 $c3 $c4 $c5 $c6 $c7
		.db $c8 $c9 $ca $cb $cc $cd $ce $cf
		.db $d0 $d1 $d2 $d3 $d4 $d5 $d6 $d7
		.db $d8 $d9 $da $db $dc $dd $de $df
		.db $e0 $e1 $e2 $e3 $e4 $e5 $e6 $e7
		.db $e8 $e9 $ea $eb $ec $ed $ee $ef
		.db $f0 $f1 $f2 $f3 $f4 $f5 $f6 $f7
		.db $f8 $f9 $fa $fb $fc $fd $fe $ff
.ENDST

struct1End:
	.db $ff


.section StructSection SEMIFREE

/* Test inside a section */
.DSTRUCT data3 INSTANCEOF MyStruct4 VALUES
	startString:		.db "<BEGIN>"
	endString:		.db "<END>"
	dotted:			.db "<" ; Overlaps with endString

	; b0 not defined; defaults to emptyfill
	test.dots:		.db $f0
	word.l:			.db $f1
	word.h:			.db $f2
	word:			.dw $f2f1 ; Overlaps with word.l, word.h
	named:			.db $f3
	named.thing:		.db $f3 ; Overlap

	multiS1:		.db $f4
	multiS1.b0:		.db $f4 ; Overlap
	multiS1.1.b0:		.db $f4 ; Overlap
	multiS1.2.b0:		.db $f5
	multiS1.3.b0:		.db $f6
	; 4-9 will be emptyfill'd
	multiS1.10.b0:		.db $fa

	MyStruct2.b0:		.db $fb
	MyStruct2.w1: 		.dw $fdfc
	MyStruct2.b2: 		.db $fe
	MyStruct2.struct1.b0:
				.db $ff

	largedataAlias:
		.db $00 $01 $02 $03 $04 $05 $06 $07
		.db $08 $09 $0a $0b $0c $0d $0e $0f
		.db $10 $11 $12 $13 $14 $15 $16 $17
		.db $18 $19 $1a $1b $1c $1d $1e $1f
		.db $20 $21 $22 $23 $24 $25 $26 $27
		.db $28 $29 $2a $2b $2c $2d $2e $2f
		.db $30 $31 $32 $33 $34 $35 $36 $37
		.db $38 $39 $3a $3b $3c $3d $3e $3f
		.db $40 $41 $42 $43 $44 $45 $46 $47
		.db $48 $49 $4a $4b $4c $4d $4e $4f
		.db $50 $51 $52 $53 $54 $55 $56 $57
		.db $58 $59 $5a $5b $5c $5d $5e $5f
		.db $60 $61 $62 $63 $64 $65 $66 $67
		.db $68 $69 $6a $6b $6c $6d $6e $6f
		.db $70 $71 $72 $73 $74 $75 $76 $77
		.db $78 $79 $7a $7b $7c $7d $7e $7f
		.db $80 $81 $82 $83 $84 $85 $86 $87
		.db $88 $89 $8a $8b $8c $8d $8e $8f
		.db $90 $91 $92 $93 $94 $95 $96 $97
		.db $98 $99 $9a $9b $9c $9d $9e $9f
		.db $a0 $a1 $a2 $a3 $a4 $a5 $a6 $a7
		.db $a8 $a9 $aa $ab $ac $ad $ae $af
		.db $b0 $b1 $b2 $b3 $b4 $b5 $b6 $b7
		.db $b8 $b9 $ba $bb $bc $bd $be $bf
		.db $c0 $c1 $c2 $c3 $c4 $c5 $c6 $c7
		.db $c8 $c9 $ca $cb $cc $cd $ce $cf
		.db $d0 $d1 $d2 $d3 $d4 $d5 $d6 $d7
		.db $d8 $d9 $da $db $dc $dd $de $df
		.db $e0 $e1 $e2 $e3 $e4 $e5 $e6 $e7
		.db $e8 $e9 $ea $eb $ec $ed $ee $ef
		.db $f0 $f1 $f2 $f3 $f4 $f5 $f6 $f7
		.db $f8 $f9 $fa $fb $fc $fd $fe $ff
.ENDST


/* Example from the readme */
.STRUCT water
    name   ds 8
    age    db
    weight dw
.ENDST

.STRUCT drop_pair
    waterdrops: instanceof water 2
.ENDST

.DSTRUCT waterdrop INSTANCEOF water VALUES
    name:   .db "tingle"
    age:    .db 40
    weight: .dw 120
.ENDST

.DSTRUCT drops drop_pair VALUES
    waterdrops.1:        .db "qwertyui" 40
			 .dw 120
    waterdrops.2.name:   .db "tingle"
    waterdrops.2.age:    .db 40
    waterdrops.2.weight: .dw 12
.ENDST

.DSTRUCT INSTANCEOF water VALUES /* Nameless struct */
    name:   .db "nameless"
    age:    .db 50
    weight: .dw 100
.ENDST

.ends


.ENUM $d000 DESC EXPORT
	b0:	db
	b1: 	db
	b2: 	db
	w3: 	dw
	str:	instanceof MyStruct2 2

	.union un1
		b0: db
		w1: dw
		st1: instanceof MyStruct1
	.nextu un2
		w1: dw
	.endu

	unionend: .db
.ENDE


.RAMSECTION "RAM" SLOT 2

.union ram
	.union union1
		w0:		dw
		struct2		instanceof MyStruct2 5
	.nextu union2
		w0.low		db
		w0.high		db
	.endu

	unionEnd: .db

	.union union3
		.union union4
			.union union5
				b0: db
			.nextu
				w0: dw
			.endu
			b1: db
		.endu
		b2: db
	.endu

	struct1:		instanceof MyStruct1
.endu

.ENDS
