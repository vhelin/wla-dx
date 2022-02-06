
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
        .PRINT "1ST SUBSTITUTION - START\n"
        .DEFINE ABC = { "{DEF_A}{DEF_B}{DEF_C}" }
        .PRINT "1ST SUBSTITUTION - END\n"
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
Label_{%.9d{COUNT+1}}:
Label_{%.3i{COUNT+1+2+3+4+I10}}:

        .MACRO MacroTest
Hack_{\@+1000}:
Hack_{\@+1}_Hack:
Hack_{3+\@}_More:
        .ENDM

All_{1+1+1+1}_MacroTests1:
All_{I10-6}_MacroTests2:      
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

        .PRINT "I10-1 = ", I10-1, " = 9\n"
        
        .DB "05>"                ; @BT TEST-05 05 START
        .DB {"{1+2+3+4}"}        ; @BT 31 30
        .DB {"{-1+I10}"}         ; @BT 39
        .DB { "{I10-1}" }        ; @BT 39
        .DB { "{I23-I10-I3*4}" } ; @BT 31
        .DB "<05"                ; @BT END

        .DB "06>"                 ; @BT TEST-06 06 START
        .DB { "{I10-%1}" }        ; @BT 39
        .DB { "{I10-$1}" }        ; @BT 39
        .DB { "{I10-0x1}" }       ; @BT 39
        .DB { "{I10-1h}" }        ; @BT 39
        .DB { "{I10-0b00001}" }   ; @BT 39
        .DB { "{-%1+I10}" }       ; @BT 39
        .DB { "{%1+I10-2}" }      ; @BT 39
        .DB { "{-$1+I10}" }       ; @BT 39
        .DB { "{-0x1+I10}" }      ; @BT 39
        .DB { "{-1h+I10}" }       ; @BT 39
        .DB { "{-0b00001+I10}" }  ; @BT 39
        .DB { "{%.1X{I10-%1}}" }  ; @BT 39
        .DB { "{%.1x{%1+I2+6}}" } ; @BT 39
        .DB { "{%.1d{I10-$1}}" }  ; @BT 39
        .DB { "{%.1i{I10-0x1}}" } ; @BT 39
        .DB { "{%.1d{I10-1h}}" }  ; @BT 39
        .DB "<06"                 ; @BT END
        
        .db _sizeof_label1
        .ENDS
        
        
