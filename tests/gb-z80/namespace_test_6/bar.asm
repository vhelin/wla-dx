
        ; @BT linked.gb
        
        .ramsection "ramValue" slot 1 keep
            ramValue: db
        .ends

        .macro "unnamespacedMacro"
            .dw ramValue   ; FIX_REFERENCES: Reference to an unknown label "foo.ramValue".
        .endm

        .section "barSection" free keep
            .db "02>"           ; @BT TEST-02 02 START
            call foo            ; @BT CD 00 03
            .db "<02"           ; @BT END
        .ends
