
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing .ramsection's size
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
   SLOT            5       $FF01
.ENDME

.RAMSECTION "Variables_1" SLOT 3 SIZE 64 KEEP
var_1           dw
var_2           dw
var_3           dw
.ENDS

.RAMSECTION "Variables_2" SLOT 3 APPENDTO "Variables_1"
new_1           db
new_2           db
.ENDS
        
.EMPTYFILL $AA

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO
