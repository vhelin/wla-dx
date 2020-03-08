
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small test for wla-65ce02
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

;
; C65 Memory Map
;
	
.MEMORYMAP
DEFAULTSLOT     0
SLOT 0 START $0000 SIZE $2000 NAME "RAMLO"  ;  8KB
SLOT 1 START $2000 SIZE $6000 NAME "BASIC"  ; 24KB
SLOT 2 START $8000 SIZE $4000 NAME "GFX"    ; 16KB
SLOT 3 START $C000 SIZE $1000 NAME "KERNEL" ;  4KB
SLOT 4 START $D000 SIZE $1000 NAME "I/O"    ;  4KB
SLOT 5 START $E000 SIZE $2000 NAME "EDITOR" ;  8KB
.ENDME

.ROMBANKMAP
BANKSTOTAL 1
BANKSIZE $2000
BANKS 1
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG $100

.SECTION "MAIN" FORCE
LoadAddress:
	JMP LoadAddress
	
.DB "01>"
AStart:
	BCC AStart
	BCC AStart+1
	BCC AEnd
	BCC AEnd+1
AEnd:
.DB "<01"

.DB "02>"
BStart:
	BCC BStart.w
	BCC BStart.w+1
	BCC BEnd.w
	BCC BEnd.w+1
BEnd:
.DB "<02"
.ENDS
