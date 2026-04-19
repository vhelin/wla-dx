.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 256
  BANKSIZE $2000
  BANKS 256
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

; ---------------------------------------------------------------------------
; CBM80 autostart header.
;   $8000-$8001 cold-start -> start
;   $8002-$8003 warm-start -> start
;   $8004-$8008 "CBM80"
; ---------------------------------------------------------------------------
  .dw start
  .dw start
  .db $c3, $c2, $cd, $38, $30

start:
  ; Copy switch stub to RAM at $033C because we can't bank-switch while
  ; executing from the cart ROM.
  ldy #$00
  lda #<switch_stub
  sta $fb                 ; $FB/$FC zero-page pointer
  lda #>switch_stub
  sta $fc
copy_loop:
  lda ($fb),y
  sta $033c,y
  iny
  cpy #switch_stub_end - switch_stub
  bne copy_loop
  lda #$01                ; select bank 1
  jmp $033c

switch_stub:
  ; GMod3 bank register lives at $DE00 (I/O1).  Low 8 bits of A select one
  ; of up to 256 ROML banks; bit 7 of $DE00 also controls cart-off here on
  ; real hardware but VICE is fine with just the bank index.
  sta $de00
  jmp $8000
switch_stub_end:

; ===========================================================================
; Bank 1 - prints "HELLO WORLD" then switches to bank 2.
; ===========================================================================
.BANK 1 SLOT 0
.ORG 0

run_from_bank1:
  lda #$1b                ; $D011 ctrl1: screen on, text, 25 rows
  sta $d011
  lda #$08                ; $D016 ctrl2: 40-col, no multicolour
  sta $d016
  lda #$14                ; $D018 mem ptrs: screen=$0400, chargen=$1000
  sta $d018

  lda #$00                ; black
  sta $d020               ; border
  sta $d021               ; background 0

  ldx #$00
clear_loop:
  lda #$20                ; ' '
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

  ldx #$00
print_loop:
  lda message.w,x
  beq switch_to_bank2
  sta $0400,x             ; screen code
  lda #$0e                ; light blue fg
  sta $d800,x
  inx
  bne print_loop

switch_to_bank2:
  lda #$02                ; select bank 2
  jmp $033c

; Screen codes: H E L L O ' ' W O R L D
message:
  .db $08, $05, $0c, $0c, $0f, $20, $17, $0f, $12, $0c, $04, 0

; ===========================================================================
; Bank 2 - appends '!', border red, halts.
; ===========================================================================
.BANK 2 SLOT 0
.ORG 0

run_from_bank2:
  lda #$21                ; '!'
  sta $040b               ; row 0 col 11
  lda #$0e                ; light blue
  sta $d80b               ; colour RAM
  lda #$02                ; red
  sta $d020               ; border -> red = done
done:
  jmp done
