; EasyFlash CRT test

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $2000
  SLOT 1 START $A000 SIZE $2000
  SLOT 2 START $E000 SIZE $2000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 128
  BANKSIZE $2000
  BANKS 128
.ENDRO

.EMPTYFILL $ff

; Bank 0 ROML - main program after 16K mode switch
.BANK 0 SLOT 0
.ORG 0

cold_start:
  ; --- VIC-II setup ---
  lda #$1b                ; $D011 ctrl1: screen on, text, 25 rows
  sta $d011
  lda #$08                ; $D016 ctrl2: 40-col, no multicolour
  sta $d016
  lda #$14                ; $D018 mem ptrs: screen=$0400, chargen=$1000
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

  ; --- Print "HELLO WORLD" at row 0 ---
  ldx #$00
print_loop:
  lda.w message,x
  beq switch_to_bank1
  sta $0400,x             ; screen code
  lda #$0e                ; light blue fg
  sta $d800,x
  inx
  bne print_loop

switch_to_bank1:
  ; Copy bank-switch stub to RAM at $033C (cassette buffer).  EasyFlash's
  ; $DE00 register selects a 64-bank index for both ROML ($8000) and ROMH
  ; ($A000), so the ROM under our feet will change when we write it - the
  ; stub must therefore run from RAM.
  ldy #$00
  lda #<ef_switch_stub
  sta $fb                 ; $FB/$FC zero-page pointer to stub
  lda #>ef_switch_stub
  sta $fc
copy_loop:
  lda ($fb),y
  sta $033c,y
  iny
  cpy #ef_switch_stub_end - ef_switch_stub
  bne copy_loop
  lda #$01                ; argument: target bank 1
  jmp $033c

ef_switch_stub:
  sta $de00               ; $DE00 = EasyFlash bank register (0..63)
  jmp $8000               ; re-enter cart, new bank visible at $8000
ef_switch_stub_end:

; Screen codes: H E L L O ' ' W O R L D
message:
  .db $08, $05, $0c, $0c, $0f, $20, $17, $0f, $12, $0c, $04, 0


; ===========================================================================
; Bank 0 ROMH ($E000-$FFFF): Ultimax-mode boot code that arms EasyFlash for
; 16K mode and then jumps to cold_start at $8000.
; ===========================================================================
.BANK 1 SLOT 2
.ORG 0

  .DSB $1F00, $ff

startup:
  sei                     ; disable IRQs during boot
  ldx #$ff
  txs                     ; reset stack pointer ($01FF)
  cld                     ; clear decimal flag (standard 6502 boot ritual)

  ; Copy the small boot stub to $0100 (bottom of the stack page is fine
  ; here because we just reset SP and aren't pushing).
  ldx #(boot_stub_end - boot_stub - 1)
boot_copy:
  lda.w boot_stub,x
  sta $0100,x
  dex
  bpl boot_copy
  jmp $0100               ; execute stub from RAM

boot_stub:
  ; $DE02 = EasyFlash control register: bit 7=LED, bit 2=MEMCTL mode,
  ; bits 1..0 = GAME/EXROM. $87 = %1000 0111 -> LED on, 16K game mode
  ; (ROML+ROMH visible at $8000 and $A000, no Ultimax).
  lda #$87
  sta $de02
  jmp $8000               ; jump into bank 0's cold_start
boot_stub_end:

  .DSB $1FFA - (boot_stub_end - ($E000)), $ff

  ; CPU reset / NMI / IRQ vectors at the very top of ROMH.
  .dw startup             ; NMI    ($FFFA)
  .dw startup             ; RESET  ($FFFC)
  .dw startup             ; IRQ/BRK($FFFE)

; ===========================================================================
; Bank 1 ROML - appends '!', border red, halts.
; ===========================================================================
.BANK 2 SLOT 0
.ORG 0

run_from_bank1:
  lda #$21                ; '!'
  sta $040b               ; row 0 col 11
  lda #$0e                ; light blue
  sta $d80b               ; colour RAM
  lda #$02                ; red
  sta $d020               ; border -> red = done
done:
  jmp done

; Bank 1 ROMH
.BANK 3 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 4 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 5 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 6 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 7 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 8 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 9 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 10 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 11 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 12 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 13 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 14 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 15 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 16 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 17 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 18 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 19 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 20 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 21 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 22 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 23 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 24 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 25 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 26 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 27 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 28 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 29 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 30 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 31 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 32 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 33 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 34 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 35 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 36 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 37 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 38 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 39 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 40 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 41 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 42 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 43 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 44 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 45 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 46 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 47 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 48 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 49 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 50 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 51 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 52 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 53 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 54 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 55 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 56 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 57 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 58 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 59 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 60 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 61 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 62 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 63 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 64 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 65 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 66 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 67 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 68 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 69 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 70 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 71 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 72 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 73 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 74 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 75 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 76 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 77 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 78 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 79 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 80 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 81 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 82 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 83 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 84 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 85 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 86 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 87 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 88 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 89 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 90 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 91 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 92 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 93 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 94 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 95 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 96 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 97 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 98 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 99 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 100 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 101 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 102 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 103 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 104 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 105 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 106 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 107 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 108 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 109 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 110 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 111 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 112 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 113 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 114 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 115 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 116 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 117 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 118 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 119 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 120 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 121 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 122 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 123 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 124 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 125 SLOT 1
.ORG 0
.DSB $2000, $FF

.BANK 126 SLOT 0
.ORG 0
.DSB $2000, $FF

.BANK 127 SLOT 1
.ORG 0
.DSB $2000, $FF
