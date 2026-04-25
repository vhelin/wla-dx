; ============================================================================
; tests/cx4/hg51b_instructions_test/main.s
;
; HG51B instruction exerciser.  The Cx4 microcode runs ~14 tests and stores
; each result into a distinct GPR.  The CPU renders a 16-row table showing
; the expected byte-0 value next to the actual 3-byte GPR contents, plus a
; mismatch marker.
;
; Screen layout (row, col are 0-based; col = word column in 32-wide BG1):
;   row 2..17 : one row per GPR (R0..R15)
;       col 2  : GPR index tile (0..F)
;       col 4-5: expected byte0 (from ExpectedB0 table, rendered once)
;       col 8-9: actual byte0 (from MMIO readback)
;       col 11-12: actual byte1
;       col 14-15: actual byte2
;       col 17 : "!" if byte0 mismatch, blank otherwise
;   row 19 : cache.pc + cache.page + status (3 hex pairs, cols 2-3 / 5-6 / 8-9)
;   row 20 : R15 byte0 (should be AB)       at cols 2-3
;
; Font tiles:  0=space, 8='!', 9..24 = '0'..'F' (and double as letters A-F).
; ============================================================================

.LOROM

.SNESHEADER
    ID      "WLAC"
    NAME    "CX4 INSTRUCTIONS    "
    LOROM
    SLOWROM
    CARTRIDGETYPE $F3
    ROMSIZE       $08
    SRAMSIZE      $00
    COUNTRY       $01
    LICENSEECODE  $00
    VERSION       $00
.ENDSNES

.MEMORYMAP
    SLOTSIZE    $8000
    DEFAULTSLOT 0
    SLOT 0      $8000
.ENDME

.ROMBANKSIZE $8000
.ROMBANKS    2

.SNESNATIVEVECTOR
    COP    EmptyHandler
    BRK    EmptyHandler
    ABORT  EmptyHandler
    NMI    EmptyHandler
    IRQ    EmptyHandler
.ENDNATIVEVECTOR

.SNESEMUVECTOR
    COP    EmptyHandler
    ABORT  EmptyHandler
    NMI    EmptyHandler
    RESET  Reset
    IRQBRK EmptyHandler
.ENDEMUVECTOR


; ---------------------------------------------------------------------------
.DEFINE INIDISP   $2100
.DEFINE BGMODE    $2105
.DEFINE BG1SC     $2107
.DEFINE BG12NBA   $210B
.DEFINE VMAIN     $2115
.DEFINE VMADDL    $2116
.DEFINE VMDATAL   $2118
.DEFINE CGADD     $2121
.DEFINE CGDATA    $2122
.DEFINE TM        $212C

.DEFINE CX4_CACHE_PAGE    $7F48     ; write 0 then touch pc/base to load
.DEFINE CX4_CACHE_BASE_L  $7F49     ; 24-bit SNES-bus addr of microcode
.DEFINE CX4_CACHE_BASE_M  $7F4A
.DEFINE CX4_CACHE_BASE_H  $7F4B
.DEFINE CX4_CACHE_PB_L    $7F4D     ; program-bank register
.DEFINE CX4_CACHE_PB_H    $7F4E
.DEFINE CX4_CACHE_PC      $7F4F     ; writing here clears halt and starts exec
.DEFINE CX4_SOFT_RESET    $7F53     ; STZ to rearm the core
.DEFINE CX4_STATUS        $7F5F
.DEFINE CX4_GPR_BASE      $7F80     ; R0 byte0; GPR N byte K = $7F80 + N*3 + K
.DEFINE CX4_R1_B0         $7F83     ; R1 byte0 = $7F80 + 1*3
.DEFINE CX4_R15_B0        $7FAD     ; R15 byte0 = $7F80 + 15*3

; Scratch vars in low RAM (bank 0 DP $00xx).
;   _tmp     : caller scratch in DumpLoop (X*3). LDY _tmp is a 16-bit load
;              (X/Y=16), so $01 MUST stay zero.
;   _tmp_hi  : always $00.  Reserved so LDY _tmp reads a clean 16-bit value.
;   _actual  : last GPR byte0 read, for mismatch compare
;   _vscr    : SetVma's private scratch (byte, but reserve 2)
.DEFINE _tmp     $00
.DEFINE _tmp_hi  $01
.DEFINE _actual  $02
.DEFINE _vscr    $03
; $04 is _vscr+1 (used by SetVma).
.DEFINE _actual1 $05             ; actual byte1, used by the byte1 mismatch
.DEFINE _actual2 $06             ; actual byte2, used by the byte2 mismatch
                                  ; compare on the right side of the table.
.DEFINE _diag    $07             ; debug scratch (survives SetVma)


.BANK 0 SLOT 0
.SECTION "Boot" SEMIFREE

Reset:
    SEI
    CLC
    XCE
    REP     #$30
    LDX     #$1FFF
    TXS
    PHK
    PLB
    SEP     #$20

    LDA     #$8F
    STA     INIDISP

    ; Ensure DP scratch $01 (_tmp_hi) is zero so LDY _tmp gives a clean 16-bit.
    STZ     _tmp_hi

    ; ---- copy RamKickBlob to $7E:0200 ------------------------------------
    ; (NOT $7E:0000 -- DP stores would clobber it via DP=$0000 aliasing.)
    LDX     #0
-   LDA.L   RamKickBlob,X
    STA.L   $7E0200,X
    INX
    CPX     #(RamKickBlobEnd - RamKickBlob)
    BNE     -

    ; ---- palette: 0=black, 1=light blue (normal), 2=unused --------------
    STZ     CGADD
    STZ     CGDATA
    STZ     CGDATA
    LDA     #<LightBlue
    STA     CGDATA
    LDA     #>LightBlue
    STA     CGDATA

    LDA     #$80
    STA     VMAIN

    ; ---- upload font -----------------------------------------------------
    REP     #$20
    LDA     #$0000
    STA     VMADDL
    LDX     #0
-   LDA.L   Font,X
    STA     VMDATAL
    INX
    INX
    CPX     #(FontEnd - Font)
    BNE     -

    ; ---- clear tilemap $2000 (1024 words) to '-' (tile 1) ----------------
    LDA     #$2000
    STA     VMADDL
    LDA     #$0001
    LDX     #0
-   STA     VMDATAL
    INX
    CPX     #$0400
    BNE     -
    SEP     #$20

    ; ---- BG setup --------------------------------------------------------
    STZ     BGMODE
    LDA     #$20
    STA     BG1SC
    STZ     BG12NBA
    LDA     #$01
    STA     TM

    ; ---- render static template: index tile + expected byte0 per row -----
    LDX     #0
TemplateLoop:
    ; col 2: GPR index tile (9..24 = '0'..'F')
    TXA
    CLC
    ADC     #2                  ; row = 2 + X
    LDY     #2
    JSR     SetVma
    TXA
    CLC
    ADC     #9
    STA     VMDATAL
    STZ     VMDATAL+1

    ; col 4: expected byte0
    TXA
    CLC
    ADC     #2
    LDY     #4
    JSR     SetVma
    LDA.W   ExpectedB0,X
    JSR     HexByte

    ; col 7: expected byte1  (right-side table of packed-test expectations)
    TXA
    CLC
    ADC     #2
    LDY     #7
    JSR     SetVma
    LDA.W   ExpectedB1,X
    JSR     HexByte

    ; col 24: expected byte2 (test C results live in the upper byte)
    TXA
    CLC
    ADC     #2
    LDY     #24
    JSR     SetVma
    LDA.W   ExpectedB2,X
    JSR     HexByte

    INX
    CPX     #16
    BNE     TemplateLoop

    ; ---- seed Cx4 inputs: R0=0, R1=$11 -----------------------------------
    ; The microcode uses r1 as a known test value.  We zero R0 for
    ; symmetry and then fill R2..R15 with $00 so that any GPR the
    ; microcode DOESN'T write shows up as $00 (instantly visible as a
    ; mismatch against ExpectedB0 for that row).
    STZ     CX4_SOFT_RESET

    STZ     CX4_GPR_BASE+0
    STZ     CX4_GPR_BASE+1
    STZ     CX4_GPR_BASE+2
    LDA     #$11
    STA     CX4_R1_B0
    STZ     CX4_R1_B0+1
    STZ     CX4_R1_B0+2

    ; Clear R2..R15 (14 GPRs * 3 bytes = 42) so non-executed tests stay $00.
    LDX     #0
    LDA     #$00
-   STA     CX4_GPR_BASE+6,X    ; R2 byte0 base = $7F86
    INX
    CPX     #42
    BNE     -

    ; Set the Cx4 cache.base to point at our Cx4Program in ROM.  The
    ; Cx4 pulls microcode bytes from this SNES-bus address when we
    ; write cache.page later.  `:Cx4Program` yields the ROM bank byte.
    LDA     #<Cx4Program
    STA     CX4_CACHE_BASE_L
    LDA     #>Cx4Program
    STA     CX4_CACHE_BASE_M
    LDA     #:Cx4Program
    STA     CX4_CACHE_BASE_H
    STZ     CX4_CACHE_PB_L
    STZ     CX4_CACHE_PB_H

    ; Kick + poll from WRAM.  This JSL runs the RamKickBlob we copied
    ; to $7E:0200: it zeros cache.page and cache.pc (triggering the
    ; Cx4 to load microcode and begin executing) and then spins on
    ; R15 byte-0 until it reads $AB (the "microcode finished"
    ; sentinel).  Running from WRAM is mandatory: the HG51B's busy
    ; state returns IO garbage for bank-00 ROM fetches in ares, so a
    ; ROM-resident polling loop would read garbage opcodes.
    JSL     $7E0200

    ; ---- kick block 2 ----------------------------------------------------
    ; Page-1 microcode lives at Cx4Program2.  GPRs persist from block 1.
    ; Clear R15 byte0 first so the poll waits for block 2's fresh $AB
    ; write instead of returning immediately on block 1's stale sentinel.
    STZ     CX4_R15_B0
    LDA     #<Cx4Program2
    STA     CX4_CACHE_BASE_L
    LDA     #>Cx4Program2
    STA     CX4_CACHE_BASE_M
    LDA     #:Cx4Program2
    STA     CX4_CACHE_BASE_H
    JSL     $7E0200

    ; ---- dump R0..R15 readback -------------------------------------------
    LDX     #0
DumpLoop:
    ; Row (2+X).  Compute GPR N's byte-0 MMIO offset = X*3 and stash in _tmp.
    TXA
    STA     _tmp
    ASL
    CLC
    ADC     _tmp
    STA     _tmp                ; _tmp = X*3

    ; actual byte0 at col 11
    TXA
    CLC
    ADC     #2
    LDY     #11
    JSR     SetVma
    LDY     _tmp
    LDA     CX4_GPR_BASE,Y
    STA     _actual
    JSR     HexByte

    ; actual byte1 at col 14
    TXA
    CLC
    ADC     #2
    LDY     #14
    JSR     SetVma
    LDY     _tmp
    LDA     CX4_GPR_BASE+1,Y
    STA     _actual1
    JSR     HexByte

    ; actual byte2 at col 17
    TXA
    CLC
    ADC     #2
    LDY     #17
    JSR     SetVma
    LDY     _tmp
    LDA     CX4_GPR_BASE+2,Y
    STA     _actual2
    JSR     HexByte

    ; byte0 mismatch marker at col 20.  EOR path + diag render.
    ; DEBUG: snapshot both operands BEFORE the EOR for diag render.
    LDA     _actual
    STA     $08                 ; probe: actual at compare time
    LDA.W   ExpectedB0,X
    STA     $09                 ; probe: expected at compare time
    EOR     _actual
    STA     _diag
    TXA
    CLC
    ADC     #2
    LDY     #20
    JSR     SetVma
    LDA     _diag
    BEQ     _b0ok
    LDA     #8                  ; '!'
    STA     VMDATAL
    STZ     VMDATAL+1
    BRA     _b0done
_b0ok:
    LDA     #1                  ; '-'
    STA     VMDATAL
    STZ     VMDATAL+1
_b0done:
    ; DEBUG: render XOR diff as hex at col 22-23.
    TXA
    CLC
    ADC     #2
    LDY     #22
    JSR     SetVma
    LDA     _diag
    JSR     HexByte
    ; DEBUG: render _actual snapshot at col 28-29.
    TXA
    CLC
    ADC     #2
    LDY     #28
    JSR     SetVma
    LDA     $08
    JSR     HexByte
    ; DEBUG: render expected snapshot at col 30-31.
    TXA
    CLC
    ADC     #2
    LDY     #30
    JSR     SetVma
    LDA     $09
    JSR     HexByte

    ; byte1 mismatch marker at col 21.
    LDA.W   ExpectedB1,X
    EOR     _actual1
    STA     _diag
    TXA
    CLC
    ADC     #2
    LDY     #21
    JSR     SetVma
    LDA     _diag
    BEQ     _b1ok
    LDA     #8                  ; '!'
    STA     VMDATAL
    STZ     VMDATAL+1
    BRA     _b1done
_b1ok:
    LDA     #1                  ; '-'
    STA     VMDATAL
    STZ     VMDATAL+1
_b1done:

    ; byte2 mismatch marker at col 27 (right of the expected-byte2 cell).
    TXA
    CLC
    ADC     #2
    LDY     #27
    JSR     SetVma
    LDA     _actual2
    CMP.W   ExpectedB2,X
    BEQ     +
    LDA     #8                  ; '!'
    STA     VMDATAL
    STZ     VMDATAL+1
    BRA     ++
+   LDA     #1                  ; '-'
    STA     VMDATAL
    STZ     VMDATAL+1
++:

    INX
    CPX     #16
    BEQ     DumpLoopDone
    JMP     DumpLoop
DumpLoopDone:

    ; ---- row 19 : cache.pc / cache.page / status -------------------------
    LDA     #19
    LDY     #2
    JSR     SetVma
    LDA     CX4_CACHE_PC
    JSR     HexByte

    LDA     #19
    LDY     #5
    JSR     SetVma
    LDA     CX4_CACHE_PAGE
    JSR     HexByte

    LDA     #19
    LDY     #8
    JSR     SetVma
    LDA     CX4_STATUS
    JSR     HexByte

    ; ---- row 20 : R15 byte0 (should be AB) -------------------------------
    LDA     #20
    LDY     #2
    JSR     SetVma
    LDA     CX4_R15_B0
    JSR     HexByte

    LDA     #$0F
    STA     INIDISP

Forever:
    WAI
    BRA     Forever


; ---------------------------------------------------------------------------
; Helpers.  Entry state: SEP #$20, REP #$10, DB=0.
; ---------------------------------------------------------------------------

; SetVma : A = row (8-bit), Y = col (16-bit, really 0..31).
; VMADDR := $2000 + row*32 + col.  All math done in 8-bit M mode.
SetVma:
    STA     _vscr+1             ; stash row
    TYA                         ; Y is 16-bit; transfers low byte to A in M=8
    STA     _vscr               ; stash col lo
    LDA     _vscr+1
    LSR
    LSR
    LSR
    CLC
    ADC     #$20
    STA     VMADDL+1            ; $2117 high byte
    LDA     _vscr+1
    AND     #$07
    ASL
    ASL
    ASL
    ASL
    ASL                         ; (row & 7) << 5
    CLC
    ADC     _vscr               ; + col
    STA     VMADDL              ; $2116 low byte
    RTS

; HexByte : write byte in A as 2 hex tiles at current VMADDR (auto-inc).
HexByte:
    PHA
    LSR
    LSR
    LSR
    LSR
    CLC
    ADC     #9
    STA     VMDATAL
    STZ     VMDATAL+1
    PLA
    AND     #$0F
    CLC
    ADC     #9
    STA     VMDATAL
    STZ     VMDATAL+1
    RTS

EmptyHandler:
    RTI

; ---------------------------------------------------------------------------
; RamKickBlob : copied to $7E:0200 (NOT $7E:0000 - see cx4_notes.md #1).
; JSL-able (RTL-terminated).  Responsibilities:
;   1. Zero cache.page and cache.pc, which causes the Cx4 to (re)load
;      cache from [cache.base] and begin executing at pc=0.
;   2. Spin-poll R15 byte-0 until it becomes $AB, the sentinel our
;      microcode writes as its very last act.  A nested X/Y counter
;      gives a bounded timeout (~several-second wall clock) so a
;      broken microcode doesn't hang the console.
; MUST run from WRAM: while the HG51B is busy loading/running, ares
; returns IO-window garbage for bank-00 ROM fetches, so a ROM-resident
; polling loop would execute wrong opcodes.
; ---------------------------------------------------------------------------
RamKickBlob:
    STZ     CX4_CACHE_PAGE      ; arm cache (causes fetch from cache.base)
    STZ     CX4_CACHE_PC        ; pc = 0, clears halt -> microcode runs
    LDX     #$0000
    LDY     #$0040              ; timeout: ~4M polls
_rkpoll:
    LDA     CX4_R15_B0          ; read R15 byte0
    CMP     #$AB                ; == microcode's "done" sentinel?
    BEQ     _rkdone
    DEX
    BNE     _rkpoll
    DEY
    BNE     _rkpoll
_rkdone:
    RTL
RamKickBlobEnd:

.ENDS


; ---------------------------------------------------------------------------
; Data
; ---------------------------------------------------------------------------
.SECTION "Data" SEMIFREE

.DEFINE LightBlue $7F90

; Expected byte0 and byte1 per GPR.  Each R2..R14 packs two independent
; HG51B tests: byte0 = test A result, byte1 = test B result.  byte2 is
; usually $00; R6 is the exception (its SXB test intentionally leaves
; $FF in byte2 to visibly prove the full 24-bit A path).
;   See cx4_prog.s header for per-slot rationale.
ExpectedB0:
    .DB $00,$11,$16,$3F,$F0,$FC,$55,$00
    .DB $33,$20,$55,$A0,$10,$55,$2A,$AB
ExpectedB1:
    .DB $BA,$EE,$2A,$3F,$08,$01,$80,$80
    .DB $AA,$40,$AA,$B0,$20,$32,$55,$00
ExpectedB2:
    .DB $B5,$6E,$22,$EE,$22,$11,$FF,$FF
    .DB $44,$20,$77,$C0,$30,$00,$77,$00

Cx4Program:
    .INCBIN "cx4_prog.bin"
Cx4ProgramEnd:
; Page-1 microcode starts at ROM offset $400 inside cx4_prog.bin.
.DEFINE Cx4Program2 Cx4Program+$400

; Monochrome 2bpp font.  Tile 0 = space, 8 = '!', 9..24 = '0'..'F'
; (the hex tiles double as letter tiles for A-F).
Font:
    ; 0 : space
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    ; 1 : '-' (horizontal dash, used as pass-marker)
    .DB $00,$00, $00,$00, $00,$00, $78,$00, $00,$00, $00,$00, $00,$00, $00,$00
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    ; 8 : '!'
    .DB $10,$00, $10,$00, $10,$00, $10,$00, $10,$00, $00,$00, $10,$00, $00,$00
    ; 9 : 0
    .DB $70,$00, $88,$00, $98,$00, $A8,$00, $C8,$00, $88,$00, $70,$00, $00,$00
    ; 10 : 1
    .DB $20,$00, $60,$00, $20,$00, $20,$00, $20,$00, $20,$00, $70,$00, $00,$00
    ; 11 : 2
    .DB $70,$00, $88,$00, $08,$00, $10,$00, $20,$00, $40,$00, $F8,$00, $00,$00
    ; 12 : 3
    .DB $70,$00, $88,$00, $08,$00, $30,$00, $08,$00, $88,$00, $70,$00, $00,$00
    ; 13 : 4
    .DB $10,$00, $30,$00, $50,$00, $90,$00, $F8,$00, $10,$00, $10,$00, $00,$00
    ; 14 : 5
    .DB $F8,$00, $80,$00, $F0,$00, $08,$00, $08,$00, $88,$00, $70,$00, $00,$00
    ; 15 : 6
    .DB $70,$00, $80,$00, $F0,$00, $88,$00, $88,$00, $88,$00, $70,$00, $00,$00
    ; 16 : 7
    .DB $F8,$00, $08,$00, $10,$00, $20,$00, $20,$00, $20,$00, $20,$00, $00,$00
    ; 17 : 8
    .DB $70,$00, $88,$00, $88,$00, $70,$00, $88,$00, $88,$00, $70,$00, $00,$00
    ; 18 : 9
    .DB $70,$00, $88,$00, $88,$00, $78,$00, $08,$00, $88,$00, $70,$00, $00,$00
    ; 19 : A
    .DB $70,$00, $88,$00, $88,$00, $F8,$00, $88,$00, $88,$00, $88,$00, $00,$00
    ; 20 : B
    .DB $F0,$00, $88,$00, $88,$00, $F0,$00, $88,$00, $88,$00, $F0,$00, $00,$00
    ; 21 : C
    .DB $70,$00, $88,$00, $80,$00, $80,$00, $80,$00, $88,$00, $70,$00, $00,$00
    ; 22 : D
    .DB $F0,$00, $88,$00, $88,$00, $88,$00, $88,$00, $88,$00, $F0,$00, $00,$00
    ; 23 : E
    .DB $F8,$00, $80,$00, $80,$00, $F0,$00, $80,$00, $80,$00, $F8,$00, $00,$00
    ; 24 : F
    .DB $F8,$00, $80,$00, $80,$00, $F0,$00, $80,$00, $80,$00, $80,$00, $00,$00
FontEnd:

.ENDS
