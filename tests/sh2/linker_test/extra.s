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

.BANK 0 SLOT 0
.ORG $100

.SECTION "EXTRA" FORCE
ExternalBranchTarget:
  bt MainForward              ; @BT TEST-02 -a $100 START 89 81 00 09 00 0B 00 09 12 34 AA AA 89 AB CD EF END
  nop
ExternalSubroutine:
  rts
  nop
ExternalWord:
  .dw $1234
  .db $aa, $aa
ExternalLong:
  .db $89, $ab, $cd, $ef
.ENDS