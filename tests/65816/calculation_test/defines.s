
        .memorymap
          slotsize $8000
          defaultslot 1
          slot 0 $0000
          slot 1 $8000
        .endme

        .lorom

        .rombanksize $8000
        .rombanks 3
        
        .define VALUE_7 = VALUE_3*2+1
        .define VALUE_8 = -VALUE_7+8+7

        .export VALUE_7
        .export VALUE_8
        
