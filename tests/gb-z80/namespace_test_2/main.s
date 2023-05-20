
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


        
        .include "fileA.asm" namespace "foo" isolated

        .dstruct Instance2 instanceof foo.Instance DATA "tingle", 10, 11
        
        .ramsection "ram" slot 2
foo:      instanceof foo.Instance
        .ends
        

        .include "fileB.asm" namespace "yuh"

        yuh.DefineCockooInstance Cockoo1 1 2
        yuh.DefineCockooInstance Cockoo2 3 4

        .dstruct Cockoo3 instanceof Cockoo data 5 6
        
