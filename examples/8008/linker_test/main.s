
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small test for wla-8008
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0       $0000
   SLOT            1       $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $1000
   SLOT            3       $D000
   SLOT            4       $E000
   SLOT            5       $F000
.ENDME

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

.EMPTYFILL $AA

.BANK 0 SLOT 0
.ORG 0

.SECTION "MAIN" FORCE
.DW $10
.DW LibraryMain
.DB $11
Main:	nop
	jmp Main
.DB $10
.ENDS

.ORG $1000
.SECTION "EXTRAS" FORCE
.DB "AA>"
ExtrasStart:
	jfc ExtrasStart
	jfp ExtrasStart
	jfs ExtrasEnd
	jfz ExtrasEnd
ExtrasEnd:
.DB "<AA"
.DB " "
.DB "BB>"
	jtc ExtrasStart
	jtp ExtrasStart
.DB "<BB"
.ENDS

;
; Here is a test for structs and child labels
;

.STRUCT MyStruct
    ByteVal         DB
.ENDST

.ORG $1100
.SECTION "STRUCTS" FORCE

Label_A:
@Child:     ; This label works (it is Label_A@Child)
.DSTRUCT @Instance INSTANCEOF MyStruct VALUES
    ByteVal         .DB $FF
.ENDST

Label_B:
@Child:     ; This label works (it is Label_B@Child)
.DSTRUCT @Instance INSTANCEOF MyStruct VALUES
    ByteVal         .DB $A5
.ENDST

Label_C:
@Child:     ; This label works (it is Label_B@Child)
.DSTRUCT @@Instance INSTANCEOF MyStruct VALUES
    ByteVal         .DB $A5
.endst

Label_D:
@Child:     ; This label works (it is Label_B@Child)
.DSTRUCT Instance INSTANCEOF MyStruct VALUES
    ByteVal         .DB $A5
.ENDST

.ENDS
