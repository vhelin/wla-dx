; @BT linked.rom

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 $000000 $100000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.EMPTYFILL $AA

.BANK 0 SLOT 0
.ORG 0

RomStart:
  mov #1,r0                  ; @BT TEST-01 -a 0 START E0 01 70 01 30 1C AF FB 00 09 END
  add #1,r0
  add r1,r0
  bra RomStart
  nop

.ORG $20
  .db $12, $34, $56, $78     ; @BT TEST-02 -a $20 START 12 34 56 78 END