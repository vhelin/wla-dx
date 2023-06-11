
        .define SOME_VALUE 6

        .macro "fooMacro" args myArg
            .db myArg, \1, \1+1
        .endm
        
