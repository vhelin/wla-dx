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
.ORG 0

.SECTION "MAIN" FORCE
MainStart:
  bt MainForward              ; @BT TEST-01 -a 0 START 89 01 00 09 00 09 8B FB A0 7A 00 09 B0 7A 00 09 91 7A D2 3E AF F4 00 09 END
  nop
  nop
MainForward:
  bf MainStart
  bra ExternalBranchTarget
  nop
  bsr ExternalSubroutine
  nop
  mov.w @(ExternalWord,pc),r1
  mov.l @(ExternalLong,pc),r2
  bra MainStart
  nop
.ENDS