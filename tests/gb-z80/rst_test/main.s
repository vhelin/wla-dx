
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing WLA DX's RST parsing functionality
; written by ville helin <ville.helin@iki.fi> in 2019
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

.section "Section-LABELS" FORCE
label_00:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_08:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_10:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_18:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_20:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_28:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_30:
	.db 1, 2, 3, 4, 5, 6, 7, 8
label_38:
.ends

; @BT linked.rom

.section "Section-RSTs" FORCE
.db "01>"            ; @BT TEST-01 01 START
	RST 0            ; @BT C7
	RST label_00     ; @BT C7
	RST label_08+8   ; @BT D7
	RST 8+label_10   ; @BT DF
	RST label_28+$10 ; @BT FF
.db "<01"            ; @BT END
.ends

        ; .ROMSIZE test
        .org $0148-3
        .db "02>"       ; @BT TEST-02 02 START
        .org $0148+1    ; @BT 11
        .db "<02"       ; @BT END
        
