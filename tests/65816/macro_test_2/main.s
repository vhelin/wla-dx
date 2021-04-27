
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

.section "SectionA" force
main:
	.db "01>"
        .dbm FILTER_MACRO_B, $123456, $ABCDEF, "0123456789"
	.db "<01"

	.db "02>"
        .dwm FILTER_MACRO_W, $123456, $ABCDEF, "0123456789"
	.db "<02"
	
	.db "03>"
        .dlm FILTER_MACRO_L, $123456, $ABCDEF, "0123456789"
	.db "<03"

        .db "04>"
        .dbm FILTER_MACRO_2, $123456, $ABCDEF, "01234"
	.db "<04"

        .db "05>"
        .dwm FILTER_MACRO_2, $123456, $ABCDEF, "01234"
	.db "<05"

        .db "06>"
        .dlm FILTER_MACRO_2, $123456, $ABCDEF, "01234"
	.db "<06"
.ends

//////////////////////////////////////////////////////////////////////
// test 2
//////////////////////////////////////////////////////////////////////

.bank 1 slot 0

.MACRO make_rst
  // these tests start from 07
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

.REPEAT 8 INDEX IDX
  make_rst IDX IDX+1
.ENDR

//////////////////////////////////////////////////////////////////////
// test 3
//////////////////////////////////////////////////////////////////////

.bank 1 slot 0

.macro string_macro_deep
  .db "16>"
  .db "\1"
  .db "\2"
  .db "<16"
.endm

.macro string_macro
  .db "15>"
label_\1:
  .db "\1"
label_\2:
  .db "\2"
  .db "<15"
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

        .db "17>"
        is_immediate_1 #5
        .db "<17"

        .db "18>"
        is_immediate_1 4
        .db "<18"
