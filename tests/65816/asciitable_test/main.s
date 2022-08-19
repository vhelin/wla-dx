
.MEMORYMAP
DEFAULTSLOT 0
SLOTSIZE $100
SLOT 0 $0000
.ENDME

.ROMBANKSIZE $100
.ROMBANKS 1

.ORG $00

        ; @BT result.rom

        .db "03>"               ; @BT TEST-03 03 START
        .asc "AIHZ!"            ; @BT 41 49 48 5A 21
        .db "<03"               ; @BT END

        .db "04>"               ; @BT TEST-04 04 START
        .db asc('A'), asc('I'), ; @BT 41 49
        .db ASC('H'), ASC('Z'), ; @BT 48 5A
        .db asc('!') + 1        ; @BT 22
        .db "<04"               ; @BT END

        .db "05>"               ; @BT TEST-05 05 START
        AND asc('A')            ; @BT 25 41
        AND ASC('I'), X         ; @BT 35 49
        AND (ASC('H'), X)       ; @BT 21 48
        .db "<05"               ; @BT END

.ASCIITABLE
MAP "A" TO "Z" = 0
MAP "I" = $70
MAP "Z" = $80
MAP "!" = $90
MAP '\n' = $50
.ENDA

        .db "01>"               ; @BT TEST-01 01 START
        .asc "AIHZ!"            ; @BT 00 70 07 80 90
        .db "<01"               ; @BT END

        .db "02>"               ; @BT TEST-02 02 START
        .db asc('A'), asc('I'), ; @BT 00 70
        .db ASC('H'), ASC('Z'), ; @BT 07 80
        .db asc('!') + 1        ; @BT 91
        .db "<02"               ; @BT END

        .db "06>"               ; @BT TEST-06 06 START
        AND asc('A')            ; @BT 25 00
        AND ASC('I'), X         ; @BT 35 70
        AND (ASC('H'), X)       ; @BT 21 07
        .db "<06"               ; @BT END

        .db "07>"                                       ; @BT TEST-07 07 START
        .db ASC('A') + (2*5) - asc('B') + 1             ; @BT 0A
        AND ASC('A') + (2*5) - asc('B') + 1             ; @BT 25 0A
        AND ASC('A') + (2*5) - asc('B') + 1, X          ; @BT 35 0A
        AND (ASC('A') + (2*5) - asc('B') + 1, X)        ; @BT 21 0A
        .db "<07"                                       ; @BT END
        
.define CALCULATION_1 = ASC('A')*100 + asc('B')*5 - 4
.define CALCULATION_2 = ASC('!')-$80

        .db "08>"                               ; @BT TEST-08 08
        .db CALCULATION_1, CALCULATION_2-1      ; @BT START 01 0F
        .db "<08"                               ; @BT END

        .db "09>"                               ; @BT TEST-09 09 START
        .db asc('\n'), asc($a), asc(10)         ; @BT 50 50 50
        .db asc('\n') + asc(10),                ; @BT A0
        .db asc(0b1010) + '\n', '\n'            ; @BT 5A 0A
        .db "<09"                               ; @BT END

