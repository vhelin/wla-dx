
        .macro "myMacro2" args myArg
          .db myArg+2
        .endm
        
        .macro "myMacro" args myArg
          .db myArg, myArg+1
          myMacro2 myArg
        .endm

        .struct "MyStruct"
          foo: db
        .endst
        
