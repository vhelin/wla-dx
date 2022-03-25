
        ; WARNING: here we had a bug in WLA v10.1...

.MEMORYMAP          ;Memory Map (For wla-gb)
SLOTSIZE $8000
DEFAULTSLOT 0
SLOT 0 $0000
.ENDME

.ROMBANKMAP         ;ROM Bank Map (no mapping)
BANKSTOTAL 2
BANKSIZE $8000
BANKS 1
BANKSIZE $1
BANKS 1
.ENDRO

.BANK 0 SLOT 0
.ORG 0
.dsb $8000 1
