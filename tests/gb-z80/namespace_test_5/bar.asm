
        .define SOME_VALUE2 1
        .redefine SOME_VALUE2 SOME_VALUE2*2-1
        .define SOME_VALUE3 SOME_VALUE2
        .undefine SOME_VALUE3
        
someLabel:
        fooMacro SOME_VALUE
        .db SOME_VALUE
        .db SOME_VALUE2

        .define myArg 15

        .macro "barMacro" args myArg
            .db myArg, \1+1
        .endm

        barMacro 8
        .db myArg

        .struct Bar
          value: db
        .endst

        .ramsection "ram" bank 0 slot 2 keep returnorg
          foo: instanceof Bar
        .ends

        .macro theMacro
          .dstruct instance instanceof Bar values
            value .db 9
          .endst
          .db _sizeof_Bar.value
        .endm

        theMacro
        
