
.gbheader
	version $0f
	version 15
	countrycode 1
	ramsize 2
	cartridgetype 7
	name "HELLO WORLD"
	version $0f
	licenseecodenew "FI"
	ramsize $2
	countrycode 1
	name "HELLO WORLD"
	destinationcode 1
	cartridgetype $7
	licenseecodenew "FI"
	ramsize 2
	countrycode $1
	destinationcode $1
.endgb
	
.MEMORYMAP
DEFAULTSLOT 1.01
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $6000
.ENDME

.ROMBANKMAP
BANKSTOTAL 3
BANKSIZE $2000
BANKS 1
BANKSIZE $6000
BANKS 1
BANKSIZE $2000
BANKS 1
.ENDRO

; @BT linked.gb
        
        .bank 0 slot 0
        .org 0

start_0000:
        .db 1, VALUE_2+1, 3

        .org $100
start_0100:
        .db 1, VALUE_2, 3, 4, 5
start_0105:
        .db 1, 2, 3, VALUE_2*VALUE_2

        .section "force" FORCE
start_0109:
        .db 1, VALUE_2, VALUE_2+1, 4
start_010D:
        .db 1, VALUE_2
        .ends

start_010F:
        .db VALUE_2-1, 2, VALUE_2+1

        .define VALUE_3 = 3
        .export VALUE_3
        
        .ramsection "ram" bank 0 slot 0 keep
vbi_counter:     db
player_lives:   db
        .ends

        .bank 2 slot 0
        .org 0

        .SECTION "More GFX data" KEEP

SRC_SoundEnginesMap:
        .ds 2048, 1
SRC_StartMap:
        .ds 256, 2
SRC_StartPal:
        .ds 256, 3
SRC_StartPic:
        .ds 256, 4

__start2_END:

        .db "01>"               ; @BT TEST-01 01 START
        .dw _sizeof_SRC_SoundEnginesMap ; @BT 00 08
        .dw _sizeof_SRC_StartMap ; @BT 00 01
        .dw _sizeof_SRC_StartPal ; @BT 00 01
        .dw _sizeof_SRC_StartPic ; @BT 00 01
        .db "<01"                ; @BT END
        
        .ENDS

        .bank 1 slot 1
        .org 1024*3
data1:  .ds 256, $11
        .org 1024*3+1024        ; Leave a 768 byte hole here
data2:  .ds 512, 5
        
_check: .db "02>"               ; @BT TEST-02 02 START
        .dw _sizeof_SRC_SoundEnginesMap ; @BT 00 08
        .dw _sizeof_SRC_StartMap ; @BT 00 01
        .dw _sizeof_SRC_StartPal ; @BT 00 01
        .dw _sizeof_SRC_StartPic ; @BT 00 01
        .dw _sizeof_data1        ; @BT 00 01
        .dw _sizeof_data2        ; @BT 00 02
        .db "<02"                ; @BT END
        
