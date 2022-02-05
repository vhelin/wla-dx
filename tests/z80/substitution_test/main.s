
        .MEMORYMAP
        DEFAULTSLOT 0
        SLOTSIZE $2000
        SLOT 0 $0000
        SLOT 1 $2000
        .ENDME

        .ROMBANKMAP
        BANKSTOTAL 2
        BANKSIZE $2000
        BANKS 2
        .ENDRO


        .BANK 0 SLOT 0
        .ORG 0

        
        ; @BT linked.rom
        

        .DEFINE I0 = 0
        .DEFINE I1 = 1
        .DEFINE I10 = 5+3+2
        .DEFINE I23 = 20+3
        .DEFINE ABC = "ABC"
        .DEFINE VALUE_{I0} 0
        .EXPORT VALUE_{I0}

        .SECTION "TEST-01" FORCE        
test_{I1}:
test_{I1}_address_{I0}
        .DB "01("
        .DB "A1B"
        .DB ")01"

        .DB "02("
substitution_of_{I10}
        .DB "10"
        .DB ")02"

        .DB "03("
        .DB "123"
        .DB ")03"

        .DB "04("
        .DB "ABCDEF"
        .DB ")04"

        .DB "01>"               ; @BT TEST-01 01 START
        .DB substitution_of_{I10}                 ; @BT 0C
        .DB 1 + substitution_of_{I10} + 2 - 1 - 1 ; @BT 0D
        .DB test_{I1}_address_{I0} + 3            ; @BT 03
        .DB "<01"               ; @BT END

        .DB "02>"               ; @BT TEST-02 02 START
        .DB "{ABC}"             ; @BT 7B 41 42 43 7D
        .DB "<02"               ; @BT END
        .ENDS
        
        
