
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
        .define VALUE_5 = max(max(VALUE_1, VALUE_4+1), min(min(VALUE_1+0, 1+VALUE_3-1), max(VALUE_4-1, 0+1+2-VALUE_3)))
        .define VALUE_15 = VALUE_16 - VALUE_1
        .define VALUE_16 = max(max(VALUE_1, VALUE_4*4), min(min(VALUE_1+0, 1+VALUE_3-1), max(VALUE_4-1, 0+1+2-VALUE_3)))
        .define FIVE_POINT_FIVEONE = 5.51
        .define POINT_FIVE = 0.5
        
        .export VALUE_3, VALUE_4, VALUE_5, VALUE_15, VALUE_16, FIVE_POINT_FIVEONE, POINT_FIVE
