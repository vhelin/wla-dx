.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 64
  BANKSIZE $2000
  BANKS 64
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

; ---------------------------------------------------------------------------
; CBM80 autostart header. Note cold_start and warm_start are separate so a
; warm reset (RUN/STOP+RESTORE) re-enters via warm_start.
;   $8000-$8001 cold-start -> cold_start
;   $8002-$8003 warm-start -> warm_start
;   $8004-$8008 "CBM80"
; ---------------------------------------------------------------------------
  .dw cold_start
  .dw warm_start
  .db $c3, $c2, $cd, $38, $30

cold_start:
  ; Copy the switch stub to $033C (cassette buffer, free RAM). The stub
  ; performs a WRITE to $DE00+X which the C64GS cartridge hardware decodes
  ; as "select bank X"; the value written is ignored. VICE's c64gs device
  ; explicitly does not decode a read at $DE00, so we must use a store.
  ldy #$00
  lda #<switch_stub
  sta $fb                 ; $FB/$FC = zero-page pointer to stub
  lda #>switch_stub
  sta $fc
copy_loop:
  lda ($fb),y
  sta $033c,y
  iny
  cpy #switch_stub_end - switch_stub
  bne copy_loop
  lda #$01                ; argument = target bank number (1)
  jmp $033c

warm_start:
  jmp cold_start          ; warm entry just restarts from scratch

switch_stub:
  ; On entry A = desired bank.
  tax                     ; move bank index into X (addressing uses X)
  sta $de00,x             ; store-anywhere-in-$DE00-range = select bank X
  jmp $8000               ; re-enter cart, new bank visible at $8000
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
  lda #$02                ; target bank 2
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

