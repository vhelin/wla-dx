
        .section "someSection" free
someLabel:
someLabel2:
        ret
        .ends

        .macro someMacro
        call someLabel2
        call someLabel2+2
        .endm

        .macro "myMacro" args myArg
        .print "myArg equals ", myArg, "\n"

        .if myArg < 0 || myArg > 15
            .fail "invalid arg"
        .endif

        .db myArg + 1
        .endm
        
