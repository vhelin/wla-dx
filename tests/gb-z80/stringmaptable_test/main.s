.memorymap
    defaultslot 0
    slot 0 $0000 $0010 "ROMSwitchSlot"
.endme

.rombankmap
    bankstotal $1
    banksize $0010
    banks $1
.endro

	
.stringmaptable charmap1 "charmap.tbl"

.section "ROM Bank A" bank 0 slot "ROMSwitchSlot" keep
.stringmap charmap1, "ABCBABA"
.ends

	.stringmaptable charmap2 "cm.tbl"
	.section "ROM BANK B" bank 0 slot "ROMSwitchSlot" keep
	.stringmap charmap2, "DEFED"
	.ends
	
