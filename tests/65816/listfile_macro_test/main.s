.memorymap
slotsize $8000
slot 0 $8000
defaultslot 0
.endme

.rombankmap
bankstotal 1
banksize $8000
banks 1
.endro

.bank 0 slot 0
.org 0

.macro FOURBYTES
  .db 1,2,3,4
.endm
        
.include "3bytes.i"
        
.macro TWOBYTES
  .db $11
  THREEBYTES
  .db $22
.endm

.section "Main" force
  .db $AA
  TWOBYTES
  TWOBYTES
  .db $BB
.ends
