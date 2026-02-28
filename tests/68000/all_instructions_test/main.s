
; @BT linked.rom

; NOTE: 68000 opcodes, as WLA DX understands them, can be found in /i68000.c
; Please verify using external documentation that the opcodes in the comments are correct,
; and that all opcodes are covered by the tests below.

; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, one test at a time

; NOTE: When you define a definition or a function the name must be unique across the entire file,
; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T5" and
; all function names in test 05 with "F5_" - in future tests we should follow this naming.

; MC68000 is big-endian. All words/longs stored high-byte first.
; MOVE word layout:
;   [15:12]=size: B=0001, W=0011, L=0010
;   [11:9] =dest EA reg, [8:6]=dest EA mode
;   [5:3]  =src EA mode, [2:0]=src EA reg
;
; All byte values in ;@BT are given as $XX hex.

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 $000000 $100000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.BANK 0 SLOT 0
.ORG 0

; @BT linked.rom

.db "01>" ; @BT TEST-01 01 START

;==========================================================
; TEST 1: MOVE
;==========================================================
TEST_MOVE:
  MOVE.B D0,D1                        ; @BT $12 $00
  MOVE.W D2,D3                        ; @BT $36 $02
  MOVE.L D4,D5                        ; @BT $2A $04
  MOVE.B #$42,D0                      ; @BT $10 $3C $00 $42
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  MOVE.B (A0),D0                      ; @BT $10 $10
  MOVE.W (A1)+,D1                     ; @BT $32 $19
  MOVE.L -(A2),D2                     ; @BT $24 $22
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
  MOVE.L ($1000).W,D4                 ; @BT $28 $38 $10 $00
  MOVE.L ($00010000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
  MOVE.B D0,(A0)                      ; @BT $10 $80
  MOVE.W D1,(A1)+                     ; @BT $32 $C1
  MOVE.L D2,-(A2)                     ; @BT $25 $02
  MOVE.W D3,($0020,A3)                ; @BT $37 $43 $00 $20
  MOVE.L D0,($2000).W                 ; @BT $21 $C0 $20 $00
  MOVE.L D1,($00020000).L             ; @BT $23 $C1 $00 $02 $00 $00

;==========================================================
; TEST 2: MOVEA
;==========================================================
TEST_MOVEA:
; MOVEA.W D0,A0: size=W(11), opmode=001, destAn=000, src=(000,000) = $3040
  MOVEA.W D0,A0                       ; @BT $30 $40
; MOVEA.L D1,A1: size=L(10), opmode=001, destAn=001, src=(000,001) = $2241
  MOVEA.L D1,A1                       ; @BT $22 $41
; MOVEA.W #$1000,A2: destAn=010, src=(111,100) = $347C + $1000
  MOVEA.W #$1000,A2                   ; @BT $34 $7C $10 $00
; MOVEA.L A3,A4: destAn=100, src=(001,011) = $284B
  MOVEA.L A3,A4                       ; @BT $28 $4B

;==========================================================
; TEST 3: MOVEQ
;==========================================================
TEST_MOVEQ:
; 0111 Dn 0 data8
  MOVEQ #0,D0                         ; @BT $70 $00
  MOVEQ #1,D1                         ; @BT $72 $01
  MOVEQ #-1,D2                        ; @BT $74 $FF
  MOVEQ #127,D7                       ; @BT $7E $7F

;==========================================================
; TEST 4: MOVEM
;==========================================================
TEST_MOVEM:
; MOVEM.W D0-D3,(A0): opcode=$4890, mask D0-D3=$000F
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
; MOVEM.L D0/D1/A0/A1,-(A7): opcode=$48E7, rev.mask=$C0C0
  MOVEM.L D0/D1/A0/A1,-(A7)          ; @BT $48 $E7 $C0 $C0
; MOVEM.W (A0),D0-D3: opcode=$4C90, mask=$000F
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
; MOVEM.L (A7)+,D0/D1/A0/A1: opcode=$4CDF, mask=$0303
  MOVEM.L (A7)+,D0/D1/A0/A1          ; @BT $4C $DF $03 $03

;==========================================================
; TEST 5: MOVEP
;==========================================================
TEST_MOVEP:
; MOVEP.W D0,($10,A0): $0188 + $0010
  MOVEP.W D0,($10,A0)                 ; @BT $01 $88 $00 $10
; MOVEP.L D1,($20,A1): $03C9 + $0020
  MOVEP.L D1,($20,A1)                 ; @BT $03 $C9 $00 $20
; MOVEP.W ($10,A0),D0: $0108 + $0010
  MOVEP.W ($10,A0),D0                 ; @BT $01 $08 $00 $10
; MOVEP.L ($20,A1),D1: $0349 + $0020
  MOVEP.L ($20,A1),D1                 ; @BT $03 $49 $00 $20

;==========================================================
; TEST 6: ADD / ADDA / ADDQ / ADDX
;==========================================================
TEST_ADD:
; ADD.B D1,D0: 1101_000_000_000_001 = $D001
  ADD.B D1,D0                         ; @BT $D0 $01
; ADD.W D2,D3: 1101_011_001_000_010 = $D642
  ADD.W D2,D3                         ; @BT $D6 $42
; ADD.L D4,D5: 1101_101_010_000_100 = $DA84
  ADD.L D4,D5                         ; @BT $DA $84
; ADDI.B #$10,D0: $0600 + $0010
  ADD.B #$10,D0                       ; @BT $06 $00 $00 $10
; ADDI.W #$1000,D1: $0641 + $1000
  ADD.W #$1000,D1                     ; @BT $06 $41 $10 $00
; ADDI.L #$10000,D2: $0682 + $00010000
  ADD.L #$10000,D2                    ; @BT $06 $82 $00 $01 $00 $00

TEST_ADDA:
; ADDA.W D0,A0: opmode=011 => 1101_000_011_000_000 = $D0C0
  ADDA.W D0,A0                        ; @BT $D0 $C0
; ADDA.L D1,A1: opmode=111 => 1101_001_111_000_001 = $D3C1
  ADDA.L D1,A1                        ; @BT $D3 $C1
; ADDA.W #$100,A2: 1101_010_011_111_100 = $D4FC + $0100
  ADDA.W #$100,A2                     ; @BT $D4 $FC $01 $00
; ADDA.L #$10000,A0: 1101_000_111_111_100 = $D1FC + $00010000
  ADDA.L #$10000,A0                   ; @BT $D1 $FC $00 $01 $00 $00

TEST_ADDQ:
; ADDQ.B #1,D0: 0101_001_0_00_000_000 = $5200
  ADDQ.B #1,D0                        ; @BT $52 $00
; ADDQ.W #4,D1: 0101_100_0_01_000_001 = $5841
  ADDQ.W #4,D1                        ; @BT $58 $41
; ADDQ.L #8,D7: data=000(=8): 0101_000_0_10_000_111 = $5087
  ADDQ.L #8,D7                        ; @BT $50 $87
; ADDQ.W #2,A0: ea=(001,000): 0101_010_0_01_001_000 = $5448
  ADDQ.W #2,A0                        ; @BT $54 $48

TEST_ADDX:
; ADDX.B D1,D0: 1101_000_1_00_000_001 = $D101
  ADDX.B D1,D0                        ; @BT $D1 $01
; ADDX.W D3,D2: 1101_010_1_01_000_011 = $D543
  ADDX.W D3,D2                        ; @BT $D5 $43
; ADDX.L D5,D4: 1101_100_1_10_000_101 = $D985
  ADDX.L D5,D4                        ; @BT $D9 $85
; ADDX.W -(A1),-(A0): mem mode: 1101_000_1_01_001_001 = $D149
  ADDX.W -(A1),-(A0)                  ; @BT $D1 $49

;==========================================================
; TEST 7: SUB / SUBA / SUBQ / SUBX
;==========================================================
TEST_SUB:
; SUB.B D1,D0: $9001
  SUB.B D1,D0                         ; @BT $90 $01
; SUB.W D2,D3: $9642
  SUB.W D2,D3                         ; @BT $96 $42
; SUB.L D4,D5: $9A84
  SUB.L D4,D5                         ; @BT $9A $84
; SUBI.B #$10,D0: $0400 + $0010
  SUB.B #$10,D0                       ; @BT $04 $00 $00 $10
; SUBI.W #$100,D1: $0441 + $0100
  SUB.W #$100,D1                      ; @BT $04 $41 $01 $00
; SUBI.L #$10000,D2: $0482 + $00010000
  SUB.L #$10000,D2                    ; @BT $04 $82 $00 $01 $00 $00

TEST_SUBA:
; SUBA.W D0,A0: 1001_000_011_000_000 = $90C0
  SUBA.W D0,A0                        ; @BT $90 $C0
; SUBA.L D1,A1: 1001_001_111_000_001 = $93C1
  SUBA.L D1,A1                        ; @BT $93 $C1
; SUBA.W #$100,A0: 1001_000_011_111_100 = $90FC + $0100
  SUBA.W #$100,A0                     ; @BT $90 $FC $01 $00
; SUBA.L #$10000,A1: 1001_001_111_111_100 = $93FC + $00010000
  SUBA.L #$10000,A1                   ; @BT $93 $FC $00 $01 $00 $00

TEST_SUBQ:
; SUBQ.B #1,D0: $5300
  SUBQ.B #1,D0                        ; @BT $53 $00
; SUBQ.W #4,D1: $5941
  SUBQ.W #4,D1                        ; @BT $59 $41
; SUBQ.L #8,D7: $5187
  SUBQ.L #8,D7                        ; @BT $51 $87
; SUBQ.W #2,A0: ea=(001,000): 0101_010_1_01_001_000 = $5548
  SUBQ.W #2,A0                        ; @BT $55 $48

TEST_SUBX:
; SUBX.B D1,D0: 1001_000_1_00_000_001 = $9101
  SUBX.B D1,D0                        ; @BT $91 $01
; SUBX.W -(A1),-(A0): 1001_000_1_01_001_001 = $9149
  SUBX.W -(A1),-(A0)                  ; @BT $91 $49

;==========================================================
; TEST 8: MUL / DIV
;==========================================================
TEST_MULDIV:
; MULS.W D1,D0: 1100_000_111_000_001 = $C1C1
  MULS.W D1,D0                        ; @BT $C1 $C1
; MULU.W D2,D3: 1100_011_011_000_010 = $C6C2
  MULU.W D2,D3                        ; @BT $C6 $C2
; DIVS.W D1,D0: 1000_000_111_000_001 = $81C1
  DIVS.W D1,D0                        ; @BT $81 $C1
; DIVU.W D2,D3: 1000_011_011_000_010 = $86C2
  DIVU.W D2,D3                        ; @BT $86 $C2

;==========================================================
; TEST 9: AND / ANDI
;==========================================================
TEST_AND:
; AND.B D1,D0: 1100_000_000_000_001 = $C001
  AND.B D1,D0                         ; @BT $C0 $01
; AND.W D2,D3: 1100_011_001_000_010 = $C642
  AND.W D2,D3                         ; @BT $C6 $42
; AND.L D4,D5: 1100_101_010_000_100 = $CA84
  AND.L D4,D5                         ; @BT $CA $84
; ANDI.B #$0F,D0: $0200 + $000F
  AND.B #$0F,D0                       ; @BT $02 $00 $00 $0F
; ANDI.W #$FF00,D1: $0241 + $FF00
  AND.W #$FF00,D1                     ; @BT $02 $41 $FF $00
; ANDI.L #$FFFF0000,D2: $0282 + $FFFF0000
  AND.L #$FFFF0000,D2                 ; @BT $02 $82 $FF $FF $00 $00

;==========================================================
; TEST 10: OR / ORI
;==========================================================
TEST_OR:
; OR.B D1,D0: 1000_000_000_000_001 = $8001
  OR.B D1,D0                          ; @BT $80 $01
; OR.W D2,D3: $8642
  OR.W D2,D3                          ; @BT $86 $42
; OR.L D4,D5: $8A84
  OR.L D4,D5                          ; @BT $8A $84
; ORI.B #$0F,D0: $0000 + $000F
  OR.B #$0F,D0                        ; @BT $00 $00 $00 $0F
; ORI.W #$FF00,D1: $0041 + $FF00
  OR.W #$FF00,D1                      ; @BT $00 $41 $FF $00
; ORI.L #$FFFF0000,D2: $0082 + $FFFF0000
  OR.L #$FFFF0000,D2                  ; @BT $00 $82 $FF $FF $00 $00

;==========================================================
; TEST 11: EOR / EORI
;==========================================================
TEST_EOR:
; EOR.B D0,D1: 1011_000_1_00_000_001 = $B101
  EOR.B D0,D1                         ; @BT $B1 $01
; EOR.W D2,D3: $B543
  EOR.W D2,D3                         ; @BT $B5 $43
; EOR.L D4,D5: $B985
  EOR.L D4,D5                         ; @BT $B9 $85
; EORI.B #$AA,D0: $0A00 + $00AA
  EOR.B #$AA,D0                       ; @BT $0A $00 $00 $AA
; EORI.W #$AAAA,D1: $0A41 + $AAAA
  EOR.W #$AAAA,D1                     ; @BT $0A $41 $AA $AA
; EORI.L #$AAAAAAAA,D2: $0A82 + $AAAAAAAA
  EOR.L #$AAAAAAAA,D2                 ; @BT $0A $82 $AA $AA $AA $AA

;==========================================================
; TEST 12: NOT
;==========================================================
TEST_NOT:
; NOT.B D0: 0100_0110_00_000_000 = $4600
  NOT.B D0                            ; @BT $46 $00
; NOT.W D1: $4641
  NOT.W D1                            ; @BT $46 $41
; NOT.L D2: $4682
  NOT.L D2                            ; @BT $46 $82

;==========================================================
; TEST 13: NEG / NEGX
;==========================================================
TEST_NEG:
; NEG.B D0: 0100_0100_00_000_000 = $4400
  NEG.B D0                            ; @BT $44 $00
; NEG.W D1: $4441
  NEG.W D1                            ; @BT $44 $41
; NEG.L D2: $4482
  NEG.L D2                            ; @BT $44 $82

TEST_NEGX:
; NEGX.B D0: $4000
  NEGX.B D0                           ; @BT $40 $00
; NEGX.W D1: $4041
  NEGX.W D1                           ; @BT $40 $41
; NEGX.L D2: $4082
  NEGX.L D2                           ; @BT $40 $82

;==========================================================
; TEST 14: CLR
;==========================================================
TEST_CLR:
; CLR.B D0: 0100_0010_00_000_000 = $4200
  CLR.B D0                            ; @BT $42 $00
; CLR.W D1: $4241
  CLR.W D1                            ; @BT $42 $41
; CLR.L D2: $4282
  CLR.L D2                            ; @BT $42 $82

;==========================================================
; TEST 15: EXT
;==========================================================
TEST_EXT:
; EXT.W D0 (byte->word): 0100_1000_10_000_000 = $4880
  EXT.W D0                            ; @BT $48 $80
; EXT.L D1 (word->long): 0100_1000_11_000_001 = $48C1
  EXT.L D1                            ; @BT $48 $C1

;==========================================================
; TEST 16: SWAP
;==========================================================
TEST_SWAP:
; SWAP D0: 0100_1000_01_000_000 = $4840
  SWAP D0                             ; @BT $48 $40
; SWAP D7: $4847
  SWAP D7                             ; @BT $48 $47

;==========================================================
; TEST 17: SHIFTS AND ROTATES
; Register: 1110 cnt d ss i tt rrr
;   d=direction(0=R,1=L), ss=size(00=B,01=W,10=L)
;   i=1(imm count),0(reg count), tt=00(AS),01(LS),10(ROX),11(RO)
; Memory: 1110 0tt d 11 ea (word only)
;==========================================================
TEST_SHIFTS:
; ASL.B #1,D0: cnt=001,d=1,ss=00,i=1,tt=00 => $E320
  ASL.B #1,D0                         ; @BT $E3 $00
; ASL.W #2,D1: cnt=010,ss=01 => $E561
  ASL.W #2,D1                         ; @BT $E5 $41
; ASL.L #4,D2: cnt=100,ss=10 => $E9A2
  ASL.L #4,D2                         ; @BT $E9 $82
; ASR.B #1,D0: d=0 => $E220
  ASR.B #1,D0                         ; @BT $E2 $00
; ASR.W #2,D1: $E461
  ASR.W #2,D1                         ; @BT $E4 $41
; ASR.L #1,D3: cnt=001,ss=10 => $E2A3
  ASR.L #1,D3                         ; @BT $E2 $83
; LSL.B #1,D0: tt=01 => $E328
  LSL.B #1,D0                         ; @BT $E3 $08
; LSR.B #1,D0: d=0,tt=01 => $E228
  LSR.B #1,D0                         ; @BT $E2 $08
; LSL.W #2,D1: $E569
  LSL.W #2,D1                         ; @BT $E5 $49
; LSR.W #2,D1: $E469
  LSR.W #2,D1                         ; @BT $E4 $49
; ROL.B #1,D0: tt=11 => $E338
  ROL.B #1,D0                         ; @BT $E3 $18
; ROR.B #1,D0: d=0,tt=11 => $E238
  ROR.B #1,D0                         ; @BT $E2 $18
; ROL.W #2,D1: $E579
  ROL.W #2,D1                         ; @BT $E5 $59
; ROR.W #2,D1: $E479
  ROR.W #2,D1                         ; @BT $E4 $59
; ROXL.B #1,D0: tt=10 => $E330
  ROXL.B #1,D0                        ; @BT $E3 $10
; ROXR.B #1,D0: d=0 => $E230
  ROXR.B #1,D0                        ; @BT $E2 $10
; Memory shifts (word): 1110 0tt d 11 ea
; ASL.W (A0): tt=00,d=1,ea=(010,000) => $E1D0
  ASL.W (A0)                          ; @BT $E1 $D0
; ASR.W (A0): d=0 => $E0D0
  ASR.W (A0)                          ; @BT $E0 $D0
; LSL.W (A1): tt=01,d=1,ea=(010,001) => $E3D1
  LSL.W (A1)                          ; @BT $E3 $D1
; LSR.W (A1): d=0 => $E2D1
  LSR.W (A1)                          ; @BT $E2 $D1
; ROL.W (A2): tt=11,d=1,ea=(010,010) => $E7D2
  ROL.W (A2)                          ; @BT $E7 $D2
; ROR.W (A2): d=0 => $E6D2
  ROR.W (A2)                          ; @BT $E6 $D2
; ROXL.W (A3): tt=10,d=1,ea=(010,011) => $E5D3
  ROXL.W (A3)                         ; @BT $E5 $D3
; ROXR.W (A3): d=0 => $E4D3
  ROXR.W (A3)                         ; @BT $E4 $D3
; Shift by register: ASL.W D1,D0: cnt=D1=001,d=1,ss=01,i=0,tt=00 => $E340
  ASL.W D1,D0                         ; @BT $E3 $60
; ASR.W D1,D0: d=0 => $E240
  ASR.W D1,D0                         ; @BT $E2 $60

;==========================================================
; TEST 18: BIT MANIPULATION
;==========================================================
TEST_BITS:
; BTST D0,D1: 0000_000_100_000_001 = $0101
  BTST D0,D1                          ; @BT $01 $01
; BTST #3,D0: 0000_1000_00_000_000 = $0800 + $0003
  BTST #3,D0                          ; @BT $08 $00 $00 $03
; BSET D0,D1: 0000_000_111_000_001 = $01C1
  BSET D0,D1                          ; @BT $01 $C1
; BSET #3,D0: 0000_1000_11_000_000 = $08C0 + $0003
  BSET #3,D0                          ; @BT $08 $C0 $00 $03
; BCLR D0,D1: 0000_000_110_000_001 = $0181
  BCLR D0,D1                          ; @BT $01 $81
; BCLR #3,D0: $0880 + $0003
  BCLR #3,D0                          ; @BT $08 $80 $00 $03
; BCHG D0,D1: 0000_000_101_000_001 = $0141
  BCHG D0,D1                          ; @BT $01 $41
; BCHG #3,D0: $0840 + $0003
  BCHG #3,D0                          ; @BT $08 $40 $00 $03

;==========================================================
; TEST 19: CMP / CMPA / CMPM
;==========================================================
TEST_CMP:
; CMP.B D1,D0: 1011_000_000_000_001 = $B001
  CMP.B D1,D0                         ; @BT $B0 $01
; CMP.W D2,D3: $B642
  CMP.W D2,D3                         ; @BT $B6 $42
; CMP.L D4,D5: $BA84
  CMP.L D4,D5                         ; @BT $BA $84
; CMPI.B #$10,D0: $0C00 + $0010
  CMP.B #$10,D0                       ; @BT $0C $00 $00 $10
; CMPI.W #$1000,D1: $0C41 + $1000
  CMP.W #$1000,D1                     ; @BT $0C $41 $10 $00
; CMPI.L #$10000,D2: $0C82 + $00010000
  CMP.L #$10000,D2                    ; @BT $0C $82 $00 $01 $00 $00

TEST_CMPA:
; CMPA.W D0,A0: opmode=011 => $B0C0
  CMPA.W D0,A0                        ; @BT $B0 $C0
; CMPA.L D1,A1: opmode=111 => $B3C1
  CMPA.L D1,A1                        ; @BT $B3 $C1
; CMPA.W #$1000,A0: 1011_000_011_111_100 = $B0FC + $1000
  CMPA.W #$1000,A0                    ; @BT $B0 $FC $10 $00
; CMPA.L #$10000,A1: 1011_001_111_111_100 = $B3FC + $00010000
  CMPA.L #$10000,A1                   ; @BT $B3 $FC $00 $01 $00 $00

TEST_CMPM:
; CMPM.B (A0)+,(A1)+: 1011_001_1_00_001_000 = $B308
  CMPM.B (A0)+,(A1)+                  ; @BT $B3 $08
; CMPM.W (A2)+,(A3)+: $B74A
  CMPM.W (A2)+,(A3)+                  ; @BT $B7 $4A
; CMPM.L (A2)+,(A3)+: $B78A
  CMPM.L (A2)+,(A3)+                  ; @BT $B7 $8A

;==========================================================
; TEST 20: TST
;==========================================================
TEST_TST:
; TST.B D0: 0100_1010_00_000_000 = $4A00
  TST.B D0                            ; @BT $4A $00
; TST.W D1: $4A41
  TST.W D1                            ; @BT $4A $41
; TST.L D2: $4A82
  TST.L D2                            ; @BT $4A $82
; TST.W (A0): $4A50
  TST.W (A0)                          ; @BT $4A $50

;==========================================================
; TEST 21: TAS
;==========================================================
TEST_TAS:
; TAS D0: 0100_1010_11_000_000 = $4AC0
  TAS D0                              ; @BT $4A $C0
; TAS (A0): $4AD0
  TAS (A0)                            ; @BT $4A $D0

;==========================================================
; TEST 22: BCD — ABCD, SBCD, NBCD
;==========================================================
TEST_BCD:
; ABCD D1,D0: 1100_000_1_00000_001 = $C101
  ABCD D1,D0                          ; @BT $C1 $01
; ABCD -(A1),-(A0): 1100_000_1_00001_001 = $C109
  ABCD -(A1),-(A0)                    ; @BT $C1 $09
; SBCD D1,D0: 1000_000_1_00000_001 = $8101
  SBCD D1,D0                          ; @BT $81 $01
; SBCD -(A1),-(A0): $8109
  SBCD -(A1),-(A0)                    ; @BT $81 $09
; NBCD D0: 0100_1000_00_000_000 = $4800
  NBCD D0                             ; @BT $48 $00

;==========================================================
; TEST 23: BRANCH (Bcc)
; Short (.B): 0110_cccc_disp8  Long (.W): 0110_cccc_00000000 + word
; Each branch targets a label after 0-10 random instructions (prefer complex).
;==========================================================
TEST_BRANCH:
; BRA.B to next+10 bytes: disp=10 => $600A
  BRA.B T23_1                         ; @BT $60 $0A
  NOP                                 ; @BT $4E $71
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T23_1:
; BEQ.B to next+8 bytes: disp=8 => $6708
  BEQ.B T23_2                         ; @BT $67 $08
  CLR.B D0                            ; @BT $42 $00
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T23_2:
; BNE.B to next+10 bytes: disp=10 => $660A
  BNE.B T23_3                         ; @BT $66 $0A
  TST.B D0                            ; @BT $4A $00
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T23_3:
; BCC.B to next+6 bytes: disp=6 => $6406
  BCC.B T23_4                         ; @BT $64 $06
  SWAP D0                             ; @BT $48 $40
  AND.B #$0F,D0                       ; @BT $02 $00 $00 $0F
T23_4:
; BCS.B to next+10 bytes: disp=10 => $650A
  BCS.B T23_5                         ; @BT $65 $0A
  NOT.B D0                            ; @BT $46 $00
  OR.W #$FF00,D1                      ; @BT $00 $41 $FF $00
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
T23_5:
; BVC.B to next+6 bytes: disp=6 => $6806
  BVC.B T23_6                         ; @BT $68 $06
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,($2000).W                 ; @BT $21 $C0 $20 $00
T23_6:
; BVS.B to next+6 bytes: disp=6 => $6906
  BVS.B T23_7                         ; @BT $69 $06
  EXT.W D0                            ; @BT $48 $80
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
T23_7:
; BPL.B to next+8 bytes: disp=8 => $6A08
  BPL.B T23_8                         ; @BT $6A $08
  ASL.B #1,D0                         ; @BT $E3 $00
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T23_8:
; BMI.B to next+6 bytes: disp=6 => $6B06
  BMI.B T23_9                         ; @BT $6B $06
  LSR.B #1,D0                         ; @BT $E2 $08
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T23_9:
; BGE.B to next+8 bytes: disp=8 => $6C08
  BGE.B T23_10                        ; @BT $6C $08
  ROXL.B #1,D0                        ; @BT $E3 $10
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T23_10:
; BLT.B to next+8 bytes: disp=8 => $6D08
  BLT.B T23_11                        ; @BT $6D $08
  ROXR.B #1,D0                        ; @BT $E2 $10
  LEA ($10,A0),A4                     ; @BT $49 $E8 $00 $10
  NOP                                 ; @BT $4E $71
T23_11:
; BGT.B to next+6 bytes: disp=6 => $6E06
  BGT.B T23_12                        ; @BT $6E $06
  ROL.B #1,D0                         ; @BT $E3 $18
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T23_12:
; BLE.B to next+6 bytes: disp=6 => $6F06
  BLE.B T23_13                        ; @BT $6F $06
  ROR.B #1,D0                         ; @BT $E2 $18
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
T23_13:
; BHI.B to next+8 bytes: disp=8 => $6208
  BHI.B T23_14                        ; @BT $62 $08
  LSL.B #1,D0                         ; @BT $E3 $08
  MOVE.L ($00010000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T23_14:
; BLS.B to next+8 bytes: disp=8 => $6308
  BLS.B T23_15                        ; @BT $63 $08
  LSR.B #1,D0                         ; @BT $E2 $08
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
  NOP                                 ; @BT $4E $71
T23_15:
; BRA.W to next+12 bytes: disp=14 => $6000000E
  BRA.W T23_16                        ; @BT $60 $00 $00 $0E
  NOP                                 ; @BT $4E $71
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T23_16:
; BSR.B to next+8 bytes: disp=8 => $6108
  BSR.B T23_17                        ; @BT $61 $08
  CLR.B D0                            ; @BT $42 $00
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T23_17:
; BSR.W to next+10 bytes: disp=12 => $6100000C
  BSR.W T23_18                        ; @BT $61 $00 $00 $0C
  TST.B D0                            ; @BT $4A $00
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T23_18:

;==========================================================
; TEST 24: DBcc
; 0101_cccc_11001_Dn + disp word
; Each DBcc targets a label after 0-10 random instructions (prefer complex).
;==========================================================
TEST_DBCC:
; DBT D0: cccc=0001, D0=000 => $50C8, disp=8
  DBT D0,T24_1                        ; @BT $50 $C8 $00 $08
  NOP                                 ; @BT $4E $71
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T24_1:
; DBF D1: cccc=0000, D1=001 => $51C9, disp=10
  DBF D1,T24_2                        ; @BT $51 $C9 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
  NOP                                 ; @BT $4E $71
T24_2:
; DBRA D2: alias DBF, D2=010 => $51CA, disp=10
  DBRA D2,T24_3                       ; @BT $51 $CA $00 $0A
  TST.B D0                            ; @BT $4A $00
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T24_3:
; DBHI D3: cccc=0010, D3=011 => $52CB, disp=10
  DBHI D3,T24_4                       ; @BT $52 $CB $00 $0A
  SWAP D0                             ; @BT $48 $40
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T24_4:
; DBLS D4: cccc=0011, D4=100 => $53CC, disp=8
  DBLS D4,T24_5                       ; @BT $53 $CC $00 $08
  NOT.B D0                            ; @BT $46 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T24_5:
; DBCC D5: cccc=0100, D5=101 => $54CD, disp=10
  DBCC D5,T24_6                       ; @BT $54 $CD $00 $0A
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,($2000).W                 ; @BT $21 $C0 $20 $00
  NOP                                 ; @BT $4E $71
T24_6:
; DBCS D6: cccc=0101, D6=110 => $55CE, disp=8
  DBCS D6,T24_7                       ; @BT $55 $CE $00 $08
  EXT.W D0                            ; @BT $48 $80
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
T24_7:
; DBNE D7: cccc=0110, D7=111 => $56CF, disp=8
  DBNE D7,T24_8                       ; @BT $56 $CF $00 $08
  ASL.B #1,D0                         ; @BT $E3 $00
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T24_8:
; DBEQ D0: cccc=0111, D0=000 => $57C8, disp=12
  DBEQ D0,T24_9                       ; @BT $57 $C8 $00 $0C
  LSR.B #1,D0                         ; @BT $E2 $08
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  NOP                                 ; @BT $4E $71
T24_9:
; DBVC D1: cccc=1000, D1=001 => $58C9, disp=12
  DBVC D1,T24_10                      ; @BT $58 $C9 $00 $0C
  ROXL.B #1,D0                        ; @BT $E3 $10
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T24_10:
; DBVS D2: cccc=1001, D2=010 => $59CA, disp=8
  DBVS D2,T24_11                      ; @BT $59 $CA $00 $08
  ROXR.B #1,D0                        ; @BT $E2 $10
  LEA ($10,A0),A4                     ; @BT $49 $E8 $00 $10
T24_11:
; DBPL D3: cccc=1010, D3=011 => $5ACB, disp=10
  DBPL D3,T24_12                      ; @BT $5A $CB $00 $0A
  ROL.B #1,D0                         ; @BT $E3 $18
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T24_12:
; DBMI D4: cccc=1011, D4=100 => $5BCC, disp=10
  DBMI D4,T24_13                      ; @BT $5B $CC $00 $0A
  ROR.B #1,D0                         ; @BT $E2 $18
  MOVE.L ($00010000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T24_13:
; DBGE D5: cccc=1100, D5=101 => $5CCD, disp=10
  DBGE D5,T24_14                      ; @BT $5C $CD $00 $0A
  LSL.B #1,D0                         ; @BT $E3 $08
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  NOP                                 ; @BT $4E $71
T24_14:
; DBLT D6: cccc=1101, D6=110 => $5DCE, disp=12
  DBLT D6,T24_15                      ; @BT $5D $CE $00 $0C
  LSR.B #1,D0                         ; @BT $E2 $08
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
T24_15:
; DBGT D7: cccc=1110, D7=111 => $5ECF, disp=10
  DBGT D7,T24_16                      ; @BT $5E $CF $00 $0A
  NOP                                 ; @BT $4E $71
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T24_16:
; DBLE D0: cccc=1111, D0=000 => $5FC8, disp=10
  DBLE D0,T24_17                      ; @BT $5F $C8 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T24_17:

;==========================================================
; TEST 25: Scc — Set According to Condition
; 0101_cccc_11_000_Dn (byte)
;==========================================================
TEST_SCC:
; ST D0: cccc=0001 => $50C0
  ST D0                               ; @BT $50 $C0
; SF D1: cccc=0000 => $51C1
  SF D1                               ; @BT $51 $C1
; SHI D2: cccc=0010 => $52C2
  SHI D2                              ; @BT $52 $C2
; SLS D3: cccc=0011 => $53C3
  SLS D3                              ; @BT $53 $C3
; SCC D4: cccc=0100 => $54C4
  SCC D4                              ; @BT $54 $C4
; SCS D5: cccc=0101 => $55C5
  SCS D5                              ; @BT $55 $C5
; SNE D6: cccc=0110 => $56C6
  SNE D6                              ; @BT $56 $C6
; SEQ D7: cccc=0111 => $57C7
  SEQ D7                              ; @BT $57 $C7
; SVC D0: cccc=1000 => $58C0
  SVC D0                              ; @BT $58 $C0
; SVS D1: cccc=1001 => $59C1
  SVS D1                              ; @BT $59 $C1
; SPL D2: cccc=1010 => $5AC2
  SPL D2                              ; @BT $5A $C2
; SMI D3: cccc=1011 => $5BC3
  SMI D3                              ; @BT $5B $C3
; SGE D4: cccc=1100 => $5CC4
  SGE D4                              ; @BT $5C $C4
; SLT D5: cccc=1101 => $5DC5
  SLT D5                              ; @BT $5D $C5
; SGT D6: cccc=1110 => $5EC6
  SGT D6                              ; @BT $5E $C6
; SLE D7: cccc=1111 => $5FC7
  SLE D7                              ; @BT $5F $C7

;==========================================================
; TEST 26: JMP, JSR, RTS, RTR, RTE, NOP
;==========================================================
TEST_JUMP:
; JMP (A0): 0100_1110_11_010_000 = $4ED0
  JMP (A0)                            ; @BT $4E $D0
; JMP ($1000).W: $4EF8 + $1000
  JMP ($1000).W                       ; @BT $4E $F8 $10 $00
; JMP ($00001000).L: $4EF9 + $00001000
  JMP ($00001000).L                   ; @BT $4E $F9 $00 $00 $10 $00
; JSR (A0): 0100_1110_10_010_000 = $4E90
  JSR (A0)                            ; @BT $4E $90
; JSR ($1000).W: $4EB8 + $1000
  JSR ($1000).W                       ; @BT $4E $B8 $10 $00
; JSR ($00001000).L: $4EB9 + $00001000
  JSR ($00001000).L                   ; @BT $4E $B9 $00 $00 $10 $00
; RTS: $4E75
  RTS                                 ; @BT $4E $75
; RTR: $4E77
  RTR                                 ; @BT $4E $77
; RTE: $4E73 (supervisor)
  RTE                                 ; @BT $4E $73
; NOP: $4E71
  NOP                                 ; @BT $4E $71

;==========================================================
; TEST 27: LINK / UNLK
;==========================================================
TEST_LINK:
; LINK A6,#-8: $4E56 + $FFF8
  LINK A6,#-8                         ; @BT $4E $56 $FF $F8
; LINK A7,#-64: $4E57 + $FFC0
  LINK A7,#-64                        ; @BT $4E $57 $FF $C0
; UNLK A6: $4E5E
  UNLK A6                             ; @BT $4E $5E
; UNLK A7: $4E5F
  UNLK A7                             ; @BT $4E $5F

;==========================================================
; TEST 28: TRAP / TRAPV / RESET / STOP / ILLEGAL
;==========================================================
TEST_TRAP:
; TRAP #0: $4E40
  TRAP #0                             ; @BT $4E $40
; TRAP #1: $4E41
  TRAP #1                             ; @BT $4E $41
; TRAP #15: $4E4F
  TRAP #15                            ; @BT $4E $4F
; TRAPV: $4E76
  TRAPV                               ; @BT $4E $76
; RESET: $4E70 (supervisor)
  RESET                               ; @BT $4E $70
; STOP #$2700: $4E72 + $2700
  STOP #$2700                         ; @BT $4E $72 $27 $00
; ILLEGAL: $4AFC
  ILLEGAL                             ; @BT $4A $FC

;==========================================================
; TEST 29: LEA / PEA
;==========================================================
TEST_LEA_PEA:
; LEA (A0),A1: 0100_001_111_010_000 = $43D0
  LEA (A0),A1                         ; @BT $43 $D0
; LEA ($1000).W,A2: $45F8 + $1000
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
; LEA ($00001000).L,A3: $47F9 + $00001000
  LEA ($00001000).L,A3                ; @BT $47 $F9 $00 $00 $10 $00
; LEA ($10,A0),A4: $49E8 + $0010
  LEA ($10,A0),A4                     ; @BT $49 $E8 $00 $10
; PEA (A0): 0100_1000_01_010_000 = $4850
  PEA (A0)                            ; @BT $48 $50
; PEA ($1000).W: $4878 + $1000
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
; PEA ($00001000).L: $4879 + $00001000
  PEA ($00001000).L                   ; @BT $48 $79 $00 $00 $10 $00

;==========================================================
; TEST 30: CHK
;==========================================================
TEST_CHK:
; CHK D1,D0: 0100_000_110_000_001 = $4181
  CHK D1,D0                           ; @BT $41 $81
; CHK ($1000).W,D2: $45B8 + $1000
  CHK ($1000).W,D2                    ; @BT $45 $B8 $10 $00

;==========================================================
; TEST 31: EXG
;==========================================================
TEST_EXG:
; EXG D0,D1: opmode=01000 => $C141
  EXG D0,D1                           ; @BT $C1 $41
; EXG A0,A1: opmode=01001 => $C149
  EXG A0,A1                           ; @BT $C1 $49
; EXG D0,A0: opmode=10001 => $C188
  EXG D0,A0                           ; @BT $C1 $88

;==========================================================
; TEST 32: MOVE to/from CCR, SR, USP
;==========================================================
TEST_SR_CCR:
; MOVE D0,CCR: $44C0
  MOVE D0,CCR                         ; @BT $44 $C0
; MOVE D1,SR: $46C1 (supervisor)
  MOVE D1,SR                          ; @BT $46 $C1
; MOVE SR,D2: $40C2
  MOVE SR,D2                          ; @BT $40 $C2
; MOVE.L A0,USP: $4E60 (supervisor)
  MOVE.L A0,USP                       ; @BT $4E $60
; MOVE.L USP,A1: $4E69
  MOVE.L USP,A1                       ; @BT $4E $69

;==========================================================
; TEST 33: Immediate ops on SR/CCR
; CCR ops require .B, SR ops require .W (size is enforced by the decoder).
; Byte immediates are padded to a word on output (e.g. #$00 -> $0000).
;==========================================================
TEST_SR_IMM:
; ANDI.B #$00,CCR: $023C + $0000
  ANDI.B #$00,CCR                     ; @BT $02 $3C $00 $00
; ANDI.W #$FF00,SR: $027C + $FF00
  ANDI.W #$FF00,SR                    ; @BT $02 $7C $FF $00
; ORI.B #$00,CCR: $003C + $0000
  ORI.B #$00,CCR                      ; @BT $00 $3C $00 $00
; ORI.W #$0000,SR: $007C + $0000
  ORI.W #$0000,SR                     ; @BT $00 $7C $00 $00
; EORI.B #$00,CCR: $0A3C + $0000
  EORI.B #$00,CCR                     ; @BT $0A $3C $00 $00
; EORI.W #$0000,SR: $0A7C + $0000
  EORI.W #$0000,SR                    ; @BT $0A $7C $00 $00

;==========================================================
; TEST 34: Stack push/pop
;==========================================================
TEST_STACK:
; MOVE.L D0,-(A7): dest=(100,111),src=(000,000) = $2F00
  MOVE.L D0,-(A7)                     ; @BT $2F $00
; MOVE.L (A7)+,D0: src=(011,111),dest=(000,000) = $201F
  MOVE.L (A7)+,D0                     ; @BT $20 $1F
; MOVE.W D1,-(A7): dest=(100,111),src=(000,001) = $3F01
  MOVE.W D1,-(A7)                     ; @BT $3F $01
; MOVE.W (A7)+,D1: src=(011,111),dest=(000,001) = $321F
  MOVE.W (A7)+,D1                     ; @BT $32 $1F

;==========================================================
; TEST 35: PC-Relative
;==========================================================
TEST_PCREL:
; MOVE.W (d16,PC),D0: ea=(111,010), disp=CADDR offset => $303A + $FC2D
  MOVE.W (0,PC),D0                    ; @BT $30 $3A $FC $11

;==========================================================
; TEST 36: Brief Extension Word (index)
; WLA DX only supports bare Dn index registers (no .W/.L suffix).
; The extension word always uses word-sized index: high byte = Dn<<4, low byte = d8.
;==========================================================
TEST_INDEX:
; MOVE.W (4,A0,D1),D2
; Brief ext word: D1<<4=$10, disp=$04 => $1004; opcode $3430
  MOVE.W (4,A0,D1),D2                 ; @BT $34 $30 $10 $04

;==========================================================
; TEST 37: Explicit immediate mnemonics
;==========================================================
TEST_IMM_MNEMONICS:
; ADDI.B #$10,D0: $0600 + $0010
  ADDI.B #$10,D0                      ; @BT $06 $00 $00 $10
; SUBI.W #$0100,D1: $0441 + $0100
  SUBI.W #$0100,D1                    ; @BT $04 $41 $01 $00
; ANDI.B #$0F,D0: $0200 + $000F
  ANDI.B #$0F,D0                      ; @BT $02 $00 $00 $0F
; ORI.B #$0F,D0: $0000 + $000F
  ORI.B #$0F,D0                       ; @BT $00 $00 $00 $0F
; EORI.B #$AA,D0: $0A00 + $00AA
  EORI.B #$AA,D0                      ; @BT $0A $00 $00 $AA
; CMPI.B #$10,D0: $0C00 + $0010
  CMPI.B #$10,D0                      ; @BT $0C $00 $00 $10

  .db "<01" ; @BT END

.db "02>" ; @BT TEST-02 02 START

TEST2_ALL:
  SUBX.B D1,D0                        ; @BT $91 $01
  MOVE.L D1,($00020000).L             ; @BT $23 $C1 $00 $02 $00 $00
  ROL.W (A2)                          ; @BT $E7 $D2
  SUBI.W #$0100,D1                    ; @BT $04 $41 $01 $00
  EXG A0,A1                           ; @BT $C1 $49
  NOT.B D0                            ; @BT $46 $00
  SEQ D7                              ; @BT $57 $C7
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
  NEGX.W D1                           ; @BT $40 $41
  ANDI.W #$FF00,SR                    ; @BT $02 $7C $FF $00
  MOVEA.L D1,A1                       ; @BT $22 $41
  SWAP D7                             ; @BT $48 $47
  MOVE.L ($00010000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
  RTR                                 ; @BT $4E $77
  ADD.W D2,D3                         ; @BT $D6 $42
  NOT.L D2                            ; @BT $46 $82
; DBT D0: cccc=0001, D0=000 => $50C8, disp=8
  DBT D0,T2_24_1                        ; @BT $50 $C8 $00 $08
  NOP                                 ; @BT $4E $71
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T2_24_1:
; DBF D1: cccc=0000, D1=001 => $51C9, disp=10
  DBF D1,T2_24_2                        ; @BT $51 $C9 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
  NOP                                 ; @BT $4E $71
T2_24_2:
; DBRA D2: alias DBF, D2=010 => $51CA, disp=10
  DBRA D2,T2_24_3                       ; @BT $51 $CA $00 $0A
  TST.B D0                            ; @BT $4A $00
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T2_24_3:
; DBHI D3: cccc=0010, D3=011 => $52CB, disp=10
  DBHI D3,T2_24_4                       ; @BT $52 $CB $00 $0A
  SWAP D0                             ; @BT $48 $40
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T2_24_4:
; DBLS D4: cccc=0011, D4=100 => $53CC, disp=8
  DBLS D4,T2_24_5                       ; @BT $53 $CC $00 $08
  NOT.B D0                            ; @BT $46 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T2_24_5:
; DBCC D5: cccc=0100, D5=101 => $54CD, disp=10
  DBCC D5,T2_24_6                       ; @BT $54 $CD $00 $0A
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,($2000).W                 ; @BT $21 $C0 $20 $00
  NOP                                 ; @BT $4E $71
T2_24_6:
; DBCS D6: cccc=0101, D6=110 => $55CE, disp=8
  DBCS D6,T2_24_7                       ; @BT $55 $CE $00 $08
  EXT.W D0                            ; @BT $48 $80
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
T2_24_7:
; DBNE D7: cccc=0110, D7=111 => $56CF, disp=8
  DBNE D7,T2_24_8                       ; @BT $56 $CF $00 $08
  ASL.B #1,D0                         ; @BT $E3 $00
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T2_24_8:
; DBEQ D0: cccc=0111, D0=000 => $57C8, disp=12
  DBEQ D0,T2_24_9                       ; @BT $57 $C8 $00 $0C
  LSR.B #1,D0                         ; @BT $E2 $08
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  NOP                                 ; @BT $4E $71
T2_24_9:
; DBVC D1: cccc=1000, D1=001 => $58C9, disp=12
  DBVC D1,T2_24_10                      ; @BT $58 $C9 $00 $0C
  ROXL.B #1,D0                        ; @BT $E3 $10
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T2_24_10:
; DBVS D2: cccc=1001, D2=010 => $59CA, disp=8
  DBVS D2,T2_24_11                      ; @BT $59 $CA $00 $08
  ROXR.B #1,D0                        ; @BT $E2 $10
  LEA ($10,A0),A4                     ; @BT $49 $E8 $00 $10
T2_24_11:
; DBPL D3: cccc=1010, D3=011 => $5ACB, disp=10
  DBPL D3,T2_24_12                      ; @BT $5A $CB $00 $0A
  ROL.B #1,D0                         ; @BT $E3 $18
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T2_24_12:
; DBMI D4: cccc=1011, D4=100 => $5BCC, disp=10
  DBMI D4,T2_24_13                      ; @BT $5B $CC $00 $0A
  ROR.B #1,D0                         ; @BT $E2 $18
  MOVE.L ($00010000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T2_24_13:
; DBGE D5: cccc=1100, D5=101 => $5CCD, disp=10
  DBGE D5,T2_24_14                      ; @BT $5C $CD $00 $0A
  LSL.B #1,D0                         ; @BT $E3 $08
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  NOP                                 ; @BT $4E $71
T2_24_14:
; DBLT D6: cccc=1101, D6=110 => $5DCE, disp=12
  DBLT D6,T2_24_15                      ; @BT $5D $CE $00 $0C
  LSR.B #1,D0                         ; @BT $E2 $08
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
T2_24_15:
; DBGT D7: cccc=1110, D7=111 => $5ECF, disp=10
  DBGT D7,T2_24_16                      ; @BT $5E $CF $00 $0A
  NOP                                 ; @BT $4E $71
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T2_24_16:
; DBLE D0: cccc=1111, D0=000 => $5FC8, disp=10
  DBLE D0,T2_24_17                      ; @BT $5F $C8 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T2_24_17:

;==========================================================
; TEST 25: Scc — Set According to Condition
; 0101_cccc_11_000_Dn (byte)
;==========================================================
  ROR.B #1,D0                         ; @BT $E2 $18
  ABCD D1,D0                          ; @BT $C1 $01
  ST D0                               ; @BT $50 $C0
  MOVE.L USP,A1                       ; @BT $4E $69
  SUBQ.B #1,D0                        ; @BT $53 $00
  JSR (A0)                            ; @BT $4E $90
  CMP.W D2,D3                         ; @BT $B6 $42
  ASL.W #2,D1                         ; @BT $E5 $41
  MOVEA.W D0,A0                       ; @BT $30 $40
  ROL.B #1,D0                         ; @BT $E3 $18
  AND.B D1,D0                         ; @BT $C0 $01
  LINK A7,#-64                        ; @BT $4E $57 $FF $C0
  SBCD -(A1),-(A0)                    ; @BT $81 $09
  AND.W #$FF00,D1                     ; @BT $02 $41 $FF $00
  MOVE.W D2,D3                        ; @BT $36 $02
  OR.W D2,D3                          ; @BT $86 $42
  BTST #3,D0                          ; @BT $08 $00 $00 $03
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  MULU.W D2,D3                        ; @BT $C6 $C2
  NEG.B D0                            ; @BT $44 $00
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  SBCD D1,D0                          ; @BT $81 $01
  RTE                                 ; @BT $4E $73
  SVS D1                              ; @BT $59 $C1
  BCLR D0,D1                          ; @BT $01 $81
  ORI.B #$0F,D0                       ; @BT $00 $00 $00 $0F
  MOVEQ #0,D0                         ; @BT $70 $00
  MOVEQ #1,D1                         ; @BT $72 $01
  BTST D0,D1                          ; @BT $01 $01
  ROR.W (A2)                          ; @BT $E6 $D2
  LSR.W #2,D1                         ; @BT $E4 $49
  TRAP #1                             ; @BT $4E $41
  NBCD D0                             ; @BT $48 $00
  ASL.W D1,D0                         ; @BT $E3 $60
  CMPI.B #$10,D0                      ; @BT $0C $00 $00 $10
  CMPM.W (A2)+,(A3)+                  ; @BT $B7 $4A
  LSR.B #1,D0                         ; @BT $E2 $08
  MOVEQ #-1,D2                        ; @BT $74 $FF
  RTS                                 ; @BT $4E $75
  ASL.W (A0)                          ; @BT $E1 $D0
  BSET D0,D1                          ; @BT $01 $C1
  SPL D2                              ; @BT $5A $C2
  MOVE.B D0,D1                        ; @BT $12 $00
  TST.W (A0)                          ; @BT $4A $50
  BSET #3,D0                          ; @BT $08 $C0 $00 $03
  SUB.W D2,D3                         ; @BT $96 $42
  MOVEM.L D0/D1/A0/A1,-(A7)          ; @BT $48 $E7 $C0 $C0
  NEG.W D1                            ; @BT $44 $41
  SGE D4                              ; @BT $5C $C4
  CMP.B #$10,D0                       ; @BT $0C $00 $00 $10
  ASR.W (A0)                          ; @BT $E0 $D0
  ABCD -(A1),-(A0)                    ; @BT $C1 $09
  SHI D2                              ; @BT $52 $C2
  TST.L D2                            ; @BT $4A $82
  MOVE.L D0,($2000).W                 ; @BT $21 $C0 $20 $00
  ADD.B D1,D0                         ; @BT $D0 $01
  ADD.B #$10,D0                       ; @BT $06 $00 $00 $10
  ADDX.L D5,D4                        ; @BT $D9 $85
  MOVE.W D1,-(A7)                     ; @BT $3F $01
  ADDQ.B #1,D0                        ; @BT $52 $00
  CMP.L #$10000,D2                    ; @BT $0C $82 $00 $01 $00 $00
  EORI.B #$AA,D0                      ; @BT $0A $00 $00 $AA
  CMP.L D4,D5                         ; @BT $BA $84
  ADDQ.L #8,D7                        ; @BT $50 $87
  MOVE.W D3,($0020,A3)                ; @BT $37 $43 $00 $20
  EORI.B #$00,CCR                     ; @BT $0A $3C $00 $00
  TAS (A0)                            ; @BT $4A $D0
  PEA (A0)                            ; @BT $48 $50
  AND.W D2,D3                         ; @BT $C6 $42
  AND.B #$0F,D0                       ; @BT $02 $00 $00 $0F
  MOVEM.L (A7)+,D0/D1/A0/A1          ; @BT $4C $DF $03 $03
  ANDI.B #$00,CCR                     ; @BT $02 $3C $00 $00
  CMP.B D1,D0                         ; @BT $B0 $01
  SUBA.L D1,A1                        ; @BT $93 $C1
  TRAP #15                            ; @BT $4E $4F
  MOVEA.L A3,A4                       ; @BT $28 $4B
  MOVE.B D0,(A0)                      ; @BT $10 $80
  PEA ($00001000).L                   ; @BT $48 $79 $00 $00 $10 $00
  ADDQ.W #2,A0                        ; @BT $54 $48
  DIVU.W D2,D3                        ; @BT $86 $C2
  MOVEQ #127,D7                       ; @BT $7E $7F
  MOVE.L D4,D5                        ; @BT $2A $04
  CHK ($1000).W,D2                    ; @BT $45 $B8 $10 $00
  SNE D6                              ; @BT $56 $C6
  MOVE.L ($1000).W,D4                 ; @BT $28 $38 $10 $00
  SLS D3                              ; @BT $53 $C3
  MOVE.L A0,USP                       ; @BT $4E $60
  TRAP #0                             ; @BT $4E $40
  CMPM.B (A0)+,(A1)+                  ; @BT $B3 $08
  MOVEP.L D1,($20,A1)                 ; @BT $03 $C9 $00 $20
  NEGX.B D0                           ; @BT $40 $00
  MOVE D0,CCR                         ; @BT $44 $C0
  ROXL.B #1,D0                        ; @BT $E3 $10
  MOVEP.L ($20,A1),D1                 ; @BT $03 $49 $00 $20
  MOVE SR,D2                          ; @BT $40 $C2
  MOVE.W (4,A0,D1),D2                 ; @BT $34 $30 $10 $04
  ROXR.B #1,D0                        ; @BT $E2 $10
  LEA ($10,A0),A4                     ; @BT $49 $E8 $00 $10
  LSL.B #1,D0                         ; @BT $E3 $08
  ROL.W #2,D1                         ; @BT $E5 $59
  ADD.L #$10000,D2                    ; @BT $06 $82 $00 $01 $00 $00
; BRA.B to next+10 bytes: disp=10 => $600A
  BRA.B T2_23_1                         ; @BT $60 $0A
  NOP                                 ; @BT $4E $71
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T2_23_1:
; BEQ.B to next+8 bytes: disp=8 => $6708
  BEQ.B T2_23_2                         ; @BT $67 $08
  CLR.B D0                            ; @BT $42 $00
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T2_23_2:
; BNE.B to next+10 bytes: disp=10 => $660A
  BNE.B T2_23_3                         ; @BT $66 $0A
  TST.B D0                            ; @BT $4A $00
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T2_23_3:
; BCC.B to next+6 bytes: disp=6 => $6406
  BCC.B T2_23_4                         ; @BT $64 $06
  SWAP D0                             ; @BT $48 $40
  AND.B #$0F,D0                       ; @BT $02 $00 $00 $0F
T2_23_4:
; BCS.B to next+10 bytes: disp=10 => $650A
  BCS.B T2_23_5                         ; @BT $65 $0A
  NOT.B D0                            ; @BT $46 $00
  OR.W #$FF00,D1                      ; @BT $00 $41 $FF $00
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
T2_23_5:
; BVC.B to next+6 bytes: disp=6 => $6806
  BVC.B T2_23_6                         ; @BT $68 $06
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,($2000).W                 ; @BT $21 $C0 $20 $00
T2_23_6:
; BVS.B to next+6 bytes: disp=6 => $6906
  BVS.B T2_23_7                         ; @BT $69 $06
  EXT.W D0                            ; @BT $48 $80
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
T2_23_7:
; BPL.B to next+8 bytes: disp=8 => $6A08
  BPL.B T2_23_8                         ; @BT $6A $08
  ASL.B #1,D0                         ; @BT $E3 $00
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T2_23_8:
; BMI.B to next+6 bytes: disp=6 => $6B06
  BMI.B T2_23_9                         ; @BT $6B $06
  LSR.B #1,D0                         ; @BT $E2 $08
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T2_23_9:
; BGE.B to next+8 bytes: disp=8 => $6C08
  BGE.B T2_23_10                        ; @BT $6C $08
  ROXL.B #1,D0                        ; @BT $E3 $10
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T2_23_10:
; BLT.B to next+8 bytes: disp=8 => $6D08
  BLT.B T2_23_11                        ; @BT $6D $08
  ROXR.B #1,D0                        ; @BT $E2 $10
  LEA ($10,A0),A4                     ; @BT $49 $E8 $00 $10
  NOP                                 ; @BT $4E $71
T2_23_11:
; BGT.B to next+6 bytes: disp=6 => $6E06
  BGT.B T2_23_12                        ; @BT $6E $06
  ROL.B #1,D0                         ; @BT $E3 $18
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T2_23_12:
; BLE.B to next+6 bytes: disp=6 => $6F06
  BLE.B T2_23_13                        ; @BT $6F $06
  ROR.B #1,D0                         ; @BT $E2 $18
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
T2_23_13:
; BHI.B to next+8 bytes: disp=8 => $6208
  BHI.B T2_23_14                        ; @BT $62 $08
  LSL.B #1,D0                         ; @BT $E3 $08
  MOVE.L ($00010000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T2_23_14:
; BLS.B to next+8 bytes: disp=8 => $6308
  BLS.B T2_23_15                        ; @BT $63 $08
  LSR.B #1,D0                         ; @BT $E2 $08
  MOVE.W ($0010,A3),D3                ; @BT $36 $2B $00 $10
  NOP                                 ; @BT $4E $71
T2_23_15:
; BRA.W to next+12 bytes: disp=14 => $6000000E
  BRA.W T2_23_16                        ; @BT $60 $00 $00 $0E
  NOP                                 ; @BT $4E $71
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
T2_23_16:
; BSR.B to next+8 bytes: disp=8 => $6108
  BSR.B T2_23_17                        ; @BT $61 $08
  CLR.B D0                            ; @BT $42 $00
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T2_23_17:
; BSR.W to next+10 bytes: disp=12 => $6100000C
  BSR.W T2_23_18                        ; @BT $61 $00 $00 $0C
  TST.B D0                            ; @BT $4A $00
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #$1234,D1                    ; @BT $32 $3C $12 $34
T2_23_18:

;==========================================================
; TEST 24: DBcc
; 0101_cccc_11001_Dn + disp word
; Each DBcc targets a label after 0-10 random instructions (prefer complex).
;==========================================================
  ADDX.B D1,D0                        ; @BT $D1 $01
  SUBQ.W #2,A0                        ; @BT $55 $48
  SUB.L #$10000,D2                    ; @BT $04 $82 $00 $01 $00 $00
  MOVE.L D0,-(A7)                     ; @BT $2F $00
  ADD.L D4,D5                         ; @BT $DA $84
  ADDA.W D0,A0                        ; @BT $D0 $C0
  MOVE D1,SR                          ; @BT $46 $C1
  ROXL.W (A3)                         ; @BT $E5 $D3
  EOR.B D0,D1                         ; @BT $B1 $01
  EORI.W #$0000,SR                    ; @BT $0A $7C $00 $00
  SWAP D0                             ; @BT $48 $40
  ASR.W D1,D0                         ; @BT $E2 $60
  MOVE.W (A1)+,D1                     ; @BT $32 $19
  BCHG #3,D0                          ; @BT $08 $40 $00 $03
  MOVE.B #$42,D0                      ; @BT $10 $3C $00 $42
  MULS.W D1,D0                        ; @BT $C1 $C1
  JSR ($00001000).L                   ; @BT $4E $B9 $00 $00 $10 $00
  TRAPV                               ; @BT $4E $76
  CMP.W #$1000,D1                     ; @BT $0C $41 $10 $00
  EOR.B #$AA,D0                       ; @BT $0A $00 $00 $AA
  ADDX.W D3,D2                        ; @BT $D5 $43
  OR.B #$0F,D0                        ; @BT $00 $00 $00 $0F
  ANDI.B #$0F,D0                      ; @BT $02 $00 $00 $0F
  LEA ($1000).W,A2                    ; @BT $45 $F8 $10 $00
  EXT.W D0                            ; @BT $48 $80
  LSR.W (A1)                          ; @BT $E2 $D1
  SUBQ.L #8,D7                        ; @BT $51 $87
  CMPA.W D0,A0                        ; @BT $B0 $C0
  NEG.L D2                            ; @BT $44 $82
  ASL.B #1,D0                         ; @BT $E3 $00
  MOVE.W (0,PC),D0                    ; @BT $30 $3A $F8 $E3
  SLT D5                              ; @BT $5D $C5
  SLE D7                              ; @BT $5F $C7
  JMP ($1000).W                       ; @BT $4E $F8 $10 $00
  TST.W D1                            ; @BT $4A $41
  PEA ($1000).W                       ; @BT $48 $78 $10 $00
  SGT D6                              ; @BT $5E $C6
  EOR.W D2,D3                         ; @BT $B5 $43
  ASR.W #2,D1                         ; @BT $E4 $41
  CLR.L D2                            ; @BT $42 $82
  MOVE.L (A7)+,D0                     ; @BT $20 $1F
  STOP #$2700                         ; @BT $4E $72 $27 $00
  SMI D3                              ; @BT $5B $C3
  ROR.W #2,D1                         ; @BT $E4 $59
  OR.B D1,D0                          ; @BT $80 $01
  MOVE.W D1,(A1)+                     ; @BT $32 $C1
  ADDX.W -(A1),-(A0)                  ; @BT $D1 $49
  EOR.L #$AAAAAAAA,D2                 ; @BT $0A $82 $AA $AA $AA $AA
  BCLR #3,D0                          ; @BT $08 $80 $00 $03
  EXT.L D1                            ; @BT $48 $C1
  LSL.W (A1)                          ; @BT $E3 $D1
  SUBX.W -(A1),-(A0)                  ; @BT $91 $49
  BCHG D0,D1                          ; @BT $01 $41
  NOP                                 ; @BT $4E $71
  ADDQ.W #4,D1                        ; @BT $58 $41
  MOVE.L #$12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  JSR ($1000).W                       ; @BT $4E $B8 $10 $00
  LSL.W #2,D1                         ; @BT $E5 $49
  SCC D4                              ; @BT $54 $C4
  SUB.W #$100,D1                      ; @BT $04 $41 $01 $00
  ADDI.B #$10,D0                      ; @BT $06 $00 $00 $10
  NEGX.L D2                           ; @BT $40 $82
  SUB.L D4,D5                         ; @BT $9A $84
  SVC D0                              ; @BT $58 $C0
  SF D1                               ; @BT $51 $C1
  NOT.W D1                            ; @BT $46 $41
  TAS D0                              ; @BT $4A $C0
  CMPM.L (A2)+,(A3)+                  ; @BT $B7 $8A
  JMP ($00001000).L                   ; @BT $4E $F9 $00 $00 $10 $00
  SUB.B D1,D0                         ; @BT $90 $01
  ADDA.L D1,A1                        ; @BT $D3 $C1
  ILLEGAL                             ; @BT $4A $FC
  DIVS.W D1,D0                        ; @BT $81 $C1
  ROXR.W (A3)                         ; @BT $E4 $D3
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
  ORI.W #$0000,SR                     ; @BT $00 $7C $00 $00
  UNLK A6                             ; @BT $4E $5E
  LINK A6,#-8                         ; @BT $4E $56 $FF $F8
  ADD.W #$1000,D1                     ; @BT $06 $41 $10 $00
  OR.W #$FF00,D1                      ; @BT $00 $41 $FF $00
  SCS D5                              ; @BT $55 $C5
  CLR.B D0                            ; @BT $42 $00
  AND.L D4,D5                         ; @BT $CA $84
  OR.L D4,D5                          ; @BT $8A $84
  MOVEA.W #$1000,A2                   ; @BT $34 $7C $10 $00
  ASL.L #4,D2                         ; @BT $E9 $82
  SUBA.W D0,A0                        ; @BT $90 $C0
  MOVEP.W D0,($10,A0)                 ; @BT $01 $88 $00 $10
  EXG D0,A0                           ; @BT $C1 $88
  AND.L #$FFFF0000,D2                 ; @BT $02 $82 $FF $FF $00 $00
  EOR.L D4,D5                         ; @BT $B9 $85
  TST.B D0                            ; @BT $4A $00
  CMPA.L D1,A1                        ; @BT $B3 $C1
  EXG D0,D1                           ; @BT $C1 $41
  MOVE.W (A7)+,D1                     ; @BT $32 $1F
  MOVE.L D2,-(A2)                     ; @BT $25 $02
  EOR.W #$AAAA,D1                     ; @BT $0A $41 $AA $AA
  MOVEP.W ($10,A0),D0                 ; @BT $01 $08 $00 $10
  MOVE.L -(A2),D2                     ; @BT $24 $22
  CHK D1,D0                           ; @BT $41 $81
  ORI.B #$00,CCR                      ; @BT $00 $3C $00 $00
  CLR.W D1                            ; @BT $42 $41
  LEA ($00001000).L,A3                ; @BT $47 $F9 $00 $00 $10 $00
  ADDA.W #$100,A2                     ; @BT $D4 $FC $01 $00
  MOVE.B (A0),D0                      ; @BT $10 $10
  ASR.L #1,D3                         ; @BT $E2 $83
  OR.L #$FFFF0000,D2                  ; @BT $00 $82 $FF $FF $00 $00
  UNLK A7                             ; @BT $4E $5F
  RESET                               ; @BT $4E $70
  LEA (A0),A1                         ; @BT $43 $D0
  SUB.B #$10,D0                       ; @BT $04 $00 $00 $10
  SUBQ.W #4,D1                        ; @BT $59 $41
  JMP (A0)                            ; @BT $4E $D0
  ASR.B #1,D0                         ; @BT $E2 $00

; potential quick-form optimization candidates (must stay non-quick)
  ADD.W #1,D0                         ; @BT $06 $40 $00 $01
  ADD.L #8,D3                         ; @BT $06 $83 $00 $00 $00 $08
  SUB.W #1,D0                         ; @BT $04 $40 $00 $01
  SUB.L #8,D3                         ; @BT $04 $83 $00 $00 $00 $08
  MOVE.L #1,D0                        ; @BT $20 $3C $00 $00 $00 $01
  MOVE.L #-1,D7                       ; @BT $2E $3C $FF $FF $FF $FF

  .db "<02" ; @BT END

.db "03>" ; @BT TEST-03 03 START

TEST3_ALL:
  SUBX.B D1,D0                        ; @BT $91 $01
  MOVE.L D1,(($20000+$3-$3)).L             ; @BT $23 $C1 $00 $02 $00 $00
  ROL.W (A2)                          ; @BT $E7 $D2
  SUBI.W #($100+$3-$3),D1                    ; @BT $04 $41 $01 $00
  EXG A0,A1                           ; @BT $C1 $49
  NOT.B D0                            ; @BT $46 $00
  SEQ D7                              ; @BT $57 $C7
  MOVE.W (($10+$3-$3),A3),D3                ; @BT $36 $2B $00 $10
  NEGX.W D1                           ; @BT $40 $41
  ANDI.W #($FF00+$3-$3),SR                    ; @BT $02 $7C $FF $00
  MOVEA.L D1,A1                       ; @BT $22 $41
  SWAP D7                             ; @BT $48 $47
  MOVE.L (($10000+$3-$3)).L,D5             ; @BT $2A $39 $00 $01 $00 $00
  RTR                                 ; @BT $4E $77
  ADD.W D2,D3                         ; @BT $D6 $42
  NOT.L D2                            ; @BT $46 $82
; DBT D0: cccc=0001, D0=000 => $50C8, disp=8
  DBT D0,T3_24_1                        ; @BT $50 $C8 $00 $08
  NOP                                 ; @BT $4E $71
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
T3_24_1:
; DBF D1: cccc=0000, D1=001 => $51C9, disp=10
  DBF D1,T3_24_2                        ; @BT $51 $C9 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
  NOP                                 ; @BT $4E $71
T3_24_2:
; DBRA D2: alias DBF, D2=010 => $51CA, disp=10
  DBRA D2,T3_24_3                       ; @BT $51 $CA $00 $0A
  TST.B D0                            ; @BT $4A $00
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
T3_24_3:
; DBHI D3: cccc=0010, D3=011 => $52CB, disp=10
  DBHI D3,T3_24_4                       ; @BT $52 $CB $00 $0A
  SWAP D0                             ; @BT $48 $40
  LEA (($1000+$3-$3)).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T3_24_4:
; DBLS D4: cccc=0011, D4=100 => $53CC, disp=8
  DBLS D4,T3_24_5                       ; @BT $53 $CC $00 $08
  NOT.B D0                            ; @BT $46 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T3_24_5:
; DBCC D5: cccc=0100, D5=101 => $54CD, disp=10
  DBCC D5,T3_24_6                       ; @BT $54 $CD $00 $0A
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
  NOP                                 ; @BT $4E $71
T3_24_6:
; DBCS D6: cccc=0101, D6=110 => $55CE, disp=8
  DBCS D6,T3_24_7                       ; @BT $55 $CE $00 $08
  EXT.W D0                            ; @BT $48 $80
  MOVE.W (($10+$3-$3),A3),D3                ; @BT $36 $2B $00 $10
T3_24_7:
; DBNE D7: cccc=0110, D7=111 => $56CF, disp=8
  DBNE D7,T3_24_8                       ; @BT $56 $CF $00 $08
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
T3_24_8:
; DBEQ D0: cccc=0111, D0=000 => $57C8, disp=12
  DBEQ D0,T3_24_9                       ; @BT $57 $C8 $00 $0C
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
  NOP                                 ; @BT $4E $71
T3_24_9:
; DBVC D1: cccc=1000, D1=001 => $58C9, disp=12
  DBVC D1,T3_24_10                      ; @BT $58 $C9 $00 $0C
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
T3_24_10:
; DBVS D2: cccc=1001, D2=010 => $59CA, disp=8
  DBVS D2,T3_24_11                      ; @BT $59 $CA $00 $08
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (($10+$3-$3),A0),A4                     ; @BT $49 $E8 $00 $10
T3_24_11:
; DBPL D3: cccc=1010, D3=011 => $5ACB, disp=10
  DBPL D3,T3_24_12                      ; @BT $5A $CB $00 $0A
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T3_24_12:
; DBMI D4: cccc=1011, D4=100 => $5BCC, disp=10
  DBMI D4,T3_24_13                      ; @BT $5B $CC $00 $0A
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  MOVE.L (($10000+$3-$3)).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T3_24_13:
; DBGE D5: cccc=1100, D5=101 => $5CCD, disp=10
  DBGE D5,T3_24_14                      ; @BT $5C $CD $00 $0A
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  NOP                                 ; @BT $4E $71
T3_24_14:
; DBLT D6: cccc=1101, D6=110 => $5DCE, disp=12
  DBLT D6,T3_24_15                      ; @BT $5D $CE $00 $0C
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
  MOVE.W (($10+$3-$3),A3),D3                ; @BT $36 $2B $00 $10
T3_24_15:
; DBGT D7: cccc=1110, D7=111 => $5ECF, disp=10
  DBGT D7,T3_24_16                      ; @BT $5E $CF $00 $0A
  NOP                                 ; @BT $4E $71
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
T3_24_16:
; DBLE D0: cccc=1111, D0=000 => $5FC8, disp=10
  DBLE D0,T3_24_17                      ; @BT $5F $C8 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  LEA (($1000+$3-$3)).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T3_24_17:

;==========================================================
; TEST 25: Scc — Set According to Condition
; 0101_cccc_11_000_Dn (byte)
;==========================================================
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  ABCD D1,D0                          ; @BT $C1 $01
  ST D0                               ; @BT $50 $C0
  MOVE.L USP,A1                       ; @BT $4E $69
  SUBQ.B #(5-4),D0                        ; @BT $53 $00
  JSR (A0)                            ; @BT $4E $90
  CMP.W D2,D3                         ; @BT $B6 $42
  ASL.W #(6-4),D1                         ; @BT $E5 $41
  MOVEA.W D0,A0                       ; @BT $30 $40
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  AND.B D1,D0                         ; @BT $C0 $01
  LINK A7,#(0-(69-5))                        ; @BT $4E $57 $FF $C0
  SBCD -(A1),-(A0)                    ; @BT $81 $09
  AND.W #($FF00+$3-$3),D1                     ; @BT $02 $41 $FF $00
  MOVE.W D2,D3                        ; @BT $36 $02
  OR.W D2,D3                          ; @BT $86 $42
  BTST #(7-4),D0                          ; @BT $08 $00 $00 $03
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  MULU.W D2,D3                        ; @BT $C6 $C2
  NEG.B D0                            ; @BT $44 $00
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
  SBCD D1,D0                          ; @BT $81 $01
  RTE                                 ; @BT $4E $73
  SVS D1                              ; @BT $59 $C1
  BCLR D0,D1                          ; @BT $01 $81
  ORI.B #($F+$3-$3),D0                       ; @BT $00 $00 $00 $0F
  MOVEQ #(6/3-2),D0                         ; @BT $70 $00
  MOVEQ #(5-4),D1                         ; @BT $72 $01
  BTST D0,D1                          ; @BT $01 $01
  ROR.W (A2)                          ; @BT $E6 $D2
  LSR.W #(6-4),D1                         ; @BT $E4 $49
  TRAP #(5-4)                             ; @BT $4E $41
  NBCD D0                             ; @BT $48 $00
  ASL.W D1,D0                         ; @BT $E3 $60
  CMPI.B #($10+$3-$3),D0                      ; @BT $0C $00 $00 $10
  CMPM.W (A2)+,(A3)+                  ; @BT $B7 $4A
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVEQ #(0-(6-5)),D2                        ; @BT $74 $FF
  RTS                                 ; @BT $4E $75
  ASL.W (A0)                          ; @BT $E1 $D0
  BSET D0,D1                          ; @BT $01 $C1
  SPL D2                              ; @BT $5A $C2
  MOVE.B D0,D1                        ; @BT $12 $00
  TST.W (A0)                          ; @BT $4A $50
  BSET #(7-4),D0                          ; @BT $08 $C0 $00 $03
  SUB.W D2,D3                         ; @BT $96 $42
  MOVEM.L D0/D1/A0/A1,-(A7)          ; @BT $48 $E7 $C0 $C0
  NEG.W D1                            ; @BT $44 $41
  SGE D4                              ; @BT $5C $C4
  CMP.B #($10+$3-$3),D0                       ; @BT $0C $00 $00 $10
  ASR.W (A0)                          ; @BT $E0 $D0
  ABCD -(A1),-(A0)                    ; @BT $C1 $09
  SHI D2                              ; @BT $52 $C2
  TST.L D2                            ; @BT $4A $82
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
  ADD.B D1,D0                         ; @BT $D0 $01
  ADD.B #($10+$3-$3),D0                       ; @BT $06 $00 $00 $10
  ADDX.L D5,D4                        ; @BT $D9 $85
  MOVE.W D1,-(A7)                     ; @BT $3F $01
  ADDQ.B #(5-4),D0                        ; @BT $52 $00
  CMP.L #($10000+$3-$3),D2                    ; @BT $0C $82 $00 $01 $00 $00
  EORI.B #($AA+$3-$3),D0                      ; @BT $0A $00 $00 $AA
  CMP.L D4,D5                         ; @BT $BA $84
  ADDQ.L #(12-4),D7                        ; @BT $50 $87
  MOVE.W D3,(($20+$3-$3),A3)                ; @BT $37 $43 $00 $20
  EORI.B #($1-$1),CCR                     ; @BT $0A $3C $00 $00
  TAS (A0)                            ; @BT $4A $D0
  PEA (A0)                            ; @BT $48 $50
  AND.W D2,D3                         ; @BT $C6 $42
  AND.B #($F+$3-$3),D0                       ; @BT $02 $00 $00 $0F
  MOVEM.L (A7)+,D0/D1/A0/A1          ; @BT $4C $DF $03 $03
  ANDI.B #($1-$1),CCR                     ; @BT $02 $3C $00 $00
  CMP.B D1,D0                         ; @BT $B0 $01
  SUBA.L D1,A1                        ; @BT $93 $C1
  TRAP #(19-4)                            ; @BT $4E $4F
  MOVEA.L A3,A4                       ; @BT $28 $4B
  MOVE.B D0,(A0)                      ; @BT $10 $80
  PEA (($1000+$3-$3)).L                   ; @BT $48 $79 $00 $00 $10 $00
  ADDQ.W #(6-4),A0                        ; @BT $54 $48
  DIVU.W D2,D3                        ; @BT $86 $C2
  MOVEQ #(131-4),D7                       ; @BT $7E $7F
  MOVE.L D4,D5                        ; @BT $2A $04
  CHK (($1000+$3-$3)).W,D2                    ; @BT $45 $B8 $10 $00
  SNE D6                              ; @BT $56 $C6
  MOVE.L (($1000+$3-$3)).W,D4                 ; @BT $28 $38 $10 $00
  SLS D3                              ; @BT $53 $C3
  MOVE.L A0,USP                       ; @BT $4E $60
  TRAP #(6/3-2)                             ; @BT $4E $40
  CMPM.B (A0)+,(A1)+                  ; @BT $B3 $08
  MOVEP.L D1,(($20+$3-$3),A1)                 ; @BT $03 $C9 $00 $20
  NEGX.B D0                           ; @BT $40 $00
  MOVE D0,CCR                         ; @BT $44 $C0
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVEP.L (($20+$3-$3),A1),D1                 ; @BT $03 $49 $00 $20
  MOVE SR,D2                          ; @BT $40 $C2
  MOVE.W ((8-4),A0,D1),D2                 ; @BT $34 $30 $10 $04
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (($10+$3-$3),A0),A4                     ; @BT $49 $E8 $00 $10
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  ROL.W #(6-4),D1                         ; @BT $E5 $59
  ADD.L #($10000+$3-$3),D2                    ; @BT $06 $82 $00 $01 $00 $00
; BRA.B to next+10 bytes: disp=10 => $600A
  BRA.B T3_23_1                         ; @BT $60 $0A
  NOP                                 ; @BT $4E $71
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
T3_23_1:
; BEQ.B to next+8 bytes: disp=8 => $6708
  BEQ.B T3_23_2                         ; @BT $67 $08
  CLR.B D0                            ; @BT $42 $00
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
T3_23_2:
; BNE.B to next+10 bytes: disp=10 => $660A
  BNE.B T3_23_3                         ; @BT $66 $0A
  TST.B D0                            ; @BT $4A $00
  LEA (($1000+$3-$3)).W,A2                    ; @BT $45 $F8 $10 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T3_23_3:
; BCC.B to next+6 bytes: disp=6 => $6406
  BCC.B T3_23_4                         ; @BT $64 $06
  SWAP D0                             ; @BT $48 $40
  AND.B #($F+$3-$3),D0                       ; @BT $02 $00 $00 $0F
T3_23_4:
; BCS.B to next+10 bytes: disp=10 => $650A
  BCS.B T3_23_5                         ; @BT $65 $0A
  NOT.B D0                            ; @BT $46 $00
  OR.W #($FF00+$3-$3),D1                      ; @BT $00 $41 $FF $00
  MOVE.W (($10+$3-$3),A3),D3                ; @BT $36 $2B $00 $10
T3_23_5:
; BVC.B to next+6 bytes: disp=6 => $6806
  BVC.B T3_23_6                         ; @BT $68 $06
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
T3_23_6:
; BVS.B to next+6 bytes: disp=6 => $6906
  BVS.B T3_23_7                         ; @BT $69 $06
  EXT.W D0                            ; @BT $48 $80
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
T3_23_7:
; BPL.B to next+8 bytes: disp=8 => $6A08
  BPL.B T3_23_8                         ; @BT $6A $08
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T3_23_8:
; BMI.B to next+6 bytes: disp=6 => $6B06
  BMI.B T3_23_9                         ; @BT $6B $06
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
T3_23_9:
; BGE.B to next+8 bytes: disp=8 => $6C08
  BGE.B T3_23_10                        ; @BT $6C $08
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
T3_23_10:
; BLT.B to next+8 bytes: disp=8 => $6D08
  BLT.B T3_23_11                        ; @BT $6D $08
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (($10+$3-$3),A0),A4                     ; @BT $49 $E8 $00 $10
  NOP                                 ; @BT $4E $71
T3_23_11:
; BGT.B to next+6 bytes: disp=6 => $6E06
  BGT.B T3_23_12                        ; @BT $6E $06
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
T3_23_12:
; BLE.B to next+6 bytes: disp=6 => $6F06
  BLE.B T3_23_13                        ; @BT $6F $06
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
T3_23_13:
; BHI.B to next+8 bytes: disp=8 => $6208
  BHI.B T3_23_14                        ; @BT $62 $08
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  MOVE.L (($10000+$3-$3)).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T3_23_14:
; BLS.B to next+8 bytes: disp=8 => $6308
  BLS.B T3_23_15                        ; @BT $63 $08
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVE.W (($10+$3-$3),A3),D3                ; @BT $36 $2B $00 $10
  NOP                                 ; @BT $4E $71
T3_23_15:
; BRA.W to next+12 bytes: disp=14 => $6000000E
  BRA.W T3_23_16                        ; @BT $60 $00 $00 $0E
  NOP                                 ; @BT $4E $71
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
T3_23_16:
; BSR.B to next+8 bytes: disp=8 => $6108
  BSR.B T3_23_17                        ; @BT $61 $08
  CLR.B D0                            ; @BT $42 $00
  LEA (($1000+$3-$3)).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T3_23_17:
; BSR.W to next+10 bytes: disp=12 => $6100000C
  BSR.W T3_23_18                        ; @BT $61 $00 $00 $0C
  TST.B D0                            ; @BT $4A $00
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #($1234+$3-$3),D1                    ; @BT $32 $3C $12 $34
T3_23_18:

;==========================================================
; TEST 24: DBcc
; 0101_cccc_11001_Dn + disp word
; Each DBcc targets a label after 0-10 random instructions (prefer complex).
;==========================================================
  ADDX.B D1,D0                        ; @BT $D1 $01
  SUBQ.W #(6-4),A0                        ; @BT $55 $48
  SUB.L #($10000+$3-$3),D2                    ; @BT $04 $82 $00 $01 $00 $00
  MOVE.L D0,-(A7)                     ; @BT $2F $00
  ADD.L D4,D5                         ; @BT $DA $84
  ADDA.W D0,A0                        ; @BT $D0 $C0
  MOVE D1,SR                          ; @BT $46 $C1
  ROXL.W (A3)                         ; @BT $E5 $D3
  EOR.B D0,D1                         ; @BT $B1 $01
  EORI.W #($1-$1),SR                    ; @BT $0A $7C $00 $00
  SWAP D0                             ; @BT $48 $40
  ASR.W D1,D0                         ; @BT $E2 $60
  MOVE.W (A1)+,D1                     ; @BT $32 $19
  BCHG #(7-4),D0                          ; @BT $08 $40 $00 $03
  MOVE.B #($42+$3-$3),D0                      ; @BT $10 $3C $00 $42
  MULS.W D1,D0                        ; @BT $C1 $C1
  JSR (($1000+$3-$3)).L                   ; @BT $4E $B9 $00 $00 $10 $00
  TRAPV                               ; @BT $4E $76
  CMP.W #($1000+$3-$3),D1                     ; @BT $0C $41 $10 $00
  EOR.B #($AA+$3-$3),D0                       ; @BT $0A $00 $00 $AA
  ADDX.W D3,D2                        ; @BT $D5 $43
  OR.B #($F+$3-$3),D0                        ; @BT $00 $00 $00 $0F
  ANDI.B #($F+$3-$3),D0                      ; @BT $02 $00 $00 $0F
  LEA (($1000+$3-$3)).W,A2                    ; @BT $45 $F8 $10 $00
  EXT.W D0                            ; @BT $48 $80
  LSR.W (A1)                          ; @BT $E2 $D1
  SUBQ.L #(12-4),D7                        ; @BT $51 $87
  CMPA.W D0,A0                        ; @BT $B0 $C0
  NEG.L D2                            ; @BT $44 $82
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  MOVE.W ((6/3-2),PC),D0                    ; @BT $30 $3A $F4 $CD
  SLT D5                              ; @BT $5D $C5
  SLE D7                              ; @BT $5F $C7
  JMP (($1000+$3-$3)).W                       ; @BT $4E $F8 $10 $00
  TST.W D1                            ; @BT $4A $41
  PEA (($1000+$3-$3)).W                       ; @BT $48 $78 $10 $00
  SGT D6                              ; @BT $5E $C6
  EOR.W D2,D3                         ; @BT $B5 $43
  ASR.W #(6-4),D1                         ; @BT $E4 $41
  CLR.L D2                            ; @BT $42 $82
  MOVE.L (A7)+,D0                     ; @BT $20 $1F
  STOP #($2700+$3-$3)                         ; @BT $4E $72 $27 $00
  SMI D3                              ; @BT $5B $C3
  ROR.W #(6-4),D1                         ; @BT $E4 $59
  OR.B D1,D0                          ; @BT $80 $01
  MOVE.W D1,(A1)+                     ; @BT $32 $C1
  ADDX.W -(A1),-(A0)                  ; @BT $D1 $49
  EOR.L #($AAAAAAAA+$3-$3),D2                 ; @BT $0A $82 $AA $AA $AA $AA
  BCLR #(7-4),D0                          ; @BT $08 $80 $00 $03
  EXT.L D1                            ; @BT $48 $C1
  LSL.W (A1)                          ; @BT $E3 $D1
  SUBX.W -(A1),-(A0)                  ; @BT $91 $49
  BCHG D0,D1                          ; @BT $01 $41
  NOP                                 ; @BT $4E $71
  ADDQ.W #(8-4),D1                        ; @BT $58 $41
  MOVE.L #($12345678+$3-$3),D2                ; @BT $24 $3C $12 $34 $56 $78
  JSR (($1000+$3-$3)).W                       ; @BT $4E $B8 $10 $00
  LSL.W #(6-4),D1                         ; @BT $E5 $49
  SCC D4                              ; @BT $54 $C4
  SUB.W #($100+$3-$3),D1                      ; @BT $04 $41 $01 $00
  ADDI.B #($10+$3-$3),D0                      ; @BT $06 $00 $00 $10
  NEGX.L D2                           ; @BT $40 $82
  SUB.L D4,D5                         ; @BT $9A $84
  SVC D0                              ; @BT $58 $C0
  SF D1                               ; @BT $51 $C1
  NOT.W D1                            ; @BT $46 $41
  TAS D0                              ; @BT $4A $C0
  CMPM.L (A2)+,(A3)+                  ; @BT $B7 $8A
  JMP (($1000+$3-$3)).L                   ; @BT $4E $F9 $00 $00 $10 $00
  SUB.B D1,D0                         ; @BT $90 $01
  ADDA.L D1,A1                        ; @BT $D3 $C1
  ILLEGAL                             ; @BT $4A $FC
  DIVS.W D1,D0                        ; @BT $81 $C1
  ROXR.W (A3)                         ; @BT $E4 $D3
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
  ORI.W #($1-$1),SR                     ; @BT $00 $7C $00 $00
  UNLK A6                             ; @BT $4E $5E
  LINK A6,#(0-(13-5))                         ; @BT $4E $56 $FF $F8
  ADD.W #($1000+$3-$3),D1                     ; @BT $06 $41 $10 $00
  OR.W #($FF00+$3-$3),D1                      ; @BT $00 $41 $FF $00
  SCS D5                              ; @BT $55 $C5
  CLR.B D0                            ; @BT $42 $00
  AND.L D4,D5                         ; @BT $CA $84
  OR.L D4,D5                          ; @BT $8A $84
  MOVEA.W #($1000+$3-$3),A2                   ; @BT $34 $7C $10 $00
  ASL.L #(8-4),D2                         ; @BT $E9 $82
  SUBA.W D0,A0                        ; @BT $90 $C0
  MOVEP.W D0,(($10+$3-$3),A0)                 ; @BT $01 $88 $00 $10
  EXG D0,A0                           ; @BT $C1 $88
  AND.L #($FFFF0000+$3-$3),D2                 ; @BT $02 $82 $FF $FF $00 $00
  EOR.L D4,D5                         ; @BT $B9 $85
  TST.B D0                            ; @BT $4A $00
  CMPA.L D1,A1                        ; @BT $B3 $C1
  EXG D0,D1                           ; @BT $C1 $41
  MOVE.W (A7)+,D1                     ; @BT $32 $1F
  MOVE.L D2,-(A2)                     ; @BT $25 $02
  EOR.W #($AAAA+$3-$3),D1                     ; @BT $0A $41 $AA $AA
  MOVEP.W (($10+$3-$3),A0),D0                 ; @BT $01 $08 $00 $10
  MOVE.L -(A2),D2                     ; @BT $24 $22
  CHK D1,D0                           ; @BT $41 $81
  ORI.B #($1-$1),CCR                      ; @BT $00 $3C $00 $00
  CLR.W D1                            ; @BT $42 $41
  LEA (($1000+$3-$3)).L,A3                ; @BT $47 $F9 $00 $00 $10 $00
  ADDA.W #($100+$3-$3),A2                     ; @BT $D4 $FC $01 $00
  MOVE.B (A0),D0                      ; @BT $10 $10
  ASR.L #(5-4),D3                         ; @BT $E2 $83
  OR.L #($FFFF0000+$3-$3),D2                  ; @BT $00 $82 $FF $FF $00 $00
  UNLK A7                             ; @BT $4E $5F
  RESET                               ; @BT $4E $70
  LEA (A0),A1                         ; @BT $43 $D0
  SUB.B #($10+$3-$3),D0                       ; @BT $04 $00 $00 $10
  SUBQ.W #(8-4),D1                        ; @BT $59 $41
  JMP (A0)                            ; @BT $4E $D0
  ASR.B #(5-4),D0                         ; @BT $E2 $00

; potential quick-form optimization candidates (must stay non-quick)
  ADD.W #(5-4),D0                         ; @BT $06 $40 $00 $01
  ADD.L #(12-4),D3                         ; @BT $06 $83 $00 $00 $00 $08
  SUB.W #(5-4),D0                         ; @BT $04 $40 $00 $01
  SUB.L #(12-4),D3                         ; @BT $04 $83 $00 $00 $00 $08
  MOVE.L #(5-4),D0                        ; @BT $20 $3C $00 $00 $00 $01
  MOVE.L #(0-(6-5)),D7                       ; @BT $2E $3C $FF $FF $FF $FF

  .db "<03" ; @BT END

; TEST 04 define-based immediates (with chained definitions)
.define T4D_ZERO (6/3-2)
.define T4D_ONE (T4D_ZERO+1)
.define T4D_TWO (T4D_ONE+1)
.define T4D_FOUR (T4D_TWO*2)
.define T4D_EIGHT (T4D_FOUR*2)
.define T4D_0010 ($8+$8)
.define T4D_0020 (T4D_0010*2)
.define T4D_0100 (T4D_0010*T4D_0010)
.define T4D_1000 (T4D_0100*T4D_0010)
.define T4D_10000 (T4D_1000*T4D_0010)
.define T4D_20000 (T4D_10000*2)
.define T4D_1234 ($1200+$34)
.define T4D_12345678 ($12340000+$5678)
.define T4D_FF00 ($FF*256)
.define T4D_FFFF0000 ($FFFF0000+T4D_ZERO)
.define T4D_000F (T4D_EIGHT+T4D_FOUR+T4D_TWO+T4D_ONE)
.define T4D_00AA ($55*2)
.define T4D_AAAA ($5555*2)
.define T4D_AAAAAAAA ($AAAAAAAA+T4D_ZERO)
.define T4D_2700 ($2600+$100)
.define T4D_NEG1 (0-T4D_ONE)
.define T4D_127 (T4D_0100/2-T4D_ONE)
.define T4D_15 (T4D_EIGHT+T4D_FOUR+T4D_TWO+T4D_ONE)

.db "04>" ; @BT TEST-04 04 START

TEST4_ALL:
  SUBX.B D1,D0                        ; @BT $91 $01
  MOVE.L D1,(T4D_20000).L             ; @BT $23 $C1 $00 $02 $00 $00
  ROL.W (A2)                          ; @BT $E7 $D2
  SUBI.W #T4D_0100,D1                    ; @BT $04 $41 $01 $00
  EXG A0,A1                           ; @BT $C1 $49
  NOT.B D0                            ; @BT $46 $00
  SEQ D7                              ; @BT $57 $C7
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
  NEGX.W D1                           ; @BT $40 $41
  ANDI.W #T4D_FF00,SR                    ; @BT $02 $7C $FF $00
  MOVEA.L D1,A1                       ; @BT $22 $41
  SWAP D7                             ; @BT $48 $47
  MOVE.L (T4D_10000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
  RTR                                 ; @BT $4E $77
  ADD.W D2,D3                         ; @BT $D6 $42
  NOT.L D2                            ; @BT $46 $82
; DBT D0: cccc=0001, D0=000 => $50C8, disp=8
  DBT D0,T4_24_1                        ; @BT $50 $C8 $00 $08
  NOP                                 ; @BT $4E $71
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
T4_24_1:
; DBF D1: cccc=0000, D1=001 => $51C9, disp=10
  DBF D1,T4_24_2                        ; @BT $51 $C9 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
  NOP                                 ; @BT $4E $71
T4_24_2:
; DBRA D2: alias DBF, D2=010 => $51CA, disp=10
  DBRA D2,T4_24_3                       ; @BT $51 $CA $00 $0A
  TST.B D0                            ; @BT $4A $00
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T4_24_3:
; DBHI D3: cccc=0010, D3=011 => $52CB, disp=10
  DBHI D3,T4_24_4                       ; @BT $52 $CB $00 $0A
  SWAP D0                             ; @BT $48 $40
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T4_24_4:
; DBLS D4: cccc=0011, D4=100 => $53CC, disp=8
  DBLS D4,T4_24_5                       ; @BT $53 $CC $00 $08
  NOT.B D0                            ; @BT $46 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T4_24_5:
; DBCC D5: cccc=0100, D5=101 => $54CD, disp=10
  DBCC D5,T4_24_6                       ; @BT $54 $CD $00 $0A
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
  NOP                                 ; @BT $4E $71
T4_24_6:
; DBCS D6: cccc=0101, D6=110 => $55CE, disp=8
  DBCS D6,T4_24_7                       ; @BT $55 $CE $00 $08
  EXT.W D0                            ; @BT $48 $80
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
T4_24_7:
; DBNE D7: cccc=0110, D7=111 => $56CF, disp=8
  DBNE D7,T4_24_8                       ; @BT $56 $CF $00 $08
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T4_24_8:
; DBEQ D0: cccc=0111, D0=000 => $57C8, disp=12
  DBEQ D0,T4_24_9                       ; @BT $57 $C8 $00 $0C
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  NOP                                 ; @BT $4E $71
T4_24_9:
; DBVC D1: cccc=1000, D1=001 => $58C9, disp=12
  DBVC D1,T4_24_10                      ; @BT $58 $C9 $00 $0C
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
T4_24_10:
; DBVS D2: cccc=1001, D2=010 => $59CA, disp=8
  DBVS D2,T4_24_11                      ; @BT $59 $CA $00 $08
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (T4D_0010,A0),A4                     ; @BT $49 $E8 $00 $10
T4_24_11:
; DBPL D3: cccc=1010, D3=011 => $5ACB, disp=10
  DBPL D3,T4_24_12                      ; @BT $5A $CB $00 $0A
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T4_24_12:
; DBMI D4: cccc=1011, D4=100 => $5BCC, disp=10
  DBMI D4,T4_24_13                      ; @BT $5B $CC $00 $0A
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  MOVE.L (T4D_10000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T4_24_13:
; DBGE D5: cccc=1100, D5=101 => $5CCD, disp=10
  DBGE D5,T4_24_14                      ; @BT $5C $CD $00 $0A
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  NOP                                 ; @BT $4E $71
T4_24_14:
; DBLT D6: cccc=1101, D6=110 => $5DCE, disp=12
  DBLT D6,T4_24_15                      ; @BT $5D $CE $00 $0C
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
T4_24_15:
; DBGT D7: cccc=1110, D7=111 => $5ECF, disp=10
  DBGT D7,T4_24_16                      ; @BT $5E $CF $00 $0A
  NOP                                 ; @BT $4E $71
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T4_24_16:
; DBLE D0: cccc=1111, D0=000 => $5FC8, disp=10
  DBLE D0,T4_24_17                      ; @BT $5F $C8 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T4_24_17:

;==========================================================
; TEST 25: Scc — Set According to Condition
; 0101_cccc_11_000_Dn (byte)
;==========================================================
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  ABCD D1,D0                          ; @BT $C1 $01
  ST D0                               ; @BT $50 $C0
  MOVE.L USP,A1                       ; @BT $4E $69
  SUBQ.B #(5-4),D0                        ; @BT $53 $00
  JSR (A0)                            ; @BT $4E $90
  CMP.W D2,D3                         ; @BT $B6 $42
  ASL.W #(6-4),D1                         ; @BT $E5 $41
  MOVEA.W D0,A0                       ; @BT $30 $40
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  AND.B D1,D0                         ; @BT $C0 $01
  LINK A7,#(0-(69-5))                        ; @BT $4E $57 $FF $C0
  SBCD -(A1),-(A0)                    ; @BT $81 $09
  AND.W #T4D_FF00,D1                     ; @BT $02 $41 $FF $00
  MOVE.W D2,D3                        ; @BT $36 $02
  OR.W D2,D3                          ; @BT $86 $42
  BTST #(7-4),D0                          ; @BT $08 $00 $00 $03
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  MULU.W D2,D3                        ; @BT $C6 $C2
  NEG.B D0                            ; @BT $44 $00
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
  SBCD D1,D0                          ; @BT $81 $01
  RTE                                 ; @BT $4E $73
  SVS D1                              ; @BT $59 $C1
  BCLR D0,D1                          ; @BT $01 $81
  ORI.B #T4D_000F,D0                       ; @BT $00 $00 $00 $0F
  MOVEQ #(6/3-2),D0                         ; @BT $70 $00
  MOVEQ #(5-4),D1                         ; @BT $72 $01
  BTST D0,D1                          ; @BT $01 $01
  ROR.W (A2)                          ; @BT $E6 $D2
  LSR.W #(6-4),D1                         ; @BT $E4 $49
  TRAP #(5-4)                             ; @BT $4E $41
  NBCD D0                             ; @BT $48 $00
  ASL.W D1,D0                         ; @BT $E3 $60
  CMPI.B #T4D_0010,D0                      ; @BT $0C $00 $00 $10
  CMPM.W (A2)+,(A3)+                  ; @BT $B7 $4A
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVEQ #T4D_NEG1,D2                        ; @BT $74 $FF
  RTS                                 ; @BT $4E $75
  ASL.W (A0)                          ; @BT $E1 $D0
  BSET D0,D1                          ; @BT $01 $C1
  SPL D2                              ; @BT $5A $C2
  MOVE.B D0,D1                        ; @BT $12 $00
  TST.W (A0)                          ; @BT $4A $50
  BSET #(7-4),D0                          ; @BT $08 $C0 $00 $03
  SUB.W D2,D3                         ; @BT $96 $42
  MOVEM.L D0/D1/A0/A1,-(A7)          ; @BT $48 $E7 $C0 $C0
  NEG.W D1                            ; @BT $44 $41
  SGE D4                              ; @BT $5C $C4
  CMP.B #T4D_0010,D0                       ; @BT $0C $00 $00 $10
  ASR.W (A0)                          ; @BT $E0 $D0
  ABCD -(A1),-(A0)                    ; @BT $C1 $09
  SHI D2                              ; @BT $52 $C2
  TST.L D2                            ; @BT $4A $82
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
  ADD.B D1,D0                         ; @BT $D0 $01
  ADD.B #T4D_0010,D0                       ; @BT $06 $00 $00 $10
  ADDX.L D5,D4                        ; @BT $D9 $85
  MOVE.W D1,-(A7)                     ; @BT $3F $01
  ADDQ.B #(5-4),D0                        ; @BT $52 $00
  CMP.L #T4D_10000,D2                    ; @BT $0C $82 $00 $01 $00 $00
  EORI.B #T4D_00AA,D0                      ; @BT $0A $00 $00 $AA
  CMP.L D4,D5                         ; @BT $BA $84
  ADDQ.L #(12-4),D7                        ; @BT $50 $87
  MOVE.W D3,(T4D_0020,A3)                ; @BT $37 $43 $00 $20
  EORI.B #T4D_ZERO,CCR                     ; @BT $0A $3C $00 $00
  TAS (A0)                            ; @BT $4A $D0
  PEA (A0)                            ; @BT $48 $50
  AND.W D2,D3                         ; @BT $C6 $42
  AND.B #T4D_000F,D0                       ; @BT $02 $00 $00 $0F
  MOVEM.L (A7)+,D0/D1/A0/A1          ; @BT $4C $DF $03 $03
  ANDI.B #T4D_ZERO,CCR                     ; @BT $02 $3C $00 $00
  CMP.B D1,D0                         ; @BT $B0 $01
  SUBA.L D1,A1                        ; @BT $93 $C1
  TRAP #T4D_15                            ; @BT $4E $4F
  MOVEA.L A3,A4                       ; @BT $28 $4B
  MOVE.B D0,(A0)                      ; @BT $10 $80
  PEA (T4D_1000).L                   ; @BT $48 $79 $00 $00 $10 $00
  ADDQ.W #(6-4),A0                        ; @BT $54 $48
  DIVU.W D2,D3                        ; @BT $86 $C2
  MOVEQ #T4D_127,D7                       ; @BT $7E $7F
  MOVE.L D4,D5                        ; @BT $2A $04
  CHK (T4D_1000).W,D2                    ; @BT $45 $B8 $10 $00
  SNE D6                              ; @BT $56 $C6
  MOVE.L (T4D_1000).W,D4                 ; @BT $28 $38 $10 $00
  SLS D3                              ; @BT $53 $C3
  MOVE.L A0,USP                       ; @BT $4E $60
  TRAP #(6/3-2)                             ; @BT $4E $40
  CMPM.B (A0)+,(A1)+                  ; @BT $B3 $08
  MOVEP.L D1,(T4D_0020,A1)                 ; @BT $03 $C9 $00 $20
  NEGX.B D0                           ; @BT $40 $00
  MOVE D0,CCR                         ; @BT $44 $C0
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVEP.L (T4D_0020,A1),D1                 ; @BT $03 $49 $00 $20
  MOVE SR,D2                          ; @BT $40 $C2
  MOVE.W ((8-4),A0,D1),D2                 ; @BT $34 $30 $10 $04
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (T4D_0010,A0),A4                     ; @BT $49 $E8 $00 $10
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  ROL.W #(6-4),D1                         ; @BT $E5 $59
  ADD.L #T4D_10000,D2                    ; @BT $06 $82 $00 $01 $00 $00
; BRA.B to next+10 bytes: disp=10 => $600A
  BRA.B T4_23_1                         ; @BT $60 $0A
  NOP                                 ; @BT $4E $71
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T4_23_1:
; BEQ.B to next+8 bytes: disp=8 => $6708
  BEQ.B T4_23_2                         ; @BT $67 $08
  CLR.B D0                            ; @BT $42 $00
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T4_23_2:
; BNE.B to next+10 bytes: disp=10 => $660A
  BNE.B T4_23_3                         ; @BT $66 $0A
  TST.B D0                            ; @BT $4A $00
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T4_23_3:
; BCC.B to next+6 bytes: disp=6 => $6406
  BCC.B T4_23_4                         ; @BT $64 $06
  SWAP D0                             ; @BT $48 $40
  AND.B #T4D_000F,D0                       ; @BT $02 $00 $00 $0F
T4_23_4:
; BCS.B to next+10 bytes: disp=10 => $650A
  BCS.B T4_23_5                         ; @BT $65 $0A
  NOT.B D0                            ; @BT $46 $00
  OR.W #T4D_FF00,D1                      ; @BT $00 $41 $FF $00
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
T4_23_5:
; BVC.B to next+6 bytes: disp=6 => $6806
  BVC.B T4_23_6                         ; @BT $68 $06
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
T4_23_6:
; BVS.B to next+6 bytes: disp=6 => $6906
  BVS.B T4_23_7                         ; @BT $69 $06
  EXT.W D0                            ; @BT $48 $80
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
T4_23_7:
; BPL.B to next+8 bytes: disp=8 => $6A08
  BPL.B T4_23_8                         ; @BT $6A $08
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T4_23_8:
; BMI.B to next+6 bytes: disp=6 => $6B06
  BMI.B T4_23_9                         ; @BT $6B $06
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T4_23_9:
; BGE.B to next+8 bytes: disp=8 => $6C08
  BGE.B T4_23_10                        ; @BT $6C $08
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
T4_23_10:
; BLT.B to next+8 bytes: disp=8 => $6D08
  BLT.B T4_23_11                        ; @BT $6D $08
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (T4D_0010,A0),A4                     ; @BT $49 $E8 $00 $10
  NOP                                 ; @BT $4E $71
T4_23_11:
; BGT.B to next+6 bytes: disp=6 => $6E06
  BGT.B T4_23_12                        ; @BT $6E $06
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
T4_23_12:
; BLE.B to next+6 bytes: disp=6 => $6F06
  BLE.B T4_23_13                        ; @BT $6F $06
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
T4_23_13:
; BHI.B to next+8 bytes: disp=8 => $6208
  BHI.B T4_23_14                        ; @BT $62 $08
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  MOVE.L (T4D_10000).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T4_23_14:
; BLS.B to next+8 bytes: disp=8 => $6308
  BLS.B T4_23_15                        ; @BT $63 $08
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
  NOP                                 ; @BT $4E $71
T4_23_15:
; BRA.W to next+12 bytes: disp=14 => $6000000E
  BRA.W T4_23_16                        ; @BT $60 $00 $00 $0E
  NOP                                 ; @BT $4E $71
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T4_23_16:
; BSR.B to next+8 bytes: disp=8 => $6108
  BSR.B T4_23_17                        ; @BT $61 $08
  CLR.B D0                            ; @BT $42 $00
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T4_23_17:
; BSR.W to next+10 bytes: disp=12 => $6100000C
  BSR.W T4_23_18                        ; @BT $61 $00 $00 $0C
  TST.B D0                            ; @BT $4A $00
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #T4D_1234,D1                    ; @BT $32 $3C $12 $34
T4_23_18:

;==========================================================
; TEST 24: DBcc
; 0101_cccc_11001_Dn + disp word
; Each DBcc targets a label after 0-10 random instructions (prefer complex).
;==========================================================
  ADDX.B D1,D0                        ; @BT $D1 $01
  SUBQ.W #(6-4),A0                        ; @BT $55 $48
  SUB.L #T4D_10000,D2                    ; @BT $04 $82 $00 $01 $00 $00
  MOVE.L D0,-(A7)                     ; @BT $2F $00
  ADD.L D4,D5                         ; @BT $DA $84
  ADDA.W D0,A0                        ; @BT $D0 $C0
  MOVE D1,SR                          ; @BT $46 $C1
  ROXL.W (A3)                         ; @BT $E5 $D3
  EOR.B D0,D1                         ; @BT $B1 $01
  EORI.W #T4D_ZERO,SR                    ; @BT $0A $7C $00 $00
  SWAP D0                             ; @BT $48 $40
  ASR.W D1,D0                         ; @BT $E2 $60
  MOVE.W (A1)+,D1                     ; @BT $32 $19
  BCHG #(7-4),D0                          ; @BT $08 $40 $00 $03
  MOVE.B #($42+$3-$3),D0                      ; @BT $10 $3C $00 $42
  MULS.W D1,D0                        ; @BT $C1 $C1
  JSR (T4D_1000).L                   ; @BT $4E $B9 $00 $00 $10 $00
  TRAPV                               ; @BT $4E $76
  CMP.W #T4D_1000,D1                     ; @BT $0C $41 $10 $00
  EOR.B #T4D_00AA,D0                       ; @BT $0A $00 $00 $AA
  ADDX.W D3,D2                        ; @BT $D5 $43
  OR.B #T4D_000F,D0                        ; @BT $00 $00 $00 $0F
  ANDI.B #T4D_000F,D0                      ; @BT $02 $00 $00 $0F
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  EXT.W D0                            ; @BT $48 $80
  LSR.W (A1)                          ; @BT $E2 $D1
  SUBQ.L #(12-4),D7                        ; @BT $51 $87
  CMPA.W D0,A0                        ; @BT $B0 $C0
  NEG.L D2                            ; @BT $44 $82
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  MOVE.W ((6/3-2),PC),D0                    ; @BT $30 $3A $F0 $B7
  SLT D5                              ; @BT $5D $C5
  SLE D7                              ; @BT $5F $C7
  JMP (T4D_1000).W                       ; @BT $4E $F8 $10 $00
  TST.W D1                            ; @BT $4A $41
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
  SGT D6                              ; @BT $5E $C6
  EOR.W D2,D3                         ; @BT $B5 $43
  ASR.W #(6-4),D1                         ; @BT $E4 $41
  CLR.L D2                            ; @BT $42 $82
  MOVE.L (A7)+,D0                     ; @BT $20 $1F
  STOP #T4D_2700                         ; @BT $4E $72 $27 $00
  SMI D3                              ; @BT $5B $C3
  ROR.W #(6-4),D1                         ; @BT $E4 $59
  OR.B D1,D0                          ; @BT $80 $01
  MOVE.W D1,(A1)+                     ; @BT $32 $C1
  ADDX.W -(A1),-(A0)                  ; @BT $D1 $49
  EOR.L #T4D_AAAAAAAA,D2                 ; @BT $0A $82 $AA $AA $AA $AA
  BCLR #(7-4),D0                          ; @BT $08 $80 $00 $03
  EXT.L D1                            ; @BT $48 $C1
  LSL.W (A1)                          ; @BT $E3 $D1
  SUBX.W -(A1),-(A0)                  ; @BT $91 $49
  BCHG D0,D1                          ; @BT $01 $41
  NOP                                 ; @BT $4E $71
  ADDQ.W #(8-4),D1                        ; @BT $58 $41
  MOVE.L #T4D_12345678,D2                ; @BT $24 $3C $12 $34 $56 $78
  JSR (T4D_1000).W                       ; @BT $4E $B8 $10 $00
  LSL.W #(6-4),D1                         ; @BT $E5 $49
  SCC D4                              ; @BT $54 $C4
  SUB.W #T4D_0100,D1                      ; @BT $04 $41 $01 $00
  ADDI.B #T4D_0010,D0                      ; @BT $06 $00 $00 $10
  NEGX.L D2                           ; @BT $40 $82
  SUB.L D4,D5                         ; @BT $9A $84
  SVC D0                              ; @BT $58 $C0
  SF D1                               ; @BT $51 $C1
  NOT.W D1                            ; @BT $46 $41
  TAS D0                              ; @BT $4A $C0
  CMPM.L (A2)+,(A3)+                  ; @BT $B7 $8A
  JMP (T4D_1000).L                   ; @BT $4E $F9 $00 $00 $10 $00
  SUB.B D1,D0                         ; @BT $90 $01
  ADDA.L D1,A1                        ; @BT $D3 $C1
  ILLEGAL                             ; @BT $4A $FC
  DIVS.W D1,D0                        ; @BT $81 $C1
  ROXR.W (A3)                         ; @BT $E4 $D3
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
  ORI.W #T4D_ZERO,SR                     ; @BT $00 $7C $00 $00
  UNLK A6                             ; @BT $4E $5E
  LINK A6,#(0-(13-5))                         ; @BT $4E $56 $FF $F8
  ADD.W #T4D_1000,D1                     ; @BT $06 $41 $10 $00
  OR.W #T4D_FF00,D1                      ; @BT $00 $41 $FF $00
  SCS D5                              ; @BT $55 $C5
  CLR.B D0                            ; @BT $42 $00
  AND.L D4,D5                         ; @BT $CA $84
  OR.L D4,D5                          ; @BT $8A $84
  MOVEA.W #T4D_1000,A2                   ; @BT $34 $7C $10 $00
  ASL.L #(8-4),D2                         ; @BT $E9 $82
  SUBA.W D0,A0                        ; @BT $90 $C0
  MOVEP.W D0,(T4D_0010,A0)                 ; @BT $01 $88 $00 $10
  EXG D0,A0                           ; @BT $C1 $88
  AND.L #T4D_FFFF0000,D2                 ; @BT $02 $82 $FF $FF $00 $00
  EOR.L D4,D5                         ; @BT $B9 $85
  TST.B D0                            ; @BT $4A $00
  CMPA.L D1,A1                        ; @BT $B3 $C1
  EXG D0,D1                           ; @BT $C1 $41
  MOVE.W (A7)+,D1                     ; @BT $32 $1F
  MOVE.L D2,-(A2)                     ; @BT $25 $02
  EOR.W #T4D_AAAA,D1                     ; @BT $0A $41 $AA $AA
  MOVEP.W (T4D_0010,A0),D0                 ; @BT $01 $08 $00 $10
  MOVE.L -(A2),D2                     ; @BT $24 $22
  CHK D1,D0                           ; @BT $41 $81
  ORI.B #T4D_ZERO,CCR                      ; @BT $00 $3C $00 $00
  CLR.W D1                            ; @BT $42 $41
  LEA (T4D_1000).L,A3                ; @BT $47 $F9 $00 $00 $10 $00
  ADDA.W #T4D_0100,A2                     ; @BT $D4 $FC $01 $00
  MOVE.B (A0),D0                      ; @BT $10 $10
  ASR.L #(5-4),D3                         ; @BT $E2 $83
  OR.L #T4D_FFFF0000,D2                  ; @BT $00 $82 $FF $FF $00 $00
  UNLK A7                             ; @BT $4E $5F
  RESET                               ; @BT $4E $70
  LEA (A0),A1                         ; @BT $43 $D0
  SUB.B #T4D_0010,D0                       ; @BT $04 $00 $00 $10
  SUBQ.W #(8-4),D1                        ; @BT $59 $41
  JMP (A0)                            ; @BT $4E $D0
  ASR.B #(5-4),D0                         ; @BT $E2 $00

; potential quick-form optimization candidates (must stay non-quick)
  ADD.W #(5-4),D0                         ; @BT $06 $40 $00 $01
  ADD.L #(12-4),D3                         ; @BT $06 $83 $00 $00 $00 $08
  SUB.W #(5-4),D0                         ; @BT $04 $40 $00 $01
  SUB.L #(12-4),D3                         ; @BT $04 $83 $00 $00 $00 $08
  MOVE.L #(5-4),D0                        ; @BT $20 $3C $00 $00 $00 $01
  MOVE.L #T4D_NEG1,D7                       ; @BT $2E $3C $FF $FF $FF $FF

  .db "<04" ; @BT END

; TEST 05 function-based immediates (cross-references to T4D defines and F5 functions)
.function F5_ZERO() T4D_ONE-T4D_ONE
.function F5_0010() T4D_FOUR*T4D_FOUR
.function F5_0020(x) x+x
.function F5_0100() F5_0010()*F5_0010()
.function F5_1000(a,b) a*b
.function F5_1234(base,offset) base+offset
.function F5_10000(a,b) a*b
.function F5_20000(x) x*T4D_TWO
.function F5_12345678() F5_1234(F5_1000(F5_0100(),F5_0010()),$234)*$10000+$5678
.function F5_FF00(base,n) (base-n)*base
.function F5_AAAAAAAA() ($AAAAAAAA+F5_ZERO())
.function F5_00AA() T4D_000F*$B+T4D_FOUR+T4D_ONE
.function F5_AAAA() F5_00AA()*F5_0100()+F5_00AA()
.function F5_2700(base,mult) base*mult
.function F5_NEG1(x) F5_ZERO()-x
.function F5_127() F5_0100()/T4D_TWO-T4D_ONE

.db "05>" ; @BT TEST-05 05 START

TEST5_ALL:
  SUBX.B D1,D0                        ; @BT $91 $01
  MOVE.L D1,(F5_20000(T4D_10000)).L             ; @BT $23 $C1 $00 $02 $00 $00
  ROL.W (A2)                          ; @BT $E7 $D2
  SUBI.W #T4D_0100,D1                    ; @BT $04 $41 $01 $00
  EXG A0,A1                           ; @BT $C1 $49
  NOT.B D0                            ; @BT $46 $00
  SEQ D7                              ; @BT $57 $C7
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
  NEGX.W D1                           ; @BT $40 $41
  ANDI.W #F5_FF00(F5_0100(),T4D_ONE),SR                    ; @BT $02 $7C $FF $00
  MOVEA.L D1,A1                       ; @BT $22 $41
  SWAP D7                             ; @BT $48 $47
  MOVE.L (F5_10000(T4D_1000,T4D_0010)).L,D5             ; @BT $2A $39 $00 $01 $00 $00
  RTR                                 ; @BT $4E $77
  ADD.W D2,D3                         ; @BT $D6 $42
  NOT.L D2                            ; @BT $46 $82
; DBT D0: cccc=0001, D0=000 => $50C8, disp=8
  DBT D0,T5_24_1                        ; @BT $50 $C8 $00 $08
  NOP                                 ; @BT $4E $71
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
T5_24_1:
; DBF D1: cccc=0000, D1=001 => $51C9, disp=10
  DBF D1,T5_24_2                        ; @BT $51 $C9 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
  NOP                                 ; @BT $4E $71
T5_24_2:
; DBRA D2: alias DBF, D2=010 => $51CA, disp=10
  DBRA D2,T5_24_3                       ; @BT $51 $CA $00 $0A
  TST.B D0                            ; @BT $4A $00
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
T5_24_3:
; DBHI D3: cccc=0010, D3=011 => $52CB, disp=10
  DBHI D3,T5_24_4                       ; @BT $52 $CB $00 $0A
  SWAP D0                             ; @BT $48 $40
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T5_24_4:
; DBLS D4: cccc=0011, D4=100 => $53CC, disp=8
  DBLS D4,T5_24_5                       ; @BT $53 $CC $00 $08
  NOT.B D0                            ; @BT $46 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T5_24_5:
; DBCC D5: cccc=0100, D5=101 => $54CD, disp=10
  DBCC D5,T5_24_6                       ; @BT $54 $CD $00 $0A
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
  NOP                                 ; @BT $4E $71
T5_24_6:
; DBCS D6: cccc=0101, D6=110 => $55CE, disp=8
  DBCS D6,T5_24_7                       ; @BT $55 $CE $00 $08
  EXT.W D0                            ; @BT $48 $80
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
T5_24_7:
; DBNE D7: cccc=0110, D7=111 => $56CF, disp=8
  DBNE D7,T5_24_8                       ; @BT $56 $CF $00 $08
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T5_24_8:
; DBEQ D0: cccc=0111, D0=000 => $57C8, disp=12
  DBEQ D0,T5_24_9                       ; @BT $57 $C8 $00 $0C
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
  NOP                                 ; @BT $4E $71
T5_24_9:
; DBVC D1: cccc=1000, D1=001 => $58C9, disp=12
  DBVC D1,T5_24_10                      ; @BT $58 $C9 $00 $0C
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
T5_24_10:
; DBVS D2: cccc=1001, D2=010 => $59CA, disp=8
  DBVS D2,T5_24_11                      ; @BT $59 $CA $00 $08
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (T4D_0010,A0),A4                     ; @BT $49 $E8 $00 $10
T5_24_11:
; DBPL D3: cccc=1010, D3=011 => $5ACB, disp=10
  DBPL D3,T5_24_12                      ; @BT $5A $CB $00 $0A
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T5_24_12:
; DBMI D4: cccc=1011, D4=100 => $5BCC, disp=10
  DBMI D4,T5_24_13                      ; @BT $5B $CC $00 $0A
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  MOVE.L (F5_10000(T4D_1000,T4D_0010)).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T5_24_13:
; DBGE D5: cccc=1100, D5=101 => $5CCD, disp=10
  DBGE D5,T5_24_14                      ; @BT $5C $CD $00 $0A
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  NOP                                 ; @BT $4E $71
T5_24_14:
; DBLT D6: cccc=1101, D6=110 => $5DCE, disp=12
  DBLT D6,T5_24_15                      ; @BT $5D $CE $00 $0C
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
T5_24_15:
; DBGT D7: cccc=1110, D7=111 => $5ECF, disp=10
  DBGT D7,T5_24_16                      ; @BT $5E $CF $00 $0A
  NOP                                 ; @BT $4E $71
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
T5_24_16:
; DBLE D0: cccc=1111, D0=000 => $5FC8, disp=10
  DBLE D0,T5_24_17                      ; @BT $5F $C8 $00 $0A
  CLR.B D0                            ; @BT $42 $00
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T5_24_17:

;==========================================================
; TEST 25: Scc — Set According to Condition
; 0101_cccc_11_000_Dn (byte)
;==========================================================
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  ABCD D1,D0                          ; @BT $C1 $01
  ST D0                               ; @BT $50 $C0
  MOVE.L USP,A1                       ; @BT $4E $69
  SUBQ.B #(5-4),D0                        ; @BT $53 $00
  JSR (A0)                            ; @BT $4E $90
  CMP.W D2,D3                         ; @BT $B6 $42
  ASL.W #(6-4),D1                         ; @BT $E5 $41
  MOVEA.W D0,A0                       ; @BT $30 $40
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  AND.B D1,D0                         ; @BT $C0 $01
  LINK A7,#(0-(69-5))                        ; @BT $4E $57 $FF $C0
  SBCD -(A1),-(A0)                    ; @BT $81 $09
  AND.W #F5_FF00(F5_0100(),T4D_ONE),D1                     ; @BT $02 $41 $FF $00
  MOVE.W D2,D3                        ; @BT $36 $02
  OR.W D2,D3                          ; @BT $86 $42
  BTST #(7-4),D0                          ; @BT $08 $00 $00 $03
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
  MULU.W D2,D3                        ; @BT $C6 $C2
  NEG.B D0                            ; @BT $44 $00
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
  SBCD D1,D0                          ; @BT $81 $01
  RTE                                 ; @BT $4E $73
  SVS D1                              ; @BT $59 $C1
  BCLR D0,D1                          ; @BT $01 $81
  ORI.B #T4D_000F,D0                       ; @BT $00 $00 $00 $0F
  MOVEQ #(6/3-2),D0                         ; @BT $70 $00
  MOVEQ #(5-4),D1                         ; @BT $72 $01
  BTST D0,D1                          ; @BT $01 $01
  ROR.W (A2)                          ; @BT $E6 $D2
  LSR.W #(6-4),D1                         ; @BT $E4 $49
  TRAP #(5-4)                             ; @BT $4E $41
  NBCD D0                             ; @BT $48 $00
  ASL.W D1,D0                         ; @BT $E3 $60
  CMPI.B #T4D_0010,D0                      ; @BT $0C $00 $00 $10
  CMPM.W (A2)+,(A3)+                  ; @BT $B7 $4A
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVEQ #F5_NEG1(T4D_ONE),D2                        ; @BT $74 $FF
  RTS                                 ; @BT $4E $75
  ASL.W (A0)                          ; @BT $E1 $D0
  BSET D0,D1                          ; @BT $01 $C1
  SPL D2                              ; @BT $5A $C2
  MOVE.B D0,D1                        ; @BT $12 $00
  TST.W (A0)                          ; @BT $4A $50
  BSET #(7-4),D0                          ; @BT $08 $C0 $00 $03
  SUB.W D2,D3                         ; @BT $96 $42
  MOVEM.L D0/D1/A0/A1,-(A7)          ; @BT $48 $E7 $C0 $C0
  NEG.W D1                            ; @BT $44 $41
  SGE D4                              ; @BT $5C $C4
  CMP.B #T4D_0010,D0                       ; @BT $0C $00 $00 $10
  ASR.W (A0)                          ; @BT $E0 $D0
  ABCD -(A1),-(A0)                    ; @BT $C1 $09
  SHI D2                              ; @BT $52 $C2
  TST.L D2                            ; @BT $4A $82
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
  ADD.B D1,D0                         ; @BT $D0 $01
  ADD.B #T4D_0010,D0                       ; @BT $06 $00 $00 $10
  ADDX.L D5,D4                        ; @BT $D9 $85
  MOVE.W D1,-(A7)                     ; @BT $3F $01
  ADDQ.B #(5-4),D0                        ; @BT $52 $00
  CMP.L #F5_10000(T4D_1000,T4D_0010),D2                    ; @BT $0C $82 $00 $01 $00 $00
  EORI.B #T4D_00AA,D0                      ; @BT $0A $00 $00 $AA
  CMP.L D4,D5                         ; @BT $BA $84
  ADDQ.L #(12-4),D7                        ; @BT $50 $87
  MOVE.W D3,(F5_0020(F5_0010()),A3)                ; @BT $37 $43 $00 $20
  EORI.B #T4D_ZERO,CCR                     ; @BT $0A $3C $00 $00
  TAS (A0)                            ; @BT $4A $D0
  PEA (A0)                            ; @BT $48 $50
  AND.W D2,D3                         ; @BT $C6 $42
  AND.B #T4D_000F,D0                       ; @BT $02 $00 $00 $0F
  MOVEM.L (A7)+,D0/D1/A0/A1          ; @BT $4C $DF $03 $03
  ANDI.B #T4D_ZERO,CCR                     ; @BT $02 $3C $00 $00
  CMP.B D1,D0                         ; @BT $B0 $01
  SUBA.L D1,A1                        ; @BT $93 $C1
  TRAP #T4D_15                            ; @BT $4E $4F
  MOVEA.L A3,A4                       ; @BT $28 $4B
  MOVE.B D0,(A0)                      ; @BT $10 $80
  PEA (T4D_1000).L                   ; @BT $48 $79 $00 $00 $10 $00
  ADDQ.W #(6-4),A0                        ; @BT $54 $48
  DIVU.W D2,D3                        ; @BT $86 $C2
  MOVEQ #F5_127(),D7                       ; @BT $7E $7F
  MOVE.L D4,D5                        ; @BT $2A $04
  CHK (T4D_1000).W,D2                    ; @BT $45 $B8 $10 $00
  SNE D6                              ; @BT $56 $C6
  MOVE.L (T4D_1000).W,D4                 ; @BT $28 $38 $10 $00
  SLS D3                              ; @BT $53 $C3
  MOVE.L A0,USP                       ; @BT $4E $60
  TRAP #(6/3-2)                             ; @BT $4E $40
  CMPM.B (A0)+,(A1)+                  ; @BT $B3 $08
  MOVEP.L D1,(F5_0020(F5_0010()),A1)                 ; @BT $03 $C9 $00 $20
  NEGX.B D0                           ; @BT $40 $00
  MOVE D0,CCR                         ; @BT $44 $C0
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVEP.L (F5_0020(F5_0010()),A1),D1                 ; @BT $03 $49 $00 $20
  MOVE SR,D2                          ; @BT $40 $C2
  MOVE.W ((8-4),A0,D1),D2                 ; @BT $34 $30 $10 $04
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (T4D_0010,A0),A4                     ; @BT $49 $E8 $00 $10
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  ROL.W #(6-4),D1                         ; @BT $E5 $59
  ADD.L #F5_10000(T4D_1000,T4D_0010),D2                    ; @BT $06 $82 $00 $01 $00 $00
; BRA.B to next+10 bytes: disp=10 => $600A
  BRA.B T5_23_1                         ; @BT $60 $0A
  NOP                                 ; @BT $4E $71
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T5_23_1:
; BEQ.B to next+8 bytes: disp=8 => $6708
  BEQ.B T5_23_2                         ; @BT $67 $08
  CLR.B D0                            ; @BT $42 $00
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
T5_23_2:
; BNE.B to next+10 bytes: disp=10 => $660A
  BNE.B T5_23_3                         ; @BT $66 $0A
  TST.B D0                            ; @BT $4A $00
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
T5_23_3:
; BCC.B to next+6 bytes: disp=6 => $6406
  BCC.B T5_23_4                         ; @BT $64 $06
  SWAP D0                             ; @BT $48 $40
  AND.B #T4D_000F,D0                       ; @BT $02 $00 $00 $0F
T5_23_4:
; BCS.B to next+10 bytes: disp=10 => $650A
  BCS.B T5_23_5                         ; @BT $65 $0A
  NOT.B D0                            ; @BT $46 $00
  OR.W #F5_FF00(F5_0100(),T4D_ONE),D1                      ; @BT $00 $41 $FF $00
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
T5_23_5:
; BVC.B to next+6 bytes: disp=6 => $6806
  BVC.B T5_23_6                         ; @BT $68 $06
  NEG.B D0                            ; @BT $44 $00
  MOVE.L D0,(($2000+$3-$3)).W                 ; @BT $21 $C0 $20 $00
T5_23_6:
; BVS.B to next+6 bytes: disp=6 => $6906
  BVS.B T5_23_7                         ; @BT $69 $06
  EXT.W D0                            ; @BT $48 $80
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
T5_23_7:
; BPL.B to next+8 bytes: disp=8 => $6A08
  BPL.B T5_23_8                         ; @BT $6A $08
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
  NOP                                 ; @BT $4E $71
T5_23_8:
; BMI.B to next+6 bytes: disp=6 => $6B06
  BMI.B T5_23_9                         ; @BT $6B $06
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T5_23_9:
; BGE.B to next+8 bytes: disp=8 => $6C08
  BGE.B T5_23_10                        ; @BT $6C $08
  ROXL.B #(5-4),D0                        ; @BT $E3 $10
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
T5_23_10:
; BLT.B to next+8 bytes: disp=8 => $6D08
  BLT.B T5_23_11                        ; @BT $6D $08
  ROXR.B #(5-4),D0                        ; @BT $E2 $10
  LEA (T4D_0010,A0),A4                     ; @BT $49 $E8 $00 $10
  NOP                                 ; @BT $4E $71
T5_23_11:
; BGT.B to next+6 bytes: disp=6 => $6E06
  BGT.B T5_23_12                        ; @BT $6E $06
  ROL.B #(5-4),D0                         ; @BT $E3 $18
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
T5_23_12:
; BLE.B to next+6 bytes: disp=6 => $6F06
  BLE.B T5_23_13                        ; @BT $6F $06
  ROR.B #(5-4),D0                         ; @BT $E2 $18
  MOVEM.W (A0),D0-D3                  ; @BT $4C $90 $00 $0F
T5_23_13:
; BHI.B to next+8 bytes: disp=8 => $6208
  BHI.B T5_23_14                        ; @BT $62 $08
  LSL.B #(5-4),D0                         ; @BT $E3 $08
  MOVE.L (F5_10000(T4D_1000,T4D_0010)).L,D5             ; @BT $2A $39 $00 $01 $00 $00
T5_23_14:
; BLS.B to next+8 bytes: disp=8 => $6308
  BLS.B T5_23_15                        ; @BT $63 $08
  LSR.B #(5-4),D0                         ; @BT $E2 $08
  MOVE.W (T4D_0010,A3),D3                ; @BT $36 $2B $00 $10
  NOP                                 ; @BT $4E $71
T5_23_15:
; BRA.W to next+12 bytes: disp=14 => $6000000E
  BRA.W T5_23_16                        ; @BT $60 $00 $00 $0E
  NOP                                 ; @BT $4E $71
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
T5_23_16:
; BSR.B to next+8 bytes: disp=8 => $6108
  BSR.B T5_23_17                        ; @BT $61 $08
  CLR.B D0                            ; @BT $42 $00
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  NOP                                 ; @BT $4E $71
T5_23_17:
; BSR.W to next+10 bytes: disp=12 => $6100000C
  BSR.W T5_23_18                        ; @BT $61 $00 $00 $0C
  TST.B D0                            ; @BT $4A $00
  MOVEM.L D0/D1/A0/A1,-(A7)           ; @BT $48 $E7 $C0 $C0
  MOVE.W #F5_1234(T4D_1000,$234),D1                    ; @BT $32 $3C $12 $34
T5_23_18:

;==========================================================
; TEST 24: DBcc
; 0101_cccc_11001_Dn + disp word
; Each DBcc targets a label after 0-10 random instructions (prefer complex).
;==========================================================
  ADDX.B D1,D0                        ; @BT $D1 $01
  SUBQ.W #(6-4),A0                        ; @BT $55 $48
  SUB.L #F5_10000(T4D_1000,T4D_0010),D2                    ; @BT $04 $82 $00 $01 $00 $00
  MOVE.L D0,-(A7)                     ; @BT $2F $00
  ADD.L D4,D5                         ; @BT $DA $84
  ADDA.W D0,A0                        ; @BT $D0 $C0
  MOVE D1,SR                          ; @BT $46 $C1
  ROXL.W (A3)                         ; @BT $E5 $D3
  EOR.B D0,D1                         ; @BT $B1 $01
  EORI.W #T4D_ZERO,SR                    ; @BT $0A $7C $00 $00
  SWAP D0                             ; @BT $48 $40
  ASR.W D1,D0                         ; @BT $E2 $60
  MOVE.W (A1)+,D1                     ; @BT $32 $19
  BCHG #(7-4),D0                          ; @BT $08 $40 $00 $03
  MOVE.B #($42+$3-$3),D0                      ; @BT $10 $3C $00 $42
  MULS.W D1,D0                        ; @BT $C1 $C1
  JSR (T4D_1000).L                   ; @BT $4E $B9 $00 $00 $10 $00
  TRAPV                               ; @BT $4E $76
  CMP.W #T4D_1000,D1                     ; @BT $0C $41 $10 $00
  EOR.B #T4D_00AA,D0                       ; @BT $0A $00 $00 $AA
  ADDX.W D3,D2                        ; @BT $D5 $43
  OR.B #T4D_000F,D0                        ; @BT $00 $00 $00 $0F
  ANDI.B #T4D_000F,D0                      ; @BT $02 $00 $00 $0F
  LEA (T4D_1000).W,A2                    ; @BT $45 $F8 $10 $00
  EXT.W D0                            ; @BT $48 $80
  LSR.W (A1)                          ; @BT $E2 $D1
  SUBQ.L #(12-4),D7                        ; @BT $51 $87
  CMPA.W D0,A0                        ; @BT $B0 $C0
  NEG.L D2                            ; @BT $44 $82
  ASL.B #(5-4),D0                         ; @BT $E3 $00
  MOVE.W ((6/3-2),PC),D0                    ; @BT $30 $3A $EC $A1
  SLT D5                              ; @BT $5D $C5
  SLE D7                              ; @BT $5F $C7
  JMP (T4D_1000).W                       ; @BT $4E $F8 $10 $00
  TST.W D1                            ; @BT $4A $41
  PEA (T4D_1000).W                       ; @BT $48 $78 $10 $00
  SGT D6                              ; @BT $5E $C6
  EOR.W D2,D3                         ; @BT $B5 $43
  ASR.W #(6-4),D1                         ; @BT $E4 $41
  CLR.L D2                            ; @BT $42 $82
  MOVE.L (A7)+,D0                     ; @BT $20 $1F
  STOP #F5_2700(F5_0100(),$27)                         ; @BT $4E $72 $27 $00
  SMI D3                              ; @BT $5B $C3
  ROR.W #(6-4),D1                         ; @BT $E4 $59
  OR.B D1,D0                          ; @BT $80 $01
  MOVE.W D1,(A1)+                     ; @BT $32 $C1
  ADDX.W -(A1),-(A0)                  ; @BT $D1 $49
  EOR.L #F5_AAAAAAAA(),D2                 ; @BT $0A $82 $AA $AA $AA $AA
  BCLR #(7-4),D0                          ; @BT $08 $80 $00 $03
  EXT.L D1                            ; @BT $48 $C1
  LSL.W (A1)                          ; @BT $E3 $D1
  SUBX.W -(A1),-(A0)                  ; @BT $91 $49
  BCHG D0,D1                          ; @BT $01 $41
  NOP                                 ; @BT $4E $71
  ADDQ.W #(8-4),D1                        ; @BT $58 $41
  MOVE.L #F5_12345678(),D2                ; @BT $24 $3C $12 $34 $56 $78
  JSR (T4D_1000).W                       ; @BT $4E $B8 $10 $00
  LSL.W #(6-4),D1                         ; @BT $E5 $49
  SCC D4                              ; @BT $54 $C4
  SUB.W #T4D_0100,D1                      ; @BT $04 $41 $01 $00
  ADDI.B #T4D_0010,D0                      ; @BT $06 $00 $00 $10
  NEGX.L D2                           ; @BT $40 $82
  SUB.L D4,D5                         ; @BT $9A $84
  SVC D0                              ; @BT $58 $C0
  SF D1                               ; @BT $51 $C1
  NOT.W D1                            ; @BT $46 $41
  TAS D0                              ; @BT $4A $C0
  CMPM.L (A2)+,(A3)+                  ; @BT $B7 $8A
  JMP (T4D_1000).L                   ; @BT $4E $F9 $00 $00 $10 $00
  SUB.B D1,D0                         ; @BT $90 $01
  ADDA.L D1,A1                        ; @BT $D3 $C1
  ILLEGAL                             ; @BT $4A $FC
  DIVS.W D1,D0                        ; @BT $81 $C1
  ROXR.W (A3)                         ; @BT $E4 $D3
  MOVEM.W D0-D3,(A0)                  ; @BT $48 $90 $00 $0F
  ORI.W #T4D_ZERO,SR                     ; @BT $00 $7C $00 $00
  UNLK A6                             ; @BT $4E $5E
  LINK A6,#(0-(13-5))                         ; @BT $4E $56 $FF $F8
  ADD.W #T4D_1000,D1                     ; @BT $06 $41 $10 $00
  OR.W #F5_FF00(F5_0100(),T4D_ONE),D1                      ; @BT $00 $41 $FF $00
  SCS D5                              ; @BT $55 $C5
  CLR.B D0                            ; @BT $42 $00
  AND.L D4,D5                         ; @BT $CA $84
  OR.L D4,D5                          ; @BT $8A $84
  MOVEA.W #T4D_1000,A2                   ; @BT $34 $7C $10 $00
  ASL.L #(8-4),D2                         ; @BT $E9 $82
  SUBA.W D0,A0                        ; @BT $90 $C0
  MOVEP.W D0,(T4D_0010,A0)                 ; @BT $01 $88 $00 $10
  EXG D0,A0                           ; @BT $C1 $88
  AND.L #T4D_FFFF0000,D2                 ; @BT $02 $82 $FF $FF $00 $00
  EOR.L D4,D5                         ; @BT $B9 $85
  TST.B D0                            ; @BT $4A $00
  CMPA.L D1,A1                        ; @BT $B3 $C1
  EXG D0,D1                           ; @BT $C1 $41
  MOVE.W (A7)+,D1                     ; @BT $32 $1F
  MOVE.L D2,-(A2)                     ; @BT $25 $02
  EOR.W #T4D_AAAA,D1                     ; @BT $0A $41 $AA $AA
  MOVEP.W (T4D_0010,A0),D0                 ; @BT $01 $08 $00 $10
  MOVE.L -(A2),D2                     ; @BT $24 $22
  CHK D1,D0                           ; @BT $41 $81
  ORI.B #T4D_ZERO,CCR                      ; @BT $00 $3C $00 $00
  CLR.W D1                            ; @BT $42 $41
  LEA (T4D_1000).L,A3                ; @BT $47 $F9 $00 $00 $10 $00
  ADDA.W #T4D_0100,A2                     ; @BT $D4 $FC $01 $00
  MOVE.B (A0),D0                      ; @BT $10 $10
  ASR.L #(5-4),D3                         ; @BT $E2 $83
  OR.L #T4D_FFFF0000,D2                  ; @BT $00 $82 $FF $FF $00 $00
  UNLK A7                             ; @BT $4E $5F
  RESET                               ; @BT $4E $70
  LEA (A0),A1                         ; @BT $43 $D0
  SUB.B #T4D_0010,D0                       ; @BT $04 $00 $00 $10
  SUBQ.W #(8-4),D1                        ; @BT $59 $41
  JMP (A0)                            ; @BT $4E $D0
  ASR.B #(5-4),D0                         ; @BT $E2 $00

; potential quick-form optimization candidates (must stay non-quick)
  ADD.W #(5-4),D0                         ; @BT $06 $40 $00 $01
  ADD.L #(12-4),D3                         ; @BT $06 $83 $00 $00 $00 $08
  SUB.W #(5-4),D0                         ; @BT $04 $40 $00 $01
  SUB.L #(12-4),D3                         ; @BT $04 $83 $00 $00 $00 $08
  MOVE.L #(5-4),D0                        ; @BT $20 $3C $00 $00 $00 $01
  MOVE.L #F5_NEG1(T4D_ONE),D7                       ; @BT $2E $3C $FF $FF $FF $FF

  .db "<05" ; @BT END
