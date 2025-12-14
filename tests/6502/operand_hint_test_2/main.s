.memorymap
    defaultslot 0
    slot 0 $0000 $0010 "ROMSwitchSlot"
.endme

.rombankmap
    bankstotal $1
    banksize $0010
    banks $1
.endro

        ; @BT result.rom
        
.redef SUFFIX = 1
.section "ROM Bank $000" bank 0 slot "ROMSwitchSlot" org 2 FORCE

        .db "01>"               ; @BT TEST-01 01 START
MyLabel_{SUFFIX}:
    .if SUFFIX == 1
        lda #MyLabel_{SUFFIX} - MyLabel_{SUFFIX}.b ; @BT A9 00
    .endif
        .db "<01"               ; @BT END

.ends
