
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing WLA DX's CADDR functionality
; written by ville helin <ville.helin@iki.fi> in 2018
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

; @BT linked.rom
        
;------------------------------------------------------------------------------
; FORCE section tests
;------------------------------------------------------------------------------

        .bank 0 slot 0
        .org 0
        .section "Section1" force
        .db "03>"               ; @BT TEST-03 03 START
        .dw caddr, CADDR, caddr+1, CADDR-1  ; @BT 03 00 05 00 08 00 08 00
        .db caddr & $ff, (CADDR >> 1) & $FF ; @BT 0B 06
        .db "<03"               ; @BT END
        .ends

        .bank 0 slot 0
        .org $500
        .section "Section2" force
        .db "04>"               ; @BT TEST-04 04 START
        .dw caddr, CADDR, caddr+1, CADDR-1  ; @BT 03 05 05 05 08 05 08 05
        .db caddr & $ff, (CADDR >> 1) & $FF ; @BT 0B 86
        .db "<04"               ; @BT END
        .ends

        .bank 1 slot 1
        .org 0
        .section "Section3" force
        .db "05>"               ; @BT TEST-05 05 START
        .dw caddr, CADDR, caddr+1, CADDR-1  ; @BT 03 40 05 40 08 40 08 40
        .db caddr & $ff, (CADDR >> 8) & $FF ; @BT 0B 40
        .db "<05"               ; @BT END
        .ends

        .bank 1 slot 1
        .org $500
        .section "Section4" force
        .db "06>"               ; @BT TEST-06 06 START
        .dw caddr, CADDR, caddr+1, CADDR-1  ; @BT 03 45 05 45 08 45 08 45
        .db caddr & $ff, (CADDR >> 8) & $FF ; @BT 0B 45
        .db "<06"               ; @BT END
        .ends

;------------------------------------------------------------------------------
; .BASE tests
;------------------------------------------------------------------------------

        .bank 0 slot 0
        .base $00
        .org $100
        .section "Section5" force
        .db "07>"               ; @BT TEST-07 07 START
        .dw caddr, CADDR, caddr+1, CADDR-1                       ; @BT 03 01 05 01 08 01 08 01
        .db caddr & $ff, (CADDR >> 8) & $FF, (CADDR >> 16) & $ff ; @BT 0B 01 00
        .db "<07"               ; @BT END
        .ends

        .bank 0 slot 0
        .base $80
        .org $200
        .section "Section6" force
        .db "08>"               ; @BT TEST-08 08 START
        .dw caddr, CADDR, caddr+1, CADDR-1                   ; @BT 03 02 05 02 08 02 08 02
        .db caddr & $ff, (CADDR >> 8) & $FF, bankbyte(CADDR) ; @BT 0B 02 80
        .db "<08"               ; @BT END
        .ends

        .bank 0 slot 0
        .base $f0
        .org $300
        .section "Section7" force
        .db "09>"               ; @BT TEST-09 09 START
        .dw caddr, CADDR, caddr+1, CADDR-1                   ; @BT 03 03 05 03 08 03 08 03
        .db caddr & $ff, (CADDR >> 8) & $FF, bankbyte(CADDR) ; @BT 0B 03 F0
        .db "<09"               ; @BT END
        .ends

;------------------------------------------------------------------------------
; More CADDR tests
;------------------------------------------------------------------------------

        .bank 0 slot 0
        .base 0
        .org $400
root1:
@spritesTable:
        .db "01>"               ; @BT TEST-01 01 START
        .db @sprites0-CADDR     ; @BT 06
        .db @sprites1-CADDR     ; @BT 12
        .db @sprites2-CADDR     ; @BT 1A
        .db "<01"               ; @BT END
        
@sprites0:
        .db $10
        .db $10 $10 $10 $10
        .db $10 $10 $10 $10
        .db $10 $10 $10 $10
        
@sprites1:
        .db $20
        .db $20 $20 $20 $20
        .db $20 $20 $20 $20

@sprites2:
        .db $30
        .db $30 $30 $30 $30
        .db $30 $30 $30 $30
        .db $30 $30 $30 $30
        .db $30 $30 $30 $30

;------------------------------------------------------------------------------
; More CADDR tests
;------------------------------------------------------------------------------

        .section "TEST-02" free keep
root2:
@spritesTable:
@myLabelChild:
        .db "02>"               ; @BT TEST-02 02 START
        .db CADDR-CADDR         ; @BT 00
        .db @sprites0-CADDR     ; @BT 06
        .db @sprites1-CADDR     ; @BT 12
        .db @sprites2-CADDR     ; @BT 1A
        .db "<02"               ; @BT END
        
@sprites0:
        .db $10
        .db $10 $10 $10 $10
        .db $10 $10 $10 $10
        .db $10 $10 $10 $10
        
@sprites1:
        .db $20
        .db $20 $20 $20 $20
        .db $20 $20 $20 $20

@sprites2:
        .db $30
        .db $30 $30 $30 $30
        .db $30 $30 $30 $30
        .db $30 $30 $30 $30
        .db $30 $30 $30 $30
        .ends

;------------------------------------------------------------------------------
; Yet another CADDR test
;------------------------------------------------------------------------------

        .section "TEST-10+11+12" free keep
root3:
@myLabelChild:
        .db "10>"               ; @BT TEST-10 10 START
        .db CADDR-@myLabelChild ; @BT 03
        .db @sprites0-CADDR     ; @BT 04
        .db "<10"               ; @BT END

@sprites0:
        .db $40
        .db $40 $40 $40 $40
        .db $40 $40 $40 $40

@sprites1:
        .db $40
        .db $40 $40 $40 $40
        .db $40 $40 $40 $40

@sprites2:
        .db $40
        .db $40 $40 $40 $40
        .db $40 $40 $40 $40

root4:
@myLabelChild:
        .db "11>"               ; @BT TEST-11 11 START
        .db CADDR-@myLabelChild ; @BT 03
        .db @sprites0-CADDR     ; @BT 09
@here:
        .rept @here-@myLabelChild
        .db $AB                 ; @BT AB AB AB AB AB
        .endr

        .db "<11"               ; @BT END

@sprites0:
        .db $40
        .db $40 $40 $40 $40
        .db $40 $40 $40 $40

@sprites1:
        .db $40
        .db $40 $40 $40 $40
        .db $40 $40 $40 $40

@sprites2:
        .db $40
        .db $40 $40 $40 $40
        .db $40 $40 $40 $40

        .db "12>"               ; @BT TEST-12 12 START

        .rept @sprites1 - @sprites0 - 8
        .db $11                 ; @BT 11
        .endr

        .rept root4@sprites1 - root4@sprites0 - 7
        .db $22 $22             ; @BT 22 22

        .db "<12"               ; @BT END
        .ends
