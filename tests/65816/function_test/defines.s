
//////////////////////////////////////////////////////////////////////
// function tests
//////////////////////////////////////////////////////////////////////
        
        .memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
        .endme                    

        .lorom

        .rombanksize $8000
        .rombanks 3

//////////////////////////////////////////////////////////////////////
// definitions
//////////////////////////////////////////////////////////////////////

        .bank 0 slot 0
        .org 0

        .function sumAB(var1,var2) var1+var2

        .define VALUE_1 = sumAB(3,-1)-1
        .define VALUE_3 = VALUE_1 + sumAB(VALUE_2,0)
        .define VALUE_4 = VALUE_1*4
        
        .export VALUE_3, VALUE_4
        
