
;������������������������������������������������������������������������������
; a small example for testing WLA DX's RST parsing functionality
; written by ville helin <ville.helin@iki.fi> in 2019
;������������������������������������������������������������������������������

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

.section "Section-LABELS" FORCE
label_00:
    .db 1
label_01:
    .db 1
label_02:
    .db 1
label_03:
    .db 1
label_04:
    .db 1
label_05:
    .db 1
label_06:
    .db 1
label_07:
.ends

; @BT linked.rom

.section "Section-RSTs" FORCE
.db "01>"                   ; @BT TEST-01 01 START
    RST 0                   ; @BT 05
    RST label_00            ; @BT 05
    RST label_00+1          ; @BT 0D
    RST ONE+1               ; @BT 15
    RST THREE+1+TWO+ONE     ; @BT 3D
.db "<01"                   ; @BT END
.ends
