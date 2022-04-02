
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example showing the wla z80 syntax
; written by ville helin <vhelin@cc.hut.fi> in 1998-2000
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INCLUDE "defines.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.BANK 0 SLOT 0
.ORG 0

.SECTION "BANKHEADER"
        .DW     MAIN            ; 07 00
        .DB     1+2+3           ; 06
        .DW     MAIN+1          ; 08 00
.ENDS



.SECTION "Beginning"

MAIN:   NOP
        JP      MORE_ACTION
.ENDS



.SECTION "Action"

MORE_ACTION:
        EXX                     ; D9
        DEC     A               ; 3D
        JR      NC, MORE_ACTION
        JP      MORE_ACTION

.ENDS


LOOSE:  .DB     $10             ; 10
        .DW     $1122           ; 22 11
        

        .BANK 4 SLOT 1
        .ORG 0

LATER:  .DB     $20             ; 20
        .DW     $3344           ; 44 33
        
