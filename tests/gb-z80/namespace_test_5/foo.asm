
        .define SOME_VALUE 6
        .define SOME_VALUE2 5

        .define myArg 11

        .macro "fooMacro" args myArg
            .db myArg, \1, \1+1
        .endm
