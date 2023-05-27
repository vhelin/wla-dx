
        .section "someSection" free
someLabel:
someLabel2:
        ret
        .ends

        .macro someMacro
        call foo.someLabel2
        call foo.someLabel2+2
        .endm

        .macro "myMacro" args myArg
        .print "myArg equals ", myArg, "\n"

        .if myArg < 0 || myArg > 15
            .fail "invalid arg"
        .endif

        .db myArg + 1
        .endm
        
        .define HIYA 1
        .db HIYA, HIYA+1
        .export HIYA
        
