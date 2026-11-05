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

        .function read_adl() adl()
        .function nested_adl(n) n+read_adl()+adl()
        .function negative_adl() -adl()
        .macro check_adl args expected
        .if adl() != expected
        .fail "Unexpected ADL mode."
        .endif
        .assert read_adl() == expected
        .endm

        check_adl 0
        .assert align() == 1
        .db "01>"                       ; @BT TEST-01 01 START
        .db adl(), read_adl()           ; @BT 00 00
        .adl on
        check_adl 1
        .assert AdL( ) == 1
        .assert negative_adl() == -1
        .define saved_adl adl()
        .db adl(), read_adl()           ; @BT 01 01
        .db nested_adl(1)               ; @BT 03
        ld.sis bc,$1234                 ; @BT 40 01 34 12
        .db adl()                       ; @BT 01
        .adl off
        check_adl 0
        ld.lil bc,$123456               ; @BT 5B 01 56 34 12
        .db adl(), saved_adl            ; @BT 00 01
        .db "<01"                       ; @BT END

        .db "02>"                       ; @BT TEST-02 02 START
        .db LATE + adl()                ; @BT 01
        .db nested_adl(LATE)            ; @BT 01
        .assert LATE + read_adl() == 1, LDERROR
        .adl on
        .db LATE + adl()                ; @BT 02
        .db nested_adl(LATE)            ; @BT 03
        .assert LATE + read_adl() == 2, LDERROR
        .adl off
        .db "<02"                       ; @BT END
