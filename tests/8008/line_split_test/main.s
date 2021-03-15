
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small parser test for wla-8008
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0 \
                   $0000
   SLOT \
        1 \
        $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $1000
   SLOT            3       $D000
   SLOT            4       $E000
   SLOT            5       $F000
.ENDME

.ROMBANKMAP
BANKSTOTAL \
        4
BANKSIZE \
        $4000
BANKS \
        4
.ENDRO

.EMPTYFILL $AA

.BANK \
        0 \
        SLOT \
        0
.ORG $100

.STRUCT \
        water
    name   ds 8
    age \
        db
    weight dw
.ENDST
        
.SECTION "TESTS" \
        KEEP

        .db "01>"
        .DSTRUCT waterdrop1 INSTANCEOF \
        water DATA \
        "W1" \
        1, 2
        .DSTRUCT \
        waterdrop2 \
        INSTANCEOF \
        water \
        DATA \
        "W2" \
        $40 \
        $80
        .db "<01"

        .db "02>"
        .db 1, 2, 3 4 5\
           6 \
           7, 8,\
           9
        .db "<02"

        .db "03>"
        ldi \
          $10
        lei \
          $20
        lhi $30
        lhi \
          $30
        .db "<03"

        .db "04>"
        .db "Long " \
          "string..."
        .db "<04"
.ENDS
