;
; Sega Mega Drive/Genesis generic MC68000 helper macros
;

.MACRO MD_DISABLE_INTERRUPTS
  move.w #$2700, sr
.ENDM

.MACRO MD_ENABLE_INTERRUPTS
  move.w #$2000, sr
.ENDM

.MACRO MD_WAIT_VBLANK
MDWaitVBlank_\@:
  move.w REG_MD_VDP_HV, d0
  andi.w #$0080, d0
  beq MDWaitVBlank_\@
.ENDM

.MACRO MD_PSG_SILENCE
  move.b #$9F, REG_MD_PSG
  move.b #$BF, REG_MD_PSG
  move.b #$DF, REG_MD_PSG
  move.b #$FF, REG_MD_PSG
.ENDM

.MACRO MD_DMA_VRAM_TRANSFER
  ; source, destination, length-in-words
  move.l #\1, d0
  move.w #\2, d1
  move.w #\3, d2
.ENDM
