
        .struct Cockoo
          color  db
          weight db
        .endst

        .dstruct CockooX instanceof Cockoo data 98, 99
        
        .macro DefineCockooInstance
        
        .dstruct \1 instanceof Cockoo values
          color:  .db \2
          weight: .db \3
        .endst
        
        .endm
