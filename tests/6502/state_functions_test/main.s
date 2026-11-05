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

        .function read_align() align()
        .function align_plus(n) read_align()+n
        .function align_negative() -align()
        .function align_smaller() align() < 32
        .assert align() == 1 && read_align() == 1

        .db "01>"                       ; @BT TEST-01 01 START
        .db align(), read_align()       ; @BT 01 01
        .db LATE + align()              ; @BT 02
        .db align_plus(LATE)            ; @BT 02
        .assert LATE + read_align() == 2, LDERROR
        .db "<01"                       ; @BT END

        .align 16
        .define saved_align align()
        .assert read_align() == 16
        .assert align_negative() == -16
        .assert align_smaller() == 1
        .db "02>"                       ; @BT TEST-02 02 START
        .db align(), read_align()       ; @BT 10 10
        .db align_plus(LATE)            ; @BT 11
        .db "<02"                       ; @BT END

        .align 32
        .assert align_smaller() == 0
        .db "03>"                       ; @BT TEST-03 03 START
        .db align(), read_align()       ; @BT 20 20
        .db saved_align                 ; @BT 10
        .db align_plus(LATE)            ; @BT 21
        .db "<03"                       ; @BT END
