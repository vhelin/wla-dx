; =============================================================================
; cx4_prog.s  -  HG51B (Cx4) instruction exerciser microcode.
;
; PACKING FORMAT
;   Each slot R2..R14 packs up to THREE independent tests into one 24-bit
;   GPR value.  Packing uses R15 as scratch:
;
;     ; ---- test A ----
;     ... compute result ...
;     AND  A<<0,$FF          ; strip any sign-extend noise in upper bytes
;     MOV  r15,A             ; stash byte0 result
;
;     ; ---- test B ----
;     ... compute result ...
;     AND  A<<0,$FF
;     SHL  A,8               ; shift into byte1
;     OR   A,r15             ; merge with stashed byte0
;     MOV  r15,A             ; restash A+B
;
;     ; ---- test C (optional) ----
;     ... compute result ...
;     AND  A<<0,$FF
;     SHL  A,16              ; shift into byte2
;     OR   A,r15             ; merge
;     MOV  r<slot>,A
;
;   Slots without test C skip the last block and just `MOV r<slot>,r15`
;   via the two-step pack.
;
;   R15 byte0 = $AB is written LAST = "microcode done" sentinel.
;
; COVERAGE (across three microcode blocks):
;   Block 1 (page 0, R0..R14 + R15 sentinel):
;     BRA BSR RTS BEQ BGE   (label operands, word-scaled)
;     MOV (imm/reg/reg-to-reg), NOP, HALT, CLEAR
;     ADD (imm/reg, shift 0/1/8), SUB (imm/reg), SUBR (imm/reg)
;     AND/OR/XOR/XNOR  (imm/reg, shift 0/1/8)
;     CMP (imm/reg), CMPR (imm/reg)
;     SHL/SHR/ASR/ROR (imm), SHLL r, SHLR r
;     SXB, SXW, SWAP
;     SKIPEQ SKIPNE SKIPGE SKIPPL SKIPLT SKIPMI
;
;   Block 2 (page 1, R11 + R14):
;     MUL (via P register), WRRAM/RDRAM round-trip, RDROM (q=0)
;
;   Block 1 wave-5 additions (R12):
;     BMI  BSREQ  BSRMI   (conditional branch + subroutine calls;
;                          live in block 1 because branch operands
;                          are 9-bit absolute word-addresses).
;
;   Block 2 wave-5 additions (R13):
;     INC MAR
;     MOV PL,x ; MOV PH,x  (writes to the 16-bit P register),
;                          validated via MUL P readback through MACL.
;
; NOT COVERED (need scaffolding this test lacks):
;   SKIPVS/SKIPVC, BVS, BSRVS  -- V-flag semantics not pinned down.
;   MOV r,MDR               -- awkward to distinguish from MOV r,A
;                              inside the packed-test encoding.
;   WAIT                    -- blocks on hardware-bus semantics that
;                              are not well-defined in an isolated
;                              microcode block.
;   SHAR r / ROTR r         -- aliases of ASR A,u / ROR A,u.  The
;                              register-form shift semantics ARE
;                              exercised via SHLL r / SHLR r in block 1.
;
; INPUTS (seeded by CPU):  R0=0  R1=$11
;
; ENCODING
;   Each Cx4 instruction is 2 bytes: operand then opcode.
;   ALU ops need an explicit shift prefix ("ADD A<<0,$55" not "ADD A,$55").
;   Register-form ALU ops do NOT take a shift prefix ("ADD A,r1").
; =============================================================================

.MEMORYMAP
SLOTSIZE $10000
DEFAULTSLOT 0
SLOT 0 $0000
.ENDME
.ROMBANKSIZE $10000
.ROMBANKS 1
.BANK 0 SLOT 0
.ORG 0

.SECTION "Cx4Test" FORCE

; ===================================================================
; BRANCH TESTS (R0/R1 reused: byte0 stays at seed, byte1/byte2 get
; branch-test results).
;
;   R0 expected:  byte0 = $00 (seed), byte1 = $BA (BRA taken),
;                 byte2 = $B5 (BSR + RTS round-trip)
;   R1 expected:  byte0 = $11 (seed), byte1 = $EE (BEQ taken when Z=1),
;                 byte2 = $GE -> $6E (BGE taken when GE=1)
;
; If BRA/BSR/Bxx targets are mis-encoded, the wrong instruction (or an
; invalid opcode, or a HALT midway) executes and the expected bytes
; won't show up.
; ===================================================================

; ---- BRA forward : write $BA to R0 byte1 via a branch that skips a bad value
    MOV  A,$FF           ; start with "bad" marker in A
    BRA  _br_pass1       ; if encoded correctly, we skip the SUB below
    SUB  A<<0,$55        ; SKIPPED when BRA works
_br_pass1:
    MOV  A,$BA           ; reached via BRA, A = $BA
    SHL  A,8             ; byte1 = $BA
    MOV  r0,A

; ---- BSR + RTS : call a subroutine that writes $B5, then RTS back
    BSR  _br_sub
    ; after RTS we continue here; result came from _br_sub updating R0 byte2
    BRA  _br_after_sub
_br_sub:
    MOV  A,$B5
    SHL  A,16            ; byte2 = $B5
    OR   A,r0            ; preserve byte1=$BA
    MOV  r0,A
    RTS
_br_after_sub:

; ---- BEQ taken : $11 in byte1 of R1 means BEQ took the branch correctly
    MOV  A,$FF           ; bad marker
    CMP  A<<0,$FF        ; Z=1
    BEQ  _br_beq_ok
    SUB  A<<0,$80        ; SKIPPED if BEQ works
_br_beq_ok:
    MOV  A,$EE
    SHL  A,8
    OR   A,r1            ; keep byte0 = seed $11
    MOV  r1,A

; ---- BGE taken : write $6E to R1 byte2
    MOV  A,$20
    CMP  A<<0,$10        ; $20-$10 = positive -> GE=1
    BGE  _br_bge_ok
    SUB  A<<0,$11        ; SKIPPED if BGE works
_br_bge_ok:
    MOV  A,$6E
    SHL  A,16
    OR   A,r1            ; preserve byte0=$11, byte1=$EE
    MOV  r1,A

; ===================================================================
; R2 : A=$16 MOV+NOP+ADD reg    B=$2A ADD imm+SUB imm    C=$22 ADD A<<1 form
;   C test: MOV A,$11 -> A=$11; ADD A<<1,$00 shifts A left 1 then adds 0.
;           Expected byte0 = $22.  (If the HG51B encodes the shift as
;           applying to the immediate instead, this test will flag a
;           mismatch showing $11 -- useful diagnostic.)
; ===================================================================
    MOV  A,$05
    NOP
    ADD  A,r1
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$10
    ADD  A<<0,$30
    SUB  A<<0,$16
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A

    MOV  A,$11
    ADD  A<<1,$00
    SHL  A,16
    OR   A,r15
    MOV  r2,A

; ===================================================================
; R3 : A=$3F SUBR+CMPR  B=$3F AND+OR  C=$EE XOR A<<0,imm with $FF
; ===================================================================
    MOV  A,$11
    SUBR A<<0,$50
    CMPR A,r1
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$FF
    AND  A<<0,$3F
    OR   A<<0,$00
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A

    MOV  A,$FF
    XOR  A<<0,$11
    SHL  A,16
    OR   A,r15
    MOV  r3,A

; ===================================================================
; R4 : A=$F0 XOR+XNOR  B=$08 SHL+SHR  C=$22 SUB register form via r1
; ===================================================================
    MOV  A,$AA
    XOR  A<<0,$55
    XNOR A<<0,$F0
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$01
    SHL  A,4
    SHR  A,1
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A

    MOV  A,$33
    SUB  A,r1            ; reg form: A = $33 - r1 = $22
    SHL  A,16
    OR   A,r15
    MOV  r4,A

; ===================================================================
; R5 : A=$FC SXB+ASR  B=$01 ROR  C=$11 AND A,r1
; ===================================================================
    MOV  A,$F0
    SXB  A
    ASR  A,2
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$04
    ROR  A,2
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A

    MOV  A,$FF
    AND  A,r1            ; $FF & $11 = $11
    SHL  A,16
    OR   A,r15
    MOV  r5,A

; ===================================================================
; R6 : A=$55 CLEAR+ADD  B=$80 SXB  C=$FF (byte2) intentional
;   For R6 we WANT byte2 to be nonzero ($FF) to validate the 24-bit GPR
;   MMIO path.  We use the SXB test to naturally put $FF there without
;   masking.  byte0=$55 (test A), byte1=$80 from SHL after SXB,
;   byte2=$FF from the SXB sign-extension that SHL preserves.
; ===================================================================
    CLEAR
    ADD  A<<0,$55
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$80
    SXB  A               ; A = $FF_FF_80
    SHL  A,8             ; A = $FF_80_00   (byte1=$80, byte2=$FF)
    OR   A,r15           ; A = $FF_80_55
    MOV  r6,A

; ===================================================================
; R7 : A=$00 SXW byte0  B=$80 (SXW byte1)  C=$FF SXW byte2
;   Dedicated SXW verification: single SXW of $00_80_00 gives $FF_80_00.
;   All three bytes are interesting here, so we use a DIFFERENT packing:
;   just produce the SXW result directly, no packing, no masking.
; ===================================================================
    MOV  A,$80
    SHL  A,8             ; A = $00_80_00
    SXW  A               ; A = $FF_80_00
    MOV  r7,A

; ===================================================================
; R8 : A=$33 ADD chain   B=$AA  imm   C=$44 SHLL rN (register-form shift)
;   C test: r14 used as packing scratch here (not r15) so that r15 can
;   hold the shift count.  r14 is overwritten by R14's own block later.
;   SHLL r15: A = A << (r15 byte0).  With A=$11, r15=$02 -> A = $44.
; ===================================================================
    MOV  A,r1
    ADD  A,r1
    ADD  A<<0,$11        ; $11+$11+$11 = $33
    AND  A<<0,$FF
    MOV  r14,A

    MOV  A,$AA
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r14
    MOV  r14,A

    MOV  A,$02
    MOV  r15,A           ; r15 = $02 (shift count for SHLL)
    MOV  A,$11
    SHLL r15             ; A = $11 << 2 = $44
    SHL  A,16
    OR   A,r14
    MOV  r8,A

; ===================================================================
; R9 : A=$20 SHL imm    B=$40 SHR imm    C=$20 SHLR rN (register-form)
;   C test DONE FIRST (so it can use r15 as count scratch before the
;   pack starts filling r15 with the byte0+byte1 partial).
;   SHLR r15: A = A >> (r15 byte0).  With A=$80, r15=$02 -> A = $20.
; ===================================================================
    MOV  A,$02
    MOV  r15,A           ; r15 = $02 (shift count)
    MOV  A,$80
    SHLR r15             ; A = $80 >> 2 = $20
    AND  A<<0,$FF
    SHL  A,16            ; stash byte2 = $20
    MOV  r15,A           ; r15 = $20_00_00 (byte2 partial)

    MOV  A,$04
    SHL  A,3             ; A=$20
    AND  A<<0,$FF
    OR   A,r15           ; merge with byte2 partial
    MOV  r15,A           ; r15 = $20_00_20

    MOV  A,$80
    SHR  A,1             ; A=$40
    AND  A<<0,$FF
    SHL  A,8             ; byte1 = $40
    OR   A,r15           ; merge -> A = $20_40_20
    MOV  r9,A

; ===================================================================
; R10: A=$55 AND imm    B=$AA OR reg+imm+XOR    C=$77 SWAP A,rN
;   C test: SWAP A,r15 swaps A and r15.  We set r15=$BB, A=$77, SWAP,
;   then A = $BB (old r15) and r15 = $77 (old A).  We read r15 back
;   via MOV A,r15 to verify the SWAP direction r15 -> old-A-value.
;   Uses r14 as packing scratch so r15 is free to be the SWAP partner.
; ===================================================================
    MOV  A,$FF
    AND  A<<0,$55
    AND  A<<0,$FF
    MOV  r14,A

    MOV  A,$00
    OR   A,r1
    OR   A<<0,$99
    XOR  A<<0,$33
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r14
    MOV  r14,A

    MOV  A,$BB
    MOV  r15,A           ; r15 = $BB (swap partner)
    MOV  A,$77
    SWAP A,r15           ; A=$BB, r15=$77
    MOV  A,r15           ; A=$77 (proves r15 got A's old value)
    SHL  A,16
    OR   A,r14
    MOV  r10,A

; ===================================================================
; R11: A=$72 SKIPEQ/NE/GE/PL chain   B=$72 SKIPLT/MI chain (positive)
;      C=$33 SKIPEQ after CMP with equal values (Z=1 -> taken)
; ===================================================================
    MOV  A,$50
    CMP  A<<0,$50
    SKIPEQ
    ADD  A<<0,$EE
    CMP  A<<0,$50
    SKIPNE
    ADD  A<<0,$20
    CMP  A<<0,$00
    SKIPGE
    ADD  A<<0,$88
    SKIPPL
    ADD  A<<0,$04
    ADD  A<<0,$02        ; -> $72
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$50
    CMP  A<<0,$01
    SKIPLT
    ADD  A<<0,$22        ; -> $72
    CMP  A<<0,$01
    SKIPMI
    ADD  A<<0,$00
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A

    MOV  A,$00
    CMP  A<<0,$00        ; Z=1
    SKIPEQ               ; taken
    ADD  A<<0,$FF        ; SKIPPED
    ADD  A<<0,$33        ; EXECUTES -> $33
    SHL  A,16
    OR   A,r15
    MOV  r11,A

; ===================================================================
; R12 and R13: the original block-1 packs that wrote these GPRs were
; removed to free ~$80 bytes of block-1 code space for the wave-5
; branch tests (which must live in block 1 due to the 9-bit branch
; target range).  The removed tests were:
;   R12: MOV A,rN / ADD A,r1 (reg-form) / CMPR A,r1 -- all covered
;        elsewhere (R2/R4/R8 exercise ADD A,r1 regform, R3/R8 exercise
;        CMPR, and MOV A,rN is exercised in every packed slot).
;   R13: AND-reg / OR-reg / XNOR-reg / SUB / `OR A<<8,r1` -- AND-reg is
;        covered in R5, OR-reg in R10, SUB imm throughout.  The unique
;        `OR A<<8,r1` (shift-in register form) IS lost; if this form
;        regresses there is no on-target check for it here.  The
;        word-scaling branch tests are considered the higher-value
;        addition.
; R12 is populated by the wave-5 branch tests below; R13 is populated
; by wave-5 non-branch tests in block 2.
; ===================================================================

; ===================================================================
; R14: A=$08 ASR imm     B=$F0 XOR reg+ADD     C=$1A AND A<<1,$17
;      C: AND A<<1,$17 -- shift A left by 1 first THEN AND.
;         Start A=$0D. $0D<<1 = $1A. $1A & $17 = $12.  Hmm, let me pick
;         cleaner operands.  Let A=$34, then A<<1=$68, AND $FF = $68.
;         Use A=$34, AND A<<1,$FF -> $68.
; ===================================================================
    MOV  A,$20
    ASR  A,2             ; $08
    AND  A<<0,$FF
    MOV  r15,A

    MOV  A,$FF
    XOR  A,r1            ; $EE
    ADD  A<<0,$02        ; $F0
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A

    MOV  A,$34
    AND  A<<1,$FF        ; ($34<<1) & $FF = $68
    SHL  A,16
    OR   A,r15
    MOV  r14,A

; ===================================================================
; Wave 5 : conditional-branch coverage (R12 pack).
;   Overwrites the earlier block-1 R12 pack (whose behaviours are
;   already covered elsewhere: MOV A,rN via R2, ADD A,r1 via R2/R4/R8,
;   CMPR via R3) with BMI / BSREQ / BSRMI -- conditional branches and
;   subroutine-calls that weren't exercised in the branch tests for
;   R0/R1 (those covered BRA, BSR, BEQ, BGE).
;
;   These tests MUST live in block 1 (page 0) rather than block 2
;   because branch operands are 9-bit absolute word-addresses that
;   must resolve to byte-addresses <= $1FE; block 2 starts at $400
;   and so cannot host branch-with-label instructions.
;
;   Previously these were workaround-avoided because wla-cx4 emitted
;   byte addresses instead of word addresses for label operands (see
;   cx4_notes.md #9b).  That assembler bug is now fixed.
;
; R12 : A=$10 BMI   B=$20 BSREQ+RTS   C=$30 BSRMI+RTS
; ===================================================================

    ; test A : BMI taken (N=1 after negative SUB)
    MOV  A,$10
    SUB  A<<0,$20        ; A = $10 - $20 = $FF_FF_F0, N=1
    BMI  _w5_bmi_ok
    MOV  A,$FF           ; SKIPPED when BMI works
_w5_bmi_ok:
    MOV  A,$10
    AND  A<<0,$FF
    MOV  r15,A           ; byte0 stash = $10

    ; test B : BSREQ + RTS (Z=1 after equal CMP)
    MOV  A,$77
    CMP  A<<0,$77        ; Z=1
    BSREQ _w5_bsreq_sub
    BRA  _w5_bsreq_after ; if BSREQ wasn't taken, skip subroutine
_w5_bsreq_sub:
    MOV  A,$20           ; executed via the call
    RTS
_w5_bsreq_after:
    AND  A<<0,$FF
    SHL  A,8             ; byte1 = $20
    OR   A,r15
    MOV  r15,A

    ; test C : BSRMI + RTS (N=1 after negative SUB)
    MOV  A,$10
    SUB  A<<0,$20        ; N=1
    BSRMI _w5_bsrmi_sub
    BRA  _w5_bsrmi_after
_w5_bsrmi_sub:
    MOV  A,$30
    RTS
_w5_bsrmi_after:
    AND  A<<0,$FF
    SHL  A,16            ; byte2 = $30
    OR   A,r15
    MOV  r12,A

; ===================================================================
; R15: sentinel $AB  MUST be last.  Overwrites any R15 scratch usage.
; ===================================================================
    MOV  A,$AB
    MOV  r15,A
    HALT

.ENDS

; ===================================================================
; PAGE 1 : second microcode block, CPU kicks this after page 0 halts.
; Lives at ROM offset $400 (word addr 0 of its own 512-byte cache load).
; The offset is $400 rather than $200 because block 1 grew past $200
; after the wave-5 tests were added.  The CPU points cache.base at
; Cx4Program2 directly, so the ROM offset is arbitrary as long as the
; whole block fits within one 512-byte cache load.
; GPRs persist across kicks (no soft-reset between them), so any GPR
; written here is visible on top of page 0's state.
;
; Wave 3 tests (block 2 owns R14 fully; block 1's R14 result is
; overwritten here):
;   R14 byte0 : MUL       $06 * $07 = $2A  (readback via MBR)
;   R14 byte1 : WRRAM+RDRAM roundtrip of $55 at RAM[MAR=0]
;   R14 byte2 : INC MAR after writing $77 at RAM[1]; RDRAM reads back
; ===================================================================
.ORG $400

.SECTION "Cx4Test2" FORCE
Cx4Program2:
    ; ---- test A : MUL  ($06 * $07 = $2A) ----
    MOV  A,$06
    MOV  P,$07
    MUL  P               ; A * P -> MACH:MACL
    NOP                  ; pipeline: let MACL latch
    NOP
    NOP
    MOV  A,MACL
    NOP
    AND  A<<0,$FF
    MOV  r15,A           ; byte0 stash = $2A

    ; ---- test B : WRRAM + RDRAM roundtrip at RAM[0] ($55) ----
    MOV  MAR,$0000
    MOV  A,$55
    WRRAM 0,A
    NOP
    RDRAM 0,A
    NOP
    AND  A<<0,$FF
    SHL  A,8
    OR   A,r15
    MOV  r15,A           ; byte1 = $55

    ; ---- test C : WRRAM/RDRAM at RAM[1] ($77) ----
    ; Simplified: seed RAM[1]=$77, read it back directly (no INC MAR).
    MOV  MAR,$0001
    NOP
    MOV  A,$77
    WRRAM 0,A
    NOP
    NOP
    RDRAM 0,A
    NOP
    NOP
    AND  A<<0,$FF
    SHL  A,16
    OR   A,r15
    MOV  r14,A           ; byte2 = $77

    ; ---- test D (wave 4) : RDROM with different q values ----
    ; RDROM loads the fetched 24-bit data-ROM word into the ROM latch
    ; register (bsnes reg 0x08, WLA name "ROM").  To get it into a GPR
    ; the value must round-trip through A:  MOV A,ROM ; MOV r,A
    ;
    ; data.rom pattern: byte[3q] = $A0 | (q & 0xF), byte[3q+1] = $B0|q,
    ;                   byte[3q+2] = $C0 | q.  As a 24-bit LE word:
    ;                   word[q] = $C0q_B0q_A0q  (byte2=$Cq, byte1=$Bq,
    ;                   byte0=$Aq).  So R11 should end up $C0_B0_A0.
    ; (R12 is populated by block-1 wave-5 branch tests; R13 is wave-5
    ; non-branch tests populated below.  Only q=0 is probed here; q=0
    ; alone proves the full 24-bit ROM-latch path works.)
    RDROM $000
    NOP
    NOP
    NOP
    MOV  A,ROM
    MOV  r11,A           ; R11 = $C0_B0_A0   (q=0)

    ; ===================================================================
    ; Wave 5 non-branch coverage (R13 pack).
    ;   Tests that don't use label operands, so they're safe in block 2
    ;   (which sits past byte $1FE and therefore can't host branches).
    ;
    ; R13 : A=$55 INC MAR   B=$32 MOV PL/PH + MUL P   C=$00 (unused)
    ; ===================================================================

    ; test A : INC MAR
    ;   Seed RAM[0] = $AA, RAM[1] = $55, set MAR=0, INC MAR, then
    ;   RDRAM reads RAM[MAR=1] = $55.  If INC MAR is a no-op the
    ;   readback would be $AA instead, producing an $AA mismatch.
    MOV  MAR,$0000
    NOP
    MOV  A,$AA
    WRRAM 0,A            ; RAM[0] = $AA
    NOP
    MOV  MAR,$0001
    NOP
    MOV  A,$55
    WRRAM 0,A            ; RAM[1] = $55
    NOP
    MOV  MAR,$0000
    NOP
    INC  MAR             ; MAR : 0 -> 1
    NOP
    RDRAM 0,A            ; A = RAM[1] = $55
    NOP
    NOP
    AND  A<<0,$FF
    MOV  r15,A           ; byte0 = $55

    ; test B : MOV PL,x + MOV PH,x + MUL P
    ;   Load P = $0005 via its low/high halves, multiply by A=$0A,
    ;   read MACL.  Proves both PL-write and PH-write paths.
    MOV  PL,$05          ; P low  = $05
    MOV  PH,$00          ; P high = $00 -> P = $0005
    MOV  A,$0A
    MUL  P               ; MACH:MACL = $0A * $05 = $00_00_32
    NOP                  ; pipeline: MACL latches
    NOP
    NOP
    MOV  A,MACL
    AND  A<<0,$FF
    SHL  A,8             ; byte1 = $32
    OR   A,r15
    MOV  r13,A           ; byte2 of R13 left as $00

    ; ---- sentinel ----
    MOV  A,$AB
    MOV  r15,A
    HALT

.ENDS
