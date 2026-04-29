        .macro META_DELETE_TARGET
        .endm

        .macro META_DELETE_CHECK
        .db "06>"               ; @BT TEST-06 06 START
        .db definedmacro("META_DELETE_TARGET") ; @BT 01
        .delmacro META_DELETE_TARGET
        .db definedmacro("META_DELETE_TARGET") ; @BT 00
        .db "<06"               ; @BT END
        .endm

        META_DELETE_CHECK