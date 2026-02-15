
        tivoli

        // NOTE: we don't run these tests here, they were copied to main.s
        
        .macro "yumMacro"
            .db "04>"           ; @BT TEST-04 04 START
            call foo            ; @BT CD 00 03
            call nam            ; @BT CD 00 06
            .db "<04"           ; @BT END
        .endm

        yumMacro
        
        .section "yum" free keep
            .db "03>"           ; @BT TEST-03 03 START
            call foo            ; @BT CD 00 03
            call nam            ; @BT CD 00 06
            call foo + 1        ; @BT CD 01 03
            call nam + 1        ; @BT CD 01 06
            .db "<03"           ; @BT END
        .ends
