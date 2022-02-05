
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
        .DB "01>"               ; @BT TEST-01 01 START
        .DB "A{I1}B"            ; @BT 41 31 42
        .DB "<01"               ; @BT END

        .DB "02>"               ; @BT TEST-02 02 START
substitution_of_{I10}
        .DB "{I10}"             ; @BT 31 30
        .DB "<02"               ; @BT END

        .DB "03>"               ; @BT TEST-03 03 START
        .DB "{I1}{I23}"         ; @BT 31 32 33
        .DB "<03"               ; @BT END

        .DB "04>"               ; @BT TEST-04 04 START
        .DB "{ABC}DEF"          ; @BT 41 42 43 44 45 46
        .DB "<04"               ; @BT END

        .DB "05>"               ; @BT TEST-05 05 START
        .DB substitution_of_{I10}                 ; @BT 0C
        .DB 1 + substitution_of_{I10} + 2 - 1 - 1 ; @BT 0D
        .DB test_{I1}_address_{I0} + 3            ; @BT 03
        .DB "<05"               ; @BT END

        .DB "06>"               ; @BT TEST-06 06 START
        .DB "{I2}"              ; @BT 7B 49 32 7D
        .DB "A{B}C"             ; @BT 41 7B 42 7D 43
        .DB "<06"               ; @BT END
        .ENDS
        
        
