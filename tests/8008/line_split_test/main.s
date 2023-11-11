
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

        ; @BT linked.rom
        
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

        .db "01>"               ; @BT TEST-01 01 START
        .DSTRUCT waterdrop1 INSTANCEOF \
        water DATA \
        "W1" \                  ; @BT 57 31 AA AA AA AA AA AA
        1, 2                    ; @BT 01 02 00
        .DSTRUCT \
        waterdrop2 \
        INSTANCEOF \
        water \
        DATA \
        "W2" \                  ; @BT 57 32 AA AA AA AA AA AA
        $40 \                   ; @BT 40
        $80                     ; @BT 80 00
        .db "<01"               ; @BT END

        .db "02>"               ; @BT TEST-02 02 START
        .db 1, 2, 3 4 5\        ; @BT 01 02 03 04 05
           6 \                  ; @BT 06
           7, 8,\               ; @BT 07 08
           9                    ; @BT 09
        .db "<02"               ; @BT END

        .db "03>"               ; @BT TEST-03 03 START
        ldi \
          $10                   ; @BT 1E 10
        lei \
          $20                   ; @BT 26 20
        lhi $30                 ; @BT 2E 30
        lhi \
          $30                   ; @BT 2E 30
        .db "<03"               ; @BT END

        .db "04>"               ; @BT TEST-04 04 START
        .db "Long " \           ; @BT 4C 6F 6E 67 20
          "string..."           ; @BT 73 74 72 69 6E 67 2E 2E 2E
        .db "<04"               ; @BT END
.ENDS
