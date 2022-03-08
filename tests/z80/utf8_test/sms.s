
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing WLA DX's UTF-8 parsing functionality
; written by ville helin <ville.helin@iki.fi> in 2022
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

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
BANKSTOTAL 2
BANKSIZE $4000
BANKS 2
.ENDRO

.EMPTYFILL $69

;------------------------------------------------------------------------------
; FORCE section tests
;------------------------------------------------------------------------------

        ; @BT linked.rom

        // a lot of 💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩
        .print "💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩💩\n"
        
        .macro X💩
        .db $20
        .endm

        .macro 💩💩💩💩💩
        .db $10
        .endm

        .define 💩🎉 $30
        .define 💩💩 1
        .define 🎉1💩🎉 2
        
.bank 0 slot 0
.org 0

        .section "🎉💩" FORCE
        .db "01>"               ; @BT TEST-01 01 START
        call 💩                 ; @BT CD 06 00
💩:
        ret                     ; @BT C9
💩💩💩:ret                      ; @BT C9
        X💩                     ; @BT 20
        💩💩💩💩💩              ; @BT 10
        .db 💩🎉                ; @BT 30
        .db 1+💩🎉+1            ; @BT 32
        .db 🎉{💩💩}💩🎉        ; @BT 02
        .db 🎉{💩💩}💩🎉-1      ; @BT 01
        .db 2+🎉{%.1d{💩💩}}💩🎉-1    ; @BT 03
        .db "<01"               ; @BT END

        .db "💩💩💩"

        
        .ends
