
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

        .function sumAB(varA,varB) varA+varB
        .function sumABC(var1,var2,var3) sumAB(var1,var2)+var3
        .function sumAB1(varA, varB) sumAB(varA, varB) + sumAB(1, 0)
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
        .db "<03"               ; @BT END
