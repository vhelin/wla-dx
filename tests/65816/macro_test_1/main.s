
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
