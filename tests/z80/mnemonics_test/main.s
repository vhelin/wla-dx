
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

        .db "09>"               ; @BT TEST-09 09 START
        .IFDEF SKIPTHENEXTBLOCK
        ld a, d
        add a, (ix+14)
        cp $bf
        ret nc
        .ENDIF
        .db 0                   ; @BT 00
        .db "<09"               ; @BT END
        
