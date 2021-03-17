
.MEMORYMAP
    DEFAULTSLOT 0
    SLOTSIZE $1000
    SLOT 0 $0000
    SLOT 1 $1000
    SLOT 2 $2000
    SLOT 3 $3000
.ENDME

.ROMBANKSIZE $1000
.ROMBANKS 4

.BANK 0 SLOT 0
.ORG 33h        

.section "LABEL 1" FORCE KEEP
label_33h:
        nop
.ends
        
.BANK 0 SLOT 1
.ORG 0

.section "LABEL 2" FORCE KEEP
label_1000h:
        nop
.ends

.ORG 1
        
        .db "01>"
        adc label_33h
        adc label_33h.b
        adc label_1000h
        adc label_1000h.l
        .db "<01"

        .db "02>"
        adc label_33h + 1
        adc label_33h.b + 1
        adc label_1000h + 1
        adc label_1000h.l + 1
        .db "<02"
        
