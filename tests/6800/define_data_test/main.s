
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

        ; @BT result.rom

        .section "TESTS" FORCE KEEP
        
        .db "01>"                    ; @BT TEST-01 01 START
        .db 10h+1+02h+$20, $20, $30  ; @BT 33 20 30
        .dsb 3, 21h                  ; @BT 21 21 21
        .dbm increment 10h+1, $20, 3 ; @BT 12 21 04
        .db "<01"                    ; @BT END
        .db "02>"                    ; @BT TEST-02 02 START
        .dw 1, 2, 3                  ; @BT 00 01 00 02 00 03
        .dsw 3, 2                    ; @BT 00 02 00 02 00 02
        .dwm increment 1, 2, 3       ; @BT 00 02 00 03 00 04
        .db "<02"                    ; @BT END
        .db "03>"                    ; @BT TEST-03 03 START
        .dl 1, 2, 3                  ; @BT 00 00 01 00 00 02 00 00 03
        .dsl 3, 2                    ; @BT 00 00 02 00 00 02 00 00 02
        .dlm increment 1, 2, 3       ; @BT 00 00 02 00 00 03 00 00 04
        .db "<03"                    ; @BT END
        .db "04>"                    ; @BT TEST-04 04 START
        .dd 1, 2, 3                  ; @BT 00 00 00 01 00 00 00 02 00 00 00 03
        .dsd 3, 2                    ; @BT 00 00 00 02 00 00 00 02 00 00 00 02
        .ddm increment 1, 2, 3       ; @BT 00 00 00 02 00 00 00 03 00 00 00 04
        .db "<04"                    ; @BT END

        .db "05>"               ; @BT TEST-05 05 START
        .db $aa                 ; @BT AA
        .dw $bbcc               ; @BT BB CC
        .dl $ddeeff             ; @BT DD EE FF
        .dd $aabbccdd           ; @BT AA BB CC DD
        .db "<05"               ; @BT END

        .db "06>"                ; @BT TEST-06 06 START
        .db label_12+1           ; @BT 13
        .dw label_1234+1         ; @BT 12 35
        .dl label_1234+$560000   ; @BT 56 12 34
        .dd label_1234+$56000000 ; @BT 56 00 12 34
        .db "<06"                ; @BT END

        .db "07>"                  ; @BT TEST-07 07 START
        .dbm increment, $fd        ; @BT FE
        .dwm increment, $fffd      ; @BT FF FE
        .dlm increment, $fffffd    ; @BT FF FF FE
        .ddm increment, $fffffffd  ; @BT FF FF FF FE
        .db "<07"                  ; @BT END
        
        .ends

        .org $12
label_12:

        .org $1234
label_1234:
