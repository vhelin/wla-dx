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
	b0:	db
	struct2: INSTANCEOF MyStruct2
	b1:	db
.ENDST

.DSTRUCT data INSTANCEOF MyStruct3 DATA $01, $02, $0302, $04, $05, $06

.db $ff


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
