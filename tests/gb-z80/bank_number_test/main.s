
.include "cgb_hardware.i"

.MEMORYMAP
SLOTSIZE $4000
DEFAULTSLOT 1
SLOT 0 $0000
SLOT 1 $4000
SLOTSIZE $2000
SLOT 2 $c000
SLOTSIZE $1000
SLOT 3 $e000
.ENDME

.ROMBANKSIZE $4000
.ROMBANKS 4
.ROMSIZE
        
.emptyfill $ee

        .define RAM_OFFSET ram_bank_2 + 1
        .define RAM_NESTED RAM_OFFSET + 1

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

        .db "03>"                     ; @BT TEST-03 03 START
        ld a, :ram_bank_2             ; @BT 3E 02
        .db bank(ram_bank_2)          ; @BT 02
        .db :ram_bank_2 + 1           ; @BT 03
        .db :(ram_bank_2 + $1000)     ; @BT 03
        .db bank(ram_bank_2 + $1000)  ; @BT 03
        .db bank($1000 + ram_bank_2)  ; @BT 03
        .db :($1000 + ram_bank_2)     ; @BT 03
        .db bank(RAM_NESTED)          ; @BT 02
        .db bank(RAM_NESTED + $1000)  ; @BT 03
        .db "<03"                     ; @BT END

        .db "04>"                     ; @BT TEST-04 04 START
        .db :ram_bank_0, :ram_bank_7  ; @BT 00 07
        .db bank(ram_bank_7)          ; @BT 07
        .db :ram_large_slot           ; @BT 02
        .db :ram_with_base            ; @BT 21
        .db bank(ram_with_base)       ; @BT 21
        .db bank(ram_with_base+$1000) ; @BT 22
        .db bank(slotbase(slot(ram_bank_2))) ; @BT 03
        .db bank(slotaddress(ram_bank_2, 2)) ; @BT 03
        .db "<04"                     ; @BT END

        .org $0148-3
        .db "02>"               ; @BT TEST-02 02 START
        .org $0148+1            ; @BT 01
        .db "<02"               ; @BT END

.RAMSECTION "RAM_2" BANK 2 SLOT 3
ram_bank_2 db
.ENDS

.RAMSECTION "RAM_0" BANK 0 SLOT 3
ram_bank_0 db
.ENDS

.RAMSECTION "RAM_7" BANK 7 SLOT 3
ram_bank_7 db
.ENDS

.RAMSECTION "RAM_large_slot" BANK 2 SLOT 2
ram_large_slot db
.ENDS

.BASE $20
.RAMSECTION "RAM_with_base" BANK 1 SLOT 3
ram_with_base db
.ENDS
.BASE 0
        
        
