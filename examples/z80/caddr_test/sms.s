
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

;------------------------------------------------------------------------------
; FORCE section tests
;------------------------------------------------------------------------------

.bank 0 slot 0
.org 0
.section "Section1" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF
.ends

.bank 0 slot 0
.org $500
.section "Section2" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF
.ends

.bank 1 slot 1
.org 0
.section "Section3" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF
.ends

.bank 1 slot 1
.org $500
.section "Section4" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF
.ends

;------------------------------------------------------------------------------
; .BASE tests
;------------------------------------------------------------------------------

.bank 0 slot 0
.base $00
.org $100
.section "Section5" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF, (CADDR >> 16) & $ff
.ends

.bank 0 slot 0
.base $80
.org $200
.section "Section6" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF, (CADDR >> 16) & $ff
.ends

.bank 0 slot 0
.base $f0
.org $300
.section "Section7" force
.dw caddr, CADDR, caddr+1, CADDR-1
.db caddr & $ff, (CADDR >> 8) & $FF, (CADDR >> 16) & $ff
.ends
