
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

        ; @BT linked.prg

.SECTION "MAIN" FORCE
LoadAddress:
	JMP LoadAddress
	
.DB "01>"                       ; @BT TEST-01 01 START
AStart:
	BCC AStart              ; @BT 90 FE
	BCC AStart+1            ; @BT 90 FD
	BCC AEnd                ; @BT 90 02
	BCC AEnd+1              ; @BT 90 01
AEnd:
.DB "<01"                       ; @BT END

.DB "02>"                       ; @BT TEST-02 02 START
BStart:
	BCC BStart.w            ; @BT 93 FE FF
	BCC BStart.w+1          ; @BT 93 FC FF
	BCC BEnd.w              ; @BT 93 04 00
	BCC BEnd.w+1            ; @BT 93 02 00
BEnd:
.DB "<02"                       ; @BT END
.ENDS
