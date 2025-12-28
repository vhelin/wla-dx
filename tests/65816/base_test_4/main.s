
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.rombanksize $8000
.rombanks 2

        ; @BT result.rom
        
.base $7E
.ramsection "RAM 1" bank 0 slot 1
	test_var1 db
	test_var2 db
        test_varX db
.ends
.base 0
.ramsection "RAM 2" bank 1 slot 1 base $7d
	test_var3 db
	test_var4 db
.ends

.bank 0 slot 0
.org 0

main:   .db "01>"               ; @BT TEST-01 01 START
	lda test_var1.l         ; @BT AF 00 80 7E
	lda test_var2.l         ; @BT AF 01 80 7E
	lda test_var1.l + 1     ; @BT AF 01 80 7E
	lda test_var2.l + 1     ; @BT AF 02 80 7E
	.db "<01"               ; @BT END
	.db " "
	.db "02>"               ; @BT TEST-02 02 START
	lda test_var3.l         ; @BT AF 00 80 7E
	lda test_var4.l         ; @BT AF 01 80 7E
	lda test_var3.l + 1     ; @BT AF 01 80 7E
	lda test_var4.l + 1     ; @BT AF 02 80 7E
	.db "<02"               ; @BT END

	.section "TEST-03" force
	.db "03>"               ; @BT TEST-03 03 START
label3: .db base(label3)+2	; @BT 02
	.db base(label4)+1	; @BT 12
	.db base(label5)+3	; @BT 03
	.db base(label6)	; @BT 1F
	.db base(label7)	; @BT 22
	.base $11
label4:	.db base(label4)+base(label7) ; @BT 33
	.db base(label5)	; @BT 00
	.db "<03"               ; @BT END
	.ends

	.db "05>"		; @BT TEST-05 05 START
label5:	.db base(label5)	; @BT 00
	.db base(label4)	; @BT 11
	.db "<05"		; @BT END

	.section "TEST-04" base $1f force
	.db "04>"               ; @BT TEST-04 04 START
label6:	.db base(label5)        ; @BT 00
	.db base(label6)	; @BT 1F
	.base $22
label7:	.db base(label7)	; @BT 22
	.base $33
label8:	.db base(label8)	; @BT 33
	.db "<04"               ; @BT END
	.ends

	.base $01

	.db "06>"		; @BT TEST-06 06 START
label9:	.db base(label5)	; @BT 00
	.db base(label4)	; @BT 11
	.db base(label9)	; @BT 01
	.db 2 + base(labelA)	; @BT 52
	.db "<06"		; @BT END
