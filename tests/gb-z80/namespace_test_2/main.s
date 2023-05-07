
.OUTNAME "main.o"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; project includes
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INCLUDE "gb_memory1.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.BANK 0 SLOT 0
.ORG $150

        .include "macro.s" namespace foo

        .section "main" free keep
        call foo.someLabel
        foo.someMacro
        foo.myMacro 1
        .ends

        .macro "myMacro2" args myArg2
        .print "myArg2 equals ", myArg2, "\n"

        .if myArg2 < 0 || myArg2 > 15
            .fail "invalid arg"
        .endif

        .db myArg2 + 1
        .endm

        myMacro2 2



        .include "helo.s" namespace "helo" isolated
        .include "moto.s" namespace "moto" isolated
        
        .section "main2" free keep
        call moto.barLabel
        .db helo.VALUE
        .ends

        
