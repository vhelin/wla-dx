
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
        
        .struct Foo
          bar_1: db
          bar_2: db
        .endst

        .macro "MyMacro3"
        .db Foo.bar_1
        .db Foo.bar_2
        .db Foo.bar_1+1
        .db Foo.bar_2+1
        .endm
        
