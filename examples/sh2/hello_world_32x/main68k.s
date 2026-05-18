.INCDIR "../../../include/sega32x"
.INCLUDE "memorymap_hot68k.i"
.INCLUDE "68k.inc"

.DEFINE REG_MD_VDP_CTRL $C00004

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

HotStart:
  move.w #$2700, sr
  move.w #$0000, REG_32X_BANK_SET
  move.b #$00, REG_32X_DREQ_STATUS

WaitMasterOk:
  move.l REG_32X_COMM0, d0
  cmpi.l #$4D5F4F4B, d0
  bne WaitMasterOk

WaitSlaveOk:
  move.l REG_32X_COMM4, d0
  cmpi.l #$535F4F4B, d0
  bne WaitSlaveOk

  bsr.w InitMdVdp
  MARS_68K_SET_FM
  clr.l REG_32X_COMM0
  clr.l REG_32X_COMM4

MainLoop:
  bra MainLoop

InitMdVdp:
  lea VdpRegisterValues, a0
  move.w #((VdpRegisterValuesEnd-VdpRegisterValues)/2)-1, d7

InitMdVdpLoop:
  move.w (a0)+, REG_MD_VDP_CTRL
  dbra d7, InitMdVdpLoop
  rts

VdpRegisterValues:
  .DW $8004, $8174, $8230, $832C
  .DW $8407, $855E, $8600, $8700
  .DW $8800, $8900, $8A01, $8B00
  .DW $8C81, $8D2E, $8E00, $8F02
  .DW $9011, $9100, $92FF
VdpRegisterValuesEnd: