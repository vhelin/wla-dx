
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

        .MACRO .str ARGS str
          .BYTE str.length
        .ENDM

        .db "01>"               ; @BT TEST-01 01 START
        .include "include1.i" namespace i1 ; @BT 05
        .include "include2.i" namespace i2 ; @BT 07
        .db "<01"                          ; @BT END

        .include "foo.asm" namespace "foo" isolated
        .db "02>"               ; @BT TEST-02 02 START
        .include "bar.asm" namespace "bar" isolated ; @BT 03 04 05
        .db "<02"                                   ; @BT END

        .section "init" free keep
          .db "03>"             ; @BT TEST-03 03 START
init:     foo.myMacro 6         ; @BT 06 07 08
          .db "<03"             ; @BT END
-:        jp -
        .ends
        
