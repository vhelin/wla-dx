
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
