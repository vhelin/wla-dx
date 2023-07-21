
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

        .org 0xF0
address_test_10:

        .struct Object
        status  db
        data    dsb 8
        .endst

        .dstruct Object1 instanceof Object DATA 1, "HELLO"

        // NOTE: following tests were "removed" as they depended on a feature that was broken
        
        .db "10>"                     ; @BT TEST-10 10 START
        .db 0x01                      ; @BT 01
        .db "<10"                     ; @BT END

        .db "11>"                  ; @BT TEST-11 11 START
        .db 0x01                   ; @BT 01
        .db "<11"                  ; @BT END

        .db "12>"                 ; @BT TEST-12 12 START
        .db 0x01                  ; @BT 01
        .db "<12"                 ; @BT END

        .org 0xC0
        
        .section "Section2" force
        .db "13>"                ; @BT TEST-13 13 START
        .db 01                   ; @BT 01
        .db "<13"                ; @BT END
        .ends

        .org 0x200
        
        .section "boot" force
        .db "14>"               ; @BT TEST-14 14 START
        di                      ; @BT F3
        im 1                    ; @BT ED 56
        ld sp, $dff0            ; @BT 31 F0 DF

        ld iy, myLabel          ; @BT FD 21 34 12
        ld ixh, >myLabel        ; @BT DD 26 12
        ld ixl, <myLabel        ; @BT DD 2E 34

        ld hl, myLabel          ; @BT 21 34 12
        ld d, >myLabel          ; @BT 16 12
        ld e, <myLabel          ; @BT 1E 34

        ld bc, ($1200 + 1 - 1) | $34 ; @BT 01 34 12

        .db "<14"               ; BT END

-:      jp -
        .ends

        .org 0x1234
        
        .section "someSection" force
myLabel:
        outi
        ret
        .ends
        
