
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
           SLOT 1 $A000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 2
           BANKSIZE $2000
           BANKS 2
        .ENDRO

        .BANK 00  SLOT 0
        .ORG    0

        .EMPTYFILL $ff
        
.struct waterDrop
size   db
weight db	
.endst
	
.STRUCT waterPool
name   ds 8
age    db
area   dw
waterDrops instanceOf waterDrop	2
data1  db
data2  db
.ENDST

.STRUCT testStruct
ok dw
.IFDEF DEBUG
not_ok dw
.ENDIF
at_last dw
.ENDST
	
.DSTRUCT waterPool1 INSTANCEOF waterPool DATA "myPool", 9, 120, 1, 2, 3, 4, 5,6
.db $ff, $fe


; @BT linked.rom
; @BT TEST-01 01 START 34 12 78 56 END

.db "01>"
.DSTRUCT testStruct1 INSTANCEOF testStruct DATA $1234, $5678
.db "<01"

.enum $a000
waterPool2 instanceof waterPool
.ende



.STRUCT sState
	OnEnter DL
	OnExit  DD
	OnSomething DSB 1
	OnMore  DW
.ENDST

; @BT TEST-02 02 START 56 34 12 EF CD AB 89 EE 34 12   03 02 01 07 06 05 04 BB 78 56   13 12 11 17 16 15 14 10 56 34 END

.DB "02>"
	
.DSTRUCT State1 INSTANCEOF sState VALUES
	OnEnter:  .DL $123456
	OnExit:	  .DD $89ABCDEF
	OnSomething: .DB $EE
	OnMore:	.DW $1234
.ENDST

.DSTRUCT State2 INSTANCEOF sState VALUES
	OnEnter:  .DL $010203
	OnExit:	  .DD $04050607
	OnSomething: .DB $BB
	OnMore:	.DW $5678
.ENDST

.DSTRUCT State3 INSTANCEOF sState $111213 $14151617 $10 $3456

.DB "<02"

        
        .db "03>"               ; @BT TEST-03 03 START

        .struct ThreeBytes
        Byte1 DB
        Rest  DW
        .endst

        .db _sizeof_ThreeBytes  ; @BT 03

        .dstruct ThreeBytes1 INSTANCEOF ThreeBytes VALUES
        Byte1:  .DB 1           ; @BT 01
        Rest:   .DW $0302       ; @BT 02 03
        .endst

        .db _sizeof_ThreeBytes1 ; @BT 03

        .dstruct ThreeBytes2 INSTANCEOF ThreeBytes SIZE 3 VALUES
        Byte1:  .DB 4           ; @BT 04
        Rest:   .DW $0605       ; @BT 05 06
        .endst

        .db _sizeof_ThreeBytes2 ; @BT 03
        
        .dstruct ThreeBytes3 INSTANCEOF ThreeBytes SIZE 5 VALUES
        Byte1:  .DB 4           ; @BT 04
        Rest:   .DW $0605       ; @BT 05 06
                                ; @BT FF FF
        .endst

        .db _sizeof_ThreeBytes3 ; @BT 05
        
        .dstruct ThreeBytes4 INSTANCEOF ThreeBytes VALUES
        Byte1:  .DB 7           ; @BT 07
        Rest:   .DW $0908       ; @BT 08 09
        .endst

        .db _sizeof_ThreeBytes4 ; @BT 03
        
        .db "<03"               ; @BT END
        

        .db "04>"               ; @BT TEST-04 04 START

        .struct FourBytes SIZE 6
        Byte1 DB
        Byte2 DB
        Rest  DW
        .endst

        .db _sizeof_FourBytes   ; @BT 06

        .dstruct FourBytes1 INSTANCEOF FourBytes VALUES
        Byte1:  .DB 1           ; @BT 01
        Byte2:  .DB 2           ; @BT 02
        Rest:   .DW $0403       ; @BT 03 04
                                ; @BT FF FF
        .endst

        .db _sizeof_FourBytes1  ; @BT 06
        
        .dstruct FourBytes2 INSTANCEOF FourBytes SIZE 8 VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF FF FF
        .endst

        .db _sizeof_FourBytes2  ; @BT 08

        .db "<04"               ; @BT END
        

        .BANK 1 SLOT 1
        .ORG 0

        .db "05>"               ; @BT TEST-05 05 START
        
        .dstruct FourBytes3 INSTANCEOF FourBytes NOLABELS SIZE 8 VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF FF FF
        .endst

        .dstruct FourBytes4 INSTANCEOF FourBytes SIZE 8 VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF FF FF
        .endst

AfterFourBytes4:
        
        .dw FourBytes4          ; @BT 0B A0
        .dw FourBytes4.Byte1    ; @BT 0B A0
        .dw FourBytes4.Byte2    ; @BT 0C A0
        .dw FourBytes4.Rest     ; @BT 0D A0
        .dw AfterFourBytes4     ; @BT 13 A0

        .dstruct INSTANCEOF FourBytes NOLABELS SIZE 8 VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF FF FF
        .endst

        .dstruct INSTANCEOF FourBytes NOLABELS SIZE 8 VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF FF FF
        .endst

        .dstruct INSTANCEOF FourBytes NOLABELS VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF
        .endst

        .dstruct INSTANCEOF FourBytes NOLABELS VALUES
        Byte1:  .DB 5           ; @BT 05
        Byte2:  .DB 6           ; @BT 06
        Rest:   .DW $0807       ; @BT 07 08
                                ; @BT FF FF
        .endst

AfterAll:

        .dw AfterAll            ; @BT 39 A0

        .dstruct INSTANCEOF ThreeBytes NOLABELS DATA 1, $0302   ; @BT 01 02 03
        .dstruct INSTANCEOF ThreeBytes NOLABELS SIZE 4 DATA 1, $0302 ; @BT 01 02 03 FF
        .dstruct INSTANCEOF ThreeBytes SIZE 5 NOLABELS DATA 1, $0302 ; @BT 01 02 03 FF FF
        .dstruct INSTANCEOF FourBytes NOLABELS DATA 5, 6, $0807 ; @BT 05 06 07 08 FF FF
        .dstruct INSTANCEOF FourBytes NOLABELS DATA 5, 6, $0807 ; @BT 05 06 07 08 FF FF
        .dstruct INSTANCEOF FourBytes NOLABELS SIZE 8 DATA 5, 6, $0807 ; @BT 05 06 07 08 FF FF FF FF
        
        .db _paddingof_FourBytes1 ; @BT 02
        .db _paddingof_FourBytes2 ; @BT 04
        .db _paddingof_FourBytes3 ; @BT 04
        .db _paddingof_FourBytes4 ; @BT 04
        
        .db "<05"               ; @BT END

        .export _paddingof_FourBytes1, _paddingof_FourBytes2, _paddingof_FourBytes3, _paddingof_FourBytes4
