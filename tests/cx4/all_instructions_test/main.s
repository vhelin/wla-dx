        .MEMORYMAP
          SLOTSIZE $8000
          DEFAULTSLOT 0
          SLOT 0 START $0000
        .ENDME

        .ROMBANKSIZE $8000
        .ROMBANKS 1

        .BANK 0 SLOT 0
        .ORG 0

        .DEFINE T2_DELTA    0
        .DEFINE T2_SHIFT_00 0
        .DEFINE T2_SHIFT_01 1
        .DEFINE T2_SHIFT_08 8
        .DEFINE T2_SHIFT_16 16
        .DEFINE T2_X_00     $12
        .DEFINE T2_X_01     $23
        .DEFINE T2_X_02     $34
        .DEFINE T2_X_03     $44
        .DEFINE T2_X_04     $45
        .DEFINE T2_X_05     $55
        .DEFINE T2_X_06     $56
        .DEFINE T2_X_07     $67
        .DEFINE T2_X_08     $78
        .DEFINE T2_X_09     $89
        .DEFINE T2_X_10     $9A
        .DEFINE T2_X_11     $AB
        .DEFINE T2_X_12     $BC
        .DEFINE T2_X_13     $CD
        .DEFINE T2_X_14     $DE
        .DEFINE T2_X_15     $EF
        .DEFINE T2_Q_00     $123
        .DEFINE T2_Q_01     $234
        .DEFINE T2_Q_02     $2A5
        .DEFINE T2_Q_03     $3BC
        .DEFINE T2_U_00     3
        .DEFINE T2_U_01     5
        .DEFINE T2_U_02     7
        .DEFINE T2_U_03     11
        .DEFINE T2_U_04     13
        .DEFINE T2_U_05     17
        .DEFINE T2_U_06     19
        .DEFINE T2_U_07     23
        .DEFINE T2_U_08     29
        .DEFINE T2_U_09     31
        .DEFINE T2_H_00     $12
        .DEFINE T2_H_01     $23
        .DEFINE T2_H_02     $34
        .DEFINE T2_H_03     $45
        .DEFINE T2_H_04     $56
        .DEFINE T2_H_05     $67
        .DEFINE T2_H_06     $70
        .DEFINE T2_H_07     $7F

        .DEFINE T3_BASE     $20
        .DEFINE T3_STEP     3
        .DEFINE T3_SCALE    2
        .DEFINE T3_SHIFT    8
        .DEFINE T3_QBASE    $120

        ; @BT result.rom

        ; NOTE: Cx4 opcodes, as WLA DX understands them, can be found in /icx4.c
        ; TEST-01 covers every instruction table entry, including aliases.
        ; TEST-02 re-covers the table in shuffled order while using .DEFINE values and arithmetic.
        ; TEST-03 keeps a smaller focused set of expression-heavy smoke tests.

        .section "Cx4 all instructions" force

        /////////////////////////////////////////////////////////////////////////////
        // TEST-01: exhaustive pass over all icx4.c instruction entries
        /////////////////////////////////////////////////////////////////////////////

        ; 1-byte filler aligns the following instruction stream to an even byte
        ; address so that label-based branch/jump targets work under HG51B word
        ; addressing (every instruction is 2 bytes, branch operand = address/2).
        .db 0
        .db "01>"               ; @BT TEST-01 01 START
        ADD A,r0                                             ; @BT 60 80
        ADD A<<1,$12                                         ; @BT 12 85
        ASR A,r1                                             ; @BT 61 C8
        ASR A,3                                              ; @BT 03 CC
        AND A<<8,r2                                          ; @BT 62 B2
        AND A<<16,$23                                        ; @BT 23 B7
        BEQ T1_FLOW_45                                       ; @BT 23 0C
        BEQ.F T1_FLOW_49                                     ; @BT 25 0E
        BGE T1_FLOW_71                                       ; @BT 39 10
        BGE.F T1_FLOW_89                                     ; @BT 45 12
        BMI T1_FLOW_9D                                       ; @BT 4f 14
        BMI.F T1_FLOW_CD                                     ; @BT 67 16
        BRA T1_FLOW_E7                                       ; @BT 74 08
        BRA.F T1_FLOW_F7                                     ; @BT 7c 0A
        BSR T1_FLOW_45                                       ; @BT 23 28
        BSR.F T1_FLOW_49                                     ; @BT 25 2A
        BSREQ T1_FLOW_71                                     ; @BT 39 2C
        BSREQ.F T1_FLOW_89                                   ; @BT 45 2E
        BSRGE T1_FLOW_9D                                     ; @BT 4f 30
        BSRGE.F T1_FLOW_CD                                   ; @BT 67 32
        BSRMI T1_FLOW_E7                                     ; @BT 74 34
        BSRMI.F T1_FLOW_F7                                   ; @BT 7c 36
        BSRVS T1_FLOW_45                                     ; @BT 23 38
        BSRVS.F T1_FLOW_49                                   ; @BT 25 3A
        BVS T1_FLOW_71                                       ; @BT 39 18
        BVS.F T1_FLOW_89                                     ; @BT 45 1A
        CLEAR                                                ; @BT 00 F8
        CMP A,r3                                             ; @BT 63 50
        CMP A<<1,$56                                         ; @BT 56 55
        CMPR A<<8,r4                                         ; @BT 64 4A
        CMPR A<<16,$67                                       ; @BT 67 4F
        EXTS A                                               ; @BT 00 59
        EXTW A                                               ; @BT 00 5A
      T1_FLOW_45:
        HALT                                                 ; @BT 00 FC
        INC MAR                                              ; @BT 00 40
      T1_FLOW_49:
        JMP T1_FLOW_9D                                       ; @BT 4f 08
        JMP.F T1_FLOW_CD                                     ; @BT 67 0A
        JMP EQ,T1_FLOW_E7                                    ; @BT 74 0C
        JMP EQ.F,T1_FLOW_F7                                  ; @BT 7c 0E
        JMP GE,T1_FLOW_45                                    ; @BT 23 10
        JMP GE.F,T1_FLOW_49                                  ; @BT 25 12
        JMP MI,T1_FLOW_71                                    ; @BT 39 14
        JMP MI.F,T1_FLOW_89                                  ; @BT 45 16
        JMP VS,T1_FLOW_9D                                    ; @BT 4f 18
        JMP VS.F,T1_FLOW_CD                                  ; @BT 67 1A
        JSR T1_FLOW_E7                                       ; @BT 74 28
        JSR.F T1_FLOW_F7                                     ; @BT 7c 2A
        JSR EQ,T1_FLOW_45                                    ; @BT 23 2C
        JSR EQ.F,T1_FLOW_49                                  ; @BT 25 2E
        JSR GE,T1_FLOW_71                                    ; @BT 39 30
        JSR GE.F,T1_FLOW_89                                  ; @BT 45 32
        JSR MI,T1_FLOW_9D                                    ; @BT 4f 34
        JSR MI.F,T1_FLOW_CD                                  ; @BT 67 36
        JSR VS,T1_FLOW_E7                                    ; @BT 74 38
        JSR VS.F,T1_FLOW_F7                                  ; @BT 7c 3A
      T1_FLOW_71:
        LD A,r5                                              ; @BT 65 60
        LD A,$BC                                             ; @BT BC 64
        LD MAR,r6                                            ; @BT 66 62
        LD MAR,$CD                                           ; @BT CD 66
        LD MDR,r7                                            ; @BT 67 61
        LD MDR,$DE                                           ; @BT DE 65
        LD P,r0                                              ; @BT 00 63
        LD P,$EF                                             ; @BT EF 67
        MOV A,r8                                             ; @BT 68 60
        MOV A,$12                                            ; @BT 12 64
        MOV MAR,r9                                           ; @BT 69 62
        MOV MAR,$23                                          ; @BT 23 66
      T1_FLOW_89:
        MOV MDR,r10                                          ; @BT 6A 61
        MOV MDR,$34                                          ; @BT 34 65
        MOV MDR,r11                                          ; @BT 6B 61
        MOV MDR,$44                                          ; @BT 44 65
        MOV P,r1                                             ; @BT 01 63
        MOV P,$45                                            ; @BT 45 67
        MOV PH,$55                                           ; @BT 55 7D
        MOV PL,$56                                           ; @BT 56 7C
        MOV r12,A                                            ; @BT 6C E0
        MOV r13,MDR                                          ; @BT 6D E1
      T1_FLOW_9D:
        MOV r14,MDR                                          ; @BT 6E E1
        MUL r15                                              ; @BT 6F 98
        MUL $67                                              ; @BT 67 9C
        NOP                                                  ; @BT 00 00
        OR A,r0                                              ; @BT 60 B8
        OR A<<1,$78                                          ; @BT 78 BD
        RDRAM 0,A                                            ; @BT 00 68
        RDRAM 0,$89                                          ; @BT 89 6C
        RDRAM 1,A                                            ; @BT 00 69
        RDRAM 1,$9A                                          ; @BT 9A 6D
        RDRAM 2,A                                            ; @BT 00 6A
        RDRAM 2,$AB                                          ; @BT AB 6E
        RDROM A                                              ; @BT 00 70
        RDROM $123                                           ; @BT 23 75
        ROR A,r1                                             ; @BT 61 D0
        ROR A,4                                              ; @BT 04 D4
        ROTR r2                                              ; @BT 62 D0
        ROTR 5                                               ; @BT 05 D4
        RTS                                                  ; @BT 00 3C
        SHAR r3                                              ; @BT 63 C8
        SHAR 6                                               ; @BT 06 CC
        SHLL r4                                              ; @BT 64 D8
        SHLL 7                                               ; @BT 07 DC
        SHLR r5                                              ; @BT 65 C0
      T1_FLOW_CD:
        SHLR 8                                               ; @BT 08 C4
        SHL A,r6                                             ; @BT 66 D8
        SHL A,9                                              ; @BT 09 DC
        SHR A,r7                                             ; @BT 67 C0
        SHR A,10                                             ; @BT 0A C4
        SKIPEQ                                               ; @BT 01 26
        SKIPGE                                               ; @BT 01 25
        SKIPLT                                               ; @BT 00 25
        SKIPMI                                               ; @BT 01 27
        SKIPNE                                               ; @BT 00 26
        SKIPPL                                               ; @BT 00 27
        SKIPVC                                               ; @BT 00 24
        SKIPVS                                               ; @BT 01 24
      T1_FLOW_E7:
        ST r8,A                                              ; @BT 68 E0
        ST r9,MDR                                            ; @BT 69 E1
        ST r10,MDR                                           ; @BT 6A E1
        SUB A<<8,r11                                         ; @BT 6B 92
        SUB A<<16,$BC                                        ; @BT BC 97
        SUBR A,r12                                           ; @BT 6C 88
        SUBR A<<1,$CD                                        ; @BT CD 8D
        SWAP A,r2                                            ; @BT 02 F0
      T1_FLOW_F7:
        SXB A                                                ; @BT 00 59
        SXW A                                                ; @BT 00 5A
        WAIT                                                 ; @BT 00 1C
        WRRAM 0,A                                            ; @BT 00 E8
        WRRAM 0,$DE                                          ; @BT DE EC
        WRRAM 1,A                                            ; @BT 00 E9
        WRRAM 1,$EF                                          ; @BT EF ED
        WRRAM 2,A                                            ; @BT 00 EA
        WRRAM 2,$12                                          ; @BT 12 EE
        XNOR A<<8,r13                                        ; @BT 6D A2
        XNOR A<<16,$23                                       ; @BT 23 A7
        XOR A,r14                                            ; @BT 6E A8
        XOR A<<1,$34                                         ; @BT 34 AD
        .db "<01"               ; @BT END

        /////////////////////////////////////////////////////////////////////////////
        // TEST-02: shuffled coverage with definitions and arithmetic expressions
        /////////////////////////////////////////////////////////////////////////////

        .db "02>"               ; @BT TEST-02 02 START
        CMP A,T2_X_00+0                                      ; @BT 12 54
        SKIPLT                                               ; @BT 00 25
        SKIPPL                                               ; @BT 00 27
        BRA.F (T2_X_01+1-1)*2	; @BT 23 0A
        SKIPVC                                               ; @BT 00 24
        EXTS A                                               ; @BT 00 59
        BRA (T2_X_02*2/2)*2	; @BT 34 08
        SKIPGE                                               ; @BT 01 25
        ST r0,A                                              ; @BT 60 E0
        WRRAM 1,A                                            ; @BT 00 E9
        AND A<<T2_SHIFT_01,T2_X_04+T2_DELTA-T2_DELTA         ; @BT 45 B5
        ST r1,MDR                                            ; @BT 61 E1
        RDRAM 2,T2_X_06+0                                    ; @BT 56 6E
        ROR A,r2                                             ; @BT 62 D0
        XOR A<<T2_SHIFT_08,T2_X_07+1-1                       ; @BT 67 AE
        RDRAM 0,A                                            ; @BT 00 68
        JMP.F (T2_X_08*2/2)*2	; @BT 78 0A
        ST r3,MDR                                            ; @BT 63 E1
        SUB A<<T2_SHIFT_16,r4                                ; @BT 64 93
        OR A,r5                                              ; @BT 65 B8
        SXB A                                                ; @BT 00 59
        JSR EQ,(T2_X_09+T2_DELTA-T2_DELTA)*2	; @BT 89 2C
        JMP (T2_X_10+0)*2	; @BT 9A 08
        BSREQ.F (T2_X_11+1-1)*2	; @BT AB 2E
        CMP A<<T2_SHIFT_01,r6                                ; @BT 66 51
        XOR A<<T2_SHIFT_08,r7                                ; @BT 67 AA
        SKIPEQ                                               ; @BT 01 26
        BVS.F (T2_X_12*2/2)*2	; @BT BC 1A
        WRRAM 2,A                                            ; @BT 00 EA
        MUL T2_X_13+T2_DELTA-T2_DELTA                        ; @BT CD 9C
        MOV A,T2_X_14+0                                      ; @BT DE 64
        MOV MDR,r8                                           ; @BT 68 61
        MOV MAR,r9                                           ; @BT 69 62
        ASR A,T2_U_00+0                                      ; @BT 03 CC
        SHLR T2_U_01+1-1                                     ; @BT 05 C4
        BEQ (T2_X_15+1-1)*2	; @BT EF 0C
        JSR GE,(T2_X_03*2/2)*2	; @BT 44 30
        BSRVS.F (T2_X_05+T2_DELTA-T2_DELTA)*2	; @BT 55 3A
        JSR GE.F,(T2_X_00+0)*2	; @BT 12 32
        JSR VS,(T2_X_01+1-1)*2	; @BT 23 38
        BSR.F (T2_X_02*2/2)*2	; @BT 34 2A
        ROTR T2_U_02*2/2                                     ; @BT 07 D4
        MOV MDR,T2_X_04+T2_DELTA-T2_DELTA                    ; @BT 45 65
        JSR MI.F,(T2_X_06+0)*2	; @BT 56 36
        SHLL r10                                             ; @BT 6A D8
        JMP GE.F,(T2_X_07+1-1)*2	; @BT 67 12
        JSR.F (T2_X_08*2/2)*2	; @BT 78 2A
        WAIT                                                 ; @BT 00 1C
        LD MDR,r11                                           ; @BT 6B 61
        BSRGE.F (T2_X_09+T2_DELTA-T2_DELTA)*2	; @BT 89 32
        LD A,T2_X_10+0                                       ; @BT 9A 64
        JMP EQ.F,(T2_X_11+1-1)*2	; @BT AB 0E
        JMP VS,(T2_X_12*2/2)*2	; @BT BC 18
        SHR A,T2_U_03+T2_DELTA-T2_DELTA                      ; @BT 0B C4
        JMP MI.F,(T2_X_13+T2_DELTA-T2_DELTA)*2	; @BT CD 16
        SUBR A<<T2_SHIFT_16,r12                              ; @BT 6C 8B
        EXTW A                                               ; @BT 00 5A
        BSRMI.F (T2_X_14+0)*2	; @BT DE 36
        INC MAR                                              ; @BT 00 40
        WRRAM 1,T2_X_15+1-1                                  ; @BT EF ED
        MOV MDR,T2_X_03*2/2                                  ; @BT 44 65
        BSR (T2_X_05+T2_DELTA-T2_DELTA)*2	; @BT 55 28
        RDROM T2_Q_00+0                                      ; @BT 23 75
        SWAP A,r0                                            ; @BT 00 F0
        MOV P,T2_X_00+0                                      ; @BT 12 67
        ROTR r13                                             ; @BT 6D D0
        SKIPNE                                               ; @BT 00 26
        SHLR r14                                             ; @BT 6E C0
        ASR A,r15                                            ; @BT 6F C8
        JMP EQ,(T2_X_01+1-1)*2	; @BT 23 0C
        JSR MI,(T2_X_02*2/2)*2	; @BT 34 34
        LD A,r0                                              ; @BT 60 60
        SKIPVS                                               ; @BT 01 24
        LD P,T2_X_04+T2_DELTA-T2_DELTA                       ; @BT 45 67
        CMPR A,r1                                            ; @BT 61 48
        BVS (T2_X_06+0)*2	; @BT 56 18
        MOV A,r2                                             ; @BT 62 60
        ADD A<<T2_SHIFT_01,r3                                ; @BT 63 81
        BMI.F (T2_X_07+1-1)*2	; @BT 67 16
        ADD A<<T2_SHIFT_08,T2_X_08*2/2                       ; @BT 78 86
        RDRAM 0,T2_X_09+T2_DELTA-T2_DELTA                    ; @BT 89 6C
        JMP MI,(T2_X_10+0)*2	; @BT 9A 14
        SUBR A<<T2_SHIFT_16,T2_X_11+1-1                      ; @BT AB 8F
        BGE.F (T2_X_12*2/2)*2	; @BT BC 12
        BMI (T2_X_13+T2_DELTA-T2_DELTA)*2	; @BT CD 14
        OR A,T2_X_14+0                                       ; @BT DE BC
        SHL A,T2_U_04+0                                      ; @BT 0D DC
        LD MAR,T2_X_15+1-1                                   ; @BT EF 66
        SHLL T2_U_05+1-1                                     ; @BT 11 DC
        BSRGE (T2_X_03*2/2)*2	; @BT 44 30
        SHR A,r4                                             ; @BT 64 C0
        BGE (T2_X_05+T2_DELTA-T2_DELTA)*2	; @BT 55 10
        RTS                                                  ; @BT 00 3C
        SXW A                                                ; @BT 00 5A
        SKIPMI                                               ; @BT 01 27
        CLEAR                                                ; @BT 00 F8
        LD P,r1                                              ; @BT 01 63
        SHAR r5                                              ; @BT 65 C8
        SHAR T2_U_06*2/2                                     ; @BT 13 CC
        XNOR A<<T2_SHIFT_01,T2_X_00+0                        ; @BT 12 A5
        ROR A,T2_U_07+T2_DELTA-T2_DELTA                      ; @BT 17 D4
        JSR (T2_X_01+1-1)*2	; @BT 23 28
        MOV r6,MDR                                           ; @BT 66 E1
        JMP GE,(T2_X_02*2/2)*2	; @BT 34 10
        AND A<<T2_SHIFT_08,r7                                ; @BT 67 B2
        MOV PL,T2_X_04+T2_DELTA-T2_DELTA                     ; @BT 45 7C
        CMPR A<<T2_SHIFT_16,T2_X_06+0                        ; @BT 56 4F
        HALT                                                 ; @BT 00 FC
        WRRAM 2,T2_X_07+1-1                                  ; @BT 67 EE
        LD MDR,T2_X_08*2/2                                   ; @BT 78 65
        RDRAM 1,T2_X_09+T2_DELTA-T2_DELTA                    ; @BT 89 6D
        MOV PH,(T2_X_10+0) & $7F                             ; @BT 1A 7D
        JSR VS.F,(T2_X_11+1-1)*2	; @BT AB 3A
        SUB A,T2_X_12*2/2                                    ; @BT BC 94
        MOV r8,A                                             ; @BT 68 E0
        MOV r9,MDR                                           ; @BT 69 E1
        BSRMI (T2_X_13+T2_DELTA-T2_DELTA)*2	; @BT CD 34
        SHL A,r10                                            ; @BT 6A D8
        BSREQ (T2_X_14+0)*2	; @BT DE 2C
        WRRAM 0,T2_X_15+1-1                                  ; @BT EF EC
        XNOR A<<T2_SHIFT_01,r11                              ; @BT 6B A1
        RDROM A                                              ; @BT 00 70
        RDRAM 1,A                                            ; @BT 00 69
        MOV MAR,T2_X_03*2/2                                  ; @BT 44 66
        MOV MDR,r12                                          ; @BT 6C 61
        MUL r13                                              ; @BT 6D 98
        BEQ.F (T2_X_05+T2_DELTA-T2_DELTA)*2	; @BT 55 0E
        JSR EQ.F,(T2_X_00+0)*2	; @BT 12 2E
        NOP                                                  ; @BT 00 00
        LD MAR,r14                                           ; @BT 6E 62
        BSRVS (T2_X_01+1-1)*2	; @BT 23 38
        JMP VS.F,(T2_X_02*2/2)*2	; @BT 34 1A
        WRRAM 0,A                                            ; @BT 00 E8
        RDRAM 2,A                                            ; @BT 00 6A
        MOV P,r2                                             ; @BT 02 63
        .db "<02"               ; @BT END

        /////////////////////////////////////////////////////////////////////////////
        // TEST-03: focused calculation and alias smoke tests
        /////////////////////////////////////////////////////////////////////////////

        .db "03>"               ; @BT TEST-03 03 START
        BRA (T3_BASE-(T3_STEP+2))*2
; @BT 1B 08
        JMP EQ.F,((T3_BASE+T3_STEP)*3)*2
; @BT 69 0E
        MOV P,T3_BASE+T3_STEP                              ; @BT 23 67
        MOV PL,(T3_BASE+T3_STEP)*T3_SCALE                  ; @BT 46 7C
        MOV PH,(T3_BASE*T3_SCALE)+T3_STEP+$12              ; @BT 55 7D
        ADD A<<T3_SHIFT,T3_BASE+5                          ; @BT 25 86
        SUB A<<16,T3_BASE+T3_STEP+$99                      ; @BT BC 97
        SHR A,T3_STEP+7                                    ; @BT 0A C4
        RDROM T3_QBASE+T3_STEP                             ; @BT 23 75
        WRRAM 2,(T3_BASE+T3_STEP)*T3_SCALE                 ; @BT 46 EE
        .db "<03"               ; @BT END

        .ends
