
.MEMORYMAP
SLOTSIZE $8000
DEFAULTSLOT 0
SLOT 0 $8000
.ENDME

.ROMBANKSIZE $8000
.ROMBANKS 8

        ; @BT result.rom
        
.DEFINE N 3
.BANK N-3 SLOT 0
.ORG $0000

.DEFINE ADDR 20

.PRINTV HEX abba
.printt "h\n"
.PRINTV HEX man
.printt "h\n"

.SECTION "Test" FORCE
        .db "01>"                            ; @BT TEST-01 01 START
        .db 0ah+1h+2h, 11h, 0abH, dena, pena ; @BT 0D 11 AB 0B 22
        .db "<01"                            ; @BT END
        .db "02>"                            ; @BT TEST-02 02 START
        and 10.b                             ; @BT 25 0A
        and 10.w                             ; @BT 2D 0A 00
        and $10.b                            ; @BT 25 10
        and $10.w                            ; @BT 2D 10 00
        and 10h.b                            ; @BT 25 10
        and 10h.w                            ; @BT 2D 10 00
        and %10.b                            ; @BT 25 02
        and %10.w                            ; @BT 2D 02 00
        and ADDR.B                           ; @BT 25 14
        and ADDR.W                           ; @BT 2D 14 00
        and ADDR-4.b                         ; @BT 25 10
        and ADDR+4.w                         ; @BT 2D 18 00
        .db "<02"                            ; @BT END
        .db "03>"                            ; @BT TEST-03 03 START
label:  lda #(label&$ff)                     ; @BT A9 32
        lda #(label>>8)                      ; @BT A9 80
        stx 100                              ; @BT 86 64
        sta 1000.w                           ; @BT 8D E8 03
        and 10                               ; @BT 25 0A
        stx 1000.w                           ; @BT 8E E8 03
        .define const 2
        lda #const+1                         ; @BT A9 03
        .db "<03"                            ; @BT END
.ENDS

.section "Names, names, names..." free
        .db name4, 0, name3, 1, name2, 2, name1, 3
.ends

.db "04>"                       ; @BT TEST-04 04 START
.dbsin 0.2, 10, 3.2, 120, 1.3   ; @BT 01 08 0F 15 1C 22 29 2F 35 3B 41
.db "<04"                       ; @BT END
