
        .ramsection "ramValue" slot 1 keep
            ramValue: db
        .ends

        .macro "unnamespacedMacro"
            .dw ramValue   ; FIX_REFERENCES: Reference to an unknown label "foo.ramValue".
        .endm
        
