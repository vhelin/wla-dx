
        .memorymap
          slotsize $2000
          defaultslot 1
          slot 0 $0000
          slot 1 $2000
        .endme

        .rombanksize $2000
        .rombanks 2

        .bank 0 slot 0

        .section "Section1" force

        ; @BT result.rom
        .db "01>"          ; @BT TEST-01 01 START
        res 0, (ix+$12), d ; @BT DD CB 12 82
        res 0,(iy+$69),a   ; @BT FD CB 69 87
        .db "<01"          ; @BT END

        .db "02>"          ; @BT TEST-02 02 START
        res 3, (ix+$34), h ; @BT DD CB 34 9C
        res 3, (iy+$56)    ; @BT FD CB 56 9E
        .db "<02"          ; @BT END

        .db "03>"          ; @BT TEST-03 03 START
        rl (iy+$12),d      ; @BT FD CB 12 12
        rlc (ix+$34), e    ; @BT DD CB 34 03
        .db "<03"          ; @BT END

        .ends

        .db "04>"          ; @BT TEST-04 04 START
        rr (iy+56h),a      ; @BT FD CB 56 1F
        rrc (ix+$67), l    ; @BT DD CB 67 0D
        .db "<04"          ; @BT END

        .db "05>"          ; @BT TEST-05 05 START
        set 2, (ix+$01), d ; @BT DD CB 01 D2
        set 4,(iy+$23),b   ; @BT FD CB 23 E0
        .db "<05"          ; @BT END

        .db "06>"          ; @BT TEST-06 06 START
        sla (ix+$70), l    ; @BT DD CB 70 25
        sl1 (ix+$21),a     ; @BT DD CB 21 37
        .db "<06"          ; @BT END

        .db "07>"          ; @BT TEST-07 07 START
        sra (iy+$51), b    ; @BT FD CB 51 28
        srl (ix+$32),h     ; @BT DD CB 32 3C
        .db "<07"          ; @BT END

        .db "08>"               ; @BT TEST-08 08 START
        ex af',af               ; @BT 08
        ex af',af               ; @BT 08
        ex de',hl'              ; @BT EB
        ex de,hl                ; @BT EB
        ex hl',de'              ; @BT EB
        ex hl,de                ; @BT EB
        ex (sp),hl'             ; @BT E3
        ex (sp),hl              ; @BT E3
        ex (sp),ix              ; @BT DD E3
        ex (sp),iy              ; @BT FD E3
        ex hl',(sp)             ; @BT E3
        ex hl,(sp)              ; @BT E3
        ex ix,(sp)              ; @BT DD E3
        ex iy,(sp)              ; @BT FD E3        
        .db "<08"               ; @BT END

        .function sumAB(varA, varB) (varA + varB)
        
        .db "09>"               ; @BT TEST-09 09 START
        .IFDEF SKIPTHENEXTBLOCK
        ld a, d
        add a, (ix+14)
        ex (sp),hl
        add a, (ix+sumAB(10,4))
        add a, (ix+sumDE(10,4))
        cp $bf
        ret nc
        .ENDIF
        .db 0                   ; @BT 00
        .db "<09"               ; @BT END

        .db "10>"                     ; @BT TEST-10 10 START
address_test_10:
        ld e,(iy+address_test_10)     ; @BT FD 5E FD
        ld e,(iy+address_test_10+1)   ; @BT FD 5E FB
        ld e,(iy+0+address_test_10)   ; @BT FD 5E F7
        ld e,(iy+0+address_test_10+1) ; @BT FD 5E F5
        .db "<10"                     ; @BT END

        .struct Object
        status  db
        data    dsb 8
        .endst

        .dstruct Object1 instanceof Object DATA 1, "HELLO"
        
        .db "11>"                  ; @BT TEST-11 11 START
        ld e,(iy+Object1.data)     ; @BT FD 5E F2
        ld e,(iy+Object1.data+1)   ; @BT FD 5E F0
        ld e,(iy+Object1.data)     ; @BT FD 5E EC
        ld e,(iy+Object1.data+1)   ; @BT FD 5E EA
        ld e,(iy-0+Object1.data)   ; @BT FD 5E E6
        ld e,(iy-0+Object1.data+1) ; @BT FD 5E E4
        ld e,(iy-0+Object1.data)   ; @BT FD 5E E0
        ld e,(iy-0+Object1.data+1) ; @BT FD 5E DE
        .db "<11"                  ; @BT END

        .db "12>"                 ; @BT TEST-12 12 START
        ld e,(iy+CALCULATION_1)   ; @BT FD 5E 02
        ld e,(iy+CALCULATION_2+2) ; @BT FD 5E 03
        ld e,(iy+CALCULATION_3+3) ; @BT FD 5E 04
        ld e,(iy+Objecto.data)    ; @BT FD 5E 01
        ld e,(iy+Objecto.data+1)  ; @BT FD 5E 02
        .dw CALCULATION_1         ; @BT 02 00
        .dw CALCULATION_2+2       ; @BT 03 00
        .dw CALCULATION_3+3       ; @BT 04 00
        .db VALUE_4               ; @BT 04
        .db VALUE_4+1             ; @BT 05
        .dl CALCULATION_1         ; @BT 02 00 00
        .dl CALCULATION_2+2       ; @BT 03 00 00
        .dl CALCULATION_3+3       ; @BT 04 00 00
        .dd CALCULATION_1         ; @BT 02 00 00 00
        .dd CALCULATION_2+2       ; @BT 03 00 00 00
        .dd CALCULATION_3+3       ; @BT 04 00 00 00
        .db "<12"                 ; @BT END

        .define CALCULATION_1 = 0+Objecto.data+1
        .define CALCULATION_2 = 0+Objecto.data+0
        .define CALCULATION_3 = Objecto.data
        .define VALUE_10 = 5+4+1
        .define VALUE_6 = 7-1
        .define VALUE_4 VALUE_10 - VALUE_6
        
        .struct Objecto
        status  db
        data    dsb 8
        .endst

        .section "Section2" force
        .db "13>"                ; @BT TEST-13 13 START
        ld e,(iy+Object2.data)   ; @BT FD 5E 07
        ld e,(iy+Object2.data+1) ; @BT FD 5E 05
        .db "<13"                ; @BT END
        .ends
        
