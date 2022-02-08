
.include "main.inc"

        ; @BT linked.gb
main:
        .db "01>"                           ; @BT TEST-01 01 START
	LDA test_var1 + _sizeof_test_struct ; @BT A5 02
	LDA test_var1 - _sizeof_test_struct ; @BT A5 FE
        .db "<01"                           ; @BT END
        
        
