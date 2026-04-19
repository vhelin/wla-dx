; 4KB Ultimax cartridge: "HELLO WORLD!" rendered via chargen embedded in ROMH.
;
; Two ROM banks:
;   bank 0 -> ROML, visible at $8000-$8FFF (contains CBM80 header + code)
;   bank 1 -> ROMH, visible at $F000-$FFFF in Ultimax mode
;
; In Ultimax mode the VIC-II fetches character bitmaps from ROMH. A 4K ROMH
; chip at $F000 appears in VICE's romh_banks[] at offsets $3000..$3FFF, which
; means VIC char base $3000 ($D018 = $1C) maps 1:1 to this bank's offsets.

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 START $8000 SIZE $1000
  SLOT 1 START $F000 SIZE $1000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 2
  BANKSIZE $1000
  BANKS 2
.ENDRO

.EMPTYFILL $00

; ===========================================================================
; Bank 0 - ROML at $8000
; ===========================================================================
.BANK 0 SLOT 0
.ORG 0

; CBM80 autostart header (not used by Ultimax boot, but harmless).
  .dw start
  .dw start
  .db $c3, $c2, $cd, $38, $30

start:
  lda #$1b                ; $D011 ctrl1: screen on, text, 25 rows
  sta $d011
  lda #$08                ; $D016 ctrl2: 40-col, no multicolour
  sta $d016
  lda #$1c                ; $D018 mem ptrs: screen=$0400, char base=$3000
  sta $d018               ;   (Ultimax: chargen fetched from ROMH)

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

; ===========================================================================
; Bank 1 - ROMH at $F000-$FFFF
; VIC-II in Ultimax mode reads chargen from ROMH; char base $3000 lands at
; this bank's offsets $0000+. Each 8-byte entry is a glyph at
; (screen_code * 8). Hardware reset/NMI/IRQ vectors at $0FFA/$FFC/$FFE.
; ===========================================================================
.BANK 1 SLOT 1
.ORG $0020
  .db $7c, $66, $66, $66, $66, $66, $7c, $00   ; 'D' (4)
.ORG $0028
  .db $7e, $60, $60, $78, $60, $60, $7e, $00   ; 'E' (5)
.ORG $0040
  .db $66, $66, $66, $7e, $66, $66, $66, $00   ; 'H' (8)
.ORG $0060
  .db $60, $60, $60, $60, $60, $60, $7e, $00   ; 'L' (12)
.ORG $0078
  .db $3c, $66, $66, $66, $66, $66, $3c, $00   ; 'O' (15)
.ORG $0090
  .db $7c, $66, $66, $7c, $78, $6c, $66, $00   ; 'R' (18)
.ORG $00b8
  .db $63, $63, $63, $6b, $7f, $77, $63, $00   ; 'W' (23)
.ORG $0100
  .db $00, $00, $00, $00, $00, $00, $00, $00   ; ' ' (32)
.ORG $0108
  .db $18, $18, $18, $18, $00, $00, $18, $00   ; '!' (33)

.ORG $0FFA
  .dw start                ; NMI   ($FFFA)
  .dw start                ; RESET ($FFFC) - boots into Ultimax on C64 reset
  .dw start                ; IRQ   ($FFFE)
