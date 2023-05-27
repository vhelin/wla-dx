
.memorymap
	slotsize $8000
	defaultslot 1
	slot 0 $0000
	slot 1 $8000
.endme

.rombanksize $8000
.rombanks 1

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
