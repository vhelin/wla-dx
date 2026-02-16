
; testing .continue

.MEMORYMAP
DEFAULTSLOT 1
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $6000
SLOT 2 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 2
BANKSIZE $2000
BANKS 1
BANKSIZE $6000
BANKS 1
.ENDRO

.EMPTYFILL $ff
        
.BANK 0 SLOT 0
.ORGA 0

; @BT linked.gb

        .db "01>"               ; @BT TEST-01 01 START

        .macro test_case1
-         nop                   ; @BT 00
          jr -                  ; @BT 18 FD
        .endm

        test_case1

        .macro test_case2 ARGS foo

-         nop                   ; @BT 00
          jr -                  ; @BT 18 FD
        .endm

        test_case2
        
        .macro test_case3 ARGS foo
-         nop                   ; @BT 00
          jr -                  ; @BT 18 FD
        .endm

        test_case3

        .db "<01"               ; @BT END
