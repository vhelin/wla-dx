.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 4
  BANKSIZE $2000
  BANKS 4
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

; ---------------------------------------------------------------------------
; CBM80 autostart header.
;   $8000-$8001 cold-start vector -> start
;   $8002-$8003 warm-start vector -> start
;   $8004-$8008 magic "CBM80"     = $C3 $C2 $CD $38 $30
; ---------------------------------------------------------------------------
  .dw start
  .dw start
  .db $c3, $c2, $cd, $38, $30

start:
  ; Copy the bank-switch stub to RAM at $033C (cassette buffer).
  ; We can't bankswitch *while* executing from the cart ROM at $8000 because
  ; the ground under our feet would vanish, so the switch must happen from
  ; RAM.  The stub writes to $DE00 (I/O1: the cart's bank latch) and then
  ; jumps back to $8000 where the new bank is now visible.
  ldy #$00
  lda #<switch_stub       ; low byte of switch_stub
  sta $fb                 ; $FB/$FC is a free zero-page pointer pair
  lda #>switch_stub       ; high byte
  sta $fc
copy_loop:
  lda ($fb),y             ; read stub byte from ROM
  sta $033c,y             ; write to cassette buffer (RAM)
  iny
  cpy #switch_stub_end - switch_stub
  bne copy_loop

  lda #$01                ; argument for the stub: select bank 1
  jmp $033c               ; execute stub from RAM -> runs bank 1 from $8000

switch_stub:
  sta $de00               ; $DE00 = cart bank-select register (I/O1).
  jmp $8000               ; re-enter the cart; bank 1 now visible at $8000.
switch_stub_end:

; ===========================================================================
; Bank 1 - prints "HELLO WORLD" then switches to bank 2 (for the '!').
; ===========================================================================
.BANK 1 SLOT 0
.ORG 0

run_from_bank1:
  ; --- VIC-II setup ---
  lda #$1b                ; $D011 = control reg 1: screen on, text, 25 rows
  sta $d011
  lda #$08                ; $D016 = control reg 2: 40-col, no multicolour
  sta $d016
  lda #$14                ; $D018 = mem ptrs: screen $0400, char base $1000
  sta $d018

  lda #$00                ; black
  sta $d020               ; $D020 = border
  sta $d021               ; $D021 = background 0

  ; --- Clear screen ($0400..$07E7) + colour RAM ($D800..$DBE7) ---
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

  ; --- Print "HELLO WORLD" to row 0 ---
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
  lda #$02                ; argument for stub: select bank 2
  jmp $033c               ; run stub in RAM -> jumps back to $8000 in bank 2

; Screen codes: H E L L O ' ' W O R L D
message:
  .db $08, $05, $0c, $0c, $0f, $20, $17, $0f, $12, $0c, $04, 0

; ===========================================================================
; Bank 2 - appends '!', turns border red and halts.
; ===========================================================================
.BANK 2 SLOT 0
.ORG 0

run_from_bank2:
  lda #$21                ; '!' screen code
  sta $040b               ; column 11 of row 0 (right after "HELLO WORLD")
  lda #$0e                ; light blue
  sta $d80b               ; colour RAM for that cell
  lda #$02                ; red
  sta $d020               ; border -> red signals "done"
done:
  jmp done

.BANK 3 SLOT 0
.ORG 0

  .db 0