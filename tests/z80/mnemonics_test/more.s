
        .memorymap
          slotsize $2000
          defaultslot 1
          slot 0 $0000
          slot 1 $2000
        .endme

        .rombanksize $2000
        .rombanks 2

        .bank 0 slot 0

        .struct Object
        status  db
        data    dsb 8
        .endst

        .section "Section3" after "Section2"
        .dstruct Object2 instanceof Object DATA 2, "MOTO"
        .ends
