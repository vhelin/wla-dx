
        .macro "macroA2" args someArg
                .db someArg
        .endm

        .macro "macroB2" args someArg
                .db someArg
                macroA2 4
                .db someArg
        .endm

        .macro "caller"
                macroB2 3
        .endm
                
