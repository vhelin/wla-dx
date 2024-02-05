
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
BANKSTOTAL 2
BANKSIZE $2000
BANKS 1
BANKSIZE $6000
BANKS 1
.ENDRO

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
        
