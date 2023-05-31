
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; This example means to test "namespaces" in WLA DX
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .OUTNAME "main.o"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; project includes
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .INCLUDE "gb_memory1.i"
        .INCLUDE "defines1.i"
        .INCLUDE "cgb_hardware.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .BANK 0 SLOT 0
        .ORG $150

        ; @BT linked.gb

        .macro "macroA" args someArg
                .db someArg
        .endm

        .macro "macroB" args someArg
                .db someArg
                macroA 2
                .db someArg
        .endm

        .macro "caller"
                .db "01>"       ; @BT TEST-01 01 START
                macroB 1        ; @BT 01 02 01
                .db "<01"       ; @BT END
        .endm

        caller

        ; --------------------------------------------------------------------

        .include "include1.i" namespace i1

        i1.caller

        .db "02>"    ; @BT TEST-02 02 START
        .include "include2.i" namespace i2
                     ; @BT 03 04 03
        .db "<02"    ; @BT END
