
        .macro "namespacedMacro"
            unnamespacedMacro
            .db $15, $21
            call foo
        .endm

        namespacedMacro

        call nam

        tivoli
