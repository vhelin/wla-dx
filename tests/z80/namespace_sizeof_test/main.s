.memorymap
    defaultslot 0
    slotsize $4000
    slot 0 $0000
    slotsize $2000
    slot 1 $c000
.endme

.rombankmap
    bankstotal 1
    banksize $4000
    banks 1
.endro

.bank 0 slot 0
.org 0

; @BT linked.rom

.db "01>"                                          ; @BT TEST-01 01 START
.include "foo.s" namespace "foo"                   ; @BT 00 01 02 03 04 05 06 07 21 03 00 06 08 0E BE ED B3 C9 3E 03 09 11 22 33 FF
.db "<01"                                          ; @BT END

.db "02>"                                          ; @BT TEST-02 02 START
.db _sizeof_foo.baz                                ; @BT 08
.db "<02"                                          ; @BT END

.db "03>"                                          ; @BT TEST-03 03 START
.include "types.s" namespace "foo"                 ; @BT 02 01 02 01
.db "<03"                                          ; @BT END

.include "section.s" namespace "foo"
                                                   ; @BT TEST-04 -a $200 START 01 02 03 04 05 END
                                                   ; @BT TEST-05 -a $210 START 02 09 08 END
