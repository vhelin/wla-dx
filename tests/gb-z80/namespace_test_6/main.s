
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

        .macro "tivoli"
          .print "Tivoli is in town!\n"
        .endm

        .section "fooSection" org $300 force keep
foo:      ld b,b
        .ends
        
        .include "bar.asm"

        .db "01>"               ; @BT TEST-01 01 START
        .include "foo.asm" namespace "foo" ; @BT 00 40 15 21 CD 00 03 CD 00 06
        .db "<01"               ; @BT END

        .include "yum.asm" namespace "yum" isolated

                                ; @BT TEST-04 04 START
                                ; @BT CD 00 03
                                ; @BT CD 00 06
                                ; @BT END

                                ; @BT TEST-03 03 START
                                ; @BT CD 00 03
                                ; @BT CD 00 06
                                ; @BT CD 01 03
                                ; @BT CD 01 06
                                ; @BT END
        
        .section "namSection" org $600 force keep
nam:      ld b,b
        .ends
        
