.MEMORYMAP
    DEFAULTSLOT 0
    SLOTSIZE $1000
    SLOT 0 $1000
.ENDME 

.ROMBANKSIZE $1000
.ROMBANKS 1

.BANK 0
.ORG 0

; @BT linked.rom

        .db "01>"   ; @BT TEST-01 01 START

        sta ($12, U)            ; @BT A7 0b11011000 12
        sta ($12.b, U)          ; @BT A7 0b11011000 12
        sta ($12.w, U)          ; @BT A7 0b11011001 00 12
        
        sta ($13, PCR)          ; @BT A7 0b10011100 13
        sta ($13.b, PCR)        ; @BT A7 0b10011100 13
        sta ($13.w, PCR)        ; @BT A7 0b10011101 00 13
        sta ($14)               ; @BT A7 0b10011111 00 14

        sta 2, S                ; @BT A7 0b01100010
        sta 2.b, S              ; @BT A7 0b11101000 02
        sta 2.w, S              ; @BT A7 0b11101001 00 02

        sta $15, PCR            ; @BT A7 0b10001100 15
        sta $15.b, PCR          ; @BT A7 0b10001100 15
        sta $15.w, PCR          ; @BT A7 0b10001101 00 15

        sta $16                 ; @BT 97 16
        sta $16.b               ; @BT 97 16
        sta $16.w               ; @BT B7 00 16
        
        .8BIT

        sta ($12, U)            ; @BT A7 0b11011000 12
        sta ($12.b, U)          ; @BT A7 0b11011000 12
        sta ($12.w, U)          ; @BT A7 0b11011001 00 12        

        sta ($13, PCR)          ; @BT A7 0b10011100 13
        sta ($13.b, PCR)        ; @BT A7 0b10011100 13
        sta ($13.w, PCR)        ; @BT A7 0b10011101 00 13
        sta ($14)               ; @BT A7 0b10011111 00 14

        sta 2, S                ; @BT A7 0b11101000 02
        sta 2.b, S              ; @BT A7 0b11101000 02
        sta 2.w, S              ; @BT A7 0b11101001 00 02
        
        sta $15, PCR            ; @BT A7 0b10001100 15
        sta $15.b, PCR          ; @BT A7 0b10001100 15
        sta $15.w, PCR          ; @BT A7 0b10001101 00 15

        sta $16                 ; @BT 97 16
        sta $16.b               ; @BT 97 16
        sta $16.w               ; @BT B7 00 16

        .16BIT

        sta ($12, U)            ; @BT A7 0b11011001 00 12
        sta ($12.b, U)          ; @BT A7 0b11011000 12
        sta ($12.w, U)          ; @BT A7 0b11011001 00 12        

        sta ($13, PCR)          ; @BT A7 0b10011101 00 13
        sta ($13.b, PCR)        ; @BT A7 0b10011100 13
        sta ($13.w, PCR)        ; @BT A7 0b10011101 00 13
        sta ($14)               ; @BT A7 0b10011111 00 14
        
        sta 2, S                ; @BT A7 0b11101001 00 02
        sta 2.b, S              ; @BT A7 0b11101000 02
        sta 2.w, S              ; @BT A7 0b11101001 00 02

        sta $15, PCR            ; @BT A7 0b10001101 00 15
        sta $15.b, PCR          ; @BT A7 0b10001100 15
        sta $15.w, PCR          ; @BT A7 0b10001101 00 15

        sta $16                 ; @BT B7 00 16
        sta $16.b               ; @BT 97 16
        sta $16.w               ; @BT B7 00 16

        .db "<01"   ; @BT END

        .endasm

        HELLO ROBOT, DO NOT PARSE THIS!
        
