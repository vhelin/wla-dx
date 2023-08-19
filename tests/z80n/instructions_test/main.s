
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

; @BT linked.rom

        .db "01>"               ; @BT TEST-01 01 START
_0x0003:mul d, e                ; @BT ED 30
        add hl, $1234           ; @BT ED 34 34 12
        jp (c)                  ; @BT ED 98
        nextreg $12, a          ; @BT ED 92 12
        nreg $12, a             ; @BT ED 92 12
        nextreg $12, $34        ; @BT ED 91 12 34
        nreg $12, $34           ; @BT ED 91 12 34
        push $1234              ; @BT ED 8A 12 34
        push $1233 + 1          ; @BT ED 8A 12 34
        push $1233 + VALUE_1    ; @BT ED 8A 12 34
        push $1233 + VALUE_2    ; @BT ED 8A 12 35
        push _0x0003            ; @BT ED 8A 00 03
        push _0x0003 + VALUE_1  ; @BT ED 8A 00 04
        push _0x0003 + VALUE_2  ; @BT ED 8A 00 05
        .db "<01"               ; @BT END

        .define VALUE_1 1
        
