
.include "cgb_hardware.i"

.MEMORYMAP
SLOTSIZE $4000
DEFAULTSLOT 1
SLOT 0 $0000
SLOT 1 $4000
SLOTSIZE $2000
SLOT 2 $c000
.ENDME

.ROMBANKSIZE $4000
.ROMBANKS 4
.ROMSIZE
        
.emptyfill $ee

        ; @BT linked.gb
        
.ORGA $150

Start:
        .db "01>"                      ; @BT TEST-01 01 START
        .db $01 :Start $02 :Start+3    ; @BT 01 00 02 03
        .db $01, :Start, $02, :Start+3 ; @BT 01 00 02 03
        .db $01 | (:Start)             ; @BT 01
        .db $01 | :Start               ; @BT 01
        .db $00 + :(Start + $4000)     ; @BT 01
        .db "<01"                      ; @BT END

        .org $0148-3
        .db "02>"               ; @BT TEST-02 02 START
        .org $0148+1            ; @BT 01
        .db "<02"               ; @BT END
        
        
