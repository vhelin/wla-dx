.INCDIR "../../../include/sega32x"
.INCLUDE "../../../memorymaps/sh232x.i"
.INCLUDE "sh2.inc"
.INCLUDE "macros.inc"

.DEFINE MARS_PRIMARY_STACK $0603F800
.DEFINE MARS_SECONDARY_STACK $06040000

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $20000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

  .DD $01000000
  .REPT 63
    .DD $000003F0
  .ENDR

.ORG $100
  .DB "SEGA 32X        "

.ORG $120
  .DB "WLA DX HELLO 32X                                "

.ORG $150
  .DB "WLA DX HELLO 32X                                "

.ORG $180
  .DB "GM WLA32X-00"

.ORG $18E
  .DW $0000

.ORG $190
  .DB "J6              "

.ORG $1A0
  .DD $00000000, $0001FFFF
  .DD $00FF0000, $00FFFFFF

.ORG $1F0
  .DB "F               "

.ORG $200
  .REPT 64
    .DW $4EF9
    .DD $00880800
  .ENDR

.ORG $3C0
MarsHeader:
  .DB "WLA HELLO 32X   "
  .DD $00000000
  .DD Sh2Payload - MARS_CART_ROM
  .DD $00000000
  .DD Sh2PayloadEnd - Sh2Payload
  .DD MARS_SDRAM + (PrimaryEntry - Sh2Payload)
  .DD MARS_SDRAM + (SecondaryEntry - Sh2Payload)
  .DD MARS_SDRAM + (PrimaryVbr - Sh2Payload)
  .DD MARS_SDRAM + (SecondaryVbr - Sh2Payload)

.ORG $3F0
  .INCBIN "bootstrap.bin"

.ORG $800
  .INCBIN "m68k.bin"

.ORG $1000
Sh2Payload:

PrimaryVbr:
  .DD MARS_SDRAM + (PrimaryEntry - Sh2Payload)
  .DD MARS_PRIMARY_STACK
  .DD MARS_SDRAM + (PrimaryEntry - Sh2Payload)
  .DD MARS_PRIMARY_STACK
  .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .DD $00000000
  .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .DD $00000000
  .DD $00000000
  .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .REPT 19
    .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .ENDR
  .REPT 32
    .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .ENDR
  .REPT 8
    .DD MARS_SDRAM + (PrimaryException - Sh2Payload)
  .ENDR

SecondaryVbr:
  .DD MARS_SDRAM + (SecondaryEntry - Sh2Payload)
  .DD MARS_SECONDARY_STACK
  .DD MARS_SDRAM + (SecondaryEntry - Sh2Payload)
  .DD MARS_SECONDARY_STACK
  .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .DD $00000000
  .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .DD $00000000
  .DD $00000000
  .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .REPT 19
    .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .ENDR
  .REPT 32
    .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .ENDR
  .REPT 8
    .DD MARS_SDRAM + (SecondaryException - Sh2Payload)
  .ENDR

PrimaryEntry:
  mov.l @(PrimaryStackValue, pc), r15
  MARS_SH2_DISABLE_INTERRUPTS

  mov.l @(Comm0Address, pc), r1
  mov.l @(MasterReadyValue, pc), r0
  mov.l r0, @r1

WaitMasterRelease:
  mov.l @r1, r2
  cmp/eq r0, r2
  bt WaitMasterRelease

  bsr InitVideo
  nop
  bsr DrawText
  nop
  bsr ShowFrame
  nop
  bsr PlayBeep
  nop
  bsr DelayBetweenBeeps
  nop
  bsr PlayBeep
  nop

PrimaryLoop:
  cmp/eq r0, r0
  bt PrimaryLoop

PrimaryException:
  cmp/eq r0, r0
  bt PrimaryException

.ALIGN 4
PrimaryStackValue:
  .DD MARS_PRIMARY_STACK
Comm0Address:
  .DD MARS_SYS_COMM0
MasterReadyValue:
  .DD $4D5F4F4B

SecondaryEntry:
  mov.l @(SecondaryStackValue, pc), r15
  MARS_SH2_DISABLE_INTERRUPTS

  mov.l @(Comm4Address, pc), r1
  mov.l @(SlaveReadyValue, pc), r0
  mov.l r0, @r1

WaitSlaveRelease:
  mov.l @r1, r2
  cmp/eq r0, r2
  bt WaitSlaveRelease

SecondaryLoop:
  cmp/eq r0, r0
  bt SecondaryLoop

SecondaryException:
  cmp/eq r0, r0
  bt SecondaryException

.ALIGN 4
SecondaryStackValue:
  .DD MARS_SECONDARY_STACK
Comm4Address:
  .DD MARS_SYS_COMM4
SlaveReadyValue:
  .DD $535F4F4B

InitVideo:
  mov.l @(IntMaskAddress, pc), r1
  mov.w @(Sh2AccessValue, pc), r0
  mov.w r0, @r1

  mov.l @(PaletteAddress, pc), r1
  mov #0, r0
  mov.w r0, @r1
  add #2, r1
  mov.w @(WhiteColorValue, pc), r0
  mov.w r0, @r1

  mov.l @(FrameBufferAddress, pc), r1
  mov.w @(LineTableStartValue, pc), r3
  mov.w @(LineStrideWordsValue, pc), r4
  mov.w @(LineCountValue, pc), r2

InitLineTableLoop:
  mov.w r3, @r1
  add #2, r1
  add r4, r3
  dt r2
  bf InitLineTableLoop
  nop

  mov.l @(FrameBufferPixelsAddress, pc), r1
  mov.l @(ClearWordCount, pc), r2
  mov #0, r0

ClearFrameBufferLoop:
  mov.w r0, @r1
  add #2, r1
  dt r2
  bf ClearFrameBufferLoop
  nop

  mov.l @(VdpModeAddress, pc), r1
  mov.w @(VdpModeValue, pc), r0
  mov.w r0, @r1
  rts
  nop

.ALIGN 4
IntMaskAddress:
  .DD MARS_SYS_INTMSK
PaletteAddress:
  .DD MARS_CRAM
FrameBufferAddress:
  .DD MARS_FRAMEBUFFER
FrameBufferPixelsAddress:
  .DD MARS_FRAMEBUFFER + $0200
VdpModeAddress:
  .DD MARS_VDP_DISPMODE
ClearWordCount:
  .DD 35840
Sh2AccessValue:
  .DW MARS_SH2_ACCESS_VDP
WhiteColorValue:
  .DW MARS_COLOR_WHITE
LineTableStartValue:
  .DW $0100
LineStrideWordsValue:
  .DW 160
LineCountValue:
  .DW 224
VdpModeValue:
  .DW MARS_NTSC_FORMAT | MARS_VDP_PRIO_32X | MARS_224_LINES | MARS_VDP_MODE_256

DrawText:
  sts.l pr, @-r15
  mov.l @(TextDestinationAddress, pc), r8
  mov.l @(TextRowsAddress, pc), r10
  mov.w @(TextRowRemainderValue, pc), r12
  mov.w @(TextRowCountValue, pc), r11
  mov #0, r6
  mov.w @(PackedWhitePixels, pc), r7

DrawTextRowLoop:
  mov r10, r9
  bsr DrawTextOneRow
  nop
  add r12, r8
  mov r10, r9
  bsr DrawTextOneRow
  nop
  add r12, r8
  add #11, r10
  dt r11
  bf DrawTextRowLoop
  nop

  lds.l @r15+, pr
  rts
  nop

DrawTextOneRow:
  mov #11, r5

DrawTextCharLoop:
  mov.b @r9+, r0
  extu.b r0, r0
  mov #16, r1
  mov #5, r4

DrawTextBitLoop:
  tst r1, r0
  bt DrawTextBlankPixel
  nop
  mov.w r7, @r8
  bra DrawTextNextPixel
  nop

DrawTextBlankPixel:
  mov.w r6, @r8

DrawTextNextPixel:
  add #2, r8
  shlr r1
  dt r4
  bf DrawTextBitLoop
  nop

  mov.w r6, @r8
  add #2, r8
  dt r5
  bf DrawTextCharLoop
  nop
  rts
  nop

.ALIGN 4
TextDestinationAddress:
  .DD MARS_FRAMEBUFFER + $85A0
TextRowsAddress:
  .DD MARS_SDRAM + (TextRows - Sh2Payload)
TextRowRemainderValue:
  .DW 188
TextRowCountValue:
  .DW 7
PackedWhitePixels:
  .DW $0101

TextRows:
  .DB $11, $1F, $10, $10, $0E, $00, $11, $0E, $1E, $10, $1E
  .DB $11, $10, $10, $10, $11, $00, $11, $11, $11, $10, $11
  .DB $11, $10, $10, $10, $11, $00, $11, $11, $11, $10, $11
  .DB $1F, $1E, $10, $10, $11, $00, $15, $11, $1E, $10, $11
  .DB $11, $10, $10, $10, $11, $00, $15, $11, $14, $10, $11
  .DB $11, $10, $10, $10, $11, $00, $1B, $11, $12, $10, $11
  .DB $11, $1F, $1F, $1F, $0E, $00, $11, $0E, $11, $1F, $1E

.ALIGN 4

ShowFrame:
  mov.l @(VdpFrameControlAddress, pc), r1
  mov.w @r1, r0
  mov #1, r2
  and r2, r0
  xor r2, r0
  mov.w r0, @r1
  rts
  nop

.ALIGN 4
VdpFrameControlAddress:
  .DD MARS_VDP_FBCTL

PlayBeep:
  mov.l @(PwmControlAddress, pc), r1
  mov.w @(PwmControlValue, pc), r0
  mov.w r0, @r1

  mov.l @(PwmCycleAddress, pc), r1
  mov.w @(PwmCycleValue, pc), r0
  mov.w r0, @r1

  mov.l @(PwmMonoAddress, pc), r1
  mov.w @(BeepLoopCount, pc), r2
  mov.w @(BeepHighSample, pc), r3
  mov.w @(BeepLowSample, pc), r4

BeepLoop:
  mov.w r3, @r1
  mov.w @(BeepDelayCount, pc), r5

BeepHighDelay:
  dt r5
  bf BeepHighDelay
  nop

  mov.w r4, @r1
  mov.w @(BeepDelayCount, pc), r5

BeepLowDelay:
  dt r5
  bf BeepLowDelay
  nop

  dt r2
  bf BeepLoop
  nop
  rts
  nop

DelayBetweenBeeps:
  mov.l @(BetweenBeepsCount, pc), r0

DelayBetweenBeepsLoop:
  dt r0
  bf DelayBetweenBeepsLoop
  nop
  rts
  nop

.ALIGN 4
PwmControlAddress:
  .DD MARS_PWM_CTRL
PwmCycleAddress:
  .DD MARS_PWM_CYCLE
PwmMonoAddress:
  .DD MARS_PWM_MONO
BetweenBeepsCount:
  .DD 1600000
PwmControlValue:
  .DW MARS_PWM_CTRL_STEREO
PwmCycleValue:
  .DW 400
BeepLoopCount:
  .DW 900
BeepHighSample:
  .DW $0D80
BeepLowSample:
  .DW $0200
BeepDelayCount:
  .DW 700

.ALIGN 4
Sh2PayloadEnd: