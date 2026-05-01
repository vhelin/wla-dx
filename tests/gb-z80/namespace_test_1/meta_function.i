        .function META_FUNCTION() 7

        .db "05>"               ; @BT TEST-05 05 START
        .db definedfunction("META_FUNCTION"), definedfunction("metafunc.META_FUNCTION") ; @BT 01 01
        .db "<05"               ; @BT END