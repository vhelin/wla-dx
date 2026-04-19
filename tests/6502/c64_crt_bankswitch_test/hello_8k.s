; Minimal 8KB C64 cartridge: prints "HELLO WORLD!" at the top of the screen
; and turns the border red when done. Used for the NORMAL8K and EPYXFASTLOAD
; targets. (Ultimax-8K uses hello_ultimax8k.s, which supplies its own ROMH.)

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $2000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

; ---------------------------------------------------------------------------
; CBM80 autostart header.
;   $8000-$8001 : cold-start vector  -> start
;   $8002-$8003 : NMI / warm-start   -> start
;   $8004-$8008 : PETSCII "CBM80"
; ---------------------------------------------------------------------------
  .dw start
  .dw start
  .db $c3, $c2, $cd, $38, $30

start:
  ; --- VIC-II screen setup ---
  lda #$1b                ; $D011 ctrl1: screen on, text, 25 rows
  sta $d011
  lda #$08                ; $D016 ctrl2: 40-col, no multicolour
  sta $d016
  lda #$14                ; $D018 mem ptrs: screen=$0400, char base=$1000
  sta $d018

  lda #$00                ; black
  sta $d020               ; border
  sta $d021               ; background 0

  ; --- Clear screen + colour RAM ---
  ldx #$00
clear_loop:
  lda #$20                ; ' '
  sta $0400,x
  sta $0500,x
  sta $0600,x
  sta $06e8,x
  lda #$0e                ; light blue
  sta $d800,x             ; colour RAM: low nibble = fg colour
  sta $d900,x
  sta $da00,x
  sta $dae8,x
  inx
  bne clear_loop

  ; --- Print message ---
  ldx #$00
print_loop:
  lda message.w,x
  beq done
  sta $0400,x             ; screen code
  lda #$0e                ; light blue fg
  sta $d800,x
  inx
  bne print_loop

done:
  lda #$02                ; red
  sta $d020               ; border -> red = done
done_loop:
  jmp done_loop

; Screen codes: H E L L O ' ' W O R L D !
message:
  .db $08, $05, $0c, $0c, $0f, $20, $17, $0f, $12, $0c, $04, $21, 0
