
; testing label delta calculation in wla-gb (the assembler, not the linker)

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

.EMPTYFILL $ff
        
.BANK 0 SLOT 0
.ORGA 0

; @BT linked.gb
        
start:  nop
        or a,a
        pop af
        ld sp,test1
startend:
        .db "00>"               ; @BT TEST-00 00 START
        .define VALUE_1 = 1
        .db lobyte(startend)-VALUE_1 ; @BT 05
        .db lobyte(startend)         ; @BT 06
        .rept startend - start + startend - start index count
        .db count                    ; @BT 00 01 02 03 04 05 06 07 08 09 0A 0B
        .endr
        .rept lobyte(startend - start + startend - start)/2 index count
        .db count                    ; @BT 00 01 02 03 04 05
        .endr
        .db "<00"                    ; @BT END
        
.section "Section1" keep
        .db 0, 1, 2, 3
test1:
        .db test1 & $ff, 5, 6
test1end:
        .db "01>"               ; @BT TEST-01 01 START
        .dsb test1end-test1, 0  ; @BT 00 00 00
        .db "<01"               ; @BT END
.ends

.section "Section2" keep
test3:  ld h,e
        ld h,100
        ld bc,test3
test3end:
        .db "02>"               ; @BT TEST-02 02 START
        .dsb test1end-test1 + test3end-test3, 1 ; @BT 01 01 01 01 01 01 01 01 01
        .db "<02"               ; @BT END
.ends

.section "Section3" keep
test2:
        .db 7, 8, 9, (test1 & $ff) + 6, 11, 12, 13, 14
test2end:
        .db "03>"               ; @BT TEST-03 03 START
        .dsb test2end-test2 - (test3end-test3), 2 ; @BT 02 02
        .db "<03"               ; @BT END
.ends

.section "Section4" keep
        .db "04>"               ; @BT TEST-04 04 START
        .db test4end-test4, startend-start ; @BT 06 06
        .db "<04"               ; @BT END
.ends
        
.section "Section5" keep
test4:  .dw test1, test2, test3
test4end:
.ends
        
        .db "05>"               ; @BT TEST-05 05 START
        .dsb startend-start, 3  ; @BT 03 03 03 03 03 03
        .db "<05"               ; @BT END

TESTSTRINGSTART:
    .DB "test"
TESTSTRINGEND:
.REPEAT 16+TESTSTRINGSTART-TESTSTRINGEND
    .DB " "
.ENDR


        .section "TEST-06" free
TestCode:
        nop
-       nop
__      nop
+       nop
        nop
        nop
@InstructionUnderTest:
        .dsb 4, 0
        nop
-       nop
+       nop

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
        
        .section "TEST-11" free keep
        .db "11>"               ; @BT TEST-11 11 START
        nop                     ; @BT 00
-       nop                     ; @BT 00
__      jr +                    ; @BT 18 00
+       jr _b                   ; @BT 18 FC
--      jr _f                   ; @BT 18 02
        jr  +                   ; @BT 18 03
__      nop                     ; @BT 00
-       jr -                    ; @BT 18 FE
+       jr --                   ; @BT 18 F7
        jr  -                   ; @BT 18 FA
        .db "<11"               ; @BT END
        .ends

        .section "TEST-12" free keep
+:      .db "12>"               ; @BT TEST-12 12 START
-:      .db - 1 - 2 + 3         ; @BT 00
--:     .db -1-2+3              ; @BT 00
---:    .db 1    +   3   - 5    ; @BT FF
---     .db   2  -  1   +  1-1  ; @BT 01
--      .dw start     - 2  +  1 ; @BT FF FF
-       .db "<12"               ; @BT END
        .ends
        
        .section "TEST-13" free keep
        .db "13>"               ; @BT TEST-13 13 START
        .dw test13c-test13a     ; @BT 02 00
test13a
test13b .dw test13b-test13a     ; @BT 00 00
        .ends
        
        .section "TEST-13 APPEND 1" free appendto "TEST-13"
test13c .dw test13c - test13b   ; @BT 02 00
        .dw test13b-test13a     ; @BT 00 00
@childA
        .ends

        .section "TEST-13 APPEND 2" free appendto "TEST-13 APPEND 1"
test13d .dw test13d - test13c   ; @BT 04 00
        .dw test13b-test13a     ; @BT 00 00
@child1:
@child2:
        .dw test13d@child2 - @child1 ; @BT 00 00
test13e:
@child1:
@child2:
        .dw test13e@child{1+1} - test13e@child1 ; @BT 00 00
        .dw @child{2-1+1} - test13c@childA      ; @BT 06 00
        .dw test13f@childA - test13f            ; @BT 01 00
        .dsb startend-start, 3  ; @BT 03 03 03 03 03 03
        .dsb test1end-test1, 0  ; @BT 00 00 00
test13f:.db 1                   ; @BT 01
@childA
        .dw @childA - test13f + 1 ; @BT 02 00
        .dw 1 + @childA - test13f ; @BT 02 00
        .dw @childA + 1 - test13f ; @BT 02 00
        .db "<13"               ; @BT END
        .ends
        
        // testing that ROM size indicator is set to emptyfill
        .org $0148-3
        .db "14>"               ; @BT TEST-14 14 START
        //.db $ff               ; @BT FF
        .org $0148+1
        .db "<14"               ; @BT END

        .db "15>"               ; @BT TEST-15 15 START
        .define VALUE_100 = 100
        .define VALUE_2 = (test13f@childA - test13f)*2 + (VALUE_100 - VALUE_100) + 2 - 1 - 1
        .define VALUE_3 = (test13f@childA - test13f)*2 + (VALUE_100 - VALUE_100) + 2 - 1
        .rept (test13f@childA - test13f)*2 + (test13d@child2 - test13d@child1) - (test13b - test13a)*(test13c@childA - test13c)
        .db 3                   ; @BT 03 03
        .endr
        .rept (test13f@childA - test13f)*2 + (VALUE_100 - VALUE_100) + (VALUE_2 - VALUE_2 - VALUE_2) + 2 - (test13b - test13a)*(test13c@childA - test13c)
        .db 4                   ; @BT 04 04
        .endr
        .rept VALUE_2 + (test13f@childA - test13f) - (VALUE_2 - VALUE_2)
        .db 5                   ; @BT 05 05 05
        .endr
        .rept lobyte(VALUE_2 + (test13f@childA - test13f) - (VALUE_2 - VALUE_2))
        .db 5                   ; @BT 05 05 05
        .endr
        .rept lobyte(VALUE_3 + lobyte(test13f@childA - test13f) - lobyte(VALUE_2 - VALUE_2))
        .db 5                   ; @BT 05 05 05 05
        .endr
        .db "<15"               ; @BT END

        .db "16>"               ; @BT TEST-16 16 START
        .db :startend + 1       ; @BT 01
        .db "<16"               ; @BT END

        .assert startend == $0006
        .assert startend - 1 == $0005
        
