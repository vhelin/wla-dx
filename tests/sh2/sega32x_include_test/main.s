; @BT linked.rom

.INCDIR "../../../include/sega32x"
.INCLUDE "../../../memorymaps/sh232x.i"
.INCLUDE "sh2.inc"
.INCLUDE "macros.inc"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $10000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

Start:
  MARS_SH2_DISABLE_INTERRUPTS          ; @BT TEST-01 -a 0 START E0 0F 40 08 40 08 40 0E END
  MARS_SH2_ENABLE_INTERRUPTS           ; @BT TEST-02 -a 8 START E0 01 40 08 40 08 40 0E END
  mov.l @(MarsVdpMode,pc),r1
  mov.w @(MarsVideoValue,pc),r0
  mov.w r0,@r1
  bra Done
  nop

.ALIGN 4
MarsVdpMode:
  .DD MARS_VDP_DISPMODE
MarsVideoValue:
  .DW MARS_NTSC_FORMAT | MARS_VDP_PRIO_32X | MARS_224_LINES | MARS_VDP_MODE_256
MarsWhite:
  .DW MARS_COLOR_WHITE                 ; @BT TEST-03 -a $1C START 20 00 41 00 80 81 7F FF END

Done:
  bra Done
  nop