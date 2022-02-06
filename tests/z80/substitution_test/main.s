
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

        
        ; @BT linked.rom


        .STRUCT TestStruct
        pos_x  DW
        pos_y  Dw
        health DB
        .ENDST

        .PRINT "_sizeof_TestStruct = ", _sizeof_TestStruct, "\n"

        .BANK 0 SLOT 0
        .ORG 0

        .RAMSECTION "RAMSection" BANK 0 SLOT 0
label1: db
label2: dsb 2
        .ENDS

        .PRINT "_sizeof_label1 = "
        .PRINTV DEC _sizeof_label1
        .PRINT " = "
        .PRINTV DEC (_sizeof_label1+0)
        .PRINT "\n"
        
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

        .BANK 0 SLOT 0
        .ORG 0

        .SECTION "TEST-01" FORCE        
test_{I1}:
test_{I1}_address_{I0}
        .DB "01("
formatted_{%.4X{I10}}
formatted_{%.4X{I10}}_label:
        .DB "A1B"
{ABC}_{%.3d{I23}}:
        .DB ")01"

        .DB "02("
substitution_of_{I10}
        .DB "10"
        .DB ")02"

        .DB "01>"               ; @BT TEST-01 01 START
        .DB substitution_of_{I10}                 ; @BT 0C
        .DB 1 + substitution_of_{I10} + 2 - 1 - 1 ; @BT 0D
        .DB test_{I1}_address_{I0} + 3            ; @BT 03
        .DB formatted_{%.4X{I10}}                 ; @BT 03
        .DB 6 - formatted_{%.4X{I10}} - 2         ; @BT 01
        .DB formatted_{%.4X{I10}}_label           ; @BT 03
        .DB 6 - formatted_{%.4X{I10}}_label - 1   ; @BT 02
        .DB {ABC}_{%.3d{I23}}                     ; @BT 06
        .DB 1 + {ABC}_{%.3d{I23}} - 2             ; @BT 05
        .DB "<01"               ; @BT END

        .DEFINE COUNT = 10
        .DEFINE UNIT = 5
Label_{%.4x{COUNT}}:
Label_{%.3X{COUNT}}_{%.3X{UNIT}}:
Label_{%.9d{COUNT}}:
Label_{%.3i{COUNT}}:

        .MACRO MacroTest
Hack_{\@+2}:
Hack_{\@+1}_Hack:
        .ENDM

        MacroTest
        MacroTest
        MacroTest
        MacroTest
        
        .DB "02>"               ; @BT TEST-02 02 START
        .DB "{ABC}"             ; @BT 7B 41 42 43 7D
        .DB {"{I1}"}, {"{%.1i{I2}}"}, {"{%.1d{I3}}"} ; @BT 31 32 33
        .DB "<02"               ; @BT END

        .DB "03>"               ; @BT TEST-03 03 START
        .DB {"{ABC}"}           ; @BT 41 42 43
        .DB {   "0{%.3X{I10}}0"    } ; @BT 30 30 30 41 30
        .DB { "{ABC}0{%.3X{I10}}0" } ; @BT 41 42 43 30 30 30 41 30
        .DB "<03"               ; @BT END

        .DB "04>"                    ; @BT TEST-04 04 START
        .DB {"{_sizeof_TestStruct}"} ; @BT 35
        .DB {"{_sizeof_label1}"}     ; @BT 31
        .DB "<04"                    ; @BT END

        .db _sizeof_label1
        .ENDS
        
        
