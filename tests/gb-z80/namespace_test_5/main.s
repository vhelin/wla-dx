
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

        .include "foo.asm"

        .db "01>"               ; @BT TEST-01 01 START
        .include "bar.asm" namespace "bar" isolated ; @BT 06 06 07 06 01 08 09 0F 09 01
        .db myArg                                   ; @BT 0B
        .db "<01"                                   ; @BT END

        .db "02>"               ; @BT TEST-02 02 START
        .include "boo.asm" namespace "boo" isolated
        boo.boo                 ; @BT C3 67 01 00
                                ; @BT C3 6B 01
                                ; @BT C3 6E 01
        .db "<02"               ; @BT END

        .include "nam.asm" namespace "nam" isolated
        nam.myMacro
