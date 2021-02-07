
.MEMORYMAP
SLOTSIZE $8000
DEFAULTSLOT 0
SLOT 0 $8000
.ENDME

.ROMBANKSIZE $8000
.ROMBANKS 8

.DEFINE N 3
.BANK N-3 SLOT 0
.ORG $0000

.DEFINE ADDR 20

.PRINTV HEX abba
.printt "h\n"
.PRINTV HEX man
.printt "h\n"

.SECTION "Test" FORCE
        .db "01>"
        .db 0ah+1h+2h, 11h, 0abH, dena, pena
        .db "<01"
        .db "02>"
        and 10.b
        and 10.w
        and $10.b
        and $10.w
        and 10h.b
        and 10h.w
        and %10.b
        and %10.w
        and ADDR.B
        and ADDR.W
        and ADDR-4.b
        and ADDR+4.w
        .db "<02"
        .db "03>"
label:  lda #(label&$ff)
        lda #(label>>8)
        stx 100
        sta 1000.w
        and 10
        stx 1000.w
        .db "<03"
.ENDS

.section "Names, names, names..." free
        .db name4, 0, name3, 1, name2, 2, name1, 3
.ends

.db "04>"
.dbsin 0.2, 10, 3.2, 120, 1.3
.db "<04"
