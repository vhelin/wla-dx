
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; includes
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INC "defines.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; standard stuff?
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.DEFINE REALLY 10
.EXPORT REALLY

.define RESULT_A (4 + VALUE_A)
.define RESULT_B (6 + VALUE_B)
.define RESULT_C (RESULT_B - RESULT_A)

.BANK 0 SLOT 0
.ORG $100

	JP	MAIN

.define VALUE_0 = 1.2345
.define VALUE_1 = 111.123456789
.define VALUE_2 VALUE_0*0.987654321987654321

.SECTION "__testing"
sME:	LD	HL, sME
	LD	A, 10+REALLY
	.dw	VALUE_0
	.dw     VALUE_1
	.dw     VALUE_2
	.db "6>>>"
	.db RESULT_A + 1 - 1
	.db "<<<6|"
	.db "9>>>"
	.db RESULT_B - 2 + 2
	.db "<<<9|"
	.db "3>>>"
	.db RESULT_C + 3 - RESULT_C + RESULT_C - 3
	.db "<<<3|"
	.db "NEXT|"
	.db "6>>>"
	.db RESULT_A
	.db "<<<6|"
	.db "3>>>"
	.db RESULT_C
	.db "<<<3|"
	.db "6>>>"
	.db RESULT_A
	.db "<<<6"
	.db TEST_STRING
.ENDS
