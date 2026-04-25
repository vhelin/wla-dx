; ============================================================================
; tests/cx4/hello_world_test/main.s
;
; SNES "HELLO WORLD!" demo that really executes Cx4 (HG51B169) microcode.
;
; Target emulator:  ares (or bsnes with manifest support).
;   This ROM MUST be launched with the sibling manifest.bml file so that
;   ares routes the cart through HitachiDSP / HG51B169 low-level emulation,
;   not the Mega-Man-X2/X3 high-level Cx4 shortcut.  snes9x will show a
;   black screen (its Cx4 HLE does not match our microcode).
;
; At a glance:
;   - Backdrop black, "HELLO WORLD!" in light blue on BG1.
;   - VRAM write address is row*32 + col, computed ON THE Cx4 via a single
;     MUL opcode (see cx4_prog.s).
;   - 65816 drops inputs in Cx4 GPRs R0/R1, arms the Cx4's instruction
;     cache from cart ROM, kicks execution, polls R15 for the done
;     sentinel, reads R2, and uses that as the tilemap VRAM address.
;
; Build pipeline:
;       wla-cx4   -o cx4_prog.o cx4_prog.s
;       wlalink   -b cx4.link   cx4_prog.bin
;       wla-65816 -o main.o     main.s            ; .INCBINs cx4_prog.bin
;       wlalink   linkfile      result.rom
;       [drops manifest.bml + cx4.data.rom next to result.rom]
; ============================================================================

.LOROM

.SNESHEADER
    ID      "WLAC"
    NAME    "CX4 HELLO WORLD     "
    LOROM
    SLOWROM
    CARTRIDGETYPE $F3           ; ROM + Cx4 (matched by manifest.bml)
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
; PPU + Cx4 (HG51B169) register equates
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

; HG51B169 MMIO window (banks $00-$3F / $80-$BF, $7F40-$7FEF).
; Schema from bsnes/ares sfc/coprocessor/hitachidsp/memory.cpp.
.DEFINE CX4_CACHE_BASE_L  $7F49   ; 24-bit SNES addr the cache loads from
.DEFINE CX4_CACHE_BASE_M  $7F4A
.DEFINE CX4_CACHE_BASE_H  $7F4B
.DEFINE CX4_CACHE_PAGE    $7F48   ; arms cache load if DSP is halted
.DEFINE CX4_CACHE_PB_L    $7F4D   ; program bank (15 bit) lo
.DEFINE CX4_CACHE_PB_H    $7F4E   ; program bank hi
.DEFINE CX4_CACHE_PC      $7F4F   ; writing clears halt, starts execution
.DEFINE CX4_SOFT_RESET    $7F53   ; writing clears lock, sets halt
.DEFINE CX4_STATUS        $7F5F   ; bit7=busy, bit6=running, bit1=irq, bit0=suspend
.DEFINE CX4_R0_B0         $7F80   ; GPRs live at $7F80 + N*3 (3 bytes each)
.DEFINE CX4_R1_B0         $7F83
.DEFINE CX4_R2_B0         $7F86
.DEFINE CX4_R15_B0        $7FAD   ; GPRs end at $7FAF; R15 = R0 + 15*3


; ---------------------------------------------------------------------------
; Boot
; ---------------------------------------------------------------------------
.BANK 0 SLOT 0
.SECTION "Boot" SEMIFREE

Reset:
    SEI
    CLC
    XCE                         ; emulation -> native mode
    REP     #$30                ; 16-bit A/X/Y
    LDX     #$1FFF
    TXS
    PHK
    PLB                         ; DB = program bank = 0

    SEP     #$20                ; 8-bit A; X/Y stay 16-bit

    LDA     #$8F
    STA     INIDISP             ; force blank

    ; ---- copy RamKickBlob to WRAM $7E:0000 -------------------------------
    ; The Cx4 kick + poll must run from WRAM because when the HG51B is
    ; busy (cache loading), reads from cart ROM bank 00 return IO garbage
    ; in ares (see HitachiDSP::readROM).  WRAM reads are unaffected.
    LDX     #0
-   LDA.L   RamKickBlob,X
    STA.L   $7E0000,X
    INX
    CPX     #(RamKickBlobEnd - RamKickBlob)
    BNE     -

    ; ---- palette : entry 0 = black backdrop, entry 1 = light blue -------
    ;   Backdrop starts black.  On Cx4 success we flip it dark green and
    ;   render "HELLO WORLD!" at the tilemap offset the Cx4 computed.  On
    ;   Cx4 failure (timeout) we flip it red and render nothing.
    STZ     CGADD
    STZ     CGDATA              ; color 0 lo  (black)
    STZ     CGDATA              ; color 0 hi
    LDA     #<LightBlue
    STA     CGDATA              ; color 1 lo
    LDA     #>LightBlue
    STA     CGDATA              ; color 1 hi

    ; ---- VRAM autoincrement : +1 word after every $2119 write -------------
    LDA     #$80
    STA     VMAIN

    ; ---- upload font to VRAM $0000 (BG1 char base) ------------------------
    REP     #$20                ; 16-bit A for word VRAM writes
    LDA     #$0000
    STA     VMADDL

    LDX     #0
-   LDA.L   Font,X
    STA     VMDATAL             ; writes $2118 and $2119, addr += 1 word
    INX
    INX
    CPX     #(FontEnd - Font)
    BNE     -

    ; ---- clear BG1 tilemap at VRAM word $2000 (1024 entries) --------------
    LDA     #$2000
    STA     VMADDL
    LDA     #$0000
    LDX     #0
-   STA     VMDATAL
    INX
    CPX     #$0400              ; 1024 word entries
    BNE     -

    ; ---- BG setup (screen stays force-blanked until Cx4 result is in) ---
    STZ     BGMODE              ; mode 0 (all BGs 2bpp)
    LDA     #$20
    STA     BG1SC               ; BG1 tilemap @ VRAM word $2000, 32x32
    STZ     BG12NBA             ; BG1 char base @ VRAM word $0000
    LDA     #$01
    STA     TM                  ; enable BG1 on the main screen

    ; ---- NOW exercise the Cx4 ---------------------------------------------
    ;   The HG51B microcode (cx4_prog.s) computes  R2 = R0*32 + R1  which
    ;   is the word offset into the SNES BG1 tilemap at VRAM $2000.  We
    ;   feed it row=14, col=10 and then use R2 as the VMADDL value for
    ;   writing "HELLO WORLD!" -- so the tilemap address on screen is
    ;   literally produced by the Cx4, not the 65816.
    SEP     #$20

    ; Force DSP halted so GPR writes stick and the next cache.page arms.
    STZ     CX4_SOFT_RESET

    ; Inputs for the microcode:
    ;   R0 = row 14, R1 = col 10    (everything else zero).
    LDA     #14
    STA     CX4_R0_B0
    STZ     CX4_R0_B0+1
    STZ     CX4_R0_B0+2
    LDA     #10
    STA     CX4_R1_B0
    STZ     CX4_R1_B0+1
    STZ     CX4_R1_B0+2
    ; Clear the done sentinel R15 and the output R2.
    STZ     CX4_R15_B0
    STZ     CX4_R15_B0+1
    STZ     CX4_R15_B0+2
    STZ     CX4_R2_B0
    STZ     CX4_R2_B0+1
    STZ     CX4_R2_B0+2

    ; cache_base = 24-bit SNES address of Cx4Program in cart ROM.
    LDA     #<Cx4Program
    STA     CX4_CACHE_BASE_L
    LDA     #>Cx4Program
    STA     CX4_CACHE_BASE_M
    LDA     #:Cx4Program
    STA     CX4_CACHE_BASE_H

    ; program bank = 0  ->  cache loads from cache_base + 0.
    STZ     CX4_CACHE_PB_L
    STZ     CX4_CACHE_PB_H

    ; ---- THE GOTCHA ------------------------------------------------------
    ; Writing $7F48 sets io.cache.enable=1 so the HG51B acquires the cart
    ; bus to stream its program cache from ROM.  While the HG51B is busy,
    ; ares's HitachiDSP::readROM returns IO-register bytes instead of the
    ; CPU's actual opcodes (see readROM: "if(active() || !busy())..." --
    ; for CPU reads, active()==false, so busy path runs and the CPU reads
    ; garbage).  The kick + poll MUST therefore run from WRAM, not ROM.
    ;
    ; We copied RamKickBlob to $7E:0000 at boot; just JSL to it.  The stub
    ; does STZ $7F48 / STZ $7F4F / poll R15 for $AB with a short timeout,
    ; then RTLs.
    JSL     $7E0000

    ; After return from WRAM stub, check if microcode hit the sentinel.
    LDA     CX4_R15_B0
    CMP     #$AB
    BNE     Cx4Failed           ; timeout: paint red, no text

Cx4Ran:
    ; Paint backdrop dark green = Cx4 microcode completed.
    STZ     CGADD
    LDA     #<DarkGreen
    STA     CGDATA
    LDA     #>DarkGreen
    STA     CGDATA

    ; Use R2 (16-bit word) as the BG1 tilemap VRAM address, then write
    ; HELLO WORLD!.  The address on screen is literally whatever the Cx4
    ; microcode computed -- if it's wrong, the text is mis-positioned or
    ; invisible.
    REP     #$20
    LDA     CX4_R2_B0           ; low word of R2 = row*32 + col
    CLC
    ADC     #$2000              ; + BG1 tilemap base
    STA     VMADDL
    SEP     #$20
    LDX     #0
-   LDA.L   HelloTiles,X
    STA     VMDATAL
    STZ     VMDATAL+1
    INX
    CPX     #(HelloTilesEnd - HelloTiles)
    BNE     -

    BRA     Cx4Done

Cx4Failed:
    ; Paint backdrop red = Cx4 microcode did NOT complete.
    STZ     CGADD
    LDA     #<Red
    STA     CGDATA
    LDA     #>Red
    STA     CGDATA

Cx4Done:
    ; Screen on.
    LDA     #$0F
    STA     INIDISP

Forever:
    WAI
    BRA     Forever

EmptyHandler:
    RTI

; ---------------------------------------------------------------------------
; RamKickBlob: tiny position-independent routine that is copied verbatim to
; WRAM $7E:0000 at boot and called via JSL $7E0000.  Runs from WRAM so that
; CPU opcode fetches are unaffected by the HG51B acquiring the cart bus
; during cache load.
;
; On entry: SEP #$20, REP #$10, DB=0.
; On exit:  RTL.  R15 byte0 holds $AB iff microcode ran to completion.
; ---------------------------------------------------------------------------
RamKickBlob:
    STZ     CX4_CACHE_PAGE      ; $7F48  arm cache load (HG51B goes busy)
    STZ     CX4_CACHE_PC        ; $7F4F  pc=0, clear halt -> execution
    LDX     #$0000
    LDY     #$0040              ; ~0.5s outer
_rkpoll:
    LDA     CX4_R15_B0          ; $7FAD
    CMP     #$AB
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

; @BT result.rom

.DEFINE LightBlue $7F90         ; BGR555 : R=16, G=28, B=31 -> light blue
.DEFINE DarkGreen $0300         ; BGR555 : R=0,  G=24, B=0  -> dark green (Cx4 OK)
.DEFINE Red       $001F         ; BGR555 : R=31, G=0,  B=0  -> red (Cx4 timeout)

; Cx4 microcode blob, produced by  wlalink -b cx4.link cx4_prog.bin
Cx4Program:
    .INCBIN "cx4_prog.bin"
Cx4ProgramEnd:

; Tile indices for "HELLO WORLD!"
;   H=1 E=2 L=3 L=3 O=4 ' '=0 W=5 O=4 R=6 L=3 D=7 !=8
HelloTiles:
    .DB $01, $02, $03, $03, $04, $00, $05, $04, $06, $03, $07, $08
HelloTilesEnd:

; byte_tester signature : confirms the 12-entry tile index stream is
; present in the linked ROM.
; @BT TEST-HL HL START 01 02 03 03 04 00 05 04 06 03 07 08 END
    .DB "HL>"
    .DB $01, $02, $03, $03, $04, $00, $05, $04, $06, $03, $07, $08
    .DB "<HL"

; Monochrome 2bpp font.  Plane-0 byte then plane-1 byte per row, 8 rows/tile.
; Only plane 0 is populated (plane 1 always $00), yielding colour index 1.
Font:
    ; Tile 0 : space
    .DB $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00, $00,$00
    ; Tile 1 : H
    .DB $44,$00, $44,$00, $44,$00, $7C,$00, $44,$00, $44,$00, $44,$00, $00,$00
    ; Tile 2 : E
    .DB $7C,$00, $40,$00, $40,$00, $78,$00, $40,$00, $40,$00, $7C,$00, $00,$00
    ; Tile 3 : L
    .DB $40,$00, $40,$00, $40,$00, $40,$00, $40,$00, $40,$00, $7C,$00, $00,$00
    ; Tile 4 : O
    .DB $38,$00, $44,$00, $44,$00, $44,$00, $44,$00, $44,$00, $38,$00, $00,$00
    ; Tile 5 : W
    .DB $44,$00, $44,$00, $44,$00, $44,$00, $54,$00, $6C,$00, $44,$00, $00,$00
    ; Tile 6 : R
    .DB $78,$00, $44,$00, $44,$00, $78,$00, $50,$00, $48,$00, $44,$00, $00,$00
    ; Tile 7 : D
    .DB $78,$00, $44,$00, $44,$00, $44,$00, $44,$00, $44,$00, $78,$00, $00,$00
    ; Tile 8 : !
    .DB $10,$00, $10,$00, $10,$00, $10,$00, $10,$00, $00,$00, $10,$00, $00,$00
    ; Tile 9 : 0
    .DB $70,$00, $88,$00, $98,$00, $A8,$00, $C8,$00, $88,$00, $70,$00, $00,$00
    ; Tile 10 : 1
    .DB $20,$00, $60,$00, $20,$00, $20,$00, $20,$00, $20,$00, $70,$00, $00,$00
    ; Tile 11 : 2
    .DB $70,$00, $88,$00, $08,$00, $10,$00, $20,$00, $40,$00, $F8,$00, $00,$00
    ; Tile 12 : 3
    .DB $70,$00, $88,$00, $08,$00, $30,$00, $08,$00, $88,$00, $70,$00, $00,$00
    ; Tile 13 : 4
    .DB $10,$00, $30,$00, $50,$00, $90,$00, $F8,$00, $10,$00, $10,$00, $00,$00
    ; Tile 14 : 5
    .DB $F8,$00, $80,$00, $F0,$00, $08,$00, $08,$00, $88,$00, $70,$00, $00,$00
    ; Tile 15 : 6
    .DB $70,$00, $80,$00, $F0,$00, $88,$00, $88,$00, $88,$00, $70,$00, $00,$00
    ; Tile 16 : 7
    .DB $F8,$00, $08,$00, $10,$00, $20,$00, $20,$00, $20,$00, $20,$00, $00,$00
    ; Tile 17 : 8
    .DB $70,$00, $88,$00, $88,$00, $70,$00, $88,$00, $88,$00, $70,$00, $00,$00
    ; Tile 18 : 9
    .DB $70,$00, $88,$00, $88,$00, $78,$00, $08,$00, $88,$00, $70,$00, $00,$00
    ; Tile 19 : A (hex)
    .DB $70,$00, $88,$00, $88,$00, $F8,$00, $88,$00, $88,$00, $88,$00, $00,$00
    ; Tile 20 : B (hex)
    .DB $F0,$00, $88,$00, $88,$00, $F0,$00, $88,$00, $88,$00, $F0,$00, $00,$00
    ; Tile 21 : C (hex)
    .DB $70,$00, $88,$00, $80,$00, $80,$00, $80,$00, $88,$00, $70,$00, $00,$00
    ; Tile 22 : D (hex)
    .DB $F0,$00, $88,$00, $88,$00, $88,$00, $88,$00, $88,$00, $F0,$00, $00,$00
    ; Tile 23 : E (hex)
    .DB $F8,$00, $80,$00, $80,$00, $F0,$00, $80,$00, $80,$00, $F8,$00, $00,$00
    ; Tile 24 : F (hex)
    .DB $F8,$00, $80,$00, $80,$00, $F0,$00, $80,$00, $80,$00, $80,$00, $00,$00
FontEnd:

.ENDS
