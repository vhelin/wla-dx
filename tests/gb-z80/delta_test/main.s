
; testing label delta calculation

.MEMORYMAP
DEFAULTSLOT 1
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $6000
SLOT 2 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 2
BANKSIZE $2000
BANKS 1
BANKSIZE $6000
BANKS 1
.ENDRO

.BANK 0 SLOT 0
.ORGA 0

; @BT linked.gb
	
start:	nop
	or a,a
	pop af
	ld sp,test1
startend:

.section "Section1" keep
	.db 0, 1, 2, 3
test1:
	.db test1 & $ff, 5, 6
test1end:
	.db "01>"		; @BT TEST-01 01 START
	.dsb test1end-test1, 0	; @BT 00 00 00
	.db "<01"		; @BT END
.ends

.section "Section2" keep
test3:	ld h,e
	ld h,100
	ld bc,test3
test3end:
	.db "02>"		; @BT TEST-02 02 START
	.dsb test1end-test1 + test3end-test3, 1 ; @BT 01 01 01 01 01 01 01 01 01
	.db "<02"		; @BT END
.ends

.section "Section3" keep
test2:
	.db 7, 8, 9, (test1 & $ff) + 6, 11, 12, 13, 14
test2end:
	.db "03>"		; @BT TEST-03 03 START
	.dsb test2end-test2 - (test3end-test3), 2 ; @BT 02 02
	.db "<03"		; @BT END
.ends

.section "Section4" keep
	.db "04>"		; @BT TEST-04 04 START
	.db test4end-test4, startend-start ; @BT 06 06
	.db "<04"		; @BT END
.ends
	
.section "Section5" keep
test4:	.dw test1, test2, test3
test4end:
.ends
	
	.db "05>"		; @BT TEST-05 05 START
	.dsb startend-start, 3	; @BT 03 03 03 03 03 03
	.db "<05"		; @BT END

TESTSTRINGSTART:
    .DB "test"
TESTSTRINGEND:
.REPEAT 16+TESTSTRINGSTART-TESTSTRINGEND
    .DB " "
.ENDR


        .section "TEST-06" free
TestCode:
        nop
        nop
        nop
        nop
        nop
        nop
@InstructionUnderTest:
        .dsb 4, 0
        nop
        nop
        nop

        .define OffsetOfInstructionUnderTest1 @InstructionUnderTest - TestCode
        .export OffsetOfInstructionUnderTest1
        .define OffsetOfInstructionUnderTest2 TestCode@InstructionUnderTest - TestCode
        .export OffsetOfInstructionUnderTest2

End:    nop
        
        .db "06>"                         ; @BT TEST-06 06 START
        .db OffsetOfInstructionUnderTest1 ; @BT 06
        .db End - TestCode                ; @BT 0D
        .db End - TestCode + 1            ; @BT 0E
        .db "<06"                         ; @BT END
        .ends

        .section "TEST-07" free
        .db "07>"                         ; @BT TEST-07 07 START
        .db OffsetOfInstructionUnderTest2 ; @BT 06
        .db End - TestCode                ; @BT 0D
        .db End - TestCode + 1            ; @BT 0E
        .db "<07"                         ; @BT END
        .ends
        
        .section "TEST-08"
MoreTestCode:
        nop
        nop
@InstructionUnderTest:
        .dsb 4, 0
        nop

        .define OffsetOfInstructionUnderTest3 @InstructionUnderTest - MoreTestCode
        .export OffsetOfInstructionUnderTest3
        .define OffsetOfInstructionUnderTest4 MoreTestCode@InstructionUnderTest - MoreTestCode
        .export OffsetOfInstructionUnderTest4
        
MoreEnd:nop

        .db "08>"                          ; @BT TEST-08 08 START
        .db OffsetOfInstructionUnderTest3  ; @BT 02
        .db MoreEnd - MoreTestCode         ; @BT 07
        .db MoreEnd - MoreTestCode + 1     ; @BT 08
        .db "<08"                          ; @BT END
        .ends
        
        .section "TEST-09" free
        .db "09>"                         ; @BT TEST-09 09 START
        .db OffsetOfInstructionUnderTest4 ; @BT 02
        .db MoreEnd - MoreTestCode        ; @BT 07
        .db MoreEnd - MoreTestCode + 1    ; @BT 08
        .db "<09"                         ; @BT END
        .ends

        .section "TEST-10"
YUMTestCode:
        nop
        nop
        nop
@InstructionUnderTest:
        .dsb 2, 0
        nop
        .db "10>"                         ; @BT TEST-10 10 START
        .db @InstructionUnderTest - YUMTestCode ; @BT 03
        .db YUMTestCode@InstructionUnderTest - YUMTestCode ; @BT 03
        .db "<10"                         ; @BT END
YUMEnd: nop
        .ends
        
