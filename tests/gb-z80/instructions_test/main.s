
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
        nop                     ; @BT 00
        ei                      ; @BT FB
        di                      ; @BT F3
        daa                     ; @BT 27
        cpl a                   ; @BT 2F
        cpl                     ; @BT 2F
        add 10                  ; @BT C6 0A
        ld bc, $1234            ; @BT 01 34 12
        ld (hl-),a              ; @BT 32
        add a,a                 ; @BT 87
        nop \ nop               ; @BT 00 00
        add 10 \ ld bc, $1234   ; @BT C6 0A 01 34 12
        add 10 \ ld bc, $1234 \ nop ; @BT C6 0A 01 34 12 00
        ld bc, ($1200 + 1 - 1) | $34 ; @BT 01 34 12
        ld bc, ($1200 + 1 - 1) | $34 \ ld bc, $1234 ; @BT 01 34 12 01 34 12
        nop \ ld bc, ($1200 + 1 - 1) | $34 \ ld bc, $1234 ; @BT 00 01 34 12 01 34 12
        ld bc, $1234 \ ld bc, ($1200 + 1 - 1) | $34 \ ld bc, $1234 ; @BT 01 34 12 01 34 12 01 34 12
        ld hl, sp+10 \ nop      ; @BT F8 0A 00
        rst 7+1 \ rst $10       ; @BT CF D7
        .db "<01"               ; @BT END
