
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; equal-size FREE sections must pack in appearance order, not libc qsort order
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   SLOTSIZE        $100
   SLOT            0       $0000
.ENDME

.ROMBANKMAP
BANKSTOTAL 1
BANKSIZE $100
BANKS 1
.ENDRO

.EMPTYFILL $00

; @BT linked.rom

.BANK 0 SLOT 0
.ORG $00

.SECTION "Boot" FORCE
        .db $01
.ENDS

.SECTION "LaterName" FREE KEEP
        .db $AA, $AA
.ENDS

.SECTION "EarlierName" FREE KEEP
        .db $BB, $BB
.ENDS

.SECTION "Large" FREE KEEP
        .db $CC, $CC, $CC, $CC
.ENDS

; FORCE at $0000, then FREE biggest-first, then equal-size in source order
; @BT TEST-01 -a 0 START 01 CC CC CC CC AA AA BB BB END
