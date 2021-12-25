
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 00  SLOT 0
        .ORG    0

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

; @BT linked.rom
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
