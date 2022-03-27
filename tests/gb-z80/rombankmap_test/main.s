
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
BANKSIZE 1
BANKS 1
.ENDRO

.ROMSIZE
        
                .BANK 0 SLOT 0
                .ORG $148
org_0x148a:      .db 1
org_0x149a:      .db 2
                .ORG $148
org_0x148b:     .asc 10
org_0x149b:     nop
                .org $148
org_0x148c:     .dsb 2, 2
        
