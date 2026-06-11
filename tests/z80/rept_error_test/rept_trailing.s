; NOTE: This test was created by Claude Fable 5.
.memorymap
defaultslot 0
slot 0 $0000 $2000
.endme

.rombankmap
bankstotal 1
banksize $2000
banks 1
.endro

.bank 0 slot 0
.org 0

; The .REPT/.REPEAT option parser only stopped on end-of-line, so any
; unexpected token after the count (here "5") was rolled back and retried
; forever -- the assembler hung. It must now report a clear error instead.
.rept 2 5
        nop
.endr
