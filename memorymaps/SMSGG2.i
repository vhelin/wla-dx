; SMS/GG memory map
; by Maxim
;
; Large first chunk version
; If you're only paging in slot 2, you can fit data into the
; first 32KB better this way. The 16 byte slot is where the
; header goes so don't use it; it's there to make the numbering
; for the :label syntax work correctly.

.MEMORYMAP
    DEFAULTSLOT 0
    SLOT 0 START $0000 SIZE $7FF0 ; ROM (unpaged)
    SLOT 1 START $7FF0 SIZE $0010 ; Header
    SLOT 2 START $8000 SIZE $4000 ; ROM (paged)
    SLOT 3 START $C000 SIZE $2000 ; RAM
.ENDME

; 512KB (32*16KB) example rombankmap:
.ROMBANKMAP
    BANKSTOTAL 32
    BANKSIZE $7FF0
    BANKS 1
    BANKSIZE $0010
    BANKS 1
    BANKSIZE $4000
    BANKS 30
.ENDRO

