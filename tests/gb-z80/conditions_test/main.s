
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; This example means to test conditions in general in WLA DX
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .MEMORYMAP
        DEFAULTSLOT 1
        SLOT 0 $0000 $2000
        SLOT 1 STArT $2000 sIzE $6000
        .ENDME

        .ROMBANKMAP
        BANKSTOTAL 2
        BANKSIZE $2000
        BANKS 1
        BANKSIZE $6000
        BANKS 1
        .ENDRO

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .BANK 0 SLOT 0
        .ORG 0

        ; @BT linked.gb

        .db "00>"               ; @BT TEST-00 00 START
        .db 1 == 2              ; @BT 00
        .db 1 != 1              ; @BT 00
        .db 1 == (0+1)          ; @BT 01
        .db (1 > 2) || (1 < 2)  ; @BT 01
        .db 1 > 2 || 1 < 2      ; @BT 01
        .db (1 > 2) && (1 < 2)  ; @BT 00
        .db (2 > 1) && (1 < 2)  ; @BT 01
        .db !(1 == 2)           ; @BT 01
        .db !1                  ; @BT FE
        .db !(!1)               ; @BT 01
        .db !(0+0)              ; @BT FF
        .db !(1 > 0)            ; @BT 00
        .db !(!(1 > 0))         ; @BT 01
        .db "<00"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .db "01>"               ; @BT TEST-01 01 START
        .db 0 > 1               ; @BT 00
        .db 1 || 0              ; @BT 01
        .db (0 == 1) || (1 != 2) ; @BT 01
        .db 0 > 1 || 0 < 2      ; @BT 01
        .db (0 > 1) || (0 < 2)  ; @BT 01
        .db 0 > 1 && 0 < 2      ; @BT 00
        .db (0 > 1) || (2 < 1) || (1 != 2) || (1 == 2) ; @BT 01
        .db (0 > 1) || (2 < 1) || !(1 != 2) || (1 == 2) ; @BT 00
        .db !((0 > 1) || (2 < 1) || (1 != 2) || (1 == 2)) ; @BT 00
        .db (!1 || 1-1) + 2     ; @BT 02
        .db "<01"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .define ZERO = 0
        .define ONE  = 1

        ////////////////////////////////////////////////////////////////////////

        .db "02>"               ; @BT TEST-02 02 START
        .db !ZERO && ONE && !ZERO && (ZERO+ZERO*2+ONE) ; @BT 01
        .db ZERO == 1-1 && ONE == 1 && !ZERO && !(ZERO+ZERO*2) ; @BT 01
        .db !(!(!(ZERO == 1-1 && ONE == 1 && !ZERO && !(ZERO+ZERO*2)))) ; @BT 00
        .db 1 & 3 && 1 | 0 && 7 & 1 ; @BT 01
        .db !1 + 3 && 1 << 0 && !(!1) ; @BT 01
        .db "<02"               ; @BT END
        
        ////////////////////////////////////////////////////////////////////////

        .db "03>"               ; @BT TEST-03 03 START

        .db defined(ZERO) && defined(ONE) && !defined(TWO) ; @BT 01
        .db defined(TWO) || defined(THREE) || !defined(ZERO) ; @BT 00
        .db defined(ONE)                                     ; @BT 01
        .db !defined(ONE)                                    ; @BT FE
        .db defined(THREE) == 0                              ; @BT 01
        .db defined(ZERO) == 1                               ; @BT 01
        .db "<03"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .db "04>"               ; @BT TEST-04 04 START
        .db exists("main.s")    ; @BT 01
        .db !exists("main.s")   ; @BT FE

        .define TESTSFILE "testsfile"

        .db !exists("main.txt") && 1 && exists("linkfile") ; @BT 01
        .db exists("main.txt") || 0 & 1 || exists("linkfile") ; @BT 01
        .db !defined(HELLO) && defined(ONE) && (ONE > 0)      ; @BT 01
        .db "<04"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .define MONSTER "terrrmurrrrr"
        .define DUMMY "testing", 1, 2, 3
        .define FILE "main.s"
        .define VERSION 105

        .db "05>"               ; @BT TEST-05 05 START
        .db !(ONE == ZERO) * 2  ; @BT 02
        .db (ONE == ZERO) + (ONE == 1) + (1+ONE < 1-ONE) ; @BT 01
        .db (ZERO > -1) + !(ZERO == 0) + (TESTSFILE != "jormas.txt")*2 ; @BT 03
        .db ("ABC" != "ABX") - ("XXX" <= "ABC") ; @BT 01
        .db (0) - (ONE) - (ZERO) - (1)          ; @BT FE
        .db (0*ONE) - (ONE-ZERO) - (ZERO+1-ONE) - (-1+ONE+1) ; @BT FE
        .db ("terrrmurrrrr" != MONSTER) - (MONSTER < DUMMY) - ("ABC" != "ABX") - ("XXX" <= "ABC") ; @BT FE
        .db ("terrrmurrrrr" != MONSTER) - (MONSTER < DUMMY) - ("ABC" != "ABX") - ("XXX" <= "ABC") -
            ("terrrmurrrrr" != MONSTER) - (MONSTER < DUMMY) - ("ABC" != "ABX") - ("XXX" <= "ABC") ; @BT FC
        .db -("XXX" > "ABC")    ; @BT FF
        .db -(-("XXX" > "ABC")) ; @BT 01
        .db -(-(-("XXX" > "ABC"))) ; @BT FF
        .db -(-(-("XXX" > "ABC"))) - (-(-("XXX" > "ABC"))) ; @BT FE
        .db "<05"               ; @BT END
        
        ////////////////////////////////////////////////////////////////////////
        
        .db "06>"               ; @BT TEST-06 06 START

        .db "ABCD" < "ABCE"     ; @BT 01
        .db "ABCE" < "ABCD"     ; @BT 00
        .db "ABCE" > "ABCD"     ; @BT 01
        .db "ABCD" > "ABCE"     ; @BT 00
        .db "ABCD" <= "ABCD"    ; @BT 01
        .db "ABCD" <= "ABCE"    ; @BT 01
        .db "ABCD" >= "ABCD"    ; @BT 01
        .db "ABCE" >= "ABCD"    ; @BT 01
        .db "ABCD" == "ABCD"    ; @BT 01
        .db "ABCE" != "ABCD"    ; @BT 01
        .db "HELLO" == "ABCD"   ; @BT 00
        .db "ABCE" != "ABCE"    ; @BT 00
        .db "<06"               ; @BT END

        ////////////////////////////////////////////////////////////////////////
        
        .db "07>"               ; @BT TEST-07 07 START
        .db MONSTER >= DUMMY    ; @BT 00
        .db "terrrmurrrrr" != MONSTER || MONSTER < DUMMY && "ABC" != "ABX" || "XXX" <= "ABC" ; @BT 01
        .db FILE == "main.s" && VERSION > 102 ; @BT 01
        .db "HELLO" != MONSTER && "1234" < "5678" && VERSION == VERSION || (!1) ; @BT 01
        .db !("main.o" != FILE) ; @BT 00
        .db "<07"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .db "08>"               ; @BT TEST-08 08 START
        .db 0 || VALUE_1        ; @BT 01
        .db VALUE_3 && VALUE_1  ; @BT 01
        .db VALUE_0 && VALUE_1  ; @BT 00
        .db (0+2-1-1) || (1+VALUE_1*2-2) ; @BT 01
        .db (VALUE_1*3) && (VALUE_5-VALUE_4) ; @BT 01
        .db VALUE_1 & VALUE_3 && VALUE_1 | VALUE_0 && 7 & VALUE_1 ; @BT 01
        .db STRING_CELLO < "hello" ; @BT 01
        .db "cello" > STRING_HELLO ; @BT 00
        .db VALUE_0 < VALUE_1      ; @BT 01
        .db VALUE_0 > VALUE_1      ; @BT 00
        .db (VALUE_0 < VALUE_1) && 1 ; @BT 01
        .db (VALUE_0 > VALUE_1) || (VALUE_0 < VALUE_1) ; @BT 01
        .db (STRING_CELLO < "hello") && (STRING_CELLO > STRING_HELLO) ; @BT 00
        .db (STRING_CELLO < "hello") || (STRING_CELLO > STRING_HELLO) ; @BT 01
        .db (STRING_CELLO < "hello") < 2 ; @BT 01
        .db (STRING_CELLO < "hello") > 2 ; @BT 00
        .db (STRING_CELLO < "hello") > 0 ; @BT 01
        .db (STRING_CELLO < "hello")*2 ; @BT 02
        .db "<08"               ; @BT END
        
        .db "09>"               ; @BT TEST-09 09 START
        .db 0 == VALUE_0        ; @BT 01
        .db 0 == VALUE_1        ; @BT 00
        .db 0 != VALUE_0        ; @BT 00
        .db 0 != VALUE_1        ; @BT 01
        .db (0 == VALUE_0) || (0 == VALUE_1) ; @BT 01
        .db (0 == VALUE_0) && (0 == VALUE_1) ; @BT 00
        .db (0 == VALUE_0) > (0 == VALUE_1) ; @BT 01
        .db (0 == VALUE_0) < (0 == VALUE_1) ; @BT 00
        .db "<09"               ; @BT END

        .db "10>"               ; @BT TEST-10 10 START
        .db DEFINITION_VALUE_1 + 1 ; @BT 02
        .db VALUE_1 + DEFINITION_VALUE_1 + 1 ; @BT 03
        .db "<10"                  ; @BT END

        .db "11>"               ; @BT TEST-11 11 START
        .db STRING_CELLO <= "hello" ; @BT 01
        .db "cello" >= STRING_HELLO ; @BT 00
        .db VALUE_0 <= VALUE_1      ; @BT 01
        .db VALUE_0 >= VALUE_1      ; @BT 00
        .db (VALUE_0 <= VALUE_1) && 1 ; @BT 01
        .db (VALUE_0 >= VALUE_1) || (VALUE_0 <= VALUE_1) ; @BT 01
        .db (STRING_CELLO <= "hello") && (STRING_CELLO >= STRING_HELLO) ; @BT 00
        .db (STRING_CELLO <= "hello") || (STRING_CELLO >= STRING_HELLO) ; @BT 01
        .db (STRING_CELLO <= "hello") <= 2 ; @BT 01
        .db (STRING_CELLO <= "hello") >= 2 ; @BT 00
        .db (STRING_CELLO <= "hello") >= 0 ; @BT 01
        .db (STRING_CELLO <= "hello")*2 ; @BT 02
        .db STRING_HELLO >= "hello"     ; @BT 01
        .db STRING_HELLO <= "hello"     ; @BT 01
        .db STRING_HELLO == "hello"     ; @BT 01
        .db STRING_CELLO == "hello"     ; @BT 00
        .db STRING_HELLO != "hello"     ; @BT 00
        .db STRING_CELLO != "hello"     ; @BT 01
        .db (STRING_HELLO != "hello") || (STRING_HELLO >= "hello") ; @BT 01
        .db (STRING_HELLO != "hello")*100 || (STRING_HELLO >= "hello")*9 ; @BT 01
        .db ((STRING_HELLO != "hello") || (STRING_HELLO >= "hello"))*2 ; @BT 02
        .db "<11"                      ; @BT END

        .db "12>"               ; @BT TEST-12 12 START
        .macro define_byte args argument
        .db \1, argument, \1+1, argument+2
        .endm
        define_byte VALUE_0 <= VALUE_1      ; @BT 01 01 02 03
        define_byte (STRING_CELLO <= "hello") && (STRING_CELLO >= STRING_HELLO) ; @BT 00 00 01 02
        define_byte (STRING_CELLO <= "hello") || (STRING_CELLO >= STRING_HELLO) ; @BT 01 01 02 03
        define_byte ((STRING_CELLO <= "hello") || (STRING_CELLO >= STRING_HELLO) && ("hello" >= STRING_CELLO)) ; @BT 01 01 02 03
        define_byte (STRING_HELLO != "hello") || (STRING_HELLO >= "hello") ; @BT 01 01 02 03
        define_byte (STRING_HELLO == "hello")*3 ; @BT 03 03 04 05
        define_byte ((STRING_HELLO != "hello") || (STRING_HELLO >= "hello"))*2 ; @BT 02 02 03 04
        define_byte ((STRING_HELLO != "hello") || (STRING_HELLO >= "hello"))*2 ; @BT 02 02 03 04
        define_byte(STRING_HELLO == "hello") ; @BT 01 01 02 03
        define_byte((STRING_HELLO == "hello")*3) ; @BT 03 03 04 05
        define_byte(((STRING_HELLO != "hello") || (STRING_HELLO >= "hello"))*2) ; @BT 02 02 03 04
        .db "<12"                           ; @BT END

        .db "13>"               ; @BT TEST-13 13 START
        .db -(1+2) - (-(-(-1))) ; @BT FE
        .db -(1+2) < -(-(-1))   ; @BT 01
        .db (-(1+2)) < (-(-(-1))) ; @BT 01
        .db (-(1+2)) < (-(-(-1))) && (-(1+2)) < (-(-(-1))) ; @BT 01
        .db (-(1+2)) < (-(-(-1))) && (-(1+2)) > (-(-(-1))) ; @BT 00
        .db (-(VALUE_1+2)) < (-(-(-1))) && (-(1+VALUE_2)) < (-(-(-1))) ; @BT 01
        .db (-(1+VALUE_2)) < (-(-(-1))) && (-(1+2)) > (-(-(-VALUE_1))) ; @BT 00
        .db (-(1+VALUE_2)) < (-(-(-1))) > (-(1+2)) > (-(-(-VALUE_1))) ; @BT 01
        .db -(-1+1) || -(2-1)   ; @BT 01
        .db -(-1+1) || -(VALUE_2-1) ; @BT 01
        .db -(-1+1) || -(VALUE_2-2) ; @BT 00
        .db -(-1+1) || -1       ; @BT 01
        .db "<13"               ; @BT END

        .db "14>"               ; @BT TEST-14 14 START
        .macro negative args argument
        .db -\1, -argument
        .endm
        negative 2 > 1          ; @BT FF FF
        negative (2 > 1) > 0    ; @BT FF FF
        negative ((2 > 1) > 0) < 2 ; @BT FF FF
        negative ((2 > 1) > 0) < (2 < 3)*2 ; @BT FF FF
        negative ((2 > VALUE_1) > VALUE_0) < (2 < 3)*2 ; @BT FF FF
        negative ((2 > 1) > 0) > (2 < 3)*2 ; @BT 00 00
        negative ((2 > 1) > 0) || (2 < 3)*2 || -(2-1-1) ; @BT FF FF
        negative ((2 > 1) > 0) || -(2-1-1) ; @BT FF FF
        negative(((2 > 1) > 0) || -(VALUE_2-1-1)) ; @BT FF FF
        .db 2 > 1               ; @BT 01
        .db "<14"               ; @BT END

        .function ADD(varA,varB) (varA+varB)
        .function ADD1(varA) (ADD(varA,1))
        .function SUB1(varA) (varA-1)

        .db "15>"               ; @BT TEST-15 15 START
        .db ADD1(1) > SUB1(2)   ; @BT 01
        .db ADD1(1) < SUB1(2)   ; @BT 00
        .db (ADD1(1) > SUB1(2)) && (-ADD1(1+1) < -SUB1(4-1)) ; @BT 01
        .db (ADD1(1) > SUB1(2)) || (-ADD1(1+1) > -SUB1(4-1)) ; @BT 01
        negative ADD1(1) > SUB1(2)                           ; @BT FF FF
        negative ADD1(1) < SUB1(2)                           ; @BT 00 00
        negative (ADD1(1) > SUB1(2))                         ; @BT FF FF
        negative (ADD1(1) < SUB1(2))                         ; @BT 00 00
        negative(ADD1(1) > SUB1(2))                          ; @BT FF FF
        negative(ADD1(1) < SUB1(2))                          ; @BT 00 00
        .db ADD1(VALUE_5) > SUB1(2)   ; @BT 01
        .db ADD1(1) < SUB1(VALUE_2)   ; @BT 00
        .db (ADD1(1) > SUB1(VALUE_2)) && (-ADD1(1+1) < -SUB1(4-1)) ; @BT 01
        .db (ADD1(1) > SUB1(VALUE_2)) || (-ADD1(1+VALUE_1) > -SUB1(4-1)) ; @BT 01
        .db (ADD1(ADD1(0)) > SUB1(VALUE_1+1)) && (-ADD1(SUB1(2+1)) < -SUB1(4-1)) ; @BT 01
        .db "<15"               ; @BT END

        .db "16>"               ; @BT TEST-16 16 START

        .if (ADD1(1) > SUB1(2)) && (-ADD1(1+1) < -SUB1(4-1))
        .db 1                   ; @BT 01
        .else
        .db 2
        .endif

        .if (ADD1(1) > SUB1(2)) && (-ADD1(1+1) > -SUB1(4-1))
        .db 1
        .else
        .db 2                   ; @BT 02
        .endif

        .if (ADD1(1) > SUB1(2)) && (-ADD1(1+1) > -SUB1(4-1))
        .db 1
        .elif (ADD1(1) > SUB1(2)) && (-ADD1(1+1) < -SUB1(4-1))
        .db 2                   ; @BT 02
        .else
        .db 3
        .endif

        .macro ifmacro
        .assert \1 == 0 || \1 == 1
        .if \1
        .db 1
        .else
        .db 2
        .endif
        .endm

        .macro notifmacro
        .assert \1 == 0 || \1 == 1
        .if !\1
        .db 1
        .else
        .db 2
        .endif
        .endm

        .macro notifmacro2 args condition
        .assert condition == 0 || condition == 1
        .if !condition
        .db 1
        .else
        .db 2
        .endif
        .endm
        
        ifmacro (ADD1(1) > SUB1(2)) && (-ADD1(1+1) < -SUB1(4-1)) ; @BT 01
        ifmacro (ADD1(1) > SUB1(2)) && (-ADD1(1+1) > -SUB1(4-1)) ; @BT 02
        notifmacro (ADD1(1) > SUB1(2)) && (-ADD1(1+1) < -SUB1(4-1)) ; @BT 02
        notifmacro (ADD1(1) > SUB1(2)) && (-ADD1(1+1) > -SUB1(4-1)) ; @BT 01
        notifmacro2 (ADD1(1) > SUB1(2)) && (-ADD1(1+1) < -SUB1(4-1)) ; @BT 02
        notifmacro2 (ADD1(1) > SUB1(2)) && (-ADD1(1+1) > -SUB1(4-1)) ; @BT 01

        .define VALUE_0x1234 0x1234

        .assert (<VALUE_0x1234 == 0x34)
        
        .db "<16"               ; @BT END
        
