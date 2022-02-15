
.memorymap
    slotsize $8000
    defaultslot 1
    slot 0 $0000
    slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

.macro macro_increment
.redefine _out \1+1
.endm

; @BT result.rom

.bank 0 slot 0
.section "Section1" force
    .DB "01>"                           ; @BT TEST-01 01 START
    .dbm macro_increment 1, 2, 3, FOUR  ; @BT 02 03 04 05
    .DB "<01"                           ; @BT END
    .DB "02>"                           ; @BT TEST-02 02 START
    .dwm macro_increment 1, 2, 3, FOUR  ; @BT 02 00 03 00 04 00 05 00
    .DB "<02"                           ; @BT END
    .DB "03>"                           ; @BT TEST-03 03 START
    .dlm macro_increment 1, 2, 3, FOUR  ; @BT 02 00 00 03 00 00 04 00 00 05 00 00
    .DB "<03"                           ; @BT END
.ends

