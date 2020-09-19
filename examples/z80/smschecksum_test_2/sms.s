
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

.EMPTYFILL $00
	
.COMPUTESMSCHECKSUM

; fill the banks with random data, but skip the SMSHEADER areas

; BANK 0

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 0 slot 0
.org 0
.section "FILLER_1" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 1

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 1 slot 0
.org 0
.section "FILLER_2" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 2

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 2 slot 0
.org 0
.section "FILLER_3" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 3

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 3 slot 0
.org 0
.section "FILLER_4" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 4

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 4 slot 0
.org 0
.section "FILLER_5" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 5

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 5 slot 0
.org 0
.section "FILLER_6" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 6

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 6 slot 0
.org 0
.section "FILLER_7" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

; BANK 7

.redefine RUNNING_NUMBER 0
.redefine ADDRESS 0	

.bank 7 slot 0
.org 7
.section "FILLER_8" force
	.repeat $4000
	.ifle ADDRESS $3ff0
	  .db RUNNING_NUMBER & $ff
	.endif
	.redefine RUNNING_NUMBER (RUNNING_NUMBER + 1)
	.redefine ADDRESS (ADDRESS + 1)
	.endr
.ends

