
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

.DSTRUCT waterPool1 INSTANCEOF waterPool DATA "myPool", 9, 120, 1, 2, 3, 4, 5,6
.db $ff, $fe

.enum $a000
waterPool2 instanceof waterPool
.ende

