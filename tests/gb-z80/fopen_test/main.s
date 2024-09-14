
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example showing and testing the wla syntax
; written by ville helin <vhelin@cc.hut.fi> in 1998-2003
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INCDIR "includes"
.INCLUDE "gb_memory.i"
.INCLUDE "defines.i"
.INCLUDE  cgb_hardware.i

.BANK 0 SLOT 0
.ORG 0

        ; @BT result.gb
        
        .db "01>"               ; @BT TEST-01 01 START

        .fopen "data2.asc" FP
        .repeat 5
          .fread FP D
          .db D                 ; @BT 61 62 63 64 65
        .endr
        .undefine D

        .db "<01"               ; @BT END


        .db "02>"               ; @BT TEST-02 02 START

        .fopen "data1.asc" FP
        .fsize FP SIZE
        .repeat SIZE
          .fread FP D
          .db D                 ; @BT 31 32 33 34 35 36 37 38 39
        .endr
        .undefine D, SIZE

        .db "<02"               ; @BT END


        .db "03>"               ; @BT TEST-03 03 START

        .fopen "data1/data1.asc" FP
        .fsize FP SIZE
        .repeat SIZE
          .fread FP D
          .db D                 ; @BT 31 32 33 34 35 36 37 38 39
        .endr
        .undefine D, SIZE

        .db "<03"               ; @BT END
        
