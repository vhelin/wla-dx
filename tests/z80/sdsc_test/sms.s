
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing wla-z80's sms functionality
; written by ville helin <vhelin@cc.hut.fi> in 2003
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
DEFAULTSLOT 0
SLOTSIZE $4000
SLOT 0 $0000
.ENDME

.ROMBANKMAP
BANKSTOTAL 1
BANKSIZE $4000
BANKS 1
.ENDRO

.sdsctag 98.2, "WLA bug causer", "", ""

.bank 0
.org 0

.section "Deliberately empty section" free
	nop
	nop
	nop
	nop
.ends
