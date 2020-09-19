
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing wla-z80's smschecksum functionality
; written by ville helin <vhelin@cc.hut.fi> in 2020
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0       $0000
   SLOT            1       $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $2000
   SLOT            3       $C000
   SLOT            4       $E000
.ENDME

.ROMBANKMAP
BANKSTOTAL 8
BANKSIZE $4000
BANKS 8
.ENDRO

.EMPTYFILL $C9
	
.COMPUTESMSCHECKSUM

.BACKGROUND "randomData.rom"

; write ROMSIZE to SMS HEADER as WLALINK will read it later...
	
.bank 1 slot 0
.orga $3fff
.section "ROMSIZE" overwrite
	.db $4C
.ends
	
