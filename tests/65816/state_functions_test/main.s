        .memorymap
        defaultslot 0
        slotsize $2000
        slot 0 $0000
        .endme

        .rombanksize $2000
        .rombanks 1
        .bank 0 slot 0
        .org 0

        ; @BT linked.rom

        .function read_accu() accu()
        .function read_index() index()
        .function read_align() align()
        .function width_sum(n) n+accu()+index()
        .function nested_width_sum(n) width_sum(n)+read_accu()
        .function signed_widths() -accu()+index()*-2+(+accu())
        .function mixed_signs() 2*-accu()+-index()
        .function largest_width() max(accu(),index())
        .function accu_smaller() accu() < index()
        .function accu_larger() accu() > index()

        .macro check_widths args expected_accu, expected_index
        .if accu() == expected_accu
        .else
        .fail "Unexpected accumulator width."
        .endif
        .if index() != expected_index
        .fail "Unexpected index register width."
        .endif
        .endm

        check_widths 8, 8
        .if align() != 1
        .fail "Initial alignment must be 1."
        .endif
        .assert AcCu( ) == 8 && InDeX( ) == 8

        .db "01>"                       ; @BT TEST-01 01 START
        .db accu(), index()             ; @BT 08 08
        .db read_accu(), read_index()   ; @BT 08 08
        .accu 16
        check_widths 16, 8
        .assert read_accu() == 16 && read_index() == 8
        .assert signed_widths() == -16
        .assert mixed_signs() == -40
        .assert largest_width() == 16
        .assert accu_larger() == 1 && accu_smaller() == 0
        .db accu(), index()             ; @BT 10 08
        .db width_sum(1)                ; @BT 19
        .db nested_width_sum(1)         ; @BT 29
        rep #$10                        ; @BT C2 10
        check_widths 16, 16
        .db accu(), index()             ; @BT 10 10
        sep #$20                        ; @BT E2 20
        check_widths 8, 16
        .assert accu_smaller() == 1 && accu_larger() == 0
        .db read_accu(), read_index()   ; @BT 08 10
        .accu 16
        .index 8
        lda.b #1                        ; @BT A9 01
        check_widths 16, 8
        .db accu(), index()             ; @BT 10 08
        .db "<01"                       ; @BT END

        .define saved_accu accu()
        .db "02>"                       ; @BT TEST-02 02 START
        .db LATE + accu()               ; @BT 11
        .db LATE + read_accu()          ; @BT 11
        .db width_sum(LATE)             ; @BT 19
        .db nested_width_sum(LATE)      ; @BT 29
        .assert LATE + read_accu() == 17, LDERROR
        .accu 8
        .index 16
        .db saved_accu                  ; @BT 10
        .db LATE + accu()               ; @BT 09
        .db LATE + read_accu()          ; @BT 09
        .db width_sum(LATE)             ; @BT 19
        .db nested_width_sum(LATE)      ; @BT 21
        .assert LATE + read_index() == 17, LDERROR
        .db "<02"                       ; @BT END

        .align 16
        .assert align() == 16 && read_align() == 16
        .include "alignment.s"
        .assert align() == 32
        .if 0
        .align 128
        .endif
        .db "03>"                       ; @BT TEST-03 03 START
        .db AlIgN( ), read_align()      ; @BT 20 20
        .db LATE + read_align()         ; @BT 21
        .db "<03"                       ; @BT END

        .section "Aligned" free align 64
        .assert get("section.alignment") == 64
        .assert align() == 32
        .align 8
        .db "04>"                       ; @BT TEST-04 04 START
        .db align(), read_align()       ; @BT 08 08
        .db "<04"                       ; @BT END
        .ends
        .assert align() == 8
