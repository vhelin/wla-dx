
//////////////////////////////////////////////////////////////////////
// macro parser test
//////////////////////////////////////////////////////////////////////
        
.memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

//////////////////////////////////////////////////////////////////////
// test 1
//////////////////////////////////////////////////////////////////////

.macro FILTER_MACRO_B
.redefine _out \1 & $FF
.endm

.macro FILTER_MACRO_W
.redefine _out = \1 & $FFFF
.endm

.macro FILTER_MACRO_L
.redefine _out \1 & $FFFFFF
.endm

.macro FILTER_MACRO_2
.redefine _out \2
.endm

.bank 0 slot 1

; @BT result.rom

.section "SectionA" force
main:
        .db "01>"                                               ; @BT TEST-01 01 START
        .dbm FILTER_MACRO_B, $123456, $ABCDEF, "0123456789"     ; @BT 56 EF 30 31 32 33 34 35 36 37 38 39
        .db "<01"                                               ; @BT END

        .db "02>"                                               ; @BT TEST-02 02 START
        .dwm FILTER_MACRO_W, $123456, $ABCDEF, "0123456789"     ; @BT 56 34 EF CD 30 00 31 00 32 00 33 00 34 00 35 00 36 00 37 00 38 00 39 00
        .db "<02"                                               ; @BT END

        .db "03>"                                               ; @BT TEST-03 03 START
        .dlm FILTER_MACRO_L, $123456, $ABCDEF, "0123456789"     ; @BT 56 34 12 EF CD AB 30 00 00 31 00 00 32 00 00 33 00 00 34 00 00 35 00 00 36 00 00 37 00 00 38 00 00 39 00 00
        .db "<03"                                               ; @BT END

        .db "04>"                                               ; @BT TEST-04 04 START
        .dbm FILTER_MACRO_2, $123456, $ABCDEF, "01234"          ; @BT 00 01 02 03 04 05 06
        .db "<04"                                               ; @BT END

        .db "05>"                                               ; @BT TEST-05 05 START
        .dwm FILTER_MACRO_2, $123456, $ABCDEF, "01234"          ; @BT 00 00 01 00 02 00 03 00 04 00 05 00 06 00
        .db "<05"                                               ; @BT END

        .db "06>"                                               ; @BT TEST-06 06 START
        .dlm FILTER_MACRO_2, $123456, $ABCDEF, "01234"          ; @BT 00 00 00 01 00 00 02 00 00 03 00 00 04 00 00 05 00 00 06 00 00
        .db "<06"                                               ; @BT END
.ends

//////////////////////////////////////////////////////////////////////
// test 2
//////////////////////////////////////////////////////////////////////

.bank 1 slot 0

.MACRO make_rst
  // these tests start from 07 (range: 07-14)
  .DEFINE CHAR = \1+7
  .IF CHAR < 10
    .DEFINE CHAR_1 = '0'
    .DEFINE CHAR_2 = '0'+CHAR
  .ELSE
    .DEFINE CHAR_1 = '1'
    .DEFINE CHAR_2 = '0'+CHAR-10
  .ENDIF
        
  .ORGA (\1 * 8)
  .SECTION "SectionB_\1" SIZE 8 FORCE KEEP
        .db CHAR_1, CHAR_2, '>'
rts_\1: .db \1, "\2"
        .db '<', CHAR_1, CHAR_2
  .ENDS

  .UNDEFINE CHAR, CHAR_1, CHAR_2
.ENDM

; @BT TEST-07 07 START 00 31 END
; @BT TEST-08 08 START 01 32 END
; @BT TEST-09 09 START 02 33 END
; @BT TEST-10 10 START 03 34 END
; @BT TEST-11 11 START 04 35 END
; @BT TEST-12 12 START 05 36 END
; @BT TEST-13 13 START 06 37 END
; @BT TEST-14 14 START 07 38 END

.REPEAT 8 INDEX IDX
  make_rst IDX IDX+1
.ENDR

//////////////////////////////////////////////////////////////////////
// test 3
//////////////////////////////////////////////////////////////////////

.bank 1 slot 0

.macro string_macro_deep
  .db "16>"     ; @BT TEST-16 16 START
  .db "\1"      ; @BT 48 49
  .db "\2"      ; @BT 59 4F
  .db "<16"     ; @BT END
.endm

.macro string_macro
  .db "15>"     ; @BT TEST-15 15 START
label_\1:
  .db "\1"      ; @BT 48 49
label_\2: 
  .db "\2"      ; @BT 59 4F
  .db "<15"     ; @BT END
  string_macro_deep \1 \2
.endm
        
 string_macro HI "YO"
        
//////////////////////////////////////////////////////////////////////
// test 4
//////////////////////////////////////////////////////////////////////

        .macro is_immediate_2
        .if \?1 != ARG_IMMEDIATE
        adc \1-2
        .else
        adc #\1+2
        .endif
        .endm
        
        .macro is_immediate_1
        .if \?1 == ARG_IMMEDIATE
        adc #\1+1
        .else
        adc \1-1
        .endif
        is_immediate_2 \1
        .endm

        .db "17>"               ; @BT TEST-17 17 START
        is_immediate_1 #5       ; @BT 69 06 69 07
        .db "<17"               ; @BT END

        .db "18>"               ; @BT TEST-18 18 START
        is_immediate_1 4        ; @BT 65 03 65 02
        .db "<18"               ; @BT END

//////////////////////////////////////////////////////////////////////
// test 5
//////////////////////////////////////////////////////////////////////

        .DEFINE DEFINITION_A 1

        .MACRO REDEFINER_1
        .REDEFINE \1 = ?1 + 1        ; \1 here is the definition's name,
        .ENDM                        ; and ?1 is its value.

        REDEFINER_1 &DEFINITION_A    ; here we feed the definition's name
        REDEFINER_1 &DEFINITION_A    ; as first argument, not it's value

        .DEFINE DEFINITION_B 1

        .MACRO REDEFINER_2
        .REDEFINE DEFINITION_B \1 + 1
        .ENDM

        REDEFINER_2 DEFINITION_B
        REDEFINER_2 DEFINITION_B

        .db "19>"               ; @BT TEST-19 19 START
        .db DEFINITION_A        ; @BT 03
        .db DEFINITION_B        ; @BT 03
        .db "<19"               ; @BT END

//////////////////////////////////////////////////////////////////////
// test 6
//////////////////////////////////////////////////////////////////////

        .define FIVE = 5
        
        .macro floor_2
        .db ?1
        .db 1+?1+1
        .db \2-4
        .endm

        .macro floor_1
        floor_2 \1 ?1
        .db ?1-1
        .endm

        .macro floor_0
        floor_1 \1
        .endm

        .db "20>"       ; @BT TEST-20 20 START
        floor_0 &FIVE   ; @BT 05 07 01 04
        floor_0(&FIVE)  ; @BT 05 07 01 04
        .db "<20"       ; @BT END

//////////////////////////////////////////////////////////////////////
// test 7
//////////////////////////////////////////////////////////////////////

        .macro addThreeValues( v1, v2, v3 )
        .db v1+v2+v3
        .endm

	.macro addFourValues(v1,v2,v3,v4) isolatelocal
	.db v1+v2+v3+v4
	.endm

	.macro oneMacro()
	.db 1
	.endm

        .db "21>"                   ; @BT TEST-21 21 START
        addThreeValues(3,4,5)       ; @BT 0C
        addThreeValues(1+2,2+2,2+3) ; @BT 0C
	addFourValues(0,1,2,3)	    ; @BT 06
	addFourValues(0,-1,1-1,0)   ; @BT FF
	oneMacro()		    ; @BT 01
        .db "<21"                   ; @BT END
