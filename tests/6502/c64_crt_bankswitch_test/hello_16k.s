.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
  SLOT 1 START $A000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 2
  BANKSIZE $2000
  BANKS 2
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

; ---------------------------------------------------------------------------
; CBM80 autostart header (first 9 bytes at $8000).
;   $8000-$8001 : cold-start vector  -> start
;   $8002-$8003 : NMI / warm-start   -> start
;   $8004-$8008 : PETSCII "CBM80"
; ---------------------------------------------------------------------------
  .dw start
  .dw start
  .db $c3, $c2, $cd, $38, $30

start:
  ; --- VIC-II screen setup ---
  lda #$1b                ; $D011 = control reg 1: screen on, text, 25 rows.
  sta $d011
  lda #$08                ; $D016 = control reg 2: 40-col, no multicolour.
  sta $d016

  ; --- Detect Ultimax vs plain 16K cart at runtime ---
  ; In 16K Ultimax ROMH is visible at $E000-$FFFF and the linker has placed
  ; a marker byte $A5 at the very start of that ROMH bank.  In normal 16K
  ; mode $E000 shows KERNAL ROM and $E000 != $A5.
  lda $e000
  cmp #$a5
  bne not_ultimax
  lda #$1c                ; Ultimax: screen=$0400, char base=$3000
  jmp set_d018            ;          (chargen read from ROMH).
not_ultimax:
  lda #$14                ; Normal:  screen=$0400, char base=$1000.
set_d018:
  sta $d018               ; $D018 = VIC-II memory pointers.

  ; --- Colours ---
  lda #$00                ; black
  sta $d020               ; border
  sta $d021               ; background 0

  ; --- Clear screen ($0400-$07E7) + colour RAM ($D800-$DBE7) ---
  ldx #$00
clear_loop:
  lda #$20                ; screen code for ' '
  sta $0400,x
  sta $0500,x
  sta $0600,x
  sta $06e8,x
  lda #$0e                ; light blue
  sta $d800,x
  sta $d900,x
  sta $da00,x
  sta $dae8,x
  inx
  bne clear_loop

  ; --- Print 'HELLO WORLD!' to row 0 ---
  ldx #$00
print_loop:
  lda message.w,x
  beq done
  sta $0400,x             ; screen code
  lda #$0e                ; light blue foreground
  sta $d800,x
  inx
  bne print_loop

done:
  lda #$02                ; red
  sta $d020               ; flash border = "done"
done_loop:
  jmp done_loop

; Screen codes: H E L L O ' ' W O R L D !
message:
  .db $08, $05, $0c, $0c, $0f, $20, $17, $0f, $12, $0c, $04, $21, 0

; ---------------------------------------------------------------------------
; Bank 1 lives at $A000-$BFFF on a normal 16K cart and at $E000-$FFFF in
; Ultimax mode (the ROMH chip).  We put:
;   - A marker byte $A5 at offset 0 (used by the Ultimax detection above).
;   - 8x8 chargen glyphs at offsets $1020-$110F so the VIC-II can fetch
;     them from ROMH when char base = $3000 in Ultimax mode.
;   - Hardware reset/NMI/IRQ vectors at $1FFA-$1FFF pointing to 'start'
;     ($8000), so on a real C64 in Ultimax mode the CPU jumps to our code
;     at power-on.
; ---------------------------------------------------------------------------
.BANK 1 SLOT 1
.ORG 0
  .db $a5
.ORG $1020
  .db $7c, $66, $66, $66, $66, $66, $7c, $00   ; 'D' (4)
.ORG $1028
  .db $7e, $60, $60, $78, $60, $60, $7e, $00   ; 'E' (5)
.ORG $1040
  .db $66, $66, $66, $7e, $66, $66, $66, $00   ; 'H' (8)
.ORG $1060
  .db $60, $60, $60, $60, $60, $60, $7e, $00   ; 'L' (12)
.ORG $1078
  .db $3c, $66, $66, $66, $66, $66, $3c, $00   ; 'O' (15)
.ORG $1090
  .db $7c, $66, $66, $7c, $78, $6c, $66, $00   ; 'R' (18)
.ORG $10b8
  .db $63, $63, $63, $6b, $7f, $77, $63, $00   ; 'W' (23)
.ORG $1108
  .db $18, $18, $18, $18, $00, $00, $18, $00   ; '!' (33)
.ORG $1100
  .db $00, $00, $00, $00, $00, $00, $00, $00   ; ' ' (32)

.ORG $1FFA
  .dw start                ; NMI vector   ($FFFA)
  .dw start                ; RESET vector ($FFFC)
  .dw start                ; IRQ/BRK vec  ($FFFE)
