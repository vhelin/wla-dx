
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.rombanksize $8000
.rombanks 1

.base $7E
.ramsection "RAM 1" bank 0 slot 1
	test_var1 db
	test_var2 db
.ends
.ramsection "RAM 2" bank 1 slot 1
	test_var3 db
	test_var4 db
.ends

.bank 0 slot 0
.org 0

main:   .db "TEST>"
	lda test_var1.l
	lda test_var2.l
	lda test_var1.l + 1
	lda test_var2.l + 1
	.db "<TEST"
	.db " "
	.db "NEXT>"
	lda test_var3.l
	lda test_var4.l
	lda test_var3.l + 1
	lda test_var4.l + 1
	.db "<NEXT"
