
        .str "hello"
        
        .struct theStruct
          value: db
        .endst

        .ramsection "ram" bank 0 slot 2 keep returnorg
          zoo: instanceof theStruct
        .ends

        .macro theMacro
          .dstruct instance instanceof theStruct values
            value .db 9
          .endst
        .endm
        
