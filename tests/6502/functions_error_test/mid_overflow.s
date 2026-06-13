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

; mid()'s "runs outside string" check used to compute start + length, which
; overflowed to a negative value for huge lengths and let the bounds check
; pass -- the copy loop then wrote ~2 GB and crashed. The assembler must now
; reject this with a clean error instead of segfaulting.
.db mid(1, 2147483647, "abc")
