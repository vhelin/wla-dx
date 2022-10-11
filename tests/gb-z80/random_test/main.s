
.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE                $4000
   SLOT            0       $0000
   SLOT            1       $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE                $2000
   SLOT            3       $C000
   SLOT            4       $E000
.ENDME

.ROMBANKMAP
BANKSTOTAL 1
BANKSIZE $4000
BANKS 1
.ENDRO

.ROMSIZE $11
        
.EMPTYFILL $69

;------------------------------------------------------------------------------
; FORCE section tests
;------------------------------------------------------------------------------

        .bank 0 slot 0
        .org 0

; @BT linked.rom

        .seed 111
        .db "01>"                  ; @BT TEST-01 01 START
        .dbrnd COUNT 5 MIN 0 MAX 9 ; @BT 00 02 06 08 02
        .db "<01"                  ; @BT END
        
        .seedrandom
        .dbrnd 10 0 127

        .seed 222
        .db "02>"                  ; @BT TEST-02 02 START
        .dbrnd COUNT 5 MIN 0 MAX 9 ; @BT 06 03 09 02 02
        .db "<02"                  ; @BT END

        .seed 111
        .db "03>"                  ; @BT TEST-03 03 START
        .db random(0, 9)           ; @BT 00
        .db random(0, 9)           ; @BT 02
        .db random(0, 9)           ; @BT 06
        .db random(0, 9)           ; @BT 08
        .db random(0, 9)           ; @BT 02
        .db "<03"                  ; @BT END

        .define VALUE_10 = 10
        
        .seed 222
        .db "04>"                  ; @BT TEST-04 04 START
        .db random(1-1, 9)         ; @BT 06
        .db random(0, 10-1)        ; @BT 03
        .db random(VALUE_10-VALUE_10, VALUE_10-1) ; @BT 09
        .db random(VALUE_10-VALUE_10, VALUE_10-1)*4-6 ; @BT 02
        .db 10+random(VALUE_10-9-1, 1+VALUE_10-2)*10-28 ; @BT 02
        .db "<04"                  ; @BT END

        .seed 111
        .db "05>"                  ; @BT TEST-05 05 START
        .db random(0, 9) + random(0, 9) + random(0, 9) ; @BT 08
        .db random(0, 9) - 2*random(0, 9) ; @BT 04
        .db "<05"                  ; @BT END
        
