
.MEMORYMAP
DEFAULTSLOT 0
SLOTSIZE $100
SLOT 0 $0000
.ENDME

.ROMBANKSIZE $100
.ROMBANKS 1

.ORG $00

        .db "03>"
        .asc "AIHZ!"
        .db "<03"

        .db "04>"
        .db asc('A'), asc('I'), ASC('H'), ASC('Z'), asc('!') + 1
        .db "<04"

        .db "05>"
        AND asc('A')
        AND ASC('I'), X
        AND (ASC('H'), X)
        .db "<05"
        
.ASCIITABLE
MAP "A" TO "Z" = 0
MAP "I" = $70
MAP "Z" = $80
MAP "!" = $90
.ENDA

        .db "01>"
        .asc "AIHZ!"
        .db "<01"

        .db "02>"
        .db asc('A'), asc('I'), ASC('H'), ASC('Z'), asc('!') + 1
        .db "<02"

        .db "06>"
        AND asc('A')
        AND ASC('I'), X
        AND (ASC('H'), X)
        .db "<06"

        .db "07>"
        .db ASC('A') + (2*5) - asc('B') + 1
        AND ASC('A') + (2*5) - asc('B') + 1
        AND ASC('A') + (2*5) - asc('B') + 1, X
        AND (ASC('A') + (2*5) - asc('B') + 1, X)
        .db "<07"
        
.define CALCULATION_1 = ASC('A')*100 + asc('B')*5 - 4
.define CALCULATION_2 = ASC('!')-$80

        .db "08>"
        .db CALCULATION_1, CALCULATION_2-1
        .db "<08"
        
