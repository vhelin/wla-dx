.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
  SLOT 1 START $A000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 8
  BANKSIZE $2000
  BANKS 8
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

; ---------------------------------------------------------------------------
; CBM80 autostart header.
;   $8000-$8001 cold-start vector -> start
;   $8002-$8003 warm-start vector -> start
;   $8004-$8008 magic "CBM80"
; ---------------------------------------------------------------------------
  .dw start
  .dw start
  .db $c3, $c2, $cd, $38, $30

start:
  ; Copy the bank-switch stub to RAM at $033C (cassette buffer).
  ; Writing to $DE00 toggles the cart bank; that write must execute from
  ; RAM because the ROM under $8000 disappears mid-instruction.
  ldy #$00
  lda #<switch_stub
  sta $fb                 ; $FB/$FC = zero-page pointer to stub in ROM
  lda #>switch_stub
  sta $fc
copy_loop:
  lda ($fb),y
  sta $033c,y             ; $033C = cassette buffer (safe RAM)
  iny
  cpy #switch_stub_end - switch_stub
  bne copy_loop
  lda #$01                ; argument: select bank 1
  jmp $033c               ; run stub from RAM

switch_stub:
  sta $de00               ; $DE00 = bank-select latch (I/O1)
  jmp $8000               ; re-enter cart with new bank visible
switch_stub_end:

; Bank 1 ROMH ($A000) unused in this cart.
.BANK 1 SLOT 1
.ORG 0

  .db 0

; ===========================================================================
; Bank 2 ROML - prints "HELLO WORLD", then switches to bank 4.
; ===========================================================================
.BANK 2 SLOT 0
.ORG 0

run_from_bank1:
  ; --- VIC-II setup ---
  lda #$1b                ; $D011 ctrl1: screen on, text mode, 25 rows
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
  sta $d800,x
  sta $d900,x
  sta $da00,x
  sta $dae8,x
  inx
  bne clear_loop

  ; --- Print message ---
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
  lda #$02                ; argument: select bank 2
  jmp $033c

; Screen codes: H E L L O ' ' W O R L D
message:
  .db $08, $05, $0c, $0c, $0f, $20, $17, $0f, $12, $0c, $04, 0

; Bank 3 ROMH unused
.BANK 3 SLOT 1
.ORG 0

  .db 0

; ===========================================================================
; Bank 4 ROML - appends '!', border red, halts.
; ===========================================================================
.BANK 4 SLOT 0
.ORG 0

run_from_bank2:
  lda #$21                ; '!' screen code
  sta $040b               ; row 0, column 11
  lda #$0e                ; light blue
  sta $d80b               ; colour RAM for that cell
  lda #$02                ; red
  sta $d020               ; border -> red = done
done:
  jmp done

; Bank 5 ROMH unused
.BANK 5 SLOT 1
.ORG 0

  .db 0