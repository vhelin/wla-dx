; @BT linked.rom

; Discussion #740: failed An/SP probes must not consume part of an expression.

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $000000 SIZE $4000
  SLOT 1 START $A00000 SIZE $2000
  SLOT 2 START $FF0000 SIZE $10000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $4000
  BANKS 1
.ENDRO

.RAMSECTION "Game RAM" BANK 0 SLOT 2
vblankOver dw
joypad1    db
joypad1O   db
joypad1N   db
joypad2    db
joypad2O   db
joypad2N   db
satMirror: ds 640
numSprites: db
.ENDS

.DEFINE sOffset 6
.DEFINE aOffset 8
.DEFINE spOffset 10
.DEFINE a0Offset 12
.DEFINE A7Offset 14

.BANK 0 SLOT 0
.ORG 0

.db "01>" ; @BT TEST-01 01 START
  clr.b (numSprites)      ; @BT $42 $39 $00 $FF $02 $88
  clr.l (satMirror+0)     ; @BT $42 $B9 $00 $FF $00 $08
  clr.l (satMirror+4)     ; @BT $42 $B9 $00 $FF $00 $0C
  rts                    ; @BT $4E $75
.db "<01" ; @BT END

.db "02>" ; @BT TEST-02 02 START
  clr.l (sBuffer+0)       ; @BT $42 $B9 $00 $00 $20 $00
  clr.l (SBuffer+1)       ; @BT $42 $B9 $00 $00 $20 $02
  clr.l (aBuffer+2)       ; @BT $42 $B9 $00 $00 $20 $04
  clr.l (ABuffer+3)       ; @BT $42 $B9 $00 $00 $20 $06
  clr.l (spBuffer+4)      ; @BT $42 $B9 $00 $00 $20 $08
  clr.l (SPBuffer+5)      ; @BT $42 $B9 $00 $00 $20 $0A
  clr.l (a0Buffer+6)      ; @BT $42 $B9 $00 $00 $20 $0C
  clr.l (A7Buffer+7)      ; @BT $42 $B9 $00 $00 $20 $0E
  clr.l (s0Buffer+8)      ; @BT $42 $B9 $00 $00 $20 $10
.db "<02" ; @BT END

.db "03>" ; @BT TEST-03 03 START
  clr.l (satMirror)      ; @BT $42 $B9 $00 $FF $00 $08
  clr.l (a0Buffer)       ; @BT $42 $B9 $00 $00 $20 $06
  clr.l (spBuffer)       ; @BT $42 $B9 $00 $00 $20 $04
  clr.l (sBuffer+1).w    ; @BT $42 $B8 $20 $01
  clr.l (aBuffer+2).w    ; @BT $42 $B8 $20 $04
  clr.l (spBuffer+3).w   ; @BT $42 $B8 $20 $07
  clr.l (a0Buffer+4).w   ; @BT $42 $B8 $20 $0A
.db "<03" ; @BT END

.db "04>" ; @BT TEST-04 04 START
  clr.l (sOffset+1,a0)     ; @BT $42 $A8 $00 $07
  clr.l (aOffset+1,sp)     ; @BT $42 $AF $00 $09
  clr.l (spOffset+1,a1,d2) ; @BT $42 $B1 $20 $0B
  clr.l (a0Offset+1,SP)    ; @BT $42 $AF $00 $0D
  clr.l (A7Offset+1,a2,d3) ; @BT $42 $B2 $30 $0F
.db "<04" ; @BT END

.db "05>" ; @BT TEST-05 05 START
  clr.l (a0)          ; @BT $42 $90
  clr.l (sp)          ; @BT $42 $97
  clr.l (SP)          ; @BT $42 $97
  clr.l (sp)+         ; @BT $42 $9F
  clr.l -(SP)         ; @BT $42 $A7
  clr.l (4,sp)        ; @BT $42 $AF $00 $04
  clr.l (4,SP,d2)     ; @BT $42 $B7 $20 $04
  clr.l 4(sp)         ; @BT $42 $AF $00 $04
  move.w sp,d0        ; @BT $30 $0F
  movem.l d0/sp,-(sp) ; @BT $48 $E7 $80 $01
.db "<05" ; @BT END

.ORG $2000
sBuffer:  .db 0
SBuffer:  .db 0
aBuffer:  .db 0
ABuffer:  .db 0
spBuffer: .db 0
SPBuffer: .db 0
a0Buffer: .db 0
A7Buffer: .db 0
s0Buffer: .db 0
