
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
        .DEFINE I2 = 2
        .DEFINE I3 = 3
        .DEFINE I10 = 5+3+2
        .DEFINE I23 = 20+3
        .DEFINE DEF_A = "A"
        .DEFINE DEF_B = "B"
        .DEFINE DEF_C = "C"
        .DEFINE ABC = { "{DEF_A}{DEF_B}{DEF_C}" }
        .DEFINE VALUE_{I0} 0
        .EXPORT VALUE_{I0}

        .SECTION "TEST-01" FORCE        
test_{I1}:
test_{I1}_address_{I0}
        .DB "01("
formatted_{%.4X{I10}}
formatted_{%.4X{I10}}_label:
        .DB "A1B"
{ABC}_{%03d{I23}}:
        .DB ")01"

        .DB "02("
substitution_of_{I10}
        .DB "10"
        .DB ")02"

        .DB "04("
        .DB "ABCDEF"
        .DB ")04"

        .DB "01>"               ; @BT TEST-01 01 START
        .DB substitution_of_{I10}                 ; @BT 0C
        .DB 1 + substitution_of_{I10} + 2 - 1 - 1 ; @BT 0D
        .DB test_{I1}_address_{I0} + 3            ; @BT 03
        .DB formatted_{%.4X{I10}}                 ; @BT 03
        .DB 6 - formatted_{%.4X{I10}} - 2         ; @BT 01
        .DB formatted_{%.4X{I10}}_label           ; @BT 03
        .DB 6 - formatted_{%.4X{I10}}_label - 1   ; @BT 02
        .DB {ABC}_{%03d{I23}}                     ; @BT 06
        .DB 1 + {ABC}_{%03d{I23}} - 2             ; @BT 05
        .DB "<01"               ; @BT END

        .DEFINE COUNT = 10
        .DEFINE UNIT = 5
Label_{%.4x{COUNT}}:
Label_{%03X{COUNT}}_{%03X{UNIT}}:
Label_{%.9d{COUNT}}:
Label_{%03i{COUNT}}:
        
        .DB "02>"               ; @BT TEST-02 02 START
        .DB "{ABC}"             ; @BT 7B 41 42 43 7D
        .DB {"{I1}"}, {"{%i{I2}}"}, {"{%1d{I3}}"} ; @BT 31 32 33
        .DB "<02"               ; @BT END

        .DB "03>"               ; @BT TEST-03 03 START
        .DB {"{ABC}"}           ; @BT 41 42 43
        .DB {   "0{%03X{I10}}0"    } ; @BT 30 30 30 41 30
        .DB { "{ABC}0{%03X{I10}}0" } ; @BT 41 42 43 30 30 30 41 30
        .DB "<03"               ; @BT END
        
        .ENDS
        
        
