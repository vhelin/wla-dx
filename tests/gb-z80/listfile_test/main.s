
; testing listfile generation

.MEMORYMAP
DEFAULTSLOT 1
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $2000
SLOT 2 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 2
BANKSIZE $2000
BANKS 2
.ENDRO

.EMPTYFILL $FF
        
; @BT linked.gb
        
        .BANK 0 SLOT 0
        .ORGA 0

start:  .db $01
        .db $02
startend:

        .section "Section1" keep free
        .db $03
        .db $04
        .db $05
        .ends

        .db $06
        .db $07
        .db $08
        .db $09

        .include "data.i"
        
        .dsb 8, $01
        .dsw 2, $2211
        .dsl 2, $332211

        .section "Section2" keep returnorg
        .db $0A
        .db $0B
        .ends

        .db $0C
        .db $0D
        .dw $5150
        .db $52

        .bank 1 slot 2
        .org 0

        .db $0E
        .db $0F

        .orga $8100

        .db $10
        .db $11
        .db $12

        .section "Section3" keep force
        .db $13
        .db $14
        .ends

        .db $15
        .db $16
        .db $17
        
