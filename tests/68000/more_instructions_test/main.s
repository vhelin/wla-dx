; @BT linked.rom

; Dedicated coverage for explicit MC68000 suffix aliases that were
; missing from the instruction table.

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 $000000 $100000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.define BIT_ZERO  (0)
.define BIT_ONE   (BIT_ZERO+1)
.define BIT_TWO   (BIT_ONE+1)
.define BIT_THREE (BIT_TWO+BIT_ONE)
.define BIT_FOUR  (BIT_TWO*2)
.define BIT_FIVE  ($0A/2)
.define BIT_SEVEN (BIT_FIVE+BIT_TWO)
.define BIT_NINE  (BIT_SEVEN+BIT_TWO)
.define BIT_FIFTEEN ($10-BIT_ONE)
.define BIT_THIRTY_ONE ($20-BIT_ONE)

.define ADDR_1234 ($1200+$34)
.define ADDR_LONG ($00120000+$3456)
.define DISP_20   ($10*2)
.define DISP_NEG2 (0-BIT_TWO)

.function MAKE_BIT(a,b) a+b
.function MAKE_ADDR(base,offset) base+offset
.function MAKE_DISP(x) x*BIT_TWO

.BANK 0 SLOT 0
.ORG 0

.db "01>" ; @BT TEST-01 01 START
  bchg.b #1,$1234.w        ; @BT $08 $78 $00 $01 $12 $34
  bchg.l d5,d6             ; @BT $0B $46
.db "<01" ; @BT END

.db "02>" ; @BT TEST-02 02 START
  bclr.b #1,$1234.w        ; @BT $08 $B8 $00 $01 $12 $34
  bclr.l d5,d6             ; @BT $0B $86
.db "<02" ; @BT END

.db "03>" ; @BT TEST-03 03 START
  bset.b #1,$1234.w        ; @BT $08 $F8 $00 $01 $12 $34
  bset.l d5,d6             ; @BT $0B $C6
.db "<03" ; @BT END

.db "04>" ; @BT TEST-04 04 START
  btst.b #1,$1234.w        ; @BT $08 $38 $00 $01 $12 $34
  btst.l d5,d6             ; @BT $0B $06
.db "<04" ; @BT END

.ORG $2000
label_byte_a:
  .db $00
label_byte_b:
  .db $00

.ORG $2100

.db "05>" ; @BT TEST-05 05 START
  bchg.b #BIT_SEVEN,(a0)                 ; @BT $08 $50 $00 $07
  bchg.b #(BIT_TWO*BIT_THREE),(a1)+      ; @BT $08 $59 $00 $06
  bchg.b #MAKE_BIT(BIT_ONE,BIT_FOUR),-(a2) ; @BT $08 $62 $00 $05
  bchg.b #MAKE_BIT(BIT_TWO,BIT_FOUR),(DISP_20,a3) ; @BT $08 $6B $00 $06 $00 $20
  bchg.b #BIT_SEVEN,label_byte_a.w       ; @BT $08 $78 $00 $07 $20 $00
  bchg.l #BIT_THIRTY_ONE,d7              ; @BT $08 $47 $00 $1F
  bchg.b #BIT_TWO,MAKE_ADDR($120000,$3456).l ; @BT $08 $79 $00 $02 $00 $12 $34 $56
.db "<05" ; @BT END

.db "06>" ; @BT TEST-06 06 START
  bchg.b d0,(a4)                  ; @BT $01 $54
  bchg.b d1,(a5)+                 ; @BT $03 $5D
  bchg.b d2,-(a6)                 ; @BT $05 $66
  bchg.b d3,(DISP_NEG2,a0)        ; @BT $07 $68 $FF $FE
  bchg.b d4,label_byte_b.w        ; @BT $09 $78 $20 $01
  bchg.l d6,d7                    ; @BT $0D $47
.db "<06" ; @BT END

.db "07>" ; @BT TEST-07 07 START
  bclr.b #BIT_ZERO,(a0)           ; @BT $08 $90 $00 $00
  bclr.b #BIT_THREE,(a1)+         ; @BT $08 $99 $00 $03
  bclr.b #BIT_FOUR,-(a2)          ; @BT $08 $A2 $00 $04
  bclr.b #MAKE_BIT(BIT_TWO,BIT_THREE),(MAKE_DISP(3),a3) ; @BT $08 $AB $00 $05 $00 $06
  bclr.b #BIT_SEVEN,label_byte_b.w ; @BT $08 $B8 $00 $07 $20 $01
  bclr.l #BIT_THIRTY_ONE,d7       ; @BT $08 $87 $00 $1F
  bclr.b #BIT_ONE,ADDR_LONG.l     ; @BT $08 $B9 $00 $01 $00 $12 $34 $56
.db "<07" ; @BT END

.db "08>" ; @BT TEST-08 08 START
  bclr.b d0,(a4)                  ; @BT $01 $94
  bclr.b d1,(a5)+                 ; @BT $03 $9D
  bclr.b d2,-(a6)                 ; @BT $05 $A6
  bclr.b d3,(DISP_NEG2,a0)        ; @BT $07 $A8 $FF $FE
  bclr.b d4,label_byte_a.w        ; @BT $09 $B8 $20 $00
  bclr.l d7,d0                    ; @BT $0F $80
.db "<08" ; @BT END

.db "09>" ; @BT TEST-09 09 START
  bset.b #BIT_ONE,(a0)            ; @BT $08 $D0 $00 $01
  bset.b #MAKE_BIT(BIT_ONE,BIT_TWO),(a1)+ ; @BT $08 $D9 $00 $03
  bset.b #BIT_FOUR,-(a2)          ; @BT $08 $E2 $00 $04
  bset.b #BIT_FIVE,(DISP_20,a3)   ; @BT $08 $EB $00 $05 $00 $20
  bset.b #BIT_SEVEN,label_byte_a.w ; @BT $08 $F8 $00 $07 $20 $00
  bset.l #BIT_THIRTY_ONE,d7       ; @BT $08 $C7 $00 $1F
  bset.b #BIT_TWO,ADDR_LONG.l     ; @BT $08 $F9 $00 $02 $00 $12 $34 $56
.db "<09" ; @BT END

.db "10>" ; @BT TEST-10 10 START
  bset.b d0,(a4)                  ; @BT $01 $D4
  bset.b d1,(a5)+                 ; @BT $03 $DD
  bset.b d2,-(a6)                 ; @BT $05 $E6
  bset.b d3,(DISP_NEG2,a0)        ; @BT $07 $E8 $FF $FE
  bset.b d4,label_byte_b.w        ; @BT $09 $F8 $20 $01
  bset.l d7,d0                    ; @BT $0F $C0
.db "<10" ; @BT END

.db "11>" ; @BT TEST-11 11 START
  btst.b #BIT_ONE,(a0)            ; @BT $08 $10 $00 $01
  btst.b #MAKE_BIT(BIT_ONE,BIT_TWO),(a1)+ ; @BT $08 $19 $00 $03
  btst.b #BIT_FOUR,-(a2)          ; @BT $08 $22 $00 $04
  btst.b #BIT_FIVE,(DISP_20,a3)   ; @BT $08 $2B $00 $05 $00 $20
  btst.b #BIT_SEVEN,label_byte_a.w ; @BT $08 $38 $00 $07 $20 $00
  btst.l #BIT_THIRTY_ONE,d7       ; @BT $08 $07 $00 $1F
  btst.b #BIT_TWO,ADDR_LONG.l     ; @BT $08 $39 $00 $02 $00 $12 $34 $56
.db "<11" ; @BT END

.db "12>" ; @BT TEST-12 12 START
  btst.b d0,(a4)                  ; @BT $01 $14
  btst.b d1,(a5)+                 ; @BT $03 $1D
  btst.b d2,-(a6)                 ; @BT $05 $26
  btst.b d3,(DISP_NEG2,a0)        ; @BT $07 $28 $FF $FE
  btst.b d4,label_byte_b.w        ; @BT $09 $38 $20 $01
  btst.l d7,d0                    ; @BT $0F $00
.db "<12" ; @BT END

.db "13>" ; @BT TEST-13 13 START
  chk.w d1,d0                     ; @BT $41 $81
  chk.w ($1234).w,d2              ; @BT $45 $B8 $12 $34
  chk.w #$1234,d3                 ; @BT $47 $BC $12 $34
.db "<13" ; @BT END

.db "14>" ; @BT TEST-14 14 START
  nbcd.b d0                       ; @BT $48 $00
  nbcd.b (a1)                     ; @BT $48 $11
  nbcd.b -(a2)                    ; @BT $48 $22
  nbcd.b $1234.w                  ; @BT $48 $38 $12 $34
.db "<14" ; @BT END

.db "15>" ; @BT TEST-15 15 START
  tas.b d0                        ; @BT $4A $C0
  tas.b (a0)                      ; @BT $4A $D0
  tas.b $1234.w                   ; @BT $4A $F8 $12 $34
.db "<15" ; @BT END