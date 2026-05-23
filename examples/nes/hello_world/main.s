.INCLUDE "../../../memorymaps/nes32.i"

.ROMBANKSIZE $8000
.ROMBANKS 1

.EMPTYFILL $00

.INESHEADER
  PRGROMSIZE 2
  CHRROMSIZE 0
  MAPPER 0
  MIRRORING HORIZONTAL
.ENDINES

.DEFINE PPUCTRL             $2000
.DEFINE PPUMASK             $2001
.DEFINE PPUSTATUS           $2002
.DEFINE PPUSCROLL           $2005
.DEFINE PPUADDR             $2006
.DEFINE PPUDATA             $2007
.DEFINE APU_PULSE1_VOLUME   $4000
.DEFINE APU_PULSE1_SWEEP    $4001
.DEFINE APU_PULSE1_TIMER    $4002
.DEFINE APU_PULSE1_LENGTH   $4003
.DEFINE APU_DMC_FREQ        $4010
.DEFINE APU_STATUS          $4015
.DEFINE APU_FRAME_COUNTER   $4017

.BANK 0 SLOT 2
.ORGA $8000

Reset:
  sei
  cld
  ldx #$40
  stx APU_FRAME_COUNTER
  ldx #$ff
  txs
  inx
  stx PPUCTRL
  stx PPUMASK
  stx APU_DMC_FREQ
  stx APU_STATUS

  bit PPUSTATUS
  jsr WaitVBlank
  jsr WaitVBlank

  jsr LoadPalette
  jsr LoadFont
  jsr ClearNametable
  jsr DrawHelloWorld
  jsr EnableDisplay
  jsr PlayBeeps

Forever:
  jmp Forever


WaitVBlank:
  bit PPUSTATUS
WaitVBlankLoop:
  bit PPUSTATUS
  bpl WaitVBlankLoop
  rts


LoadPalette:
  lda PPUSTATUS
  lda #$3f
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldx #$00
LoadPaletteLoop:
  lda.w PaletteData,x
  sta PPUDATA
  inx
  cpx #PaletteDataEnd-PaletteData
  bne LoadPaletteLoop
  rts


LoadFont:
  lda PPUSTATUS
  lda #$00
  sta PPUADDR
  sta PPUADDR
  ldx #$00
LoadFontLoop:
  lda.w FontData,x
  sta PPUDATA
  inx
  cpx #FontDataEnd-FontData
  bne LoadFontLoop
  rts


ClearNametable:
  lda PPUSTATUS
  lda #$20
  sta PPUADDR
  lda #$00
  sta PPUADDR
  ldy #$04
ClearNametablePage:
  ldx #$00
ClearNametableByte:
  sta PPUDATA
  inx
  bne ClearNametableByte
  dey
  bne ClearNametablePage
  rts


DrawHelloWorld:
  lda PPUSTATUS
  lda #$21
  sta PPUADDR
  lda #$ca
  sta PPUADDR
  ldx #$00
DrawHelloWorldLoop:
  lda.w HelloWorldText,x
  sta PPUDATA
  inx
  cpx #HelloWorldTextEnd-HelloWorldText
  bne DrawHelloWorldLoop
  rts


EnableDisplay:
  lda PPUSTATUS
  lda #$00
  sta PPUSCROLL
  sta PPUSCROLL
  sta PPUCTRL
  lda #%00001010
  sta PPUMASK
  rts


PlayBeeps:
  lda #$01
  sta APU_STATUS
  lda #$9f
  sta APU_PULSE1_VOLUME
  lda #$00
  sta APU_PULSE1_SWEEP
  lda #$7e
  sta APU_PULSE1_TIMER
  lda #$08
  sta APU_PULSE1_LENGTH
  jsr BeepDelay

  lda #$00
  sta APU_PULSE1_VOLUME
  jsr SilenceDelay

  lda #$9f
  sta APU_PULSE1_VOLUME
  lda #$a8
  sta APU_PULSE1_TIMER
  lda #$08
  sta APU_PULSE1_LENGTH
  jsr BeepDelay

  lda #$00
  sta APU_PULSE1_VOLUME
  sta APU_STATUS
  rts


BeepDelay:
  ldy #$a0
BeepDelayOuter:
  ldx #$00
BeepDelayInner:
  dex
  bne BeepDelayInner
  dey
  bne BeepDelayOuter
  rts


SilenceDelay:
  ldy #$40
SilenceDelayOuter:
  ldx #$00
SilenceDelayInner:
  dex
  bne SilenceDelayInner
  dey
  bne SilenceDelayOuter
  rts


Nmi:
  rti


Irq:
  rti


PaletteData:
  .DB $0f,$30,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f
  .DB $0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f,$0f
PaletteDataEnd:

HelloWorldText:
  .DB 1,2,3,3,4,0,5,4,6,3,7
HelloWorldTextEnd:

FontData:
  .DB $00,$00,$00,$00,$00,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $42,$42,$42,$7e,$42,$42,$42,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $7e,$40,$40,$7c,$40,$40,$7e,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $40,$40,$40,$40,$40,$40,$7e,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $3c,$42,$42,$42,$42,$42,$3c,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $42,$42,$42,$5a,$5a,$66,$42,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $7c,$42,$42,$7c,$48,$44,$42,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00

  .DB $78,$44,$42,$42,$42,$44,$78,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00
FontDataEnd:

.ORGA $fffa
  .DW Nmi
  .DW Reset
  .DW Irq