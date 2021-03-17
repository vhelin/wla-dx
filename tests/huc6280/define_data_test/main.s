
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example testing different ways to define data with WLA
; written by ville helin <ville.helin@iki.fi> in 2021
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .memorymap
        defaultslot 0
        slotsize $2000
        slot 0 $0000
        slot 1 $2000
        slot 2 $4000
        slot 3 $6000
        slot 4 $A000
        .endme

        .rombankmap
        bankstotal 4
        banksize $2000
        banks 4
        .endro

        .macro increment
        .redefine _out \1+1
        .endm

        .bank 0 slot 0
        .org 0

        .section "TESTS" FORCE KEEP
        
        .db "01>"
        .db 1, 2, 3
        .dsb 3, 2
        .dbm increment 1, 2, 3
        .db "<01"
        .db "02>"
        .dw 1, 2, 3
        .dsw 3, 2
        .dwm increment 1, 2, 3
        .db "<02"
        .db "03>"
        .dl 1, 2, 3
        .dsl 3, 2
        .dlm increment 1, 2, 3
        .db "<03"
        .db "04>"
        .dd 1, 2, 3
        .dsd 3, 2
        .ddm increment 1, 2, 3
        .db "<04"

        .db "05>"
        .db $aa
        .dw $bbcc
        .dl $ddeeff
        .dd $aabbccdd
        .db "<05"

        .db "06>"
        .db label_12+1
        .dw label_1234+1
        .dl label_1234+$560000
        .dd label_1234+$56000000
        .db "<06"

        .db "07>"
        .dbm increment, $fd
        .dwm increment, $fffd
        .dlm increment, $fffffd
        .ddm increment, $fffffffd
        .db "<07"
        
        .ends

        .org $12
label_12:

        .org $1234
label_1234:
