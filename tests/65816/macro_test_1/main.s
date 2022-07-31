
.memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

.DEF prev_term $0000

.MACRO .term ARGS str
__\._\@:
        .WORD  prev_term
        .REDEF prev_term CADDR - 2
        .BYTE  str.length, str, 0
.ENDM

.DEF prev_test $0000
        
.MACRO .test ARGS str
__\._{\@+1}:
        .PRINT __\._{\@+1}, "\n"
        .WORD  prev_test
        .REDEF prev_test __\._{\@+1}
        .BYTE  str.length, str, 0
        .DW __\._{\@+1}
        .DW __\._{\@+1}+1
.ENDM

.MACRO .storager
        .DB \@, \@+1, 1+\@+1, \@-1
.ENDM

; @BT result.rom
        
.bank 0 slot 1
.section "Bank0" force
main:
        .db "01>"       ; @BT TEST-01 01 START
        .term "YES"     ; @BT 00 00 03 59 45 53 00
        .db "<01"       ; @BT END

        .db "02>"       ; @BT TEST-02 02 START
        .term "NO"      ; @BT 0E 80 02 4E 4F 00
        .db "<02"       ; @BT END

        .db "03>"       ; @BT TEST-03 03 START
        .term "HELLO"   ; @BT 1A 80 05 48 45 4C 4C 4F 00
        .db "<03"       ; @BT END

        .db "04>"       ; @BT TEST-04 04 START
        .test "JA"      ; @BT 00 00 02 4A 41 00 2B 80 2C 80
        .db "<04"       ; @BT END

        .db "05>"       ; @BT TEST-05 05 START
        .test "NEIN"    ; @BT 2B 80 04 4E 45 49 4E 00 3B 80 3C 80
        .db "<05"       ; @BT END

        .db "06>"       ; @BT TEST-06 06 START
        .test "HALLO"   ; @BT 3B 80 05 48 41 4C 4C 4F 00 4D 80 4E 80
        .db "<06"       ; @BT END

        .db "07>"       ; @BT TEST-07 07 START
        .storager       ; @BT 00 01 02 FF
        .db "<07"       ; @BT END

MIDDLE1 = 1+2+3
MIDDLE2= 1+1+1
        MIDDLE3 = 1+2
MIDDLE4 =1+1-1
MIDDLE5=2+1+1
        MIDDLE6=1+1
        MIDDLE7= 1+1
        MIDDLE8 =1+1

        .db "08>"               ; @BT TEST-08 08 START
        .db MIDDLE1 + 1,        ; @BT 07
        .db 1 + MIDDLE2 + 1,    ; @BT 05
        .db MIDDLE3 + 1         ; @BT 04
        .db 2 + MIDDLE4 +1,     ; @BT 04
        .db 3 + MIDDLE5 + 1     ; @BT 08
        .db 1 + MIDDLE6 + 1,    ; @BT 04
        .db 2 + MIDDLE7 + 2,    ; @BT 06
        .db 3 + MIDDLE8 + 3     ; @BT 08
        .db "<08"               ; @BT END
.ends

.macro MACROX   
        .dw Start\1
        .dw Start\1End
        .dw \1Start
        .db :Start\1 + 1
        .db :Start\1End + 1
        .db :\1Start + 1
.endm

.section "TestingMACROX" force
StartHello:
StartHelloEnd:
HelloStart:
        .db "09>"       ; @BT TEST-09 09 START
        MACROX Hello    ; @BT 75 80 75 80 75 80 01 01 01
        .db "<09"       ; @BT END
.ends

.macro .differentThings1
        .if \?1 == ARG_NUMBER
                .db 1
        .elif \?1 == ARG_STRING
                .db 2
        .elif \?1 == ARG_LABEL
                .db 3
        .elif \?1 == ARG_PENDING_CALCULATION
                .db 4
        .else
                .fail "WLA IS BROKEN!"
        .endif
.endm
        
.section "TestingDifferentThings1"
        .db "10>"                       ; @BT TEST-10 10 START
TDT1:   .differentThings1 100           ; @BT 01
        .differentThings1 "HELLO"       ; @BT 02
        .differentThings1 TDT1          ; @BT 03
        .differentThings1 TDT1+1        ; @BT 04
        .db "<10"                       ; @BT END
.ends

.macro .differentThings2
.if \?2 == ARG_NUMBER
        .db 1
.endif
.if \?2 == ARG_STRING
        .db 2
.endif
.if \?2 == ARG_LABEL
        .db 3
.endif
.if \?2 == ARG_PENDING_CALCULATION
        .db 4
.endif
.endm

.macro REFERENCE_LABELS
  .dw LABEL_{\@+1}
  .dw LABEL_{\@+2}
  .dw LABEL_{\@+3}
  .dw LABEL_\@+1
  .dw LABEL_{\@+1}+1
.endm

.section "TestingDifferentThings2"
        .db "11>"                               ; @BT TEST-11 11 START
TDT2:   .differentThings2 TDT2+1, 100           ; @BT 01
        .differentThings2 TDT2, "HELLO"         ; @BT 02
        .differentThings2 "HELLO", TDT2         ; @BT 03
        .differentThings2 100, TDT2+1           ; @BT 04
        .db "<11"                               ; @BT END

LABEL_0 nop
LABEL_1 nop
LABEL_2 nop
LABEL_3 nop

        .db "12>"               ; @BT TEST-12 12 START
        REFERENCE_LABELS        ; @BT 8F 80 90 80 91 80 8F 80 90 80
        .db "<12"               ; @BT END

.MACRO  _set
        lda #\1
        sta main.w, x
.ENDM

.DEF SOME_CONST 1
        
        .db "13>"               ; @BT TEST-13 13 START
        _set SOME_CONST         ; @BT A9 01 9D 00 80
        .db "<13"               ; @BT END
.ends

        .MACRO TEST
        .IF NARGS == 0          ; inside a .MACRO NARGS holds the number of arguments
            .DB 0               ; the .MACRO was called with
        .ELIF NARGS == 1
            .DB \1
        .ELIF NARGS == 2
            .DB \1, \2
        .ELIF NARGS == 3
            .DB \1, \2, \3
        .ENDIF
        .ENDM

        .db "14>"               ; @BT TEST-14 14 START
        TEST                    ; @BT 00
        TEST 1                  ; @BT 01
        TEST 2, 3               ; @BT 02 03
        TEST 4, 5, 6            ; @BT 04 05 06
        .db "<14"               ; @BT END

        ///////////////////////////////////////////////////////////////////////////////
        // .MACRO isolation tests
        ///////////////////////////////////////////////////////////////////////////////

        // 1
        
        .bank 0 slot 1
        .orga $A000

        .macro INSIDEISOLATEDMACRO ISOLATED
@Son2:
        .endm
        
        .macro ISOLATIONTEST1 ISOLATED
IAmIsolated:
@Son2:  .dw IAmIsolated
@Son1:
        INSIDEISOLATEDMACRO
        .dw IAmIsolated@Son1
        .dw IAmIsolated@Son2
        .endm

        .db "15>"               ; @BT TEST-15 15 START
BaseMaster:
        ISOLATIONTEST1          ; @BT 03 A0 05 A0 03 A0
@Child1:.dw BaseMaster@Child1   ; @BT 09 A0
        .db "<15"               ; @BT END
        
        // 2

        .bank 0 slot 1
        .orga $A100

        .macro LOCALREFERENCE1 ISOLATED
-       .dw -
-       .dw -
        .endm

        .db "16>"               ; @BT TEST-16 16 START
-       .db 0                   ; @BT 00
        LOCALREFERENCE1         ; @BT 04 A1 06 A1
        .dw -                   ; @BT 03 A1
        .db "<16"               ; @BT END
        
        // 3

        .bank 0 slot 1
        .orga $A200

        .macro LOCALREFERENCE2 ISOLATED
__      .dw _b
        .endm

        .db "17>"               ; @BT TEST-17 17 START
__      .db 0                   ; @BT 00
        LOCALREFERENCE2         ; @BT 04 A2
        .dw _b                  ; @BT 03 A2
        .db "<17"               ; @BT END

        // 4

        .bank 0 slot 1
        .orga $A300

        .macro LOCALREFERENCE4 ISOLATED
        .db 2
__      .dw _b
        .endm
        
        .macro LOCALREFERENCE3 ISOLATED
__      .db 1
        LOCALREFERENCE4
        .dw _b
        .endm

        .db "18>"               ; @BT TEST-18 18 START
__      .db 0                   ; @BT 00
        LOCALREFERENCE3         ; @BT 01 02 06 A3 04 A3
        .dw _b                  ; @BT 03 A3
        .db "<18"               ; @BT END

        // 5

        .bank 0 slot 1
        .orga $A400

        .macro LOCALREFERENCE5
__      .db 1
        .dw _b
        .endm

        .db "19>"               ; @BT TEST-19 19 START
__      .db 0                   ; @BT 00
        LOCALREFERENCE5()       ; @BT 01 04 A4
        .dw _b                  ; @BT 04 A4
        .db "<19"               ; @BT END

        // 6

        .bank 0 slot 1
        .orga $A500

        .macro MAZE1 ISOLATED
--      .db 0
-       .db 0
        .dw -
        .dw --
        .endm

        .macro MAZE2 ISOLATED
        .dw ++
        .dw +
+       .db 0
++      .db 0
        .endm

        .db "20>"               ; @BT TEST-20 20 START
--      .db 0                   ; @BT 00
-       .db 0                   ; @BT 00
        MAZE1                   ; @BT 00 00 06 A5 05 A5
        .dw -                   ; @BT 04 A5
        .dw --                  ; @BT 03 A5
        .dw ++                  ; @BT 1A A5
        .dw +                   ; @BT 19 A5
        MAZE2                   ; @BT 18 A5 17 A5 00 00
+       .db 0                   ; @BT 00
++      .db 0                   ; @BT 00
        .db "<20"               ; @BT END

        // 7
        
        .bank 0 slot 1
        .orga $A600
        
        .macro MASK ISOLATEUNNAMED
__      .db 0
        .endm
        
        .db "21>"               ; @BT TEST-21 21 START
__      .db 0                   ; @BT 00
        MASK                    ; @BT 00
        .dw _b + $10            ; @BT 13 A6
        .db "<21"               ; @BT END

        // 8
        
        .bank 0 slot 1
        .orga $A700

        .macro KAMEN2 ISOLATED
__      .db 0
        .dw _b + $20
        .endm
        
        .macro KAMEN1 ISOLATED
__      .db 0
        KAMEN2
        .dw _b + $10
        .endm
        
        .db "22>"               ; @BT TEST-22 22 START
__      .db 0                   ; @BT 00
        KAMEN1                  ; @BT 00 00 25 A7 14 A7
        .db "<22"               ; @BT END

        // 9
        
        .bank 0 slot 1
        .orga $A800

        .macro KAMEN2B ISOLATEUNNAMED
__      .db 0
        .dw _b + $20
        .endm
        
        .macro KAMEN1B
__      .db 0
        KAMEN2B
        .dw _b + $10
        .endm
        
        .db "23>"               ; @BT TEST-23 23 START
__      .db 0                   ; @BT 00
        KAMEN1B                 ; @BT 00 00 25 A8 14 A8
        .db "<23"               ; @BT END

        // 10 (JUST CHECKING)

        .bank 0 slot 1
        .orga $A900

A900:   .db "24>"               ; @BT TEST-24 24 START
        .db :A900               ; @BT 00
        .db 1 + :A900 + 1       ; @BT 02
        .db "<24"               ; @BT END

        // 11

        .bank 0 slot 1
        .orga $AA00

        .macro ISOLATEDMACRO ISOLATED
AA03:   .db 0
@child: .db 1
        .dw @child
        .endm
        
AA00:   .db "25>"               ; @BT TEST-25 25 START
@child: ISOLATEDMACRO           ; @BT 00 01 04 AA
        .dw @child              ; @BT 03 AA
        .db "<25"               ; @BT END

        // 12

        .bank 0 slot 1
        .orga $AB00

        .macro VERYISOLATED1 ISOLATELOCAL
_hello: .db 0
        .dw _hello
        .endm

_hello: .db "26>"               ; @BT TEST-26 26 START
        .db 0, 1, 2             ; @BT 00 01 02
        VERYISOLATED1           ; @BT 00 06 AB 
        .dw _hello              ; @BT 00 AB
        .db "<26"               ; @BT END
        
        // 13

        .bank 0 slot 1
        .orga $AC00

        .macro VERYISOLATED2 ISOLATELOCAL
_hello2:.db 0
        .dw _hello2 + 1
        .endm

_hello2: .db "27>"              ; @BT TEST-27 27 START
        .db 0, 1, 2             ; @BT 00 01 02
        VERYISOLATED2           ; @BT 00 07 AC 
        .dw _hello2 + 2         ; @BT 02 AC
        .db "<27"               ; @BT END
