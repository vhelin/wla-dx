
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

        .db "01>"
        res 0, (ix+$12), d // $DD $CB $12 $82
        res 0,(iy+$69),a   // $FD $CB $69 $87
        .db "<01"

        .db "02>"
        res 3, (ix+$34), h // $DD $CB $34 $9C
        res 3, (iy+$56)    // $FD $CB $56 $9E
        .db "<02"

        .db "03>"
        rl (iy+$12),d      // $FD $CB $12 $12
        rlc (ix+$34), e    // $DD $CB $34 $03
        .db "<03"
        .ends

        .db "04>"
        rr (iy+56h),a      // $FD $CB $56 $1F
        rrc (ix+$67), l    // $DD $CB $67 $0D
        .db "<04"

        .db "05>"
        set 2, (ix+$01), d // $DD $CB $01 $D2
        set 4,(iy+$23),b   // $FD $CB $23 $E0
        .db "<05"

        .db "06>"
        sla (ix+$70), l    // $DD $CB $70 $25
        sl1 (ix+$21),a     // $DD $CB $21 $37
        .db "<06"

        .db "07>"
        sra (iy+$51), b    // $FD $CB $51 $28
        srl (ix+$32),h     // $DD $CB $32 $3C
        .db "<07"
        
