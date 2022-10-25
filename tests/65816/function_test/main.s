
//////////////////////////////////////////////////////////////////////
// function tests
//////////////////////////////////////////////////////////////////////
        
        .memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
        .endme                    

        .lorom

        .rombanksize $8000
        .rombanks 3

; @BT result.rom

//////////////////////////////////////////////////////////////////////
// tests
//////////////////////////////////////////////////////////////////////

        .bank 0 slot 0
        .org 0

        .define varA 1
        .define varB 2
        .define var1 3
        .define var2 4
        .define var3 5
        .define var4 1

        .function sumAB(varA,varB) varA+varB
        .function sumABC(var1,var2,var3) sumAB(var1,var2)+var3
        .function sumAB1(varA, varB) sumAB(varA, varB) + sumAB(1, 0)
        .function sumCD(var{%.1d{1+0}},var2) var{%.1d{0+var4}}+var{%.1d{1+1}}
        .function constant() 1+2+3

        .db "01>"               ; @BT TEST-01 01 START
        .db sumAB(1,2)          ; @BT 03
        .db 1 + sumAB(1,2)      ; @BT 04
        .db sumABC(3,4,5)       ; @BT 0C
        .db sumAB(0+1,0+1+2-1)  ; @BT 03
        .db sumABC(1+2,2+2,2+3) ; @BT 0C
        .db sumAB(1,2) + sumAB(3,4) ; @BT 0A
        .db sumABC(3,4,5) - sumABC(2,3,4) ; @BT 03
        .db sumAB(sumAB(1,2), sumAB(3,4)) ; @BT 0A
        .db constant()                    ; @BT 06
        .db constant() + 1                ; @BT 07
        .db "<01"               ; @BT END

        .db "02>"                ; @BT TEST-02 02 START
        .db sumAB(VALUE_1,1)     ; @BT 02
        .db 1 + sumAB(VALUE_1,2) ; @BT 04
        .db 1 + sumAB(VALUE_1+VALUE_2,2) ; @BT 06
        .db VALUE_2 - sumABC(VALUE_1-1, VALUE_2-2, sumAB(VALUE_1-1,VALUE_2-1)) + sumAB(constant(), VALUE_2-1) ; @BT 08
        .db sumAB1(1, 2)         ; @BT 04
        .db sumAB1(sumAB1(1, 2), 3)  ; @BT 08
        .db sumAB1(VALUE_1, VALUE_2) ; @BT 04
        .db sumAB1(VALUE_1+1, VALUE_2+2) ; @BT 07
        .db sumAB1(sumAB(1,2),sumABC(sumAB(1,2),sumAB(1,2),sumAB(1,2))) ; @BT 0D
        .db sumAB1(sumAB(VALUE_1,VALUE_2),sumABC(sumAB(VALUE_1,VALUE_2),sumAB(1,2),sumAB(1,2))) ; @BT 0D
        .db sumAB1(sumAB(VALUE_1+1,VALUE_2+1),sumABC(sumAB(VALUE_1,VALUE_2),sumAB(1,2),sumAB(1,2))) ; @BT 0F
        .db "<02"                ; @BT END

        .db "03>"               ; @BT TEST-03 03 START
        asl sumAB(5,4)          ; @BT 06 09
        cmp #sumABC(1,2,3)      ; @BT C9 06
        cmp [sumABC(1,2,3)]     ; @BT C7 06
        asl sumAB(VALUE_1,VALUE_2) ; @BT 06 03
        cmp #sumAB1(VALUE_1,VALUE_2) ; @BT C9 04
        cmp [sumABC(VALUE_1,VALUE_2,3)] ; @BT C7 06
        asl sumAB(VALUE_1+1,VALUE_2+1) ; @BT 06 05
        cmp #sumAB1(VALUE_1+1,VALUE_2-1) ; @BT C9 04
        cmp [sumABC(VALUE_1+1,VALUE_2+1,3+1)] ; @BT C7 09
        lda sumABC(1,2,3)                     ; @BT A5 06
        lda sumABC(VALUE_1,VALUE_2,3)         ; @BT A5 06
        lda sumABC(1,2,3).w                   ; @BT AD 06 00
        lda sumABC(1,2,3).w + sumABC(0,1,2).w ; @BT AD 09 00
        lda (sumABC(1,2,3))                   ; @BT B2 06
        lda sumABC(sumAB1(0+0,0-0),2,3).w     ; @BT AD 06 00
        .db "<03"               ; @BT END

        .db "04>"               ; @BT TEST-04 04 START
        .db varA                ; @BT 01
        .db varB                ; @BT 02
        .db var1                ; @BT 03
        .db var2                ; @BT 04
        .db var3                ; @BT 05
        .db "<04"               ; @BT END

        .db "05>"                ; @BT TEST-05 05 START
        .db varA + sumAB(varA,1) ; @BT 03
        .db varA + sumABC(varA,var1,var2) + var3 ; @BT 0E
        .redefine varA 2
        .db varA + sumAB(varA,1) ; @BT 05
        .db varA + sumABC(varA,var1,var2) + var3 ; @BT 10
        .db varA                 ; @BT 02
        .db sumAB(var{%.1d{2-1}},2) ; @BT 05
        .db sumCD(sumCD(1,2),max(VALUE_1+VALUE_1+1,VALUE_1+VALUE_1))+sumCD(1,2) ; @BT 09
        .db "<05"                ; @BT END

        .db "06>"               ; @BT TEST-06 06 START
        .redefine varA = 1
        .define RUNNER{%.1d{varA+0}} = varA
        .rept sumAB(1+1,0+1)
        .db sumAB1(0,RUNNER{%.1d{2-varA}}) ; @BT 02 03 04
        .redefine RUNNER{%.1d{2-varA}} = RUNNER{%.1d{0+varA}} + 1
        .endr
        .rept sumABC(0+max(1-0+0,1),0+2,0+3)+sumAB1(sumAB(0,1),0+varA*2)
        .db 1                   ; @BT 01 01 01 01 01 01 01 01 01 01
        .endr
        .db "<06"               ; @BT END

        .db "07>"               ; @BT TEST-07 07 START
        asl sumAB(VALUE_1,VALUE_3+1) ; @BT 06 05
        cmp #sumABC(1,2,VALUE_3)     ; @BT C9 06
        cmp [sumABC(1,VALUE_4/2,VALUE_4-1)] ; @BT C7 06
        .db "<07"               ; @BT END

        // just testing that 0x7F -> 0x100 jump works in listfiles
        .org $100
_0x100: .db "08>"               ; @BT TEST-08 08 START
        .db _0x100 >> 8, _0x100 & 0xff ; @BT 01 00
        .db "<08"                      ; @BT END
        
        .db "09>"               ; @BT TEST-09 09 START
        .db max(10, 5)          ; @BT 0A
        .db max(5, 6)           ; @BT 06
        .db min(10, 5)          ; @BT 05
        .db min(1, 2)           ; @BT 01
        .db max(10,5) + min(10,5) ; @BT 0F
        .db max(1,2)+max(3,4)     ; @BT 06
        .db min(1,2)+min(3,4)     ; @BT 04
        .db min(0,1)*max(1,2)+max(3,4) ; @BT 04
        .db max(10+1, 5+1)        ; @BT 0B
        .db min(10+2, 5+2)        ; @BT 07
        .db max(10+1,5+2) + min(10+1,5+2) ; @BT 12
        .db max(1+0,2+1)+max(3+2,4+3)     ; @BT 0A
        .db min(1+1,2+2)+min(3+3,4+4)     ; @BT 08
        .db min(0+0,1+1)*max(1+2,2+3)+max(3+4,4+5) ; @BT 09
        .db 1+max(10, 5)          ; @BT 0B
        .db 0+min(10, 5)-2        ; @BT 03
        .db 16-(max(10,5) + min(10,5)) ; @BT 01
        .db 2*(max(1,2)+max(3,4))+1    ; @BT 0D
        .db 2*min(1,2)+2*min(3,4)-3    ; @BT 05
        .db 2*(min(0,1)*max(1,2)+max(3,4))+1 ; @BT 09
        .db 2*max(10+1, 5+1)-11        ; @BT 0B
        .db 3*min(10+2, 5+2)-20        ; @BT 01
        .db 0x12*2-(max(10+1,5+2) + min(10+1,5+2) + 1) ; @BT 11
        .db "<09"               ; @BT END
        
        .db "10>"               ; @BT TEST-10 10 START
        .db max(VALUE_1, VALUE_2) ; @BT 02
        .db max(VALUE_2, VALUE_1) ; @BT 02
        .db min(VALUE_2, VALUE_1) ; @BT 01
        .db min(VALUE_1, VALUE_2) ; @BT 01
        .db max(VALUE_1,VALUE_2) + min(VALUE_1,VALUE_2) ; @BT 03
        .db max(VALUE_1,VALUE_2)+max(VALUE_1,VALUE_2)   ; @BT 04
        .db min(VALUE_0,VALUE_1)*max(VALUE_1,VALUE_2)+max(VALUE_1,VALUE_2) ; @BT 02
        .db max(10+VALUE_1, 5+VALUE_1) ; @BT 0B
        .db min(10+VALUE_2, 5+VALUE_2) ; @BT 07
        .db max(10+VALUE_1,5+VALUE_2) + min(10+VALUE_1,5+VALUE_2) ; @BT 12
        .db max(VALUE_1+VALUE_0,VALUE_2+VALUE_1)+max(VALUE_3+VALUE_2,VALUE_4+VALUE_3) ; @BT 0A
        .db min(VALUE_1+VALUE_1,VALUE_2+VALUE_2)+min(VALUE_3+VALUE_3,VALUE_4+VALUE_4) ; @BT 08
        .db min(VALUE_0+VALUE_0,VALUE_1+1)*max(1+VALUE_2,VALUE_2+3)+max(VALUE_3+4,VALUE_4+5) ; @BT 09
        .db 1+max(10, VALUE_4+1)  ; @BT 0B
        .db 0+min(10, VALUE_2+VALUE_3)-VALUE_2 ; @BT 03
        .db 16-(max(10,3+VALUE_2) + min(10,VALUE_4+1)) ; @BT 01
        .db VALUE_2*(max(VALUE_1,2)+max(3,VALUE_4))+VALUE_1 ; @BT 0D
        .db (VALUE_1*2)*min(VALUE_1,VALUE_1+1)+2*min(VALUE_3,VALUE_2*2)-VALUE_3 ; @BT 05
        .db 2*(min(VALUE_0,1)*max(1,VALUE_2)+max(3,VALUE_4))+VALUE_1 ; @BT 09
        .db VALUE_2*max(10+VALUE_1, VALUE_4+1+1)-11 ; @BT 0B
        .db (VALUE_1+VALUE_1+VALUE_1)*min(10+VALUE_1+1, 5+VALUE_2)-18-VALUE_2 ; @BT 01
        .db 0x12*VALUE_2-(max(10+VALUE_1,5+2) + min(10+VALUE_1,VALUE_4+1+2) + VALUE_1) ; @BT 11
        .db "<10"               ; @BT END

        .db "11>"               ; @BT TEST-11 11 START
        asl sumAB(min(5,VALUE_5*2),max(4,4))          ; @BT 06 09
        cmp #sumABC(min(1+1,max(max(-VALUE_1,0),VALUE_1)),2,3)      ; @BT C9 06
        cmp [sumABC(1,2,max(VALUE_3,-VALUE_3))]     ; @BT C7 06
        asl sumAB(max(max(-VALUE_5-VALUE_5-100, 6-VALUE_5),-VALUE_5+3+2),max(2+2-VALUE_2,VALUE_5-4)) ; @BT 06 03
        cmp #max(sumAB1(VALUE_1+0,0+VALUE_2),-VALUE_3-VALUE_5) ; @BT C9 04
        .db "<11"               ; @BT END

        .db "12>"               ; @BT TEST-12 12 START
        .define runnerA = max(min(-1-1-1-1-1, 1+1+1+1+1-1-1-1), -var3+var3*2-1) // 4
        .rept max(2+floor(17/runnerA)-1, 1)
        .db runnerA ; @BT 04 03 02 01 00
        .redefine runnerA = min(runnerA + 0, max(1+runnerA-1, runnerA-runnerA) - 1)
        .endr
        .db "<12"               ; @BT END
