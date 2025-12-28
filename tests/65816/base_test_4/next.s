
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.rombanksize $8000
.rombanks 2

	; @BT result.rom
	
	.bank 1 slot 1
	.org 0

	.base $50

	.db "07>"		; @BT TEST-07 07 START
labelA:	.db base(labelA)	; @BT 50
	.db base(label4) + base(labelA) ; @BT 61
	.db 1 + base(label9) + 1	; @BT 03
	.db "<07"			; @BT END

	.db "08>"		; @BT TEST-08 08 START
	.db base(label5)	; @BT 00
	.db base(label4)*2	; @BT 22
	.db base(label9)*3	; @BT 03
	.db 2 + base(labelA)	; @BT 52
	.db "<08"		; @BT END
	
	.if base(labelA) == $50
	.print "HELLO\n"
	.endif
	
