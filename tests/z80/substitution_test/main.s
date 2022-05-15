// MAIN.S
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
        .DEFINE DEF_I = "I"
        .PRINT "1ST SUBSTITUTION - START\n"
        .DEFINE ABC = { "{DEF_A}{DEF_B}{DEF_C}" }
        .PRINT "1ST SUBSTITUTION - END\n"
        .DEFINE VALUE_{I0} 0
        .EXPORT VALUE_{I0}

        .BANK 0 SLOT 0
        .ORG 0

        .SECTION "TESTS-A" FORCE
test_{I1}:
test_{I1}_address_{I0}
        .DB "01("
MyValueIs_{%.1X{ 0x3 }}
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
        .if \@ == 2
          .db "07>"                ; @BT TEST-07 07 START
LastNumberShouldBeTwo_{\@+1+2-3}
LastNumberShouldBeThree_{ \@ + 1 + 3 - 3 }:
InTheMiddleShouldBeFive_{  \@  *  2  +  1  }_IsThere
LastNumbersShouldBeOneTwoThree_{1 + \@ - 2}{%.1d{\@}}{\@+1}
InTheMiddleOneTwoThree_{\@ - 1}{\@}{\@+1}_IsThere
          .db {"{\@ + 3 - 3 + 1}"} ; @BT 33
          .db { "{\@*\@}" }        ; @BT 34
          .db {"{\1+\@}" }         ; @BT 35
          .db "<07"                ; @BT END
        .endif
        .if 0 + \@ + 1 == 2 + 1
        .db "08>"                 ; @BT TEST-08 08 START
        .print "-----------------------------------------------------------------\n"
        .db MyValueIs_{ %11 } + 1 ; @BT 04
        .print "-----------------------------------------------------------------\n"
        .db "<08"                 ; @BT END
        .endif
Hack_{\@+1000}:
Hack_{\@+1}_Hack:
Hack_{3+\@}_More:
        .ENDM

All_{1 + 1+1+1}_MacroTests1:
All_{I10-6}_MacroTests2:      
        MacroTest 1
        MacroTest 2
        MacroTest 3
        MacroTest 4
        
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
        .DB { "{%.1X{ I10 - %1 }}" } ; @BT 39
        .DB { "{%.1x{%1+I2+6}}" } ; @BT 39
        .DB { "{%.1d{I10-$1}}" }  ; @BT 39
        .DB { "{%.1i{I10-0x1}}" } ; @BT 39
        .DB { "{%.1d{I10-1h}}" }  ; @BT 39
        .DB "<06"                 ; @BT END

        .DB _sizeof_label1
        .ENDS
        
        

        .BANK 0 SLOT 0
        .ORG $1000

        .SECTION "TESTS-B" FORCE

        .DB "09>"                 ; @BT TEST-09 09 START
gfxHeader{5 - 1}
gfxHeaderB1{%101 - %1}
gfxHeaderB2{0b101 - 0b1}
gfxHeaderH1{0x5 - 0x1}
gfxHeaderH2{5h - 1h}
gfxHeaderH3{$5 - $1}
        .DW gfxHeader{5 - 1}         ; @BT 03 10
Data{ 1 + 1 * 1 }{%.3d{ %10 + %10 * %10 }}{      $3      +      $3       *        $3    }Label:
Data{   %.4X{   0xBEEF   }  }{ I3 }{ %.4X{ $DEAD }}
        .DW gfxHeaderB1{%101 - %1}   ; @BT 03 10
        .DW gfxHeaderB2{0b101 - 0b1} ; @BT 03 10
        .DW gfxHeaderH1{0x5 - 0x1}   ; @BT 03 10
        .DW gfxHeaderH2{5h - 1h}     ; @BT 03 10
        .DW gfxHeaderH3{$5 - $1}     ; @BT 03 10
.define COUNT2 5
        .DW gfxHeader{COUNT2 - 1}    ; @BT 03 10
        .DB "<09"                    ; @BT END

        .MACRO MAKE_DATA
        .DW \1, COUNT\2
        .ENDM

        .DB "10>"               ; @BT TEST-10 10 START
        MAKE_DATA Data{ 1 + 1 * 1 }{%.3d{%10 + %10 * %10}}{    $3    +    $3     *      $3           }Label, 2 ; @BT 05 10 05 00
        .DW Data{   %.4X{0xBEEF   }  }{ I3 }{ %.4X{ 0xDEAD }} ; @BT 05 10
        .DB "<10"               ; @BT END

        .DB "11>"               ; @BT TEST-11 11 START
        .REPEAT 5 INDEX COUNTER_FROM_{0}_TO_{%.x{ 1 + 1 + 1 + %1 }}
        .DB 4-COUNTER_FROM_{0}_TO_{%.x{ 1 + 1 + 1 + %1 }} ; @BT 04 03 02 01 00
        .ENDR
        .DB "<11"               ; @BT END

        .DB "12>"               ; @BT TEST-12 12 START
        .DEFINE FILE_ID = 0
        .PRINT "FILE_ID = ", FILE_ID, "\n"
        .INCLUDE { "{%.2d{ FILE_ID+1 }}.s" }     ; @BT 01
        .REDEFINE FILE_ID = FILE_ID + 1
        .PRINT "FILE_ID = ", FILE_ID, "\n"
        .INCLUDE {"{%.2d{FILE_ID+1}}.s"}         ; @BT 02
        .REDEFINE FILE_ID = FILE_ID + 1
        .PRINT "FILE_ID = ", FILE_ID, "\n"
        .INCLUDE { "{%.2d{ FILE_ID + 0x1 }}.s" } ; @BT 03
        .REDEFINE FILE_ID = FILE_ID + 1
        .PRINT "FILE_ID = ", FILE_ID, "\n"
        .DEFINE FILE_NAME_{%.2d{1+1-1}} { "{%.2d{ FILE_ID + %00000000000000000000000000000001 }}.s" }
        .INCLUDE FILE_NAME_{%.2d{1}}             ; @BT 04
        .DB "<12"               ; @BT END

        .DB "13>"               ; @BT TEST-13 13 START
        .DB I{1+3-1}-I{%.1d{1+1}}+I{1} ; @BT 02
        .DB {DEF_I}{3}, I{1+1}+1, I{1}       ; @BT 03 03 01
        .DB I{$3}, {DEF_I}{$1+$1}+$1, {DEF_I}{$1}  ; @BT 03 03 01
        .DB I{%11}, I{%1+%1}+%1, I{%1} ; @BT 03 03 01
        .DB "<13"               ; @BT END

        .ENDS

        .DB "14>"               ; @BT TEST-14 14 START
        .REPEAT 5 INDEX COUNTER_FROM_{0}_TO_{%.x{ 1 + 1 + 1 + %1 }}
        .DB 4-COUNTER_FROM_{0}_TO_{%.x{ 1 + 1 + 1 + %1 }} ; @BT 04 FF 03 FF 02 FF 01 FF 00 FF
        .DB $ff
        .ENDR
        .REPEAT 5 INDEX COUNTER_FROM_{0}_TO_{%.x{ 1 + 1 + 1 + %1 }}
        .DB COUNTER_FROM_{0}_TO_{%.x{ 1 + 1 + 1 + %1 }} ; @BT 00 01 02 03 04
        .ENDR
        .DB "<14"               ; @BT END

        .DEFINE SUPERDUPER 1
        
        .DB "15>"               ; @BT TEST-15 15 START
        .IF 0
        .DB "{HAH}"
        .DB { "{HOH}" }
        .ENDIF
        .DB "{HI}"              ; @BT 7B 48 49 7D
        .DB { "{SUPERDUPER}" }  ; @BT 31
        .DB "<15"               ; @BT END
        
        
