.INCDIR "../../../include/pcengine"
.INCLUDE "../../../memorymaps/pcengine.i"
.INCLUDE "hardware.inc"
.INCLUDE "vectors.inc"
.INCLUDE "macros.inc"

.ROMBANKSIZE $2000
.ROMBANKS 32

.EMPTYFILL $ff

.DEFINE VDC_MAWR              $00
.DEFINE VDC_VWR               $02
.DEFINE VDC_CR                $05
.DEFINE VDC_RCR               $06
.DEFINE VDC_BXR               $07
.DEFINE VDC_BYR               $08
.DEFINE VDC_MWR               $09
.DEFINE VDC_HSR               $0a
.DEFINE VDC_HDR               $0b
.DEFINE VDC_VSR               $0c
.DEFINE VDC_VDR               $0d
.DEFINE VDC_VCR               $0e

.DEFINE VDC_PORT_SELECT       $0200
.DEFINE VDC_PORT_DATA_LO      $0202
.DEFINE VDC_PORT_DATA_HI      $0203

.DEFINE VDC_STATUS_VBLANK     $20
.DEFINE PCE_RAM_PAGE          $f8
.DEFINE BLANK_TILE_HIGH       $02
.DEFINE TEXT_TILE_HIGH        $12
.DEFINE FONT_VRAM_ADDRESS     $2010
.DEFINE BLANK_VRAM_ADDRESS    $2000
.DEFINE HELLO_BAT_ADDRESS     $070a

.MACRO VDC_SELECT
  lda #\1
  sta.w VDC_PORT_SELECT
.ENDM

.MACRO VDC_WRITE
  lda #\1
  sta.w VDC_PORT_SELECT
  lda #\2
  sta.w VDC_PORT_DATA_LO
  lda #\3
  sta.w VDC_PORT_DATA_HI
.ENDM

.BANK 0 SLOT 7
.ORGA $e000

Reset:
  PCE_DISABLE_INTERRUPTS
  PCE_CLEAR_DECIMAL
  PCE_MAP_IO_PAGE_TO_MPR0
  lda #PCE_RAM_PAGE
  tam #2
  ldx #$ff
  txs

  lda #$07
  sta.w PCE_REG_IRQ_DISABLE
  lda #$00
  sta.w PCE_REG_TIMER_CONTROL

  jsr DisableDisplay
  jsr InitVideo
  jsr LoadPalette
  jsr LoadBlankTile
  jsr LoadFont
  jsr ClearBat
  jsr DrawHelloWorld
  jsr InitPsg
  jsr EnableDisplay
  jsr PlayBeeps

Forever:
  jmp Forever


DisableDisplay:
  VDC_WRITE VDC_CR, $00, $00
  rts


InitVideo:
  VDC_WRITE VDC_RCR, $00, $00

  VDC_WRITE VDC_BXR, $00, $00

  VDC_WRITE VDC_BYR, $00, $00

  VDC_WRITE VDC_MWR, $70, $00

  VDC_WRITE VDC_HSR, $02, $02

  VDC_WRITE VDC_HDR, $1f, $04

  VDC_WRITE VDC_VSR, $02, $17

  VDC_WRITE VDC_VDR, $df, $00

  VDC_WRITE VDC_VCR, $0c, $00
  rts


LoadPalette:
  lda #$04
  sta.w PCE_REG_VCE_CONTROL
  lda #$00
  sta.w PCE_REG_VCE_ADDRESS_LO
  sta.w PCE_REG_VCE_ADDRESS_HI
  ldx #$00
LoadPaletteLoop:
  lda.w PaletteData,x
  sta.w PCE_REG_VCE_DATA_LO
  inx
  lda.w PaletteData,x
  sta.w PCE_REG_VCE_DATA_HI
  inx
  cpx #PaletteDataEnd-PaletteData
  bne LoadPaletteLoop
  rts


LoadBlankTile:
  VDC_WRITE VDC_MAWR, $00, $20
  VDC_SELECT VDC_VWR
  ldx #$10
  lda #$00
LoadBlankTileLoop:
  sta.w VDC_PORT_DATA_LO
  sta.w VDC_PORT_DATA_HI
  dex
  bne LoadBlankTileLoop
  rts


LoadFont:
  VDC_WRITE VDC_MAWR, $10, $20
  VDC_SELECT VDC_VWR
  ldx #$00
LoadFontLoop:
  lda.w FontData,x
  sta.w VDC_PORT_DATA_LO
  inx
  lda.w FontData,x
  sta.w VDC_PORT_DATA_HI
  inx
  cpx #FontDataEnd-FontData
  bne LoadFontLoop
  rts


ClearBat:
  VDC_WRITE VDC_MAWR, $00, $00
  VDC_SELECT VDC_VWR
  ldy #$20
ClearBatPage:
  ldx #$00
ClearBatLoop:
  lda #$00
  sta.w VDC_PORT_DATA_LO
  lda #BLANK_TILE_HIGH
  sta.w VDC_PORT_DATA_HI
  inx
  bne ClearBatLoop
  dey
  bne ClearBatPage
  rts


DrawHelloWorld:
  VDC_WRITE VDC_MAWR, $0a, $07
  VDC_SELECT VDC_VWR
  ldx #$00
DrawHelloWorldLoop:
  lda.w HelloWorldText,x
  sta.w VDC_PORT_DATA_LO
  lda #TEXT_TILE_HIGH
  sta.w VDC_PORT_DATA_HI
  inx
  cpx #HelloWorldTextEnd-HelloWorldText
  bne DrawHelloWorldLoop
  rts


InitPsg:
  lda #$00
  sta.w PCE_REG_PSG_CHANNEL_SELECT
  sta.w PCE_REG_PSG_CONTROL
  lda #$ff
  sta.w PCE_REG_PSG_MAIN_VOLUME
  sta.w PCE_REG_PSG_BALANCE
  ldx #$00
LoadWaveLoop:
  lda.w SquareWave,x
  sta.w PCE_REG_PSG_WAVE_DATA
  inx
  cpx #SquareWaveEnd-SquareWave
  bne LoadWaveLoop
  rts


EnableDisplay:
  VDC_WRITE VDC_CR, $88, $00
  rts


PlayBeeps:
  lda #$80
  sta.w PCE_REG_PSG_FREQ_LO
  lda #$02
  sta.w PCE_REG_PSG_FREQ_HI
  jsr StartTone
  ldx #$12
  jsr WaitFrames
  jsr StopTone
  ldx #$08
  jsr WaitFrames

  lda #$20
  sta.w PCE_REG_PSG_FREQ_LO
  lda #$03
  sta.w PCE_REG_PSG_FREQ_HI
  jsr StartTone
  ldx #$12
  jsr WaitFrames
  jsr StopTone
  rts


StartTone:
  lda #$9f
  sta.w PCE_REG_PSG_CONTROL
  rts


StopTone:
  lda #$00
  sta.w PCE_REG_PSG_CONTROL
  rts


WaitFrames:
  jsr WaitVBlank
  dex
  bne WaitFrames
  rts


WaitVBlank:
  lda.w VDC_PORT_SELECT
WaitVBlankLoop:
  lda.w VDC_PORT_SELECT
  and #VDC_STATUS_VBLANK
  beq WaitVBlankLoop
  rts


Irq2:
Irq1:
Timer:
Nmi:
  rti


HelloWorldText:
  .DB $01,$02,$03,$03,$04,$00,$05,$04,$06,$03,$07
HelloWorldTextEnd:

PaletteData:
  .DW $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
  .DW $0000,$0000,$0000,$0000,$0000,$0000,$0000,$0000
  .DW $01ff,$01ff,$01ff,$01ff,$01ff,$01ff,$01ff,$01ff
  .DW $01ff,$01ff,$01ff,$01ff,$01ff,$01ff,$01ff,$01ff
PaletteDataEnd:

SquareWave:
  .DB $1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f,$1f
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
SquareWaveEnd:

FontData:
  .DB $42,$00,$42,$00,$42,$00,$7e,$00,$42,$00,$42,$00,$42,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

  .DB $7e,$00,$40,$00,$40,$00,$7c,$00,$40,$00,$40,$00,$7e,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

  .DB $40,$00,$40,$00,$40,$00,$40,$00,$40,$00,$40,$00,$7e,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

  .DB $3c,$00,$42,$00,$42,$00,$42,$00,$42,$00,$42,$00,$3c,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

  .DB $42,$00,$42,$00,$42,$00,$5a,$00,$5a,$00,$66,$00,$42,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

  .DB $7c,$00,$42,$00,$42,$00,$7c,$00,$48,$00,$44,$00,$42,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

  .DB $78,$00,$44,$00,$42,$00,$42,$00,$42,$00,$44,$00,$78,$00,$00,$00
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
FontDataEnd:

.ORGA $fff6
  PCE_VECTOR_TABLE Irq2, Irq1, Timer, Nmi, Reset
