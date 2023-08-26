
; testing .continue

.MEMORYMAP
DEFAULTSLOT 1
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $6000
SLOT 2 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 2
BANKSIZE $2000
BANKS 1
BANKSIZE $6000
BANKS 1
.ENDRO

.EMPTYFILL $ff
        
.BANK 0 SLOT 0
.ORGA 0

; @BT linked.gb

        .db "01>"               ; @BT TEST-01 01 START
        .repeat 8 index count
        .if count == 10
          .fail
        .endif
        .if (count & 1) == 1
          .if count == 3
            .continue           ; skip 3!
          .endif
          .db count             ; @BT 01 05 07
        .endif
        .endr
        .db "<01"               ; @BT END


        .db "02>"               ; @BT TEST-02 02 START
        .repeat 8 index count
        .if (count & 1) == 1
          .db count             ; @BT 00 01 01 02 03 03 04 05 05 06 07 07
        .endif
        .db count
        .endr
        .db "<02"               ; @BT END


        .db "03>"               ; @BT TEST-03 03 START
        .repeat 1234 index count
        .if count == 3
          .break
        .endif
        .db count               ; @BT 00 01 02
        .endr
        .db $ff                 ; @BT FF
        .db "<03"               ; @BT END


        .db "04>"               ; @BT TEST-04 04 START
        .repeat 4
        .repeat 1000 index count
        .db count               ; @BT 00 01 00 01 00 01 00 01
        .if count == 1
        .break
        .endif
        .endr
        .endr
        .db "<04"               ; @BT END


        .define counter 5

        .db "05>"               ; @BT TEST-05 05 START
        .while counter > 0
        .db 1                   ; @BT 01 01 01 01 01
        .redefine counter = counter - 1
        .endr        
        .db "<05"               ; @BT END


        .redefine counter 5

        .db "06>"               ; @BT TEST-06 06 START
        .while counter > 0
        .if counter <= 2
        .redefine counter = counter - 1
        .continue
        .endif
        .db 1                   ; @BT 01 01 01
        .redefine counter = counter - 1
        .endr        
        .db "<06"               ; @BT END
        

        .redefine counter 5
        .define inside 2
        .define ALWAYS_1 1

        .db "07>"               ; @BT TEST-07 07 START
        .while counter > 0 && ALWAYS_1 != 0
        .redefine inside 2
        .while inside > 0 && ALWAYS_1 == 1
        .db 0
        .redefine inside = inside - 1
        .endr
        .db 1                   ; @BT 00 00 01 00 00 01 00 00 01 00 00 01 00 00 01
        .redefine counter = counter - 1
        .endr        
        .db "<07"               ; @BT END
        
