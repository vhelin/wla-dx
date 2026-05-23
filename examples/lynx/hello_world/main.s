.INCDIR "../../../include/lynx"
.INCLUDE "../../../memorymaps/lynx.i"
.INCLUDE "hardware.inc"
.INCLUDE "macros.inc"

.ROMBANKSIZE $10000
.ROMBANKS 1

.EMPTYFILL $00

.DEFINE LYNX_BANK0_BLOCK_SIZE       $400
.DEFINE LYNX_DIRECTORY_OFFSET       $00CB
.DEFINE LYNX_MAIN_LOAD_ADDRESS      $0200
.DEFINE LYNX_MAIN_FILE_OFFSET       $0200
.DEFINE LYNX_BOOT_SECONDARY_RUN     $FB68

.DEFINE LYNX_IODAT_SHADOW           $1A
.DEFINE LYNX_IODIR_SHADOW           $1B
.DEFINE LYNX_VIDDMA_SHADOW          $1C
.DEFINE LYNX_SPRSYS_SHADOW          $1D
.DEFINE LYNX_FILE_ENTRY             $1E
.DEFINE LYNX_FILE_START_BLOCK       $1E
.DEFINE LYNX_FILE_BLOCK_OFFSET      $1F
.DEFINE LYNX_FILE_DEST_ADDR         $22
.DEFINE LYNX_FILE_FILE_LEN          $24
.DEFINE LYNX_FILE_CURR_BLOCK        $26
.DEFINE LYNX_FILE_BLOCK_BYTE        $27
.DEFINE LYNX_FILE_DEST_PTR          $29

.DEFINE LYNX_CLEAR_PTR              $30
.DEFINE LYNX_GLYPH_PTR              $32
.DEFINE LYNX_DEST_PTR               $34
.DEFINE LYNX_ROW_PTR                $36
.DEFINE LYNX_GLYPH_BITS             $38
.DEFINE LYNX_GLYPH_ROW              $39

.DEFINE LYNX_FRAMEBUFFER            $C038
.DEFINE LYNX_FRAMEBUFFER_SIZE       $1FE0
.DEFINE LYNX_SCREEN_STRIDE          80
.DEFINE LYNX_TEXT_DEST              $CEEF
.DEFINE LYNX_TEXT_COLOR_BYTE        $FF
.DEFINE LYNX_DISPLAY_CONTROL        $0D

.LYNXHEADER
  BANK0BLOCKSIZE LYNX_BANK0_BLOCK_SIZE
  BANK1BLOCKSIZE 0
  NAME "HELLO WORLD"
  MANUFACTURER "WLA DX"
  ROTATION NONE
  AUDIN OFF
  EEPROM NONE
.ENDLYNX

.BANK 0 SLOT 0
.ORG 0

LynxSignedBootLoader:
  .DB $FF,$B6,$BB,$82,$D5,$9F,$48,$CF
  .DB $23,$37,$8E,$07,$38,$F5,$B6,$30
  .DB $D6,$2F,$12,$29,$9F,$43,$5B,$2E
  .DB $F5,$66,$5C,$DB,$93,$1A,$78,$55
  .DB $5E,$C9,$0D,$72,$1B,$E9,$D8,$4D
  .DB $2F,$E4,$95,$C0,$4F,$7F,$1B,$66
  .DB $8B,$A7,$FC,$21

LynxSecondaryLoader:
.DEFINE LYNX_BOOT_SECONDARY_DELTA LYNX_BOOT_SECONDARY_RUN-LynxSecondaryLoader
  ldx #$00
  ldy #8
LynxSecondaryReadDirectory:
  lda.w LYNX_RCART0
  sta.b LYNX_FILE_ENTRY,x
  inx
  dey
  bne LynxSecondaryReadDirectory

  lda.b LYNX_FILE_START_BLOCK
  sta.b LYNX_FILE_CURR_BLOCK
  jsr LynxSecondarySelectBlock+LYNX_BOOT_SECONDARY_DELTA

  lda.b LYNX_FILE_BLOCK_OFFSET+1
  eor #$FF
  tay
  lda.b LYNX_FILE_BLOCK_OFFSET
  eor #$FF
  tax
  jsr LynxSecondarySkip+LYNX_BOOT_SECONDARY_DELTA

  lda.b LYNX_FILE_DEST_ADDR
  ldx.b LYNX_FILE_DEST_ADDR+1
  sta.b LYNX_FILE_DEST_PTR
  stx.b LYNX_FILE_DEST_PTR+1
  lda.b LYNX_FILE_FILE_LEN+1
  eor #$FF
  tay
  lda.b LYNX_FILE_FILE_LEN
  eor #$FF
  tax
  jsr LynxSecondaryRead+LYNX_BOOT_SECONDARY_DELTA

  jmp (LYNX_FILE_DEST_ADDR)

LynxSecondarySkip:
  inx
  bne LynxSecondarySkipRead
  iny
  beq LynxSecondaryExit
LynxSecondarySkipRead:
  jsr LynxSecondaryReadByte+LYNX_BOOT_SECONDARY_DELTA
  bra LynxSecondarySkip

LynxSecondaryRead:
  inx
  bne LynxSecondaryReadStore
  iny
  beq LynxSecondaryExit
LynxSecondaryReadStore:
  jsr LynxSecondaryReadByte+LYNX_BOOT_SECONDARY_DELTA
  sta (LYNX_FILE_DEST_PTR)
  inc.b LYNX_FILE_DEST_PTR
  bne LynxSecondaryRead
  inc.b LYNX_FILE_DEST_PTR+1
  bra LynxSecondaryRead

LynxSecondaryReadByte:
  lda.w LYNX_RCART0
  inc.b LYNX_FILE_BLOCK_BYTE
  bne LynxSecondaryExit
  inc.b LYNX_FILE_BLOCK_BYTE+1
  bne LynxSecondaryExit

LynxSecondarySelectBlock:
  pha
  phx
  phy
  lda.b LYNX_IODAT_SHADOW
  and #$FC
  tay
  ora #2
  tax
  lda.b LYNX_FILE_CURR_BLOCK
  inc.b LYNX_FILE_CURR_BLOCK
  sec
  bra LynxSecondarySelectBit
LynxSecondarySelectPulse:
  bcc LynxSecondarySelectNoData
  stx.w LYNX_IODAT
  clc
LynxSecondarySelectNoData:
  inx
  stx.w LYNX_SYSCTL1
  dex
LynxSecondarySelectBit:
  stx.w LYNX_SYSCTL1
  rol
  sty.w LYNX_IODAT
  bne LynxSecondarySelectPulse
  lda.b LYNX_IODAT_SHADOW
  sta.w LYNX_IODAT
  stz.b LYNX_FILE_BLOCK_BYTE
  lda #<($100-(>LYNX_BANK0_BLOCK_SIZE))
  sta.b LYNX_FILE_BLOCK_BYTE+1
  ply
  plx
  pla

LynxSecondaryExit:
  rts

LynxSecondaryLoaderEnd:
.ASSERT LynxSecondaryLoaderEnd == LYNX_DIRECTORY_OFFSET, LDERROR, "Lynx secondary loader must end at the directory."

LynxDirectory:
  .DB 0
  .DW LYNX_MAIN_FILE_OFFSET
  .DB $88
  .DW Start
  .DW MainEnd-Start

.ORG LYNX_MAIN_FILE_OFFSET

Start:
  LYNX_DISABLE_INTERRUPTS
  LYNX_CLEAR_DECIMAL
  ldx #$FF
  txs

  lda #$0C
  sta.w LYNX_MAPCTL

  jsr DisableTimerInterrupts
  jsr InitMikey
  jsr InitSuzy
  jsr LoadPalette
  jsr DisableDisplay
  jsr SetViewPage
  jsr ClearFramebuffer
  jsr DrawHelloWorld
  jsr InitAudio
  jsr EnableDisplay
  jsr PlayBeeps

Forever:
  jmp Forever


DisableTimerInterrupts:
  lda #LYNX_TIM_ENABLE_INT
  trb.w LYNX_TIM0CTLA
  trb.w LYNX_TIM2CTLA
  trb.w LYNX_TIM7CTLA
  lda.w LYNX_INTSET
  sta.w LYNX_INTRST
  rts


InitMikey:
  ldx #MikeyInitRegEnd-MikeyInitReg-1
InitMikeyLoop:
  ldy.w MikeyInitReg,x
  lda.w MikeyInitData,x
  sta.w LYNX_MIKEY_BASE,y
  dex
  bpl InitMikeyLoop

  lda #$1B
  sta.b LYNX_IODAT_SHADOW
  lda #$1A
  sta.b LYNX_IODIR_SHADOW
  lda #LYNX_DISPLAY_CONTROL
  sta.b LYNX_VIDDMA_SHADOW
  lda #$24
  sta.b LYNX_SPRSYS_SHADOW
  rts


InitSuzy:
  ldx #SuzyInitRegEnd-SuzyInitReg-1
InitSuzyLoop:
  ldy.w SuzyInitReg,x
  lda.w SuzyInitData,x
  sta.w LYNX_SUZY_BASE,y
  dex
  bpl InitSuzyLoop
  rts


DisableDisplay:
  stz.w LYNX_DISPCTL
  rts


EnableDisplay:
  lda #LYNX_DISPLAY_CONTROL
  sta.w LYNX_DISPCTL
  rts


SetViewPage:
  lda #<LYNX_FRAMEBUFFER
  sta.w LYNX_DISPADRL
  lda #>LYNX_FRAMEBUFFER
  sta.w LYNX_DISPADRH
  rts


LoadPalette:
  ldx #$00
LoadPaletteLoop:
  lda.w PaletteGreen,x
  sta.w LYNX_GCOLMAP,x
  lda.w PaletteRedBlue,x
  sta.w LYNX_RBCOLMAP,x
  inx
  cpx #$10
  bne LoadPaletteLoop
  rts


ClearFramebuffer:
  lda #<LYNX_FRAMEBUFFER
  sta.b LYNX_CLEAR_PTR
  lda #>LYNX_FRAMEBUFFER
  sta.b LYNX_CLEAR_PTR+1
  lda #$00
  ldx #>LYNX_FRAMEBUFFER_SIZE
ClearFramebufferPage:
  ldy #$00
ClearFramebufferPageLoop:
  sta (LYNX_CLEAR_PTR),y
  iny
  bne ClearFramebufferPageLoop
  inc.b LYNX_CLEAR_PTR+1
  dex
  bne ClearFramebufferPage
  ldy #$00
ClearFramebufferTailLoop:
  sta (LYNX_CLEAR_PTR),y
  iny
  cpy #<LYNX_FRAMEBUFFER_SIZE
  bne ClearFramebufferTailLoop
  rts


DrawHelloWorld:
  lda #<LYNX_TEXT_DEST
  sta.b LYNX_DEST_PTR
  lda #>LYNX_TEXT_DEST
  sta.b LYNX_DEST_PTR+1
  ldx #$00
DrawHelloWorldLoop:
  lda.w HelloWorldText,x
  asl a
  asl a
  asl a
  clc
  adc #<FontData
  sta.b LYNX_GLYPH_PTR
  lda #>FontData
  adc #0
  sta.b LYNX_GLYPH_PTR+1
  phx
  jsr DrawGlyph
  plx
  clc
  lda.b LYNX_DEST_PTR
  adc #6
  sta.b LYNX_DEST_PTR
  bcc DrawHelloWorldNoCarry
  inc.b LYNX_DEST_PTR+1
DrawHelloWorldNoCarry:
  inx
  cpx #HelloWorldTextEnd-HelloWorldText
  bne DrawHelloWorldLoop
  rts


DrawGlyph:
  lda.b LYNX_DEST_PTR
  sta.b LYNX_ROW_PTR
  lda.b LYNX_DEST_PTR+1
  sta.b LYNX_ROW_PTR+1
  stz.b LYNX_GLYPH_ROW
DrawGlyphRowLoop:
  ldy.b LYNX_GLYPH_ROW
  lda (LYNX_GLYPH_PTR),y
  sta.b LYNX_GLYPH_BITS
  ldy #$00

  lda.b LYNX_GLYPH_BITS
  and #%00010000
  beq DrawGlyphColumn1
  lda #LYNX_TEXT_COLOR_BYTE
  sta (LYNX_ROW_PTR),y
DrawGlyphColumn1:
  iny
  lda.b LYNX_GLYPH_BITS
  and #%00001000
  beq DrawGlyphColumn2
  lda #LYNX_TEXT_COLOR_BYTE
  sta (LYNX_ROW_PTR),y
DrawGlyphColumn2:
  iny
  lda.b LYNX_GLYPH_BITS
  and #%00000100
  beq DrawGlyphColumn3
  lda #LYNX_TEXT_COLOR_BYTE
  sta (LYNX_ROW_PTR),y
DrawGlyphColumn3:
  iny
  lda.b LYNX_GLYPH_BITS
  and #%00000010
  beq DrawGlyphColumn4
  lda #LYNX_TEXT_COLOR_BYTE
  sta (LYNX_ROW_PTR),y
DrawGlyphColumn4:
  iny
  lda.b LYNX_GLYPH_BITS
  and #%00000001
  beq DrawGlyphNextRow
  lda #LYNX_TEXT_COLOR_BYTE
  sta (LYNX_ROW_PTR),y
DrawGlyphNextRow:
  clc
  lda.b LYNX_ROW_PTR
  adc #LYNX_SCREEN_STRIDE
  sta.b LYNX_ROW_PTR
  bcc DrawGlyphNoCarry
  inc.b LYNX_ROW_PTR+1
DrawGlyphNoCarry:
  inc.b LYNX_GLYPH_ROW
  lda.b LYNX_GLYPH_ROW
  cmp #7
  bne DrawGlyphRowLoop
  rts


InitAudio:
  stz.w LYNX_AUD0VOL
  stz.w LYNX_AUD1VOL
  stz.w LYNX_AUD2VOL
  stz.w LYNX_AUD3VOL
  stz.w LYNX_AUD0CTLA
  stz.w LYNX_AUD1CTLA
  stz.w LYNX_AUD2CTLA
  stz.w LYNX_AUD3CTLA
  stz.w LYNX_MPAN
  stz.w LYNX_MSTEREO
  rts


PlayBeeps:
  lda #$10
  jsr StartTone
  ldx #$04
  jsr Delay
  jsr StopTone
  ldx #$02
  jsr Delay

  lda #$08
  jsr StartTone
  ldx #$04
  jsr Delay
  jmp StopTone


StartTone:
  tax
  stz.w LYNX_AUD0CTLA
  lda #$01
  sta.w LYNX_AUD0SHIFT
  stz.w LYNX_AUD0CTLB
  lda #LYNX_AUD_FEED_TAP_0
  sta.w LYNX_AUD0FEED
  txa
  sta.w LYNX_AUD0BKUP
  lda #$7F
  sta.w LYNX_AUD0VOL
  lda #LYNX_TIM_ENABLE_RELOAD|LYNX_TIM_ENABLE_COUNT|LYNX_TIM_CLOCK_64
  sta.w LYNX_AUD0CTLA
  rts


StopTone:
  stz.w LYNX_AUD0CTLA
  stz.w LYNX_AUD0VOL
  rts


Delay:
  ldy #$FF
DelayMiddle:
  lda #$FF
DelayInner:
  dea
  bne DelayInner
  dey
  bne DelayMiddle
  dex
  bne Delay
  rts


MikeyInitReg:
  .DB $00,$01,$08,$09,$20,$28,$30,$38,$44,$50,$8A,$8B,$8C,$92,$93
MikeyInitRegEnd:

MikeyInitData:
  .DB $9E,$18,$68,$1F,$00,$00,$00,$00,$00,$00,$1A,$1B,$04,LYNX_DISPLAY_CONTROL,$29
MikeyInitDataEnd:

SuzyInitReg:
  .DB $28,$2A,$04,$06,$92,$83,$90
SuzyInitRegEnd:

SuzyInitData:
  .DB $7F,$7F,$00,$00,$24,$F3,$01
SuzyInitDataEnd:

PaletteGreen:
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0F

PaletteRedBlue:
  .DB $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF

HelloWorldText:
  .DB 1,2,3,3,4,0,5,4,6,3,7
HelloWorldTextEnd:

FontData:
  .DB %00000,%00000,%00000,%00000,%00000,%00000,%00000,$00
  .DB %10001,%10001,%10001,%11111,%10001,%10001,%10001,$00
  .DB %11111,%10000,%10000,%11110,%10000,%10000,%11111,$00
  .DB %10000,%10000,%10000,%10000,%10000,%10000,%11111,$00
  .DB %01110,%10001,%10001,%10001,%10001,%10001,%01110,$00
  .DB %10001,%10001,%10001,%10101,%10101,%11011,%10001,$00
  .DB %11110,%10001,%10001,%11110,%10100,%10010,%10001,$00
  .DB %11110,%10001,%10001,%10001,%10001,%10001,%11110,$00
FontDataEnd:

MainEnd: