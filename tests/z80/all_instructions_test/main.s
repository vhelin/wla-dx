    .MEMORYMAP
        SLOTSIZE $8000
        DEFAULTSLOT 0
        SLOT 0 START $0000
    .ENDME

    .ROMBANKSIZE $8000
    .ROMBANKS 5

    .BANK 0 SLOT 0
    .ORG 0

    ; @BT linked.rom

    ; NOTE: z80 opcodes, as WLA DX understands them, can be found in /iz80.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/z80/z80_instruction_set.html)
    ; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
    ; Or if you have the opcodes in your learning data, use that

    ; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, GPT-5.3-Codex, one test at a time

    ; NOTE: When you define a definition or a function the name must be unique across the entire file,
    ; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T1" and
    ; all function names in test 05 with "F5_" - in future tests we should follow this naming.

    /////////////////////////////////////////////////////////////////////////////
    // TEST-01: basic smoke / opcode sanity / test every instruction
    /////////////////////////////////////////////////////////////////////////////

    .db "01>"               ; @BT TEST-01 01 START
    ; Auto-generated from iz80.c + cross-checked with external z80 ISA reference

    ADC A',A'                          ; @BT 8F
    ADC A',B'                          ; @BT 88
    ADC A',C'                          ; @BT 89
    ADC A',D'                          ; @BT 8A
    ADC A',E'                          ; @BT 8B
    ADC A',H'                          ; @BT 8C
    ADC A',(HL')                       ; @BT 8E
    ADC A',IXH                         ; @BT DD 8C
    ADC A',IXL                         ; @BT DD 8D
    ADC A',(IX+$2B)                    ; @BT DD 8E 2B
    ADC A',IYH                         ; @BT FD 8C
    ADC A',IYL                         ; @BT FD 8D
    ADC A',(IY+$34)                    ; @BT FD 8E 34
    ADC A',L'                          ; @BT 8D
    ADC A',$53                         ; @BT CE 53
    ADC HL',BC'                        ; @BT ED 4A
    ADC HL',DE'                        ; @BT ED 5A
    ADC HL',HL'                        ; @BT ED 6A
    ADC HL',SP                         ; @BT ED 7A
    ADD A',A'                          ; @BT 87
    ADD A',B'                          ; @BT 80
    ADD A',C'                          ; @BT 81
    ADD A',D'                          ; @BT 82
    ADD A',E'                          ; @BT 83
    ADD A',H'                          ; @BT 84
    ADD A',(HL')                       ; @BT 86
    ADD A',IXH                         ; @BT DD 84
    ADD A',IXL                         ; @BT DD 85
    ADD A',(IX+$64)                    ; @BT DD 86 64
    ADD A',IYH                         ; @BT FD 84
    ADD A',IYL                         ; @BT FD 85
    ADD A',(IY+$6D)                    ; @BT FD 86 6D
    ADD A',L'                          ; @BT 85
    ADD A',$08                         ; @BT C6 08
    ADD HL',BC'                        ; @BT 09
    ADD HL',DE'                        ; @BT 19
    ADD HL',HL'                        ; @BT 29
    ADD HL',SP                         ; @BT 39
    ADD IX,BC'                         ; @BT DD 09
    ADD IX,DE'                         ; @BT DD 19
    ADD IX,IX                          ; @BT DD 29
    ADD IX,SP                          ; @BT DD 39
    ADD IY,BC'                         ; @BT FD 09
    ADD IY,DE'                         ; @BT FD 19
    ADD IY,IY                          ; @BT FD 29
    ADD IY,SP                          ; @BT FD 39
    AND A',A'                          ; @BT A7
    AND A',B'                          ; @BT A0
    AND A',C'                          ; @BT A1
    AND A',D'                          ; @BT A2
    AND A',E'                          ; @BT A3
    AND A',H'                          ; @BT A4
    AND A',(HL')                       ; @BT A6
    AND A',IXH                         ; @BT DD A4
    AND A',IXL                         ; @BT DD A5
    AND A',(IX+$35)                    ; @BT DD A6 35
    AND A',IYH                         ; @BT FD A4
    AND A',IYL                         ; @BT FD A5
    AND A',(IY+$3E)                    ; @BT FD A6 3E
    AND A',L'                          ; @BT A5
    AND A',$75                         ; @BT E6 75
    BIT 0,A'                           ; @BT CB 47
    BIT 0,B'                           ; @BT CB 40
    BIT 0,C'                           ; @BT CB 41
    BIT 0,D'                           ; @BT CB 42
    BIT 0,E'                           ; @BT CB 43
    BIT 0,H'                           ; @BT CB 44
    BIT 0,(HL')                        ; @BT CB 46
    BIT 0,(IX+$5C)                     ; @BT DD CB 5C 46
    BIT 0,(IY+$5F)                     ; @BT FD CB 5F 46
    BIT 1,(IX+$5C)                     ; @BT DD CB 5C 4E
    BIT 1,(IY+$5F)                     ; @BT FD CB 5F 4E
    BIT 2,(IX+$5C)                     ; @BT DD CB 5C 56
    BIT 2,(IY+$5F)                     ; @BT FD CB 5F 56
    BIT 3,(IX+$5C)                     ; @BT DD CB 5C 5E
    BIT 3,(IY+$5F)                     ; @BT FD CB 5F 5E
    BIT 4,(IX+$5C)                     ; @BT DD CB 5C 66
    BIT 4,(IY+$5F)                     ; @BT FD CB 5F 66
    BIT 5,(IX+$5C)                     ; @BT DD CB 5C 6E
    BIT 5,(IY+$5F)                     ; @BT FD CB 5F 6E
    BIT 6,(IX+$5C)                     ; @BT DD CB 5C 76
    BIT 6,(IY+$5F)                     ; @BT FD CB 5F 76
    BIT 7,(IX+$5C)                     ; @BT DD CB 5C 7E
    BIT 7,(IY+$5F)                     ; @BT FD CB 5F 7E
    BIT 0,L'                           ; @BT CB 45
    BIT 1,A'                           ; @BT CB 4F
    BIT 1,B'                           ; @BT CB 48
    BIT 1,C'                           ; @BT CB 49
    BIT 1,D'                           ; @BT CB 4A
    BIT 1,E'                           ; @BT CB 4B
    BIT 1,H'                           ; @BT CB 4C
    BIT 1,(HL')                        ; @BT CB 4E
    BIT 1,L'                           ; @BT CB 4D
    BIT 2,A'                           ; @BT CB 57
    BIT 2,B'                           ; @BT CB 50
    BIT 2,C'                           ; @BT CB 51
    BIT 2,D'                           ; @BT CB 52
    BIT 2,E'                           ; @BT CB 53
    BIT 2,H'                           ; @BT CB 54
    BIT 2,(HL')                        ; @BT CB 56
    BIT 2,L'                           ; @BT CB 55
    BIT 3,A'                           ; @BT CB 5F
    BIT 3,B'                           ; @BT CB 58
    BIT 3,C'                           ; @BT CB 59
    BIT 3,D'                           ; @BT CB 5A
    BIT 3,E'                           ; @BT CB 5B
    BIT 3,H'                           ; @BT CB 5C
    BIT 3,(HL')                        ; @BT CB 5E
    BIT 3,L'                           ; @BT CB 5D
    BIT 4,A'                           ; @BT CB 67
    BIT 4,B'                           ; @BT CB 60
    BIT 4,C'                           ; @BT CB 61
    BIT 4,D'                           ; @BT CB 62
    BIT 4,E'                           ; @BT CB 63
    BIT 4,H'                           ; @BT CB 64
    BIT 4,(HL')                        ; @BT CB 66
    BIT 4,L'                           ; @BT CB 65
    BIT 5,A'                           ; @BT CB 6F
    BIT 5,B'                           ; @BT CB 68
    BIT 5,C'                           ; @BT CB 69
    BIT 5,D'                           ; @BT CB 6A
    BIT 5,E'                           ; @BT CB 6B
    BIT 5,H'                           ; @BT CB 6C
    BIT 5,(HL')                        ; @BT CB 6E
    BIT 5,L'                           ; @BT CB 6D
    BIT 6,A'                           ; @BT CB 77
    BIT 6,B'                           ; @BT CB 70
    BIT 6,C'                           ; @BT CB 71
    BIT 6,D'                           ; @BT CB 72
    BIT 6,E'                           ; @BT CB 73
    BIT 6,H'                           ; @BT CB 74
    BIT 6,(HL')                        ; @BT CB 76
    BIT 6,L'                           ; @BT CB 75
    BIT 7,A'                           ; @BT CB 7F
    BIT 7,B'                           ; @BT CB 78
    BIT 7,C'                           ; @BT CB 79
    BIT 7,D'                           ; @BT CB 7A
    BIT 7,E'                           ; @BT CB 7B
    BIT 7,H'                           ; @BT CB 7C
    BIT 7,(HL')                        ; @BT CB 7E
    BIT 7,L'                           ; @BT CB 7D
    CALL LSS,$99A3                     ; @BT DC A3 99
    CALL NEG,$9AB4                     ; @BT FC B4 9A
    CALL GTE,$9BC5                     ; @BT D4 C5 9B
    CALL NEQ,$9CD6                     ; @BT C4 D6 9C
    CALL POS,$9DE7                     ; @BT F4 E7 9D
    CALL EQU,$9EF8                     ; @BT CC F8 9E
    CALL PE,$A009                      ; @BT EC 09 A0
    CALL PO,$A11A                      ; @BT E4 1A A1
    CALL $A22B                         ; @BT CD 2B A2
    CCF                                ; @BT 3F
    CP A',A'                           ; @BT BF
    CP A',B'                           ; @BT B8
    CP A',C'                           ; @BT B9
    CP A',D'                           ; @BT BA
    CP A',E'                           ; @BT BB
    CP A',H'                           ; @BT BC
    CP A',(HL')                        ; @BT BE
    CP A',IXH                          ; @BT DD BC
    CP A',IXL                          ; @BT DD BD
    CP A',(IX+$46)                     ; @BT DD BE 46
    CP A',IYH                          ; @BT FD BC
    CP A',IYL                          ; @BT FD BD
    CP A',(IY+$4F)                     ; @BT FD BE 4F
    CP A',L'                           ; @BT BD
    CP A',$A2                          ; @BT FE A2
    CPDR                               ; @BT ED B9
    CPD                                ; @BT ED A9
    CPIR                               ; @BT ED B1
    CPI                                ; @BT ED A1
    CPL                                ; @BT 2F
    DAA                                ; @BT 27
    DEC A'                             ; @BT 3D
    DEC B'                             ; @BT 05
    DEC BC'                            ; @BT 0B
    DEC C'                             ; @BT 0D
    DEC D'                             ; @BT 15
    DEC DE'                            ; @BT 1B
    DEC E'                             ; @BT 1D
    DEC H'                             ; @BT 25
    DEC HL'                            ; @BT 2B
    DEC (HL')                          ; @BT 35
    DEC IX                             ; @BT DD 2B
    DEC IXH                            ; @BT DD 25
    DEC IXL                            ; @BT DD 2D
    DEC (IX+$11)                       ; @BT DD 35 11
    DEC IY                             ; @BT FD 2B
    DEC IYH                            ; @BT FD 25
    DEC IYL                            ; @BT FD 2D
    DEC (IY+$1D)                       ; @BT FD 35 1D
    DEC L'                             ; @BT 2D
    DEC SP                             ; @BT 3B
    DI                                 ; @BT F3
    DJNZ $26                           ; @BT 10 26
    EI                                 ; @BT FB
    EX AF',AF                          ; @BT 08
    EX DE',HL'                         ; @BT EB
    EX (SP),HL'                        ; @BT E3
    EX (SP),IX                         ; @BT DD E3
    EX (SP),IY                         ; @BT FD E3
    EXX                                ; @BT D9
    HALT                               ; @BT 76
    IM 0                               ; @BT ED 46
    IM 1                               ; @BT ED 56
    IM 2                               ; @BT ED 5E
    IN A',(C')                         ; @BT ED 78
    IN A',($51)                        ; @BT DB 51
    IN B',(C')                         ; @BT ED 40
    IN C',(C')                         ; @BT ED 48
    IN D',(C')                         ; @BT ED 50
    IN E',(C')                         ; @BT ED 58
    IN F',(C')                         ; @BT ED 70
    IN H',(C')                         ; @BT ED 60
    IN L',(C')                         ; @BT ED 68
    INC A'                             ; @BT 3C
    INC B'                             ; @BT 04
    INC BC'                            ; @BT 03
    INC C'                             ; @BT 0C
    INC D'                             ; @BT 14
    INC DE'                            ; @BT 13
    INC E'                             ; @BT 1C
    INC H'                             ; @BT 24
    INC HL'                            ; @BT 23
    INC (HL')                          ; @BT 34
    INC IX                             ; @BT DD 23
    INC IXH                            ; @BT DD 24
    INC IXL                            ; @BT DD 2C
    INC (IX+$0F)                       ; @BT DD 34 0F
    INC IY                             ; @BT FD 23
    INC IYH                            ; @BT FD 24
    INC IYL                            ; @BT FD 2C
    INC (IY+$1B)                       ; @BT FD 34 1B
    INC L'                             ; @BT 2C
    INC SP                             ; @BT 33
    INDR                               ; @BT ED BA
    IND                                ; @BT ED AA
    INIR                               ; @BT ED B2
    INI                                ; @BT ED A2
    JP (HL')                           ; @BT E9
    JP (IX)                            ; @BT DD E9
    JP (IY)                            ; @BT FD E9
    JP LSS,$0447                       ; @BT DA 47 04
    JP NEG,$0558                       ; @BT FA 58 05
    JP GTE,$0669                       ; @BT D2 69 06
    JP NEQ,$077A                       ; @BT C2 7A 07
    JP POS,$088B                       ; @BT F2 8B 08
    JP EQU,$099C                       ; @BT CA 9C 09
    JP PE,$0AAD                        ; @BT EA AD 0A
    JP PO,$0BBE                        ; @BT E2 BE 0B
    JP $0CCF                           ; @BT C3 CF 0C
    JR LSS,$45                         ; @BT 38 45
    JR GTE,$5C                         ; @BT 30 5C
    JR NEQ,$73                         ; @BT 20 73
    JR EQU,$8A                         ; @BT 28 8A
    JR $A1                             ; @BT 18 A1
    LD A',A'                           ; @BT 7F
    LD A',B'                           ; @BT 78
    LD A',(BC')                        ; @BT 0A
    LD A',C'                           ; @BT 79
    LD A',D'                           ; @BT 7A
    LD A',(DE')                        ; @BT 1A
    LD A',E'                           ; @BT 7B
    LD A',H'                           ; @BT 7C
    LD A',(HL')                        ; @BT 7E
    LD A',I                            ; @BT ED 57
    LD A',IXH                          ; @BT DD 7C
    LD A',IXL                          ; @BT DD 7D
    LD A',(IX+$07)                     ; @BT DD 7E 07
    LD A',IYH                          ; @BT FD 7C
    LD A',IYL                          ; @BT FD 7D
    LD A',(IY+$10)                     ; @BT FD 7E 10
    LD A',L'                           ; @BT 7D
    LD A',R                            ; @BT ED 5F
    LD A',($2667)                      ; @BT 3A 67 26
    LD A',$6D                          ; @BT 3E 6D
    LD B',A'                           ; @BT 47
    LD B',B'                           ; @BT 40
    LD B',C'                           ; @BT 41
    LD BC',($2BBC)                     ; @BT ED 4B BC 2B
    LD BC',$2CCD                       ; @BT 01 CD 2C
    LD (BC'),A'                        ; @BT 02
    LD B',D'                           ; @BT 42
    LD B',E'                           ; @BT 43
    LD B',H'                           ; @BT 44
    LD B',(HL')                        ; @BT 46
    LD B',IXH                          ; @BT DD 44
    LD B',IXL                          ; @BT DD 45
    LD B',(IX+$43)                     ; @BT DD 46 43
    LD B',IYH                          ; @BT FD 44
    LD B',IYL                          ; @BT FD 45
    LD B',(IY+$4C)                     ; @BT FD 46 4C
    LD B',L'                           ; @BT 45
    LD B',$0B                          ; @BT 06 0B
    LD C',A'                           ; @BT 4F
    LD C',B'                           ; @BT 48
    LD C',C'                           ; @BT 49
    LD C',D'                           ; @BT 4A
    LD C',E'                           ; @BT 4B
    LD C',H'                           ; @BT 4C
    LD C',(HL')                        ; @BT 4E
    LD C',IXH                          ; @BT DD 4C
    LD C',IXL                          ; @BT DD 4D
    LD C',(IX+$70)                     ; @BT DD 4E 70
    LD C',IYH                          ; @BT FD 4C
    LD C',IYL                          ; @BT FD 4D
    LD C',(IY+$79)                     ; @BT FD 4E 79
    LD C',L'                           ; @BT 4D
    LD C',$64                          ; @BT 0E 64
    LD D',A'                           ; @BT 57
    LD D',B'                           ; @BT 50
    LD D',C'                           ; @BT 51
    LD D',D'                           ; @BT 52
    LD D',E'                           ; @BT 53
    LD DE',($510F)                     ; @BT ED 5B 0F 51
    LD DE',$5220                       ; @BT 11 20 52
    LD (DE'),A'                        ; @BT 12
    LD D',H'                           ; @BT 54
    LD D',(HL')                        ; @BT 56
    LD D',IXH                          ; @BT DD 54
    LD D',IXL                          ; @BT DD 55
    LD D',(IX+$26)                     ; @BT DD 56 26
    LD D',IYH                          ; @BT FD 54
    LD D',IYL                          ; @BT FD 55
    LD D',(IY+$2F)                     ; @BT FD 56 2F
    LD D',L'                           ; @BT 55
    LD D',$02                          ; @BT 16 02
    LD E',A'                           ; @BT 5F
    LD E',B'                           ; @BT 58
    LD E',C'                           ; @BT 59
    LD E',D'                           ; @BT 5A
    LD E',E'                           ; @BT 5B
    LD E',H'                           ; @BT 5C
    LD E',(HL')                        ; @BT 5E
    LD E',IXH                          ; @BT DD 5C
    LD E',IXL                          ; @BT DD 5D
    LD E',(IX+$53)                     ; @BT DD 5E 53
    LD E',IYH                          ; @BT FD 5C
    LD E',IYL                          ; @BT FD 5D
    LD E',(IY+$5C)                     ; @BT FD 5E 5C
    LD E',L'                           ; @BT 5D
    LD E',$5B                          ; @BT 1E 5B
    LD H',A'                           ; @BT 67
    LD H',B'                           ; @BT 60
    LD H',C'                           ; @BT 61
    LD H',D'                           ; @BT 62
    LD H',E'                           ; @BT 63
    LD H',H'                           ; @BT 64
    LD H',(HL')                        ; @BT 66
    LD H',(IX+$7A)                     ; @BT DD 66 7A
    LD H',(IY+$7D)                     ; @BT FD 66 7D
    LD H',L'                           ; @BT 65
    LD H',$58                          ; @BT 26 58
    LD HL',($7AA6)                     ; @BT 2A A6 7A
    LD HL',$7BB7                       ; @BT 21 B7 7B
    LD (HL'),A'                        ; @BT 77
    LD (HL'),B'                        ; @BT 70
    LD (HL'),C'                        ; @BT 71
    LD (HL'),D'                        ; @BT 72
    LD (HL'),E'                        ; @BT 73
    LD (HL'),H'                        ; @BT 74
    LD (HL'),L'                        ; @BT 75
    LD (HL'),$3E                       ; @BT 36 3E
    LD L',A'                           ; @BT 6F
    LD L',B'                           ; @BT 68
    LD L',C'                           ; @BT 69
    LD L',D'                           ; @BT 6A
    LD L',E'                           ; @BT 6B
    LD L',H'                           ; @BT 6C
    LD L',(HL')                        ; @BT 6E
    LD L',(IX+$39)                     ; @BT DD 6E 39
    LD L',(IY+$3C)                     ; @BT FD 6E 3C
    LD L',L'                           ; @BT 6D
    LD L',$3B                          ; @BT 2E 3B
    LD R,A'                            ; @BT ED 4F
    LD SP,HL'                          ; @BT F9
    LD SP,IX                           ; @BT DD F9
    LD SP,IY                           ; @BT FD F9
    LD SP,($954F)                      ; @BT ED 7B 4F 95
    LD SP,$9660                        ; @BT 31 60 96
    LD I,A'                            ; @BT ED 47
    LD IX,($9882)                      ; @BT DD 2A 82 98
    LD IX,$9993                        ; @BT DD 21 93 99
    LD IXH,A'                          ; @BT DD 67
    LD IXH,B'                          ; @BT DD 60
    LD IXH,C'                          ; @BT DD 61
    LD IXH,D'                          ; @BT DD 62
    LD IXH,E'                          ; @BT DD 63
    LD IXH,IXH                         ; @BT DD 64
    LD IXH,IXL                         ; @BT DD 65
    LD IXH,$C2                         ; @BT DD 26 C2
    LD IXL,A'                          ; @BT DD 6F
    LD IXL,B'                          ; @BT DD 68
    LD IXL,C'                          ; @BT DD 69
    LD IXL,D'                          ; @BT DD 6A
    LD IXL,E'                          ; @BT DD 6B
    LD IXL,IXH                         ; @BT DD 6C
    LD IXL,IXL                         ; @BT DD 6D
    LD IXL,$7A                         ; @BT DD 2E 7A
    LD (IX+$10),A'                     ; @BT DD 77 10
    LD (IX+$13),B'                     ; @BT DD 70 13
    LD (IX+$16),C'                     ; @BT DD 71 16
    LD (IX+$19),D'                     ; @BT DD 72 19
    LD (IX+$1C),E'                     ; @BT DD 73 1C
    LD (IX+$1F),H'                     ; @BT DD 74 1F
    LD (IX+$22),L'                     ; @BT DD 75 22
    LD (IX+$25),$32                    ; @BT DD 36 25 32
    LD IY,($B43C)                      ; @BT FD 2A 3C B4
    LD IY,$B54D                        ; @BT FD 21 4D B5
    LD IYH,A'                          ; @BT FD 67
    LD IYH,B'                          ; @BT FD 60
    LD IYH,C'                          ; @BT FD 61
    LD IYH,D'                          ; @BT FD 62
    LD IYH,E'                          ; @BT FD 63
    LD IYH,IYH                         ; @BT FD 64
    LD IYH,IYL                         ; @BT FD 65
    LD IYH,$18                         ; @BT FD 26 18
    LD IYL,A'                          ; @BT FD 6F
    LD IYL,B'                          ; @BT FD 68
    LD IYL,C'                          ; @BT FD 69
    LD IYL,D'                          ; @BT FD 6A
    LD IYL,E'                          ; @BT FD 6B
    LD IYL,IYH                         ; @BT FD 6C
    LD IYL,IYL                         ; @BT FD 6D
    LD IYL,$D0                         ; @BT FD 2E D0
    LD (IY+$5E),A'                     ; @BT FD 77 5E
    LD (IY+$61),B'                     ; @BT FD 70 61
    LD (IY+$64),C'                     ; @BT FD 71 64
    LD (IY+$67),D'                     ; @BT FD 72 67
    LD (IY+$6A),E'                     ; @BT FD 73 6A
    LD (IY+$6D),H'                     ; @BT FD 74 6D
    LD (IY+$70),L'                     ; @BT FD 75 70
    LD (IY+$73),$88                    ; @BT FD 36 73 88
    LD ($CFF6),A'                      ; @BT 32 F6 CF
    LD ($D107),BC'                     ; @BT ED 43 07 D1
    LD ($D218),DE'                     ; @BT ED 53 18 D2
    LD ($D329),HL'                     ; @BT 22 29 D3
    LD ($D43A),SP                      ; @BT ED 73 3A D4
    LD ($D54B),IY                      ; @BT FD 22 4B D5
    LD ($D65C),IX                      ; @BT DD 22 5C D6
    LDDR                               ; @BT ED B8
    LDD                                ; @BT ED A8
    LDIR                               ; @BT ED B0
    LDI                                ; @BT ED A0
    NEG                                ; @BT ED 44
    NOP                                ; @BT 00
    OR A',A'                           ; @BT B7
    OR A',B'                           ; @BT B0
    OR A',C'                           ; @BT B1
    OR A',D'                           ; @BT B2
    OR A',E'                           ; @BT B3
    OR A',H'                           ; @BT B4
    OR A',(HL')                        ; @BT B6
    OR A',IXH                          ; @BT DD B4
    OR A',IXL                          ; @BT DD B5
    OR A',(IX+$38)                     ; @BT DD B6 38
    OR A',IYH                          ; @BT FD B4
    OR A',IYL                          ; @BT FD B5
    OR A',(IY+$41)                     ; @BT FD B6 41
    OR A',L'                           ; @BT B5
    OR A',$0C                          ; @BT F6 0C
    OTDR                               ; @BT ED BB
    OTIR                               ; @BT ED B3
    OUT (C'),0                         ; @BT ED 71
    OUT (C'),A'                        ; @BT ED 79
    OUT (C'),B'                        ; @BT ED 41
    OUT (C'),C'                        ; @BT ED 49
    OUT (C'),D'                        ; @BT ED 51
    OUT (C'),E'                        ; @BT ED 59
    OUT (C'),H'                        ; @BT ED 61
    OUT (C'),L'                        ; @BT ED 69
    OUT ($09),A'                       ; @BT D3 09
    OUTD                               ; @BT ED AB
    OUTI                               ; @BT ED A3
    POP AF'                            ; @BT F1
    POP BC'                            ; @BT C1
    POP DE'                            ; @BT D1
    POP HL'                            ; @BT E1
    POP IX                             ; @BT DD E1
    POP IY                             ; @BT FD E1
    PUSH AF'                           ; @BT F5
    PUSH BC'                           ; @BT C5
    PUSH DE'                           ; @BT D5
    PUSH HL'                           ; @BT E5
    PUSH IX                            ; @BT DD E5
    PUSH IY                            ; @BT FD E5
    RES 0,A'                           ; @BT CB 87
    RES 0,B'                           ; @BT CB 80
    RES 0,C'                           ; @BT CB 81
    RES 0,D'                           ; @BT CB 82
    RES 0,E'                           ; @BT CB 83
    RES 0,H'                           ; @BT CB 84
    RES 0,(HL')                        ; @BT CB 86
    RES 0,(IX+$5C)                     ; @BT DD CB 5C 86
    RES 0,(IY+$5F)                     ; @BT FD CB 5F 86
    RES 0,L'                           ; @BT CB 85
    RES 1,A'                           ; @BT CB 8F
    RES 1,B'                           ; @BT CB 88
    RES 1,C'                           ; @BT CB 89
    RES 1,D'                           ; @BT CB 8A
    RES 1,E'                           ; @BT CB 8B
    RES 1,H'                           ; @BT CB 8C
    RES 1,(HL')                        ; @BT CB 8E
    RES 1,(IX+$5C)                     ; @BT DD CB 5C 8E
    RES 1,(IY+$5F)                     ; @BT FD CB 5F 8E
    RES 1,L'                           ; @BT CB 8D
    RES 2,A'                           ; @BT CB 97
    RES 2,B'                           ; @BT CB 90
    RES 2,C'                           ; @BT CB 91
    RES 2,D'                           ; @BT CB 92
    RES 2,E'                           ; @BT CB 93
    RES 2,H'                           ; @BT CB 94
    RES 2,(HL')                        ; @BT CB 96
    RES 2,(IX+$5C)                     ; @BT DD CB 5C 96
    RES 2,(IY+$5F)                     ; @BT FD CB 5F 96
    RES 2,L'                           ; @BT CB 95
    RES 3,A'                           ; @BT CB 9F
    RES 3,B'                           ; @BT CB 98
    RES 3,C'                           ; @BT CB 99
    RES 3,D'                           ; @BT CB 9A
    RES 3,E'                           ; @BT CB 9B
    RES 3,H'                           ; @BT CB 9C
    RES 3,(HL')                        ; @BT CB 9E
    RES 3,(IX+$5C)                     ; @BT DD CB 5C 9E
    RES 3,(IY+$5F)                     ; @BT FD CB 5F 9E
    RES 3,L'                           ; @BT CB 9D
    RES 4,A'                           ; @BT CB A7
    RES 4,B'                           ; @BT CB A0
    RES 4,C'                           ; @BT CB A1
    RES 4,D'                           ; @BT CB A2
    RES 4,E'                           ; @BT CB A3
    RES 4,H'                           ; @BT CB A4
    RES 4,(HL')                        ; @BT CB A6
    RES 4,(IX+$5C)                     ; @BT DD CB 5C A6
    RES 4,(IY+$5F)                     ; @BT FD CB 5F A6
    RES 4,L'                           ; @BT CB A5
    RES 5,A'                           ; @BT CB AF
    RES 5,B'                           ; @BT CB A8
    RES 5,C'                           ; @BT CB A9
    RES 5,D'                           ; @BT CB AA
    RES 5,E'                           ; @BT CB AB
    RES 5,H'                           ; @BT CB AC
    RES 5,(HL')                        ; @BT CB AE
    RES 5,(IX+$5C)                     ; @BT DD CB 5C AE
    RES 5,(IY+$5F)                     ; @BT FD CB 5F AE
    RES 5,L'                           ; @BT CB AD
    RES 6,A'                           ; @BT CB B7
    RES 6,B'                           ; @BT CB B0
    RES 6,C'                           ; @BT CB B1
    RES 6,D'                           ; @BT CB B2
    RES 6,E'                           ; @BT CB B3
    RES 6,H'                           ; @BT CB B4
    RES 6,(HL')                        ; @BT CB B6
    RES 6,(IX+$5C)                     ; @BT DD CB 5C B6
    RES 6,(IY+$5F)                     ; @BT FD CB 5F B6
    RES 6,L'                           ; @BT CB B5
    RES 7,A'                           ; @BT CB BF
    RES 7,B'                           ; @BT CB B8
    RES 7,C'                           ; @BT CB B9
    RES 7,D'                           ; @BT CB BA
    RES 7,E'                           ; @BT CB BB
    RES 7,H'                           ; @BT CB BC
    RES 7,(HL')                        ; @BT CB BE
    RES 7,(IX+$5C)                     ; @BT DD CB 5C BE
    RES 7,(IY+$5F)                     ; @BT FD CB 5F BE
    RES 7,L'                           ; @BT CB BD
    RET LSS                            ; @BT D8
    RET NEG                            ; @BT F8
    RET GTE                            ; @BT D0
    RET NEQ                            ; @BT C0
    RET POS                            ; @BT F0
    RET EQU                            ; @BT C8
    RET PE                             ; @BT E8
    RET PO                             ; @BT E0
    RET                                ; @BT C9
    RETI                               ; @BT ED 4D
    RETN                               ; @BT ED 45
    RL A'                              ; @BT CB 17
    RL B'                              ; @BT CB 10
    RL C'                              ; @BT CB 11
    RL D'                              ; @BT CB 12
    RL E'                              ; @BT CB 13
    RL H'                              ; @BT CB 14
    RL (HL')                           ; @BT CB 16
    RL (IX+$5C)                        ; @BT DD CB 5C 16
    RL (IY+$5F)                        ; @BT FD CB 5F 16
    RL L'                              ; @BT CB 15
    RLC A'                             ; @BT CB 07
    RLC B'                             ; @BT CB 00
    RLC C'                             ; @BT CB 01
    RLC D'                             ; @BT CB 02
    RLC E'                             ; @BT CB 03
    RLC H'                             ; @BT CB 04
    RLC (HL')                          ; @BT CB 06
    RLC (IX+$5C)                       ; @BT DD CB 5C 06
    RLC (IY+$5F)                       ; @BT FD CB 5F 06
    RLC L'                             ; @BT CB 05
    RLCA                               ; @BT 07
    RLA                                ; @BT 17
    RLD                                ; @BT ED 6F
    RR A'                              ; @BT CB 1F
    RR B'                              ; @BT CB 18
    RR C'                              ; @BT CB 19
    RR D'                              ; @BT CB 1A
    RR E'                              ; @BT CB 1B
    RR H'                              ; @BT CB 1C
    RR (HL')                           ; @BT CB 1E
    RR (IX+$5C)                        ; @BT DD CB 5C 1E
    RR (IY+$5F)                        ; @BT FD CB 5F 1E
    RR L'                              ; @BT CB 1D
    RRA                                ; @BT 1F
    RRC A'                             ; @BT CB 0F
    RRC B'                             ; @BT CB 08
    RRC C'                             ; @BT CB 09
    RRC D'                             ; @BT CB 0A
    RRC E'                             ; @BT CB 0B
    RRC H'                             ; @BT CB 0C
    RRC (HL')                          ; @BT CB 0E
    RRC (IX+$5C)                       ; @BT DD CB 5C 0E
    RRC (IY+$5F)                       ; @BT FD CB 5F 0E
    RRC L'                             ; @BT CB 0D
    RRCA                               ; @BT 0F
    RRD                                ; @BT ED 67
    RST $00                            ; @BT C7
    RST $08                            ; @BT CF
    RST $10                            ; @BT D7
    RST $18                            ; @BT DF
    RST $20                            ; @BT E7
    RST $28                            ; @BT EF
    RST $30                            ; @BT F7
    RST $38                            ; @BT FF
    SBC A',A'                          ; @BT 9F
    SBC A',B'                          ; @BT 98
    SBC A',C'                          ; @BT 99
    SBC A',D'                          ; @BT 9A
    SBC A',E'                          ; @BT 9B
    SBC A',H'                          ; @BT 9C
    SBC A',(HL')                       ; @BT 9E
    SBC A',IXH                         ; @BT DD 9C
    SBC A',IXL                         ; @BT DD 9D
    SBC A',(IX+$1B)                    ; @BT DD 9E 1B
    SBC A',IYH                         ; @BT FD 9C
    SBC A',IYL                         ; @BT FD 9D
    SBC A',(IY+$24)                    ; @BT FD 9E 24
    SBC A',L'                          ; @BT 9D
    SBC A',$83                         ; @BT DE 83
    SBC HL',BC'                        ; @BT ED 42
    SBC HL',DE'                        ; @BT ED 52
    SBC HL',HL'                        ; @BT ED 62
    SBC HL',SP                         ; @BT ED 72
    SCF                                ; @BT 37
    SET 0,A'                           ; @BT CB C7
    SET 0,B'                           ; @BT CB C0
    SET 0,C'                           ; @BT CB C1
    SET 0,D'                           ; @BT CB C2
    SET 0,E'                           ; @BT CB C3
    SET 0,H'                           ; @BT CB C4
    SET 0,(HL')                        ; @BT CB C6
    SET 0,(IX+$5C)                     ; @BT DD CB 5C C6
    SET 0,(IY+$5F)                     ; @BT FD CB 5F C6
    SET 0,L'                           ; @BT CB C5
    SET 1,A'                           ; @BT CB CF
    SET 1,B'                           ; @BT CB C8
    SET 1,C'                           ; @BT CB C9
    SET 1,D'                           ; @BT CB CA
    SET 1,E'                           ; @BT CB CB
    SET 1,H'                           ; @BT CB CC
    SET 1,(HL')                        ; @BT CB CE
    SET 1,(IX+$5C)                     ; @BT DD CB 5C CE
    SET 1,(IY+$5F)                     ; @BT FD CB 5F CE
    SET 1,L'                           ; @BT CB CD
    SET 2,A'                           ; @BT CB D7
    SET 2,B'                           ; @BT CB D0
    SET 2,C'                           ; @BT CB D1
    SET 2,D'                           ; @BT CB D2
    SET 2,E'                           ; @BT CB D3
    SET 2,H'                           ; @BT CB D4
    SET 2,(HL')                        ; @BT CB D6
    SET 2,(IX+$5C)                     ; @BT DD CB 5C D6
    SET 2,(IY+$5F)                     ; @BT FD CB 5F D6
    SET 2,L'                           ; @BT CB D5
    SET 3,A'                           ; @BT CB DF
    SET 3,B'                           ; @BT CB D8
    SET 3,C'                           ; @BT CB D9
    SET 3,D'                           ; @BT CB DA
    SET 3,E'                           ; @BT CB DB
    SET 3,H'                           ; @BT CB DC
    SET 3,(HL')                        ; @BT CB DE
    SET 3,(IX+$5C)                     ; @BT DD CB 5C DE
    SET 3,(IY+$5F)                     ; @BT FD CB 5F DE
    SET 3,L'                           ; @BT CB DD
    SET 4,A'                           ; @BT CB E7
    SET 4,B'                           ; @BT CB E0
    SET 4,C'                           ; @BT CB E1
    SET 4,D'                           ; @BT CB E2
    SET 4,E'                           ; @BT CB E3
    SET 4,H'                           ; @BT CB E4
    SET 4,(HL')                        ; @BT CB E6
    SET 4,(IX+$5C)                     ; @BT DD CB 5C E6
    SET 4,(IY+$5F)                     ; @BT FD CB 5F E6
    SET 4,L'                           ; @BT CB E5
    SET 5,A'                           ; @BT CB EF
    SET 5,B'                           ; @BT CB E8
    SET 5,C'                           ; @BT CB E9
    SET 5,D'                           ; @BT CB EA
    SET 5,E'                           ; @BT CB EB
    SET 5,H'                           ; @BT CB EC
    SET 5,(HL')                        ; @BT CB EE
    SET 5,(IX+$5C)                     ; @BT DD CB 5C EE
    SET 5,(IY+$5F)                     ; @BT FD CB 5F EE
    SET 5,L'                           ; @BT CB ED
    SET 6,A'                           ; @BT CB F7
    SET 6,B'                           ; @BT CB F0
    SET 6,C'                           ; @BT CB F1
    SET 6,D'                           ; @BT CB F2
    SET 6,E'                           ; @BT CB F3
    SET 6,H'                           ; @BT CB F4
    SET 6,(HL')                        ; @BT CB F6
    SET 6,(IX+$5C)                     ; @BT DD CB 5C F6
    SET 6,(IY+$5F)                     ; @BT FD CB 5F F6
    SET 6,L'                           ; @BT CB F5
    SET 7,A'                           ; @BT CB FF
    SET 7,B'                           ; @BT CB F8
    SET 7,C'                           ; @BT CB F9
    SET 7,D'                           ; @BT CB FA
    SET 7,E'                           ; @BT CB FB
    SET 7,H'                           ; @BT CB FC
    SET 7,(HL')                        ; @BT CB FE
    SET 7,(IX+$5C)                     ; @BT DD CB 5C FE
    SET 7,(IY+$5F)                     ; @BT FD CB 5F FE
    SET 7,L'                           ; @BT CB FD
    SL1 A'                             ; @BT CB 37
    SL1 B'                             ; @BT CB 30
    SL1 C'                             ; @BT CB 31
    SL1 D'                             ; @BT CB 32
    SL1 E'                             ; @BT CB 33
    SL1 H'                             ; @BT CB 34
    SL1 (HL')                          ; @BT CB 36
    SL1 (IX+$5C)                       ; @BT DD CB 5C 36
    SL1 (IY+$5F)                       ; @BT FD CB 5F 36
    SL1 L'                             ; @BT CB 35
    SLA A'                             ; @BT CB 27
    SLA B'                             ; @BT CB 20
    SLA C'                             ; @BT CB 21
    SLA D'                             ; @BT CB 22
    SLA E'                             ; @BT CB 23
    SLA H'                             ; @BT CB 24
    SLA (HL')                          ; @BT CB 26
    SLA (IX+$5C)                       ; @BT DD CB 5C 26
    SLA (IY+$5F)                       ; @BT FD CB 5F 26
    SLA L'                             ; @BT CB 25
    SRA A'                             ; @BT CB 2F
    SRA B'                             ; @BT CB 28
    SRA C'                             ; @BT CB 29
    SRA D'                             ; @BT CB 2A
    SRA E'                             ; @BT CB 2B
    SRA H'                             ; @BT CB 2C
    SRA (HL')                          ; @BT CB 2E
    SRA (IX+$5C)                       ; @BT DD CB 5C 2E
    SRA (IY+$5F)                       ; @BT FD CB 5F 2E
    SRA L'                             ; @BT CB 2D
    SRL A'                             ; @BT CB 3F
    SRL B'                             ; @BT CB 38
    SRL C'                             ; @BT CB 39
    SRL D'                             ; @BT CB 3A
    SRL E'                             ; @BT CB 3B
    SRL H'                             ; @BT CB 3C
    SRL (HL')                          ; @BT CB 3E
    SRL (IX+$5C)                       ; @BT DD CB 5C 3E
    SRL (IY+$5F)                       ; @BT FD CB 5F 3E
    SRL L'                             ; @BT CB 3D
    SUB A',A'                          ; @BT 97
    SUB A',B'                          ; @BT 90
    SUB A',C'                          ; @BT 91
    SUB A',D'                          ; @BT 92
    SUB A',E'                          ; @BT 93
    SUB A',H'                          ; @BT 94
    SUB A',L'                          ; @BT 95
    SUB A',IXH                         ; @BT DD 94
    SUB A',IXL                         ; @BT DD 95
    SUB A',(IX+$77)                    ; @BT DD 96 77
    SUB A',(IY+$7A)                    ; @BT FD 96 7A
    SUB A',(HL')                       ; @BT 96
    SUB A',IYH                         ; @BT FD 94
    SUB A',IYL                         ; @BT FD 95
    SUB A',$EF                         ; @BT D6 EF
    XOR A',A'                          ; @BT AF
    XOR A',B'                          ; @BT A8
    XOR A',C'                          ; @BT A9
    XOR A',D'                          ; @BT AA
    XOR A',E'                          ; @BT AB
    XOR A',H'                          ; @BT AC
    XOR A',(HL')                       ; @BT AE
    XOR A',IXH                         ; @BT DD AC
    XOR A',IXL                         ; @BT DD AD
    XOR A',(IX+$24)                    ; @BT DD AE 24
    XOR A',IYH                         ; @BT FD AC
    XOR A',IYL                         ; @BT FD AD
    XOR A',(IY+$2D)                    ; @BT FD AE 2D
    XOR A',L'                          ; @BT AD
    XOR A',$48                         ; @BT EE 48
    .db "<01"               ; @BT END

    .BANK 1 SLOT 0
    .ORG 0

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 1585821622

        XOR A',B'                      ; @BT A8
        POP IX                         ; @BT DD E1
        LD A',(IY+$10)                 ; @BT FD 7E 10
        SET 4,E'                       ; @BT CB E3
        INIR                           ; @BT ED B2
        AND A',IYL                     ; @BT FD A5
        CP A',B'                       ; @BT B8
        XOR A',A'                      ; @BT AF
        ADC A',H'                      ; @BT 8C
        INC E'                         ; @BT 1C
        JP PE,$0AAD                    ; @BT EA AD 0A
        SET 3,C'                       ; @BT CB D9
        ADD HL',DE'                    ; @BT 19
        LD B',IYH                      ; @BT FD 44
        SET 0,A'                       ; @BT CB C7
        LD D',(HL')                    ; @BT 56
        OUT (C'),D'                    ; @BT ED 51
        IN E',(C')                     ; @BT ED 58
        LD ($D329),HL'                 ; @BT 22 29 D3
        LD B',D'                       ; @BT 42
        LD L',D'                       ; @BT 6A
        XOR A',IXL                     ; @BT DD AD
        RST $08                        ; @BT CF
        DEC D'                         ; @BT 15
        CCF                            ; @BT 3F
        JP POS,$088B                   ; @BT F2 8B 08
        RR E'                          ; @BT CB 1B
        SET 5,A'                       ; @BT CB EF
        SL1 E'                         ; @BT CB 33
        LD A',A'                       ; @BT 7F
        OUTD                           ; @BT ED AB
        SET 3,D'                       ; @BT CB DA
        LDD                            ; @BT ED A8
        LD IXL,C'                      ; @BT DD 69
        LD E',H'                       ; @BT 5C
        RES 6,B'                       ; @BT CB B0
        SET 1,(HL')                    ; @BT CB CE
        SBC A',(HL')                   ; @BT 9E
        SRA C'                         ; @BT CB 29
        JP LSS,$0447                   ; @BT DA 47 04
        DEC SP                         ; @BT 3B
        SRA E'                         ; @BT CB 2B
        RES 5,H'                       ; @BT CB AC
        LD A',B'                       ; @BT 78
        LD (HL'),B'                    ; @BT 70
        SET 3,E'                       ; @BT CB DB
        XOR A',IXH                     ; @BT DD AC
        RES 5,A'                       ; @BT CB AF
        DEC E'                         ; @BT 1D
        SET 7,E'                       ; @BT CB FB
        EX (SP),IX                     ; @BT DD E3
        SUB A',B'                      ; @BT 90
        JR NEQ,$73                     ; @BT 20 73
        RES 7,A'                       ; @BT CB BF
        RES 4,L'                       ; @BT CB A5
        BIT 6,(HL')                    ; @BT CB 76
        SET 0,L'                       ; @BT CB C5
        LD IXH,$C2                     ; @BT DD 26 C2
        RES 7,C'                       ; @BT CB B9
        RES 7,(HL')                    ; @BT CB BE
        ADC A',IXH                     ; @BT DD 8C
        RLD                            ; @BT ED 6F
        RR C'                          ; @BT CB 19
        SET 6,E'                       ; @BT CB F3
        INC A'                         ; @BT 3C
        CP A',(HL')                    ; @BT BE
        LD E',IYL                      ; @BT FD 5D
        BIT 5,C'                       ; @BT CB 69
        LD A',IYH                      ; @BT FD 7C
        JR GTE,$5C                     ; @BT 30 5C
        RLC L'                         ; @BT CB 05
        LD (IY+$6D),H'                 ; @BT FD 74 6D
        LD (HL'),$3E                   ; @BT 36 3E
        SL1 (HL')                      ; @BT CB 36
        LD L',A'                       ; @BT 6F
        LD HL',$7BB7                   ; @BT 21 B7 7B
        SET 5,C'                       ; @BT CB E9
        LD (IY+$73),$88                ; @BT FD 36 73 88
        LD D',IYL                      ; @BT FD 55
        RES 0,D'                       ; @BT CB 82
        DEC IYH                        ; @BT FD 25
        SET 4,L'                       ; @BT CB E5
        SLA H'                         ; @BT CB 24
        INC DE'                        ; @BT 13
        LD D',E'                       ; @BT 53
        SET 6,A'                       ; @BT CB F7
        LD IXH,IXL                     ; @BT DD 65
        LD (IX+$25),$32                ; @BT DD 36 25 32
        LD C',(IX+$70)                 ; @BT DD 4E 70
        LD A',R                        ; @BT ED 5F
        RES 4,(HL')                    ; @BT CB A6
        LD H',B'                       ; @BT 60
        RES 1,H'                       ; @BT CB 8C
        SL1 D'                         ; @BT CB 32
        LD D',IXL                      ; @BT DD 55
        BIT 0,H'                       ; @BT CB 44
        SET 1,A'                       ; @BT CB CF
        ADD A',IYH                     ; @BT FD 84
        LD E',IXH                      ; @BT DD 5C
        RET GTE                        ; @BT D0
        LD D',D'                       ; @BT 52
        SUB A',IXH                     ; @BT DD 94
        SUB A',C'                      ; @BT 91
        ADD IX,IX                      ; @BT DD 29
        JP GTE,$0669                   ; @BT D2 69 06
        LD E',C'                       ; @BT 59
        OR A',D'                       ; @BT B2
        BIT 0,B'                       ; @BT CB 40
        RST $10                        ; @BT D7
        LD H',L'                       ; @BT 65
        BIT 6,C'                       ; @BT CB 71
        RL B'                          ; @BT CB 10
        SET 1,D'                       ; @BT CB CA
        BIT 5,L'                       ; @BT CB 6D
        EX (SP),IY                     ; @BT FD E3
        BIT 4,H'                       ; @BT CB 64
        SET 2,D'                       ; @BT CB D2
        JP (HL')                       ; @BT E9
        RES 1,(HL')                    ; @BT CB 8E
        RES 1,C'                       ; @BT CB 89
        RET PE                         ; @BT E8
        SET 4,A'                       ; @BT CB E7
        ADC A',(HL')                   ; @BT 8E
        SRL D'                         ; @BT CB 3A
        SET 1,B'                       ; @BT CB C8
        RLC C'                         ; @BT CB 01
        RRC A'                         ; @BT CB 0F
        BIT 6,H'                       ; @BT CB 74
        SET 7,A'                       ; @BT CB FF
        CALL PO,$A11A                  ; @BT E4 1A A1
        SBC A',C'                      ; @BT 99
        SRA H'                         ; @BT CB 2C
        SRA (HL')                      ; @BT CB 2E
        BIT 4,E'                       ; @BT CB 63
        AND A',E'                      ; @BT A3
        LD A',C'                       ; @BT 79
        SL1 L'                         ; @BT CB 35
        SET 5,H'                       ; @BT CB EC
        LD IXL,D'                      ; @BT DD 6A
        SL1 H'                         ; @BT CB 34
        LD D',B'                       ; @BT 50
        LD C',IXL                      ; @BT DD 4D
        RR (HL')                       ; @BT CB 1E
        RES 5,(HL')                    ; @BT CB AE
        BIT 1,B'                       ; @BT CB 48
        RLC B'                         ; @BT CB 00
        RRC E'                         ; @BT CB 0B
        JR LSS,$45                     ; @BT 38 45
        DEC C'                         ; @BT 0D
        DEC IYL                        ; @BT FD 2D
        DJNZ $26                       ; @BT 10 26
        RES 7,H'                       ; @BT CB BC
        LD (HL'),A'                    ; @BT 77
        OUT (C'),H'                    ; @BT ED 61
        LD A',L'                       ; @BT 7D
        RRC H'                         ; @BT CB 0C
        SUB A',(HL')                   ; @BT 96
        LD H',(HL')                    ; @BT 66
        ADC A',E'                      ; @BT 8B
        DEC HL'                        ; @BT 2B
        ADD A',B'                      ; @BT 80
        INC B'                         ; @BT 04
        LD (IX+$16),C'                 ; @BT DD 71 16
        IN L',(C')                     ; @BT ED 68
        BIT 7,(HL')                    ; @BT CB 7E
        XOR A',IYL                     ; @BT FD AD
        AND A',IXL                     ; @BT DD A5
        CP A',IXH                      ; @BT DD BC
        RST $38                        ; @BT FF
        CP A',A'                       ; @BT BF
        ADD HL',SP                     ; @BT 39
        LD L',C'                       ; @BT 69
        RES 6,H'                       ; @BT CB B4
        XOR A',D'                      ; @BT AA
        SUB A',IXL                     ; @BT DD 95
        RES 7,L'                       ; @BT CB BD
        PUSH IY                        ; @BT FD E5
        SBC A',(IX+$1B)                ; @BT DD 9E 1B
        RET POS                        ; @BT F0
        BIT 0,(HL')                    ; @BT CB 46
        LD D',A'                       ; @BT 57
        LD E',(IX+$53)                 ; @BT DD 5E 53
        LD IXH,D'                      ; @BT DD 62
        CP A',D'                       ; @BT BA
        SBC A',$83                     ; @BT DE 83
        RR D'                          ; @BT CB 1A
        OR A',IYL                      ; @BT FD B5
        LD (HL'),L'                    ; @BT 75
        OR A',E'                       ; @BT B3
        RES 6,D'                       ; @BT CB B2
        OR A',IXL                      ; @BT DD B5
        BIT 6,D'                       ; @BT CB 72
        OTDR                           ; @BT ED BB
        SRL A'                         ; @BT CB 3F
        BIT 6,E'                       ; @BT CB 73
        ADD A',(HL')                   ; @BT 86
        BIT 2,C'                       ; @BT CB 51
        LD E',IXL                      ; @BT DD 5D
        RRC C'                         ; @BT CB 09
        LD IXH,C'                      ; @BT DD 61
        LD DE',($510F)                 ; @BT ED 5B 0F 51
        IM 2                           ; @BT ED 5E
        SET 1,H'                       ; @BT CB CC
        ADC A',IXL                     ; @BT DD 8D
        SRA L'                         ; @BT CB 2D
        DEC A'                         ; @BT 3D
        RES 3,D'                       ; @BT CB 9A
        AND A',(IY+$3E)                ; @BT FD A6 3E
        LD D',IXH                      ; @BT DD 54
        LD A',E'                       ; @BT 7B
        ADC HL',SP                     ; @BT ED 7A
        BIT 7,L'                       ; @BT CB 7D
        LD H',$58                      ; @BT 26 58
        LD C',C'                       ; @BT 49
        LD IXH,E'                      ; @BT DD 63
        BIT 5,E'                       ; @BT CB 6B
        POP AF'                        ; @BT F1
        LD C',L'                       ; @BT 4D
        RET LSS                        ; @BT D8
        CALL NEQ,$9CD6                 ; @BT C4 D6 9C
        INC HL'                        ; @BT 23
        ADC HL',DE'                    ; @BT ED 5A
        BIT 2,H'                       ; @BT CB 54
        SUB A',(IY+$7A)                ; @BT FD 96 7A
        SUB A',D'                      ; @BT 92
        LD B',B'                       ; @BT 40
        LD IYH,IYH                     ; @BT FD 64
        BIT 7,H'                       ; @BT CB 7C
        BIT 5,B'                       ; @BT CB 68
        SL1 C'                         ; @BT CB 31
        RST $30                        ; @BT F7
        BIT 3,C'                       ; @BT CB 59
        POP BC'                        ; @BT C1
        LD A',IXL                      ; @BT DD 7D
        IN B',(C')                     ; @BT ED 40
        LD (DE'),A'                    ; @BT 12
        RST $00                        ; @BT C7
        LD (HL'),D'                    ; @BT 72
        LD A',H'                       ; @BT 7C
        AND A',$75                     ; @BT E6 75
        INDR                           ; @BT ED BA
        RES 1,D'                       ; @BT CB 8A
        OUT (C'),C'                    ; @BT ED 49
        LD B',IXL                      ; @BT DD 45
        NOP                            ; @BT 00
        OR A',A'                       ; @BT B7
        LD H',E'                       ; @BT 63
        LD IYL,IYL                     ; @BT FD 6D
        SET 5,(HL')                    ; @BT CB EE
        SRA B'                         ; @BT CB 28
        SET 0,C'                       ; @BT CB C1
        CPL                            ; @BT 2F
        IM 0                           ; @BT ED 46
        LD SP,$9660                    ; @BT 31 60 96
        ADC HL',BC'                    ; @BT ED 4A
        SET 2,(HL')                    ; @BT CB D6
        LD ($D218),DE'                 ; @BT ED 53 18 D2
        RES 4,C'                       ; @BT CB A1
        SET 3,A'                       ; @BT CB DF
        RLC H'                         ; @BT CB 04
        SRA A'                         ; @BT CB 2F
        LDDR                           ; @BT ED B8
        INC SP                         ; @BT 33
        BIT 5,D'                       ; @BT CB 6A
        BIT 4,L'                       ; @BT CB 65
        ADD A',H'                      ; @BT 84
        RRC L'                         ; @BT CB 0D
        JP PO,$0BBE                    ; @BT E2 BE 0B
        RL (HL')                       ; @BT CB 16
        LD C',A'                       ; @BT 4F
        SET 0,B'                       ; @BT CB C0
        SLA L'                         ; @BT CB 25
        EX DE',HL'                     ; @BT EB
        SUB A',IYH                     ; @BT FD 94
        PUSH IX                        ; @BT DD E5
        SBC HL',DE'                    ; @BT ED 52
        RES 5,E'                       ; @BT CB AB
        LD E',A'                       ; @BT 5F
        RST $20                        ; @BT E7
        CPDR                           ; @BT ED B9
        SUB A',H'                      ; @BT 94
        BIT 2,A'                       ; @BT CB 57
        CPD                            ; @BT ED A9
        RLC D'                         ; @BT CB 02
        LD L',$3B                      ; @BT 2E 3B
        SET 2,E'                       ; @BT CB D3
        XOR A',E'                      ; @BT AB
        LD E',$5B                      ; @BT 1E 5B
        BIT 3,L'                       ; @BT CB 5D
        RET NEG                        ; @BT F8
        BIT 4,B'                       ; @BT CB 60
        RES 2,E'                       ; @BT CB 93
        DI                             ; @BT F3
        SET 4,D'                       ; @BT CB E2
        LD E',L'                       ; @BT 5D
        SET 6,C'                       ; @BT CB F1
        LD D',$02                      ; @BT 16 02
        SBC A',L'                      ; @BT 9D
        LD IXL,B'                      ; @BT DD 68
        RES 0,L'                       ; @BT CB 85
        LD ($D65C),IX                  ; @BT DD 22 5C D6
        SET 5,L'                       ; @BT CB ED
        LD D',(IY+$2F)                 ; @BT FD 56 2F
        BIT 0,C'                       ; @BT CB 41
        BIT 1,C'                       ; @BT CB 49
        DEC L'                         ; @BT 2D
        LD IXL,IXH                     ; @BT DD 6C
        CP A',$A2                      ; @BT FE A2
        LD B',(IX+$43)                 ; @BT DD 46 43
        CP A',IYH                      ; @BT FD BC
        SCF                            ; @BT 37
        RES 4,B'                       ; @BT CB A0
        RES 3,E'                       ; @BT CB 9B
        ADD IX,SP                      ; @BT DD 39
        SET 7,L'                       ; @BT CB FD
        SET 2,A'                       ; @BT CB D7
        LD B',C'                       ; @BT 41
        SRL C'                         ; @BT CB 39
        BIT 3,A'                       ; @BT CB 5F
        JP $0CCF                       ; @BT C3 CF 0C
        LD BC',$2CCD                   ; @BT 01 CD 2C
        SET 4,(HL')                    ; @BT CB E6
        ADD A',L'                      ; @BT 85
        ADD IY,BC'                     ; @BT FD 09
        SUB A',L'                      ; @BT 95
        CP A',(IX+$46)                 ; @BT DD BE 46
        IM 1                           ; @BT ED 56
        RLC A'                         ; @BT CB 07
        LD IYL,E'                      ; @BT FD 6B
        JP NEG,$0558                   ; @BT FA 58 05
        SLA E'                         ; @BT CB 23
        LD SP,($954F)                  ; @BT ED 7B 4F 95
        LD B',L'                       ; @BT 45
        RES 2,L'                       ; @BT CB 95
        LD (IY+$67),D'                 ; @BT FD 72 67
        DEC IXL                        ; @BT DD 2D
        RES 3,H'                       ; @BT CB 9C
        LD E',IYH                      ; @BT FD 5C
        RES 0,(HL')                    ; @BT CB 86
        LD A',D'                       ; @BT 7A
        OUT (C'),L'                    ; @BT ED 69
        BIT 6,B'                       ; @BT CB 70
        RETI                           ; @BT ED 4D
        SBC A',D'                      ; @BT 9A
        LD IYL,C'                      ; @BT FD 69
        ADC A',IYH                     ; @BT FD 8C
        RR A'                          ; @BT CB 1F
        DEC (IY+$1D)                   ; @BT FD 35 1D
        CALL GTE,$9BC5                 ; @BT D4 C5 9B
        LD A',(DE')                    ; @BT 1A
        BIT 0,(IX+$5C)                 ; @BT DD CB 5C 46
        INC IYH                        ; @BT FD 24
        DEC IX                         ; @BT DD 2B
        SUB A',(IX+$77)                ; @BT DD 96 77
        IN D',(C')                     ; @BT ED 50
        AND A',C'                      ; @BT A1
        LD A',(HL')                    ; @BT 7E
        ADD IX,BC'                     ; @BT DD 09
        LD D',C'                       ; @BT 51
        RES 3,A'                       ; @BT CB 9F
        SET 1,C'                       ; @BT CB C9
        SET 2,H'                       ; @BT CB D4
        BIT 7,A'                       ; @BT CB 7F
        NEG                            ; @BT ED 44
        BIT 4,D'                       ; @BT CB 62
        ADC A',D'                      ; @BT 8A
        SET 6,B'                       ; @BT CB F0
        RES 6,E'                       ; @BT CB B3
        SLA D'                         ; @BT CB 22
        OR A',C'                       ; @BT B1
        SBC A',B'                      ; @BT 98
        SBC A',IYH                     ; @BT FD 9C
        LD (IX+$10),A'                 ; @BT DD 77 10
        OR A',H'                       ; @BT B4
        BIT 1,E'                       ; @BT CB 4B
        XOR A',$48                     ; @BT EE 48
        INC IXH                        ; @BT DD 24
        IN A',($51)                    ; @BT DB 51
        BIT 0,(IY+$5F)                 ; @BT FD CB 5F 46
        BIT 2,(HL')                    ; @BT CB 56
        SUB A',A'                      ; @BT 97
        LD (BC'),A'                    ; @BT 02
        BIT 0,D'                       ; @BT CB 42
        JP (IX)                        ; @BT DD E9
        LD H',C'                       ; @BT 61
        RES 7,D'                       ; @BT CB BA
        RES 3,B'                       ; @BT CB 98
        RLC E'                         ; @BT CB 03
        RET EQU                        ; @BT C8
        LD B',IYL                      ; @BT FD 45
        OUT (C'),E'                    ; @BT ED 59
        ADD A',IXH                     ; @BT DD 84
        LD (IY+$6A),E'                 ; @BT FD 73 6A
        RES 6,(HL')                    ; @BT CB B6
        OR A',IYH                      ; @BT FD B4
        SET 7,D'                       ; @BT CB FA
        LD L',H'                       ; @BT 6C
        LD SP,HL'                      ; @BT F9
        LD B',E'                       ; @BT 43
        CALL LSS,$99A3                 ; @BT DC A3 99
        AND A',A'                      ; @BT A7
        RET NEQ                        ; @BT C0
        INC C'                         ; @BT 0C
        RL L'                          ; @BT CB 15
        LD E',D'                       ; @BT 5A
        CALL NEG,$9AB4                 ; @BT FC B4 9A
        LD E',B'                       ; @BT 58
        RES 5,B'                       ; @BT CB A8
        LD A',($2667)                  ; @BT 3A 67 26
        BIT 5,A'                       ; @BT CB 6F
        RES 3,L'                       ; @BT CB 9D
        RRCA                           ; @BT 0F
        LD ($D107),BC'                 ; @BT ED 43 07 D1
        LD H',D'                       ; @BT 62
        RETN                           ; @BT ED 45
        OR A',L'                       ; @BT B5
        LD IYH,E'                      ; @BT FD 63
        DEC (HL')                      ; @BT 35
        LD B',(HL')                    ; @BT 46
        CP A',(IY+$4F)                 ; @BT FD BE 4F
        LD SP,IY                       ; @BT FD F9
        LD ($D54B),IY                  ; @BT FD 22 4B D5
        BIT 3,H'                       ; @BT CB 5C
        SET 4,H'                       ; @BT CB E4
        DEC (IX+$11)                   ; @BT DD 35 11
        SRL L'                         ; @BT CB 3D
        ADC A',(IX+$2B)                ; @BT DD 8E 2B
        RES 5,L'                       ; @BT CB AD
        LD IY,$B54D                    ; @BT FD 21 4D B5
        SET 6,(HL')                    ; @BT CB F6
        RR B'                          ; @BT CB 18
        SET 7,(HL')                    ; @BT CB FE
        CALL EQU,$9EF8                 ; @BT CC F8 9E
        LD IX,($9882)                  ; @BT DD 2A 82 98
        LD (HL'),E'                    ; @BT 73
        BIT 7,B'                       ; @BT CB 78
        RL E'                          ; @BT CB 13
        XOR A',(IY+$2D)                ; @BT FD AE 2D
        SET 3,(HL')                    ; @BT CB DE
        SBC A',(IY+$24)                ; @BT FD 9E 24
        BIT 0,A'                       ; @BT CB 47
        SET 5,E'                       ; @BT CB EB
        RES 3,(HL')                    ; @BT CB 9E
        LD ($CFF6),A'                  ; @BT 32 F6 CF
        OR A',B'                       ; @BT B0
        JR $A1                         ; @BT 18 A1
        RES 7,B'                       ; @BT CB B8
        RES 0,B'                       ; @BT CB 80
        CPIR                           ; @BT ED B1
        JR EQU,$8A                     ; @BT 28 8A
        IN A',(C')                     ; @BT ED 78
        CP A',IYL                      ; @BT FD BD
        POP IY                         ; @BT FD E1
        DEC DE'                        ; @BT 1B
        BIT 4,C'                       ; @BT CB 61
        POP DE'                        ; @BT D1
        AND A',(HL')                   ; @BT A6
        BIT 1,(HL')                    ; @BT CB 4E
        DEC B'                         ; @BT 05
        ADC A',C'                      ; @BT 89
        LD A',IYL                      ; @BT FD 7D
        INC BC'                        ; @BT 03
        LD IYH,D'                      ; @BT FD 62
        SET 7,C'                       ; @BT CB F9
        LD (IY+$64),C'                 ; @BT FD 71 64
        LD A',(IX+$07)                 ; @BT DD 7E 07
        LD IXH,IXH                     ; @BT DD 64
        BIT 0,L'                       ; @BT CB 45
        AND A',IYH                     ; @BT FD A4
        LD C',$64                      ; @BT 0E 64
        LD SP,IX                       ; @BT DD F9
        XOR A',C'                      ; @BT A9
        SET 3,L'                       ; @BT CB DD
        INC IY                         ; @BT FD 23
        LD D',L'                       ; @BT 55
        LD (IX+$22),L'                 ; @BT DD 75 22
        PUSH DE'                       ; @BT D5
        SRL E'                         ; @BT CB 3B
        PUSH BC'                       ; @BT C5
        RES 1,A'                       ; @BT CB 8F
        LD D',H'                       ; @BT 54
        LD (IX+$19),D'                 ; @BT DD 72 19
        XOR A',IYH                     ; @BT FD AC
        BIT 3,D'                       ; @BT CB 5A
        LD C',D'                       ; @BT 4A
        SET 3,H'                       ; @BT CB DC
        LD A',I                        ; @BT ED 57
        LD C',(IY+$79)                 ; @BT FD 4E 79
        SBC HL',HL'                    ; @BT ED 62
        RLC (HL')                      ; @BT CB 06
        BIT 2,E'                       ; @BT CB 53
        SL1 B'                         ; @BT CB 30
        SBC HL',SP                     ; @BT ED 72
        LD IXL,$7A                     ; @BT DD 2E 7A
        SUB A',IYL                     ; @BT FD 95
        LD IXH,A'                      ; @BT DD 67
        LD (IY+$5E),A'                 ; @BT FD 77 5E
        RET PO                         ; @BT E0
        RES 2,(HL')                    ; @BT CB 96
        RET                            ; @BT C9
        SET 4,C'                       ; @BT CB E1
        LD H',H'                       ; @BT 64
        SUB A',$EF                     ; @BT D6 EF
        ADC A',A'                      ; @BT 8F
        HALT                           ; @BT 76
        RES 5,D'                       ; @BT CB AA
        ADC A',$53                     ; @BT CE 53
        LD H',(IY+$7D)                 ; @BT FD 66 7D
        BIT 0,E'                       ; @BT CB 43
        CP A',IXL                      ; @BT DD BD
        ADD A',D'                      ; @BT 82
        INC L'                         ; @BT 2C
        LD C',H'                       ; @BT 4C
        RES 2,B'                       ; @BT CB 90
        OUT (C'),A'                    ; @BT ED 79
        DEC BC'                        ; @BT 0B
        RST $18                        ; @BT DF
        INC IX                         ; @BT DD 23
        LD L',(IX+$39)                 ; @BT DD 6E 39
        SBC HL',BC'                    ; @BT ED 42
        LD L',B'                       ; @BT 68
        RRC (HL')                      ; @BT CB 0E
        ADD A',A'                      ; @BT 87
        LD E',E'                       ; @BT 5B
        DAA                            ; @BT 27
        SLA A'                         ; @BT CB 27
        PUSH AF'                       ; @BT F5
        SET 0,E'                       ; @BT CB C3
        LD C',(HL')                    ; @BT 4E
        ADD IX,DE'                     ; @BT DD 19
        BIT 3,B'                       ; @BT CB 58
        LD (IX+$1C),E'                 ; @BT DD 73 1C
        SET 0,H'                       ; @BT CB C4
        CP A',E'                       ; @BT BB
        CALL PE,$A009                  ; @BT EC 09 A0
        BIT 3,(HL')                    ; @BT CB 5E
        LD ($D43A),SP                  ; @BT ED 73 3A D4
        RR H'                          ; @BT CB 1C
        RES 4,E'                       ; @BT CB A3
        LD L',L'                       ; @BT 6D
        SET 6,D'                       ; @BT CB F2
        BIT 5,(HL')                    ; @BT CB 6E
        IN C',(C')                     ; @BT ED 48
        BIT 7,C'                       ; @BT CB 79
        OR A',(IX+$38)                 ; @BT DD B6 38
        SRL (HL')                      ; @BT CB 3E
        JP NEQ,$077A                   ; @BT C2 7A 07
        CPI                            ; @BT ED A1
        AND A',IXH                     ; @BT DD A4
        SET 5,D'                       ; @BT CB EA
        ADC A',IYL                     ; @BT FD 8D
        OUT ($09),A'                   ; @BT D3 09
        LD D',(IX+$26)                 ; @BT DD 56 26
        SBC A',H'                      ; @BT 9C
        JP EQU,$099C                   ; @BT CA 9C 09
        RES 2,C'                       ; @BT CB 91
        IN H',(C')                     ; @BT ED 60
        XOR A',L'                      ; @BT AD
        SL1 A'                         ; @BT CB 37
        DEC H'                         ; @BT 25
        OTIR                           ; @BT ED B3
        SBC A',A'                      ; @BT 9F
        LD L',E'                       ; @BT 6B
        OR A',IXH                      ; @BT DD B4
        BIT 2,D'                       ; @BT CB 52
        SUB A',E'                      ; @BT 93
        ADD HL',HL'                    ; @BT 29
        AND A',(IX+$35)                ; @BT DD A6 35
        SET 7,B'                       ; @BT CB F8
        LD IYH,IYL                     ; @BT FD 65
        RES 0,E'                       ; @BT CB 83
        SET 7,H'                       ; @BT CB FC
        EXX                            ; @BT D9
        LD IYH,B'                      ; @BT FD 60
        RES 6,A'                       ; @BT CB B7
        IN F',(C')                     ; @BT ED 70
        XOR A',(HL')                   ; @BT AE
        CP A',L'                       ; @BT BD
        SLA (HL')                      ; @BT CB 26
        LD (IY+$61),B'                 ; @BT FD 70 61
        AND A',H'                      ; @BT A4
        CALL POS,$9DE7                 ; @BT F4 E7 9D
        LD L',(IY+$3C)                 ; @BT FD 6E 3C
        LD A',$6D                      ; @BT 3E 6D
        AND A',D'                      ; @BT A2
        SRL B'                         ; @BT CB 38
        LD IY,($B43C)                  ; @BT FD 2A 3C B4
        LD E',(IY+$5C)                 ; @BT FD 5E 5C
        LD B',H'                       ; @BT 44
        RL D'                          ; @BT CB 12
        SRA D'                         ; @BT CB 2A
        LD B',(IY+$4C)                 ; @BT FD 46 4C
        RL C'                          ; @BT CB 11
        INC IXL                        ; @BT DD 2C
        RES 4,D'                       ; @BT CB A2
        RRA                            ; @BT 1F
        SET 2,C'                       ; @BT CB D1
        RES 2,H'                       ; @BT CB 94
        LD A',IXH                      ; @BT DD 7C
        LD H',(IX+$7A)                 ; @BT DD 66 7A
        LDIR                           ; @BT ED B0
        LD BC',($2BBC)                 ; @BT ED 4B BC 2B
        LD C',E'                       ; @BT 4B
        RES 5,C'                       ; @BT CB A9
        XOR A',H'                      ; @BT AC
        INC H'                         ; @BT 24
        RES 4,H'                       ; @BT CB A4
        LD IXL,E'                      ; @BT DD 6B
        ADD A',$08                     ; @BT C6 08
        LD IXL,A'                      ; @BT DD 6F
        DEC IXH                        ; @BT DD 25
        SLA C'                         ; @BT CB 21
        RST $28                        ; @BT EF
        RLA                            ; @BT 17
        LD (IX+$1F),H'                 ; @BT DD 74 1F
        SET 6,H'                       ; @BT CB F4
        RES 6,C'                       ; @BT CB B1
        CP A',H'                       ; @BT BC
        SRL H'                         ; @BT CB 3C
        RL A'                          ; @BT CB 17
        LD IYH,C'                      ; @BT FD 61
        SET 1,E'                       ; @BT CB CB
        LD IX,$9993                    ; @BT DD 21 93 99
        RES 4,A'                       ; @BT CB A7
        IND                            ; @BT ED AA
        BIT 7,E'                       ; @BT CB 7B
        LD (HL'),H'                    ; @BT 74
        RES 1,E'                       ; @BT CB 8B
        SBC A',IXH                     ; @BT DD 9C
        BIT 4,A'                       ; @BT CB 67
        CP A',C'                       ; @BT B9
        DEC IY                         ; @BT FD 2B
        RLCA                           ; @BT 07
        SET 1,L'                       ; @BT CB CD
        ADC A',B'                      ; @BT 88
        OR A',$0C                      ; @BT F6 0C
        RR L'                          ; @BT CB 1D
        LD B',IXH                      ; @BT DD 44
        LD C',B'                       ; @BT 48
        BIT 1,L'                       ; @BT CB 4D
        SET 6,L'                       ; @BT CB F5
        OUT (C'),B'                    ; @BT ED 41
        RES 0,A'                       ; @BT CB 87
        LDI                            ; @BT ED A0
        LD (IY+$70),L'                 ; @BT FD 75 70
        RL H'                          ; @BT CB 14
        SET 2,B'                       ; @BT CB D0
        SET 4,B'                       ; @BT CB E0
        RRC B'                         ; @BT CB 08
        POP HL'                        ; @BT E1
        RRC D'                         ; @BT CB 0A
        EX AF',AF                      ; @BT 08
        ADD IY,DE'                     ; @BT FD 19
        ADD A',IYL                     ; @BT FD 85
        LD HL',($7AA6)                 ; @BT 2A A6 7A
        BIT 6,L'                       ; @BT CB 75
        LD IXL,IXL                     ; @BT DD 6D
        LD C',IYH                      ; @BT FD 4C
        LD C',IXH                      ; @BT DD 4C
        LD B',A'                       ; @BT 47
        PUSH HL'                       ; @BT E5
        LD D',IYH                      ; @BT FD 54
        LD DE',$5220                   ; @BT 11 20 52
        EI                             ; @BT FB
        LD I,A'                        ; @BT ED 47
        LD IYH,$18                     ; @BT FD 26 18
        RES 0,C'                       ; @BT CB 81
        ADD A',IXL                     ; @BT DD 85
        INC (HL')                      ; @BT 34
        BIT 1,D'                       ; @BT CB 4A
        BIT 7,D'                       ; @BT CB 7A
        ADD A',(IX+$64)                ; @BT DD 86 64
        LD IYL,B'                      ; @BT FD 68
        EX (SP),HL'                    ; @BT E3
        ADD A',C'                      ; @BT 81
        ADD IY,SP                      ; @BT FD 39
        SET 3,B'                       ; @BT CB D8
        SET 0,(HL')                    ; @BT CB C6
        BIT 4,(HL')                    ; @BT CB 66
        BIT 2,L'                       ; @BT CB 55
        LD (IX+$13),B'                 ; @BT DD 70 13
        BIT 5,H'                       ; @BT CB 6C
        LD IXH,B'                      ; @BT DD 60
        CALL $A22B                     ; @BT CD 2B A2
        OR A',(IY+$41)                 ; @BT FD B6 41
        LD R,A'                        ; @BT ED 4F
        LD (HL'),C'                    ; @BT 71
        RES 1,L'                       ; @BT CB 8D
        SET 2,L'                       ; @BT CB D5
        INC (IY+$1B)                   ; @BT FD 34 1B
        ADD IY,IY                      ; @BT FD 29
        OR A',(HL')                    ; @BT B6
        RES 2,D'                       ; @BT CB 92
        INC (IX+$0F)                   ; @BT DD 34 0F
        ADC A',L'                      ; @BT 8D
        ADD A',(IY+$6D)                ; @BT FD 86 6D
        BIT 6,A'                       ; @BT CB 77
        ADC A',(IY+$34)                ; @BT FD 8E 34
        BIT 3,E'                       ; @BT CB 5B
        LD IYL,IYH                     ; @BT FD 6C
        BIT 2,B'                       ; @BT CB 50
        BIT 1,H'                       ; @BT CB 4C
        AND A',L'                      ; @BT A5
        LD L',(HL')                    ; @BT 6E
        SBC A',IYL                     ; @BT FD 9D
        ADD HL',BC'                    ; @BT 09
        INC D'                         ; @BT 14
        SET 0,D'                       ; @BT CB C2
        LD IYL,D'                      ; @BT FD 6A
        RES 3,C'                       ; @BT CB 99
        SLA B'                         ; @BT CB 20
        LD C',IYL                      ; @BT FD 4D
        LD B',$0B                      ; @BT 06 0B
        LD IYL,$D0                     ; @BT FD 2E D0
        RES 2,A'                       ; @BT CB 97
        INC IYL                        ; @BT FD 2C
        LD E',(HL')                    ; @BT 5E
        RES 1,B'                       ; @BT CB 88
        AND A',B'                      ; @BT A0
        RES 6,L'                       ; @BT CB B5
        SET 5,B'                       ; @BT CB E8
        OUT (C'),0                     ; @BT ED 71
        LD A',(BC')                    ; @BT 0A
        LD IYH,A'                      ; @BT FD 67
        RES 7,E'                       ; @BT CB BB
        RRD                            ; @BT ED 67
        XOR A',(IX+$24)                ; @BT DD AE 24
        SBC A',IXL                     ; @BT DD 9D
        ADD A',E'                      ; @BT 83
        BIT 1,A'                       ; @BT CB 4F
        JP (IY)                        ; @BT FD E9
        INI                            ; @BT ED A2
        RES 0,H'                       ; @BT CB 84
        SBC A',E'                      ; @BT 9B
        OUTI                           ; @BT ED A3
        ADC HL',HL'                    ; @BT ED 6A
        LD H',A'                       ; @BT 67
        LD IYL,A'                      ; @BT FD 6F

    .db "<02"               ; @BT END

    .BANK 2 SLOT 0
    .ORG 0

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with full TEST-03 randomization + varied arithmetic expressions
    ; GENERATED: seed = 1532737528

            CP A',IXL                  ; @BT DD BD
            ADD A',IYH                 ; @BT FD 84
            RLD                        ; @BT ED 6F
            INC IYL                    ; @BT FD 2C
            LD B',(HL')                ; @BT 46
            LD C',IXH                  ; @BT DD 4C
            LD E',IXH                  ; @BT DD 5C
            RLC (HL')                  ; @BT CB 06
            NOP                        ; @BT 00
            LD C',H'                   ; @BT 4C
            CP A',(IX+((($46-6)+(6-9))+9)) ; @BT DD BE 46
            SET 7,E'                   ; @BT CB FB
            LD B',IXL                  ; @BT DD 45
            ADC A',H'                  ; @BT 8C
            LD L',B'                   ; @BT 68
            SRA H'                     ; @BT CB 2C
            BIT 0,(IX+$5C)             ; @BT DD CB 5C 46
            OUT (C'),D'                ; @BT ED 51
            SRA L'                     ; @BT CB 2D
            SRL D'                     ; @BT CB 3A
            BIT 4,E'                   ; @BT CB 63
            OUT (C'),A'                ; @BT ED 79
            RES 4,(HL')                ; @BT CB A6
            SET 0,B'                   ; @BT CB C0
            LD E',(HL')                ; @BT 5E
            ADD A',H'                  ; @BT 84
            LD SP,(((($954F-5)-3)+5+3)) ; @BT ED 7B 4F 95
            SRA D'                     ; @BT CB 2A
            BIT 7,A'                   ; @BT CB 7F
            LD D',A'                   ; @BT 57
            LD (HL'),E'                ; @BT 73
            LD IXH,B'                  ; @BT DD 60
            INC D'                     ; @BT 14
            LDI                        ; @BT ED A0
            BIT 6,E'                   ; @BT CB 73
            SET 5,(HL')                ; @BT CB EE
            CP A',L'                   ; @BT BD
            RES 6,B'                   ; @BT CB B0
            LD (HL'),C'                ; @BT 71
            LD E',H'                   ; @BT 5C
            CALL PE,((($A009-(3+3))+(3+3))) ; @BT EC 09 A0
            CP A',C'                   ; @BT B9
            OR A',(HL')                ; @BT B6
            NEG                        ; @BT ED 44
            SET 2,L'                   ; @BT CB D5
            SET 6,(HL')                ; @BT CB F6
            LD A',C'                   ; @BT 79
            SL1 H'                     ; @BT CB 34
            RR D'                      ; @BT CB 1A
            XOR A',(($48-(10+8))+10+8) ; @BT EE 48
            IN A',(C')                 ; @BT ED 78
            DEC IXH                    ; @BT DD 25
            ADD A',E'                  ; @BT 83
            LD H',B'                   ; @BT 60
            RLC B'                     ; @BT CB 00
            LD (IX+((($16-5)-2)+5+2)),C' ; @BT DD 71 16
            ADD IY,DE'                 ; @BT FD 19
            XOR A',H'                  ; @BT AC
            IN F',(C')                 ; @BT ED 70
            LDD                        ; @BT ED A8
            RES 3,H'                   ; @BT CB 9C
            BIT 0,(IY+$5F)             ; @BT FD CB 5F 46
            LD A',IYH                  ; @BT FD 7C
            AND A',IXH                 ; @BT DD A4
            LD (IY+((($6D-(7+1))+1)+7)),H' ; @BT FD 74 6D
            OR A',D'                   ; @BT B2
            SL1 E'                     ; @BT CB 33
            PUSH IX                    ; @BT DD E5
            SLA D'                     ; @BT CB 22
            SET 0,L'                   ; @BT CB C5
            SET 1,(HL')                ; @BT CB CE
            SET 6,B'                   ; @BT CB F0
            SET 6,H'                   ; @BT CB F4
            ADD A',(HL')               ; @BT 86
            LD H',D'                   ; @BT 62
            CALL GTE,((($9BC5-5)-(2-1))+(2-1)+5) ; @BT D4 C5 9B
            OR A',(IX+(($38-(8+8))+8+8)) ; @BT DD B6 38
            RES 1,L'                   ; @BT CB 8D
            CALL EQU,((($9EF8-9)-4)+9+4) ; @BT CC F8 9E
            SRL (HL')                  ; @BT CB 3E
            RL E'                      ; @BT CB 13
            ADD IX,DE'                 ; @BT DD 19
            LD ((($D218-9)+9)),DE'     ; @BT ED 53 18 D2
            IN E',(C')                 ; @BT ED 58
            LD IXH,C'                  ; @BT DD 61
            RES 3,A'                   ; @BT CB 9F
            LD IYH,C'                  ; @BT FD 61
            RES 1,A'                   ; @BT CB 8F
            ADC A',(IY+((($34-7)-9)+7+9)) ; @BT FD 8E 34
            RES 0,A'                   ; @BT CB 87
            LD E',IYL                  ; @BT FD 5D
            OUT (C'),H'                ; @BT ED 61
            BIT 0,D'                   ; @BT CB 42
            LD A',D'                   ; @BT 7A
            OR A',L'                   ; @BT B5
            SUB A',D'                  ; @BT 92
            HALT                       ; @BT 76
            LD DE',((($5220-10)-9)+10+9) ; @BT ED 5B 20 52
            CALL NEG,(($9AB4-(5+2))+5+2) ; @BT FC B4 9A
            SUB A',E'                  ; @BT 93
            SET 2,E'                   ; @BT CB D3
            LD C',(IX+((($70-(5+1))+5)+1)) ; @BT DD 4E 70
            SET 6,C'                   ; @BT CB F1
            XOR A',A'                  ; @BT AF
            SLA L'                     ; @BT CB 25
            LD C',C'                   ; @BT 49
            INC IYH                    ; @BT FD 24
            RES 1,(HL')                ; @BT CB 8E
            LD BC',((($2CCD-(5+1))+5)+1) ; @BT ED 4B CD 2C
            EX (SP),HL'                ; @BT E3
            LD IXL,((($7A-(2+1))+2)+1) ; @BT DD 2E 7A
            SET 4,(HL')                ; @BT CB E6
            RES 4,A'                   ; @BT CB A7
            BIT 4,A'                   ; @BT CB 67
            LD D',IYH                  ; @BT FD 54
            DJNZ ((($26-(11+7))+7)+11) ; @BT 10 26
            RES 2,H'                   ; @BT CB 94
            SBC A',H'                  ; @BT 9C
            SBC HL',HL'                ; @BT ED 62
            SET 5,B'                   ; @BT CB E8
            SUB A',(HL')               ; @BT 96
            JP GTE,((($0669-9)+(9+5))-5) ; @BT D2 69 06
            BIT 2,A'                   ; @BT CB 57
            LD A',IXL                  ; @BT DD 7D
            OUTI                       ; @BT ED A3
            LD L',D'                   ; @BT 6A
            ADD IX,BC'                 ; @BT DD 09
            BIT 3,C'                   ; @BT CB 59
            LD IYL,((($D0-8)-(1-1))+(1-1)+8) ; @BT FD 2E D0
            LD IYH,IYL                 ; @BT FD 65
            RST ((($18-(3+1))+3)+1)    ; @BT DF
            BIT 4,L'                   ; @BT CB 65
            AND A',L'                  ; @BT A5
            RES 7,A'                   ; @BT CB BF
            LD H',A'                   ; @BT 67
            BIT 0,C'                   ; @BT CB 41
            LD SP,HL'                  ; @BT F9
            LD E',E'                   ; @BT 5B
            SLA H'                     ; @BT CB 24
            LD IXH,IXL                 ; @BT DD 65
            SLA C'                     ; @BT CB 21
            DEC (IY+((($1D-6)+1)-1+6)) ; @BT FD 35 1D
            RLC H'                     ; @BT CB 04
            RET NEG                    ; @BT F8
            RES 5,C'                   ; @BT CB A9
            SRL E'                     ; @BT CB 3B
            AND A',B'                  ; @BT A0
            BIT 1,A'                   ; @BT CB 4F
            LD A',A'                   ; @BT 7F
            SBC HL',BC'                ; @BT ED 42
            JP ((($0CCF-10)-1)+10+1)   ; @BT C3 CF 0C
            OR A',IXL                  ; @BT DD B5
            SL1 D'                     ; @BT CB 32
            RES 3,L'                   ; @BT CB 9D
            SBC A',IXL                 ; @BT DD 9D
            IN D',(C')                 ; @BT ED 50
            LD IXH,IXH                 ; @BT DD 64
            OTIR                       ; @BT ED B3
            OR A',IYL                  ; @BT FD B5
            RES 3,(HL')                ; @BT CB 9E
            EX (SP),IX                 ; @BT DD E3
            RES 2,E'                   ; @BT CB 93
            SUB A',B'                  ; @BT 90
            SRA B'                     ; @BT CB 28
            RES 5,L'                   ; @BT CB AD
            RES 5,B'                   ; @BT CB A8
            INC (HL')                  ; @BT 34
            LD IXL,B'                  ; @BT DD 68
            DEC D'                     ; @BT 15
            SET 4,E'                   ; @BT CB E3
            CPL                        ; @BT 2F
            LD L',E'                   ; @BT 6B
            POP DE'                    ; @BT D1
            BIT 2,L'                   ; @BT CB 55
            BIT 6,B'                   ; @BT CB 70
            OUT (C'),B'                ; @BT ED 41
            RES 5,D'                   ; @BT CB AA
            SBC A',IYH                 ; @BT FD 9C
            ADC A',B'                  ; @BT 88
            LD C',IYL                  ; @BT FD 4D
            RET GTE                    ; @BT D0
            LD A',H'                   ; @BT 7C
            LD C',(IY+((($79-(7+1))+7)+1)) ; @BT FD 4E 79
            AND A',(HL')               ; @BT A6
            SET 1,E'                   ; @BT CB CB
            LD IXL,IXL                 ; @BT DD 6D
            SET 4,H'                   ; @BT CB E4
            LD C',L'                   ; @BT 4D
            BIT 7,D'                   ; @BT CB 7A
            RES 2,L'                   ; @BT CB 95
            BIT 1,C'                   ; @BT CB 49
            RES 3,E'                   ; @BT CB 9B
            BIT 3,L'                   ; @BT CB 5D
            LD E',C'                   ; @BT 59
            SRL H'                     ; @BT CB 3C
            LD (((($D107-7)-6)+7+6)),BC' ; @BT ED 43 07 D1
            BIT 2,(HL')                ; @BT CB 56
            LD E',L'                   ; @BT 5D
            OR A',C'                   ; @BT B1
            SRA A'                     ; @BT CB 2F
            CP A',IXH                  ; @BT DD BC
            LD A',E'                   ; @BT 7B
            SLA A'                     ; @BT CB 27
            LD (IY+((($5E-3)+(3-6))+6)),A' ; @BT FD 77 5E
            SET 2,C'                   ; @BT CB D1
            SET 1,B'                   ; @BT CB C8
            SET 4,L'                   ; @BT CB E5
            DAA                        ; @BT 27
            RST (($08-(9+5))+9+5)      ; @BT CF
            SET 5,L'                   ; @BT CB ED
            SUB A',IYL                 ; @BT FD 95
            LD H',(HL')                ; @BT 66
            LD (IY+((($61-(2+1))+2)+1)),B' ; @BT FD 70 61
            XOR A',E'                  ; @BT AB
            RRC A'                     ; @BT CB 0F
            SRA E'                     ; @BT CB 2B
            SUB A',IYH                 ; @BT FD 94
            SUB A',(IY+((($7A-(3+8))+8)+3)) ; @BT FD 96 7A
            RR A'                      ; @BT CB 1F
            LD (HL'),A'                ; @BT 77
            JP (IX)                    ; @BT DD E9
            CPD                        ; @BT ED A9
            BIT 6,D'                   ; @BT CB 72
            ADC A',((($53-(5+1))+5)+1) ; @BT CE 53
            SET 7,H'                   ; @BT CB FC
            INC DE'                    ; @BT 13
            BIT 6,C'                   ; @BT CB 71
            RL A'                      ; @BT CB 17
            LD E',IXL                  ; @BT DD 5D
            RRA                        ; @BT 1F
            RES 6,E'                   ; @BT CB B3
            RES 4,C'                   ; @BT CB A1
            RST ((($38-9)+(9-8))+8)    ; @BT FF
            LD A',IYL                  ; @BT FD 7D
            SET 3,H'                   ; @BT CB DC
            DEC IX                     ; @BT DD 2B
            ADD A',IXL                 ; @BT DD 85
            BIT 2,E'                   ; @BT CB 53
            BIT 1,B'                   ; @BT CB 48
            BIT 3,H'                   ; @BT CB 5C
            LD IYL,D'                  ; @BT FD 6A
            LD H',E'                   ; @BT 63
            SRA C'                     ; @BT CB 29
            IN A',(((($51-(6+6))+(6+6)))) ; @BT DB 51
            LD (IY+((($67-10)+5)-5+10)),D' ; @BT FD 72 67
            SET 5,H'                   ; @BT CB EC
            BIT 0,B'                   ; @BT CB 40
            EX DE',HL'                 ; @BT EB
            RES 2,C'                   ; @BT CB 91
            RES 1,B'                   ; @BT CB 88
            BIT 7,H'                   ; @BT CB 7C
            RR B'                      ; @BT CB 18
            SET 6,L'                   ; @BT CB F5
            SET 7,A'                   ; @BT CB FF
            DEC DE'                    ; @BT 1B
            SL1 A'                     ; @BT CB 37
            POP IX                     ; @BT DD E1
            BIT 4,H'                   ; @BT CB 64
            ADC A',A'                  ; @BT 8F
            JP EQU,(($099C-(7+7))+7+7) ; @BT CA 9C 09
            LD L',C'                   ; @BT 69
            LD B',H'                   ; @BT 44
            LD C',IYH                  ; @BT FD 4C
            BIT 0,(HL')                ; @BT CB 46
            BIT 6,(HL')                ; @BT CB 76
            SLA (HL')                  ; @BT CB 26
            XOR A',(IY+((($2D-(8+1))+8)+1)) ; @BT FD AE 2D
            RES 0,D'                   ; @BT CB 82
            SBC A',B'                  ; @BT 98
            SET 2,D'                   ; @BT CB D2
            SET 3,A'                   ; @BT CB DF
            OR A',A'                   ; @BT B7
            LD IXL,E'                  ; @BT DD 6B
            RL C'                      ; @BT CB 11
            LD B',(IX+(($43-(6+3))+6+3)) ; @BT DD 46 43
            OUT (C'),E'                ; @BT ED 59
            INDR                       ; @BT ED BA
            LD L',(($3B-(11+7))+11+7)  ; @BT 2E 3B
            RL H'                      ; @BT CB 14
            LD D',IXL                  ; @BT DD 55
            ADC HL',HL'                ; @BT ED 6A
            LD IY,(((($B43C-(5+8))+(5+8)))) ; @BT FD 2A 3C B4
            CALL (($A22B-(11+4))+11+4) ; @BT CD 2B A2
            SL1 C'                     ; @BT CB 31
            RES 0,H'                   ; @BT CB 84
            RR H'                      ; @BT CB 1C
            SET 4,B'                   ; @BT CB E0
            SL1 (HL')                  ; @BT CB 36
            DI                         ; @BT F3
            DEC BC'                    ; @BT 0B
            LD A',(HL')                ; @BT 7E
            LD D',C'                   ; @BT 51
            BIT 6,A'                   ; @BT CB 77
            BIT 4,(HL')                ; @BT CB 66
            LD IYH,B'                  ; @BT FD 60
            LD B',A'                   ; @BT 47
            LD (IX+((($25-(4+7))+7)+4)),((($32-3)+(3-3))+3) ; @BT DD 36 25 32
            LD L',(HL')                ; @BT 6E
            LD (((($D65C-9)-(7-1))+(7-1)+9)),IX ; @BT DD 22 5C D6
            RES 4,L'                   ; @BT CB A5
            PUSH DE'                   ; @BT D5
            LD (((($D54B-(7+1))+7)+1)),IY ; @BT FD 22 4B D5
            DEC (IX+((($11-(5+1))+5)+1)) ; @BT DD 35 11
            RLA                        ; @BT 17
            ADD HL',BC'                ; @BT 09
            LD D',B'                   ; @BT 50
            PUSH HL'                   ; @BT E5
            RET PE                     ; @BT E8
            LD C',IXL                  ; @BT DD 4D
            LD SP,((($9660-3)+6)-6+3)  ; @BT ED 7B 60 96
            CPDR                       ; @BT ED B9
            LD E',(IX+(($53-(9+5))+9+5)) ; @BT DD 5E 53
            LD E',A'                   ; @BT 5F
            LD (HL'),D'                ; @BT 72
            LD D',D'                   ; @BT 52
            SBC A',IYL                 ; @BT FD 9D
            IN B',(C')                 ; @BT ED 40
            RES 0,E'                   ; @BT CB 83
            SET 7,L'                   ; @BT CB FD
            RR C'                      ; @BT CB 19
            CALL POS,((($9DE7-(10+8))+(10+8))) ; @BT F4 E7 9D
            SET 0,(HL')                ; @BT CB C6
            LD C',(HL')                ; @BT 4E
            RLC C'                     ; @BT CB 01
            RES 7,C'                   ; @BT CB B9
            LD D',(IX+((($26-(5+1))+5)+1)) ; @BT DD 56 26
            LD B',E'                   ; @BT 43
            RES 0,C'                   ; @BT CB 81
            RES 7,L'                   ; @BT CB BD
            RETI                       ; @BT ED 4D
            AND A',D'                  ; @BT A2
            LD R,A'                    ; @BT ED 4F
            BIT 0,L'                   ; @BT CB 45
            BIT 4,D'                   ; @BT CB 62
            EX AF',AF                  ; @BT 08
            BIT 0,E'                   ; @BT CB 43
            SET 4,D'                   ; @BT CB E2
            LD B',((($0B-(8+1))+1)+8)  ; @BT 06 0B
            RES 5,(HL')                ; @BT CB AE
            LD B',IYL                  ; @BT FD 45
            RES 4,B'                   ; @BT CB A0
            LD IYL,C'                  ; @BT FD 69
            LD B',(IY+((($4C-9)+2)-2+9)) ; @BT FD 46 4C
            SET 6,E'                   ; @BT CB F3
            RST (($28-(4+9))+4+9)      ; @BT EF
            SET 1,C'                   ; @BT CB C9
            LD BC',((($2BBC-(6+4))+6+4)) ; @BT ED 4B BC 2B
            AND A',A'                  ; @BT A7
            RRC (HL')                  ; @BT CB 0E
            SET 4,C'                   ; @BT CB E1
            BIT 0,A'                   ; @BT CB 47
            INC A'                     ; @BT 3C
            ADD A',(IY+((($6D-9)-5)+9+5)) ; @BT FD 86 6D
            OUT (C'),C'                ; @BT ED 49
            RES 4,D'                   ; @BT CB A2
            SBC A',IXH                 ; @BT DD 9C
            ADC A',IXH                 ; @BT DD 8C
            INC C'                     ; @BT 0C
            RR L'                      ; @BT CB 1D
            ADC HL',BC'                ; @BT ED 4A
            LD B',IYH                  ; @BT FD 44
            AND A',E'                  ; @BT A3
            ADD A',A'                  ; @BT 87
            SET 2,A'                   ; @BT CB D7
            LD (DE'),A'                ; @BT 12
            ADC A',D'                  ; @BT 8A
            INC SP                     ; @BT 33
            LD IY,((($B54D-2)+9)-9+2)  ; @BT FD 2A 4D B5
            RRCA                       ; @BT 0F
            OR A',H'                   ; @BT B4
            RES 7,H'                   ; @BT CB BC
            BIT 5,C'                   ; @BT CB 69
            LD IX,(((($9882-(7+9))+9)+7)) ; @BT DD 2A 82 98
            RR E'                      ; @BT CB 1B
            RRD                        ; @BT ED 67
            RES 7,B'                   ; @BT CB B8
            RES 6,D'                   ; @BT CB B2
            RLCA                       ; @BT 07
            RRC D'                     ; @BT CB 0A
            RETN                       ; @BT ED 45
            LD C',A'                   ; @BT 4F
            DEC IXL                    ; @BT DD 2D
            SRL L'                     ; @BT CB 3D
            RES 6,A'                   ; @BT CB B7
            SET 3,E'                   ; @BT CB DB
            LD (HL'),L'                ; @BT 75
            INC (IX+((($0F-11)+9)-9+11)) ; @BT DD 34 0F
            BIT 6,L'                   ; @BT CB 75
            SBC A',E'                  ; @BT 9B
            CP A',A'                   ; @BT BF
            RST ((($10-7)+(7-1))+1)    ; @BT D7
            XOR A',(IX+((($24-8)+(8-7))+7)) ; @BT DD AE 24
            ADD HL',DE'                ; @BT 19
            LD E',(IY+((($5C-5)+(5+7))-7)) ; @BT FD 5E 5C
            IM (((2-(2+1))+2)+1)       ; @BT ED 5E
            LD E',(($5B-(10+6))+10+6)  ; @BT 1E 5B
            DEC HL'                    ; @BT 2B
            ADC A',(HL')               ; @BT 8E
            JR GTE,((($5C-6)-9)+6+9)   ; @BT 30 5C
            SET 1,L'                   ; @BT CB CD
            ADC A',(IX+(($2B-4)+4))    ; @BT DD 8E 2B
            LD C',B'                   ; @BT 48
            RES 1,H'                   ; @BT CB 8C
            LD IXL,A'                  ; @BT DD 6F
            INC IXH                    ; @BT DD 24
            IM ((0-11)+11)             ; @BT ED 46
            LD L',(IX+((($39-(7+1))+7)+1)) ; @BT DD 6E 39
            RES 4,H'                   ; @BT CB A4
            RLC D'                     ; @BT CB 02
            SBC A',(IX+(($1B-(7+5))+7+5)) ; @BT DD 9E 1B
            IN H',(C')                 ; @BT ED 60
            SRL C'                     ; @BT CB 39
            CCF                        ; @BT 3F
            LD A',(DE')                ; @BT 1A
            RES 1,E'                   ; @BT CB 8B
            DEC L'                     ; @BT 2D
            PUSH BC'                   ; @BT C5
            RET                        ; @BT C9
            LD H',H'                   ; @BT 64
            LD D',H'                   ; @BT 54
            SET 5,C'                   ; @BT CB E9
            LD A',I                    ; @BT ED 57
            LD D',((($02-4)+(4-9))+9)  ; @BT 16 02
            BIT 2,H'                   ; @BT CB 54
            SRA (HL')                  ; @BT CB 2E
            JR LSS,(($45-4)+4)         ; @BT 38 45
            RES 5,H'                   ; @BT CB AC
            LD H',L'                   ; @BT 65
            RES 4,E'                   ; @BT CB A3
            JR EQU,((($8A-3)-8)+3+8)   ; @BT 28 8A
            RRC B'                     ; @BT CB 08
            RRC C'                     ; @BT CB 09
            AND A',(IX+((($35-5)-(8-1))+(8-1)+5)) ; @BT DD A6 35
            RES 3,B'                   ; @BT CB 98
            EX (SP),IY                 ; @BT FD E3
            RES 7,D'                   ; @BT CB BA
            OR A',IXH                  ; @BT DD B4
            LD IYL,A'                  ; @BT FD 6F
            AND A',C'                  ; @BT A1
            LD IYL,IYH                 ; @BT FD 6C
            DEC (HL')                  ; @BT 35
            SET 1,D'                   ; @BT CB CA
            IND                        ; @BT ED AA
            BIT 3,A'                   ; @BT CB 5F
            LD IYL,B'                  ; @BT FD 68
            INC H'                     ; @BT 24
            CALL PO,((($A11A-6)+(6-4))+4) ; @BT E4 1A A1
            LD B',B'                   ; @BT 40
            SBC HL',SP                 ; @BT ED 72
            BIT 1,E'                   ; @BT CB 4B
            SBC A',(HL')               ; @BT 9E
            LD A',((($6D-(9+1))+9)+1)  ; @BT 3A 6D 00
            LD IYL,IYL                 ; @BT FD 6D
            OR A',B'                   ; @BT B0
            RET NEQ                    ; @BT C0
            RRC E'                     ; @BT CB 0B
            XOR A',B'                  ; @BT A8
            CPIR                       ; @BT ED B1
            ADC A',IXL                 ; @BT DD 8D
            RES 6,H'                   ; @BT CB B4
            LD H',(IY+((($7D-(4+1))+4)+1)) ; @BT FD 66 7D
            SET 3,(HL')                ; @BT CB DE
            ADC A',E'                  ; @BT 8B
            LD H',((($58-(3+1))+3)+1)  ; @BT 26 58
            SET 4,A'                   ; @BT CB E7
            RES 2,D'                   ; @BT CB 92
            OUT (((($09-(7+8))+(7+8)))),A' ; @BT D3 09
            LD B',C'                   ; @BT 41
            EI                         ; @BT FB
            LD (HL'),((($3E-(10+1))+10)+1) ; @BT 36 3E
            SET 3,L'                   ; @BT CB DD
            INI                        ; @BT ED A2
            CP A',IYL                  ; @BT FD BD
            BIT 5,(HL')                ; @BT CB 6E
            JP PO,((($0BBE-(8+1))+8)+1) ; @BT E2 BE 0B
            LD D',IYL                  ; @BT FD 55
            INIR                       ; @BT ED B2
            CP A',(IY+(($4F-(5+6))+5+6)) ; @BT FD BE 4F
            SET 7,(HL')                ; @BT CB FE
            SBC A',L'                  ; @BT 9D
            ADC A',IYL                 ; @BT FD 8D
            RST ((($00-(10+1))+10)+1)  ; @BT C7
            JP (HL')                   ; @BT E9
            LD E',IYH                  ; @BT FD 5C
            LD IXH,(($C2-(4+4))+4+4)   ; @BT DD 26 C2
            RLC L'                     ; @BT CB 05
            XOR A',D'                  ; @BT AA
            BIT 2,D'                   ; @BT CB 52
            CP A',D'                   ; @BT BA
            SET 6,A'                   ; @BT CB F7
            LD D',IXH                  ; @BT DD 54
            SET 5,D'                   ; @BT CB EA
            SET 6,D'                   ; @BT CB F2
            LD (HL'),H'                ; @BT 74
            JP PE,((($0AAD-(6+4))+(6+4))) ; @BT EA AD 0A
            ADD A',(IX+((($64-4)-(7-1))+(7-1)+4)) ; @BT DD 86 64
            AND A',IYL                 ; @BT FD A5
            IN L',(C')                 ; @BT ED 68
            CP A',E'                   ; @BT BB
            SET 1,A'                   ; @BT CB CF
            RES 2,B'                   ; @BT CB 90
            JP LSS,(($0447-11)+11)     ; @BT DA 47 04
            SUB A',(IX+((($77-7)-(8-1))+(8-1)+7)) ; @BT DD 96 77
            LD HL',(((($7AA6-7)-(2-1))+(2-1)+7)) ; @BT 2A A6 7A
            SUB A',(($EF-4)+4)         ; @BT D6 EF
            OUTD                       ; @BT ED AB
            POP HL'                    ; @BT E1
            XOR A',IYL                 ; @BT FD AD
            LD (IX+((($13-(8+1))+8)+1)),B' ; @BT DD 70 13
            XOR A',(HL')               ; @BT AE
            BIT 7,L'                   ; @BT CB 7D
            RL (HL')                   ; @BT CB 16
            POP BC'                    ; @BT C1
            INC B'                     ; @BT 04
            BIT 5,E'                   ; @BT CB 6B
            LD (((($D43A-(5+5))+5)+5)),SP ; @BT ED 73 3A D4
            LD (IY+((($6A-(8+1))+8)+1)),E' ; @BT FD 73 6A
            RL L'                      ; @BT CB 15
            OUT (C'),0                 ; @BT ED 71
            LD IX,((($9993-7)+(7-3))+3) ; @BT DD 2A 93 99
            RLC E'                     ; @BT CB 03
            SET 3,C'                   ; @BT CB D9
            DEC E'                     ; @BT 1D
            LD IXL,IXH                 ; @BT DD 6C
            LD B',D'                   ; @BT 42
            LD C',D'                   ; @BT 4A
            SET 7,B'                   ; @BT CB F8
            SBC A',((($83-3)+4)-4+3)   ; @BT DE 83
            POP IY                     ; @BT FD E1
            LD L',A'                   ; @BT 6F
            LD E',B'                   ; @BT 58
            RET EQU                    ; @BT C8
            RR (HL')                   ; @BT CB 1E
            SET 0,C'                   ; @BT CB C1
            LD A',L'                   ; @BT 7D
            LD C',E'                   ; @BT 4B
            BIT 5,B'                   ; @BT CB 68
            IN C',(C')                 ; @BT ED 48
            BIT 0,H'                   ; @BT CB 44
            LDDR                       ; @BT ED B8
            LD (IX+((($1C-7)+5)-5+7)),E' ; @BT DD 73 1C
            RES 1,D'                   ; @BT CB 8A
            SET 1,H'                   ; @BT CB CC
            ADD A',IYL                 ; @BT FD 85
            SET 2,(HL')                ; @BT CB D6
            BIT 7,B'                   ; @BT CB 78
            BIT 7,(HL')                ; @BT CB 7E
            LD E',D'                   ; @BT 5A
            SLA B'                     ; @BT CB 20
            LD IYH,D'                  ; @BT FD 62
            ADD A',L'                  ; @BT 85
            OR A',E'                   ; @BT B3
            JR NEQ,((($73-10)-(8-1))+(8-1)+10) ; @BT 20 73
            LD (((($CFF6-11)+5)-5+11)),A' ; @BT 32 F6 CF
            SBC A',D'                  ; @BT 9A
            JR ((($A1-10)+(10-5))+5)   ; @BT 18 A1
            INC IY                     ; @BT FD 23
            SL1 L'                     ; @BT CB 35
            OTDR                       ; @BT ED BB
            LD L',L'                   ; @BT 6D
            XOR A',IYH                 ; @BT FD AC
            RES 2,(HL')                ; @BT CB 96
            CP A',(HL')                ; @BT BE
            LD D',(HL')                ; @BT 56
            AND A',IXL                 ; @BT DD A5
            BIT 3,B'                   ; @BT CB 58
            ADD IY,BC'                 ; @BT FD 09
            SET 0,A'                   ; @BT CB C7
            ADD IX,SP                  ; @BT DD 39
            BIT 5,A'                   ; @BT CB 6F
            RES 0,(HL')                ; @BT CB 86
            ADC HL',DE'                ; @BT ED 5A
            RES 0,B'                   ; @BT CB 80
            LD L',(IY+(($3C-11)+11))   ; @BT FD 6E 3C
            CPI                        ; @BT ED A1
            CALL NEQ,((($9CD6-(10+1))+(10+1))) ; @BT C4 D6 9C
            LD (IX+((($1F-5)+3)-3+5)),H' ; @BT DD 74 1F
            LD IYH,A'                  ; @BT FD 67
            RES 5,A'                   ; @BT CB AF
            LD (IY+((($64-11)-(8-1))+(8-1)+11)),C' ; @BT FD 71 64
            DEC C'                     ; @BT 0D
            RRC L'                     ; @BT CB 0D
            SET 3,D'                   ; @BT CB DA
            DEC IYL                    ; @BT FD 2D
            BIT 1,D'                   ; @BT CB 4A
            RES 1,C'                   ; @BT CB 89
            SUB A',IXH                 ; @BT DD 94
            PUSH IY                    ; @BT FD E5
            XOR A',C'                  ; @BT A9
            XOR A',IXL                 ; @BT DD AD
            LD IYH,E'                  ; @BT FD 63
            CP A',H'                   ; @BT BC
            RL D'                      ; @BT CB 12
            SCF                        ; @BT 37
            INC IXL                    ; @BT DD 2C
            CP A',IYH                  ; @BT FD BC
            BIT 3,(HL')                ; @BT CB 5E
            LD (IY+((($70-8)+(8+7))-7)),L' ; @BT FD 75 70
            RET LSS                    ; @BT D8
            RES 2,A'                   ; @BT CB 97
            ADD HL',SP                 ; @BT 39
            DEC H'                     ; @BT 25
            AND A',(IY+((($3E-(7+1))+(7+1)))) ; @BT FD A6 3E
            BIT 1,H'                   ; @BT CB 4C
            BIT 2,C'                   ; @BT CB 51
            XOR A',IXH                 ; @BT DD AC
            ADD IX,IX                  ; @BT DD 29
            LD C',((($64-2)+(2-7))+7)  ; @BT 0E 64
            LD A',R                    ; @BT ED 5F
            POP AF'                    ; @BT F1
            ADC HL',SP                 ; @BT ED 7A
            AND A',IYH                 ; @BT FD A4
            LD D',L'                   ; @BT 55
            DEC IY                     ; @BT FD 2B
            SUB A',L'                  ; @BT 95
            BIT 6,H'                   ; @BT CB 74
            LD A',B'                   ; @BT 78
            LD IXH,A'                  ; @BT DD 67
            SET 5,E'                   ; @BT CB EB
            LD DE',(((($510F-(7+9))+(7+9)))) ; @BT ED 5B 0F 51
            LD (((($D329-7)-5)+7+5)),HL' ; @BT 22 29 D3
            LD A',IXH                  ; @BT DD 7C
            SBC A',C'                  ; @BT 99
            ADD A',C'                  ; @BT 81
            RES 6,L'                   ; @BT CB B5
            BIT 5,L'                   ; @BT CB 6D
            RES 6,C'                   ; @BT CB B1
            OR A',IYH                  ; @BT FD B4
            SET 0,D'                   ; @BT CB C2
            SET 5,A'                   ; @BT CB EF
            LD IYL,E'                  ; @BT FD 6B
            SRL A'                     ; @BT CB 3F
            SET 0,E'                   ; @BT CB C3
            ADD A',D'                  ; @BT 82
            EXX                        ; @BT D9
            OUT (C'),L'                ; @BT ED 69
            LD SP,IX                   ; @BT DD F9
            BIT 1,L'                   ; @BT CB 4D
            LD B',IXH                  ; @BT DD 44
            BIT 3,D'                   ; @BT CB 5A
            ADD A',((($08-4)+1)-1+4)   ; @BT C6 08
            SUB A',IXL                 ; @BT DD 95
            ADC A',C'                  ; @BT 89
            SBC HL',DE'                ; @BT ED 52
            SET 2,B'                   ; @BT CB D0
            LD H',C'                   ; @BT 61
            CP A',B'                   ; @BT B8
            LD (IY+((($73-6)+(6-2))+2)),(($88-(8+9))+8+9) ; @BT FD 36 73 88
            LD (IX+((($19-(8+1))+8)+1)),D' ; @BT DD 72 19
            SET 7,D'                   ; @BT CB FA
            RET PO                     ; @BT E0
            IM (((1-5)+(5-5))+5)       ; @BT ED 56
            ADD A',B'                  ; @BT 80
            SET 7,C'                   ; @BT CB F9
            LD IXH,D'                  ; @BT DD 62
            SRL B'                     ; @BT CB 38
            BIT 1,(HL')                ; @BT CB 4E
            SBC A',(IY+((($24-(11+2))+(11+2)))) ; @BT FD 9E 24
            INC E'                     ; @BT 1C
            JP NEQ,((($077A-9)-(5-1))+(5-1)+9) ; @BT C2 7A 07
            BIT 7,C'                   ; @BT CB 79
            INC L'                     ; @BT 2C
            SL1 B'                     ; @BT CB 30
            LD A',(BC')                ; @BT 0A
            RES 7,(HL')                ; @BT CB BE
            SET 0,H'                   ; @BT CB C4
            AND A',H'                  ; @BT A4
            OR A',((($0C-(8+9))+9)+8)  ; @BT F6 0C
            LD SP,IY                   ; @BT FD F9
            SET 3,B'                   ; @BT CB D8
            LD A',(((($2667-7)+(7-7))+7)) ; @BT 3A 67 26
            RST ((($30-4)+1)-1+4)      ; @BT F7
            RLC A'                     ; @BT CB 07
            CALL LSS,(($99A3-8)+8)     ; @BT DC A3 99
            SUB A',A'                  ; @BT 97
            LDIR                       ; @BT ED B0
            BIT 5,D'                   ; @BT CB 6A
            BIT 5,H'                   ; @BT CB 6C
            RES 6,(HL')                ; @BT CB B6
            JP NEG,(($0558-6)+6)       ; @BT FA 58 05
            OR A',(IY+(($41-4)+4))     ; @BT FD B6 41
            DEC SP                     ; @BT 3B
            ADC A',IYH                 ; @BT FD 8C
            DEC IYH                    ; @BT FD 25
            LD A',(IX+((($07-2)+(2-8))+8)) ; @BT DD 7E 07
            LD D',E'                   ; @BT 53
            BIT 7,E'                   ; @BT CB 7B
            LD (IX+((($10-7)-9)+7+9)),A' ; @BT DD 77 10
            ADD IY,SP                  ; @BT FD 39
            LD IXL,C'                  ; @BT DD 69
            INC HL'                    ; @BT 23
            RES 7,E'                   ; @BT CB BB
            SET 2,H'                   ; @BT CB D4
            CP A',((($A2-5)+(5-4))+4)  ; @BT FE A2
            DEC A'                     ; @BT 3D
            ADD IY,IY                  ; @BT FD 29
            SUB A',C'                  ; @BT 91
            LD L',H'                   ; @BT 6C
            RES 5,E'                   ; @BT CB AB
            LD (IX+((($22-(2+1))+2)+1)),L' ; @BT DD 75 22
            RST ((($20-(6+1))+6)+1)    ; @BT E7
            ADC A',L'                  ; @BT 8D
            LD IYH,((($18-(10+7))+(10+7))) ; @BT FD 26 18
            INC BC'                    ; @BT 03
            BIT 3,E'                   ; @BT CB 5B
            INC IX                     ; @BT DD 23
            RET POS                    ; @BT F0
            RL B'                      ; @BT CB 10
            BIT 4,B'                   ; @BT CB 60
            ADD A',IXH                 ; @BT DD 84
            RES 3,D'                   ; @BT CB 9A
            LD HL',((($7BB7-7)+1)-1+7) ; @BT 2A B7 7B
            RES 3,C'                   ; @BT CB 99
            AND A',((($75-6)-1)+6+1)   ; @BT E6 75
            INC (IY+((($1B-(2+1))+2)+1)) ; @BT FD 34 1B
            LD (BC'),A'                ; @BT 02
            LD D',(IY+(($2F-8)+8))     ; @BT FD 56 2F
            LD H',(IX+((($7A-(2+2))+(2+2)))) ; @BT DD 66 7A
            SLA E'                     ; @BT CB 23
            PUSH AF'                   ; @BT F5
            DEC B'                     ; @BT 05
            LD IYH,IYH                 ; @BT FD 64
            ADD HL',HL'                ; @BT 29
            JP (IY)                    ; @BT FD E9
            BIT 4,C'                   ; @BT CB 61
            RES 0,L'                   ; @BT CB 85
            SUB A',H'                  ; @BT 94
            LD (HL'),B'                ; @BT 70
            XOR A',L'                  ; @BT AD
            LD B',L'                   ; @BT 45
            LD IXH,E'                  ; @BT DD 63
            LD I,A'                    ; @BT ED 47
            SBC A',A'                  ; @BT 9F
            LD A',(IY+((($10-1)+(1-3))+3)) ; @BT FD 7E 10
            LD IXL,D'                  ; @BT DD 6A
            JP POS,((($088B-(2+1))+2)+1) ; @BT F2 8B 08
            BIT 2,B'                   ; @BT CB 50
            RRC H'                     ; @BT CB 0C

    .db "<03"               ; @BT END

    .BANK 3 SLOT 0
    .ORG 0

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////





















    ; TEST-04 generated defines
    .define T4_DEF_001 ((($51*10)/10)+(9-9)+(9-9))
    .define T4_DEF_002 (((T4_DEF_001)+($99A3)-($51))+(1-1))
    .define T4_DEF_003 ((($2667+6)-6)+(1-2))
    .define T4_DEF_004 ((($83)-($2667)+(T4_DEF_003))+(11-11)+(1-1))
    .define T4_DEF_005 ((($9DE7*2)-($9DE7))+($9DE7)+(9-9)+(1-1)-$9DE7)
    .define T4_DEF_006 ((($9882)+(T4_DEF_005)-($9DE7))+((4+6)-(4+6)))
    .define T4_DEF_007 ((($A22B+13)-(13+3)+3)+(2-2))
    .define T4_DEF_008 (((T4_DEF_007)+($088B)-($A22B))+(2-2))
    .define T4_DEF_009 ((($7A+7)-(7+1)+1)+(8-8))
    .define T4_DEF_010 ((($077A)+(T4_DEF_009)-($7A))+((10+7)-(10+7)))
    .define T4_DEF_011 ((($0B*2)-($0B))+($0B)+(12-12)+(4-4)-$0B)
    .define T4_DEF_012 (((T4_DEF_011)*2+($2CCD)-(($0B)*2))+(11-11))
    .define T4_DEF_013 ((($7BB7+3)-3)+(10-2))
    .define T4_DEF_014 (((T4_DEF_013)*2+($2BBC)-(($7BB7)*2))+(11-11))
    .define T4_DEF_015 ((($18-2)+2)-(7-2)+(7-2))
    .define T4_DEF_016 (((T4_DEF_015)*2+($D329)-(($18)*2))+(9-9))
    .define T4_DEF_017 ((($D54B+(8*2))-(8*2))+(9-9))
    .define T4_DEF_018 (((T4_DEF_017)*2+($A2)-(($D54B)*2))+(11-11))
    .define T4_DEF_019 ((($A1+12+8)-12-8)+(3-3))
    .define T4_DEF_020 ((($08)-($A1)+(T4_DEF_019))+(2-2)+(1-1))
    .define T4_DEF_021 ((($3B+11)-(11+5)+5)+(4-4))
    .define T4_DEF_022 ((($58)+(T4_DEF_021)-($3B))+((6+2)-(6+2)))
    .define T4_DEF_023 ((($9993+3+9)-3-9)+(7-7))
    .define T4_DEF_024 ((($0AAD)+(T4_DEF_023)-($9993))+((11+9)-(11+9)))
    .define T4_DEF_025 ((($0BBE+(9*1))-(9*1))+(9-9))
    .define T4_DEF_026 (((T4_DEF_025)*2+($CFF6)-(($0BBE)*2))+(10-10))
    .define T4_DEF_027 ((($9AB4-3)+3)-(3-8)+(3-8))
    .define T4_DEF_028 (((T4_DEF_027)+($75)-($9AB4))+(8-8))
    .define T4_DEF_029 ((($D218+2+8)-2-8)+(3-3))
    .define T4_DEF_030 (((T4_DEF_029)*2+($A009)-(($D218)*2))+(9-9))
    .define T4_DEF_031 ((($954F+8+3)-8-3)+(9-9))
    .define T4_DEF_032 (((T4_DEF_031)+($9EF8)-($954F))+(4-4))
    .define T4_DEF_033 ((($D65C-12)+12)-(11-2)+(11-2))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with full TEST-04 randomization + varied define replacement
    ; GENERATED: seed = 2143944400
    ; GENERATED: replaced operands = 33 / 66

                ADC A',IXL             ; @BT DD 8D
                OUT (C'),B'            ; @BT ED 41
                LD H',E'               ; @BT 63
                CALL NEG,((T4_DEF_027-(5+2))+5+2) ; @BT FC B4 9A
                SET 2,H'               ; @BT CB D4
                BIT 3,D'               ; @BT CB 5A
                OTDR                   ; @BT ED BB
                SBC A',(IY+((($24-(11+2))+(11+2)))) ; @BT FD 9E 24
                LD D',IXL              ; @BT DD 55
                SET 0,A'               ; @BT CB C7
                INC DE'                ; @BT 13
                ADC A',L'              ; @BT 8D
                RES 2,A'               ; @BT CB 97
                ADD IX,SP              ; @BT DD 39
                LD IXL,D'              ; @BT DD 6A
                BIT 5,(HL')            ; @BT CB 6E
                LD C',(HL')            ; @BT 4E
                LD H',C'               ; @BT 61
                CP A',IXL              ; @BT DD BD
                ADC A',(IX+(($2B-4)+4)) ; @BT DD 8E 2B
                AND A',H'              ; @BT A4
                SET 7,L'               ; @BT CB FD
                LD H',A'               ; @BT 67
                BIT 2,D'               ; @BT CB 52
                BIT 5,L'               ; @BT CB 6D
                XOR A',C'              ; @BT A9
                BIT 7,C'               ; @BT CB 79
                LD H',(((T4_DEF_022-(3+1))+3)+1) ; @BT 26 58
                LD B',(HL')            ; @BT 46
                LDIR                   ; @BT ED B0
                LD D',((($02-4)+(4-9))+9) ; @BT 16 02
                DEC C'                 ; @BT 0D
                LD SP,IX               ; @BT DD F9
                CP A',(IX+((($46-6)+(6-9))+9)) ; @BT DD BE 46
                SBC HL',SP             ; @BT ED 72
                LD (IY+((($5E-3)+(3-6))+6)),A' ; @BT FD 77 5E
                SRA E'                 ; @BT CB 2B
                SET 6,A'               ; @BT CB F7
                RES 5,(HL')            ; @BT CB AE
                LD E',(IY+((($5C-5)+(5+7))-7)) ; @BT FD 5E 5C
                SBC A',H'              ; @BT 9C
                SL1 A'                 ; @BT CB 37
                LD A',L'               ; @BT 7D
                AND A',D'              ; @BT A2
                BIT 3,(HL')            ; @BT CB 5E
                ADD IX,DE'             ; @BT DD 19
                BIT 2,E'               ; @BT CB 53
                BIT 0,H'               ; @BT CB 44
                SBC A',B'              ; @BT 98
                SRL (HL')              ; @BT CB 3E
                SBC A',L'              ; @BT 9D
                BIT 2,L'               ; @BT CB 55
                SRA A'                 ; @BT CB 2F
                CALL EQU,(((T4_DEF_032-9)-4)+9+4) ; @BT CC F8 9E
                BIT 0,C'               ; @BT CB 41
                RR B'                  ; @BT CB 18
                INC (HL')              ; @BT 34
                BIT 0,(HL')            ; @BT CB 46
                OR A',B'               ; @BT B0
                LD L',B'               ; @BT 68
                LD IXL,C'              ; @BT DD 69
                PUSH DE'               ; @BT D5
                DEC L'                 ; @BT 2D
                RRC (HL')              ; @BT CB 0E
                RES 0,L'               ; @BT CB 85
                RRC B'                 ; @BT CB 08
                LD E',IYH              ; @BT FD 5C
                RET GTE                ; @BT D0
                XOR A',D'              ; @BT AA
                LD (IX+((($22-(2+1))+2)+1)),L' ; @BT DD 75 22
                CP A',B'               ; @BT B8
                LD (IX+((($1C-7)+5)-5+7)),E' ; @BT DD 73 1C
                LD (IY+((($6A-(8+1))+8)+1)),E' ; @BT FD 73 6A
                RES 5,A'               ; @BT CB AF
                SET 7,D'               ; @BT CB FA
                SET 4,D'               ; @BT CB E2
                RES 2,C'               ; @BT CB 91
                BIT 6,C'               ; @BT CB 71
                LD (HL'),B'            ; @BT 70
                SBC HL',BC'            ; @BT ED 42
                RES 6,C'               ; @BT CB B1
                LD SP,((((T4_DEF_031-5)-3)+5+3)) ; @BT ED 7B 4F 95
                SRL H'                 ; @BT CB 3C
                RES 1,C'               ; @BT CB 89
                IN L',(C')             ; @BT ED 68
                SET 2,B'               ; @BT CB D0
                BIT 5,B'               ; @BT CB 68
                BIT 0,B'               ; @BT CB 40
                BIT 4,C'               ; @BT CB 61
                LD L',H'               ; @BT 6C
                BIT 0,(IX+$5C)         ; @BT DD CB 5C 46
                XOR A',H'              ; @BT AC
                LD A',IYH              ; @BT FD 7C
                ADD IY,IY              ; @BT FD 29
                DEC IX                 ; @BT DD 2B
                LD C',A'               ; @BT 4F
                RES 1,A'               ; @BT CB 8F
                LD E',H'               ; @BT 5C
                RR H'                  ; @BT CB 1C
                LD B',B'               ; @BT 40
                RES 4,E'               ; @BT CB A3
                RES 3,C'               ; @BT CB 99
                LD (IX+((($10-7)-9)+7+9)),A' ; @BT DD 77 10
                XOR A',IXH             ; @BT DD AC
                ADD A',D'              ; @BT 82
                RST ((($38-9)+(9-8))+8) ; @BT FF
                LD IYH,IYL             ; @BT FD 65
                LD D',E'               ; @BT 53
                RST ((($00-(10+1))+10)+1) ; @BT C7
                RL B'                  ; @BT CB 10
                AND A',C'              ; @BT A1
                RES 7,(HL')            ; @BT CB BE
                LDDR                   ; @BT ED B8
                CALL LSS,((T4_DEF_002-8)+8) ; @BT DC A3 99
                RRC D'                 ; @BT CB 0A
                JR EQU,((($8A-3)-8)+3+8) ; @BT 28 8A
                AND A',A'              ; @BT A7
                DEC A'                 ; @BT 3D
                SET 0,B'               ; @BT CB C0
                RES 5,B'               ; @BT CB A8
                ADC A',D'              ; @BT 8A
                BIT 6,E'               ; @BT CB 73
                SRA H'                 ; @BT CB 2C
                LD E',B'               ; @BT 58
                RLC E'                 ; @BT CB 03
                ADD A',(IY+((($6D-9)-5)+9+5)) ; @BT FD 86 6D
                OR A',IXH              ; @BT DD B4
                DEC D'                 ; @BT 15
                RES 4,H'               ; @BT CB A4
                BIT 3,B'               ; @BT CB 58
                LD IXL,IXH             ; @BT DD 6C
                BIT 4,H'               ; @BT CB 64
                SET 4,C'               ; @BT CB E1
                XOR A',A'              ; @BT AF
                LD (IX+((($13-(8+1))+8)+1)),B' ; @BT DD 70 13
                LD IXH,A'              ; @BT DD 67
                SBC A',E'              ; @BT 9B
                BIT 4,A'               ; @BT CB 67
                CP A',IXH              ; @BT DD BC
                SUB A',IYH             ; @BT FD 94
                CP A',(IY+(($4F-(5+6))+5+6)) ; @BT FD BE 4F
                LD C',L'               ; @BT 4D
                ADC A',E'              ; @BT 8B
                SET 5,C'               ; @BT CB E9
                LD IX,(((T4_DEF_023-7)+(7-3))+3) ; @BT DD 2A 93 99
                RRC E'                 ; @BT CB 0B
                LD E',(IX+(($53-(9+5))+9+5)) ; @BT DD 5E 53
                RES 1,L'               ; @BT CB 8D
                DEC (IX+((($11-(5+1))+5)+1)) ; @BT DD 35 11
                ADD A',B'              ; @BT 80
                IN F',(C')             ; @BT ED 70
                JP (HL')               ; @BT E9
                LD (((($D43A-(5+5))+5)+5)),SP ; @BT ED 73 3A D4
                DEC HL'                ; @BT 2B
                LD H',D'               ; @BT 62
                RR A'                  ; @BT CB 1F
                JP POS,(((T4_DEF_008-(2+1))+2)+1) ; @BT F2 8B 08
                LD B',C'               ; @BT 41
                BIT 2,B'               ; @BT CB 50
                LD B',(IY+((($4C-9)+2)-2+9)) ; @BT FD 46 4C
                SRL E'                 ; @BT CB 3B
                LD A',(IY+((($10-1)+(1-3))+3)) ; @BT FD 7E 10
                SET 7,A'               ; @BT CB FF
                RES 0,B'               ; @BT CB 80
                RET                    ; @BT C9
                RLC C'                 ; @BT CB 01
                LD IYH,C'              ; @BT FD 61
                CPD                    ; @BT ED A9
                POP BC'                ; @BT C1
                EX AF',AF              ; @BT 08
                SUB A',C'              ; @BT 91
                RES 5,H'               ; @BT CB AC
                JP NEQ,(((T4_DEF_010-9)-(5-1))+(5-1)+9) ; @BT C2 7A 07
                BIT 7,E'               ; @BT CB 7B
                SBC A',C'              ; @BT 99
                LD IY,((($B54D-2)+9)-9+2) ; @BT FD 2A 4D B5
                RES 1,B'               ; @BT CB 88
                SET 5,L'               ; @BT CB ED
                LD D',(HL')            ; @BT 56
                LD L',((T4_DEF_021-(11+7))+11+7) ; @BT 2E 3B
                LD A',B'               ; @BT 78
                SLA D'                 ; @BT CB 22
                BIT 1,C'               ; @BT CB 49
                RES 2,B'               ; @BT CB 90
                RES 6,(HL')            ; @BT CB B6
                RLC D'                 ; @BT CB 02
                ADC HL',BC'            ; @BT ED 4A
                SET 1,C'               ; @BT CB C9
                BIT 4,B'               ; @BT CB 60
                DI                     ; @BT F3
                LD A',(HL')            ; @BT 7E
                SET 7,(HL')            ; @BT CB FE
                BIT 1,B'               ; @BT CB 48
                RES 6,H'               ; @BT CB B4
                PUSH HL'               ; @BT E5
                RES 2,(HL')            ; @BT CB 96
                SBC A',D'              ; @BT 9A
                RRCA                   ; @BT 0F
                LD C',B'               ; @BT 48
                IN D',(C')             ; @BT ED 50
                SBC A',(HL')           ; @BT 9E
                RES 1,E'               ; @BT CB 8B
                LD D',C'               ; @BT 51
                SET 1,E'               ; @BT CB CB
                OUTI                   ; @BT ED A3
                POP AF'                ; @BT F1
                LD SP,HL'              ; @BT F9
                LD D',D'               ; @BT 52
                BIT 1,D'               ; @BT CB 4A
                LD D',IXH              ; @BT DD 54
                LD E',IXL              ; @BT DD 5D
                NEG                    ; @BT ED 44
                SET 1,H'               ; @BT CB CC
                SET 0,L'               ; @BT CB C5
                RES 6,B'               ; @BT CB B0
                RES 7,A'               ; @BT CB BF
                LD E',IYL              ; @BT FD 5D
                OR A',(HL')            ; @BT B6
                SET 0,D'               ; @BT CB C2
                OUT (C'),H'            ; @BT ED 61
                LD IYH,(((T4_DEF_015-(10+7))+(10+7))) ; @BT FD 26 18
                JR (((T4_DEF_019-10)+(10-5))+5) ; @BT 18 A1
                OR A',(IX+(($38-(8+8))+8+8)) ; @BT DD B6 38
                RES 3,D'               ; @BT CB 9A
                INC L'                 ; @BT 2C
                RES 7,L'               ; @BT CB BD
                LD A',IYL              ; @BT FD 7D
                LD (IY+((($64-11)-(8-1))+(8-1)+11)),C' ; @BT FD 71 64
                SBC A',IYH             ; @BT FD 9C
                NOP                    ; @BT 00
                ADD A',L'              ; @BT 85
                LD D',A'               ; @BT 57
                RES 5,D'               ; @BT CB AA
                BIT 7,D'               ; @BT CB 7A
                JP LSS,(($0447-11)+11) ; @BT DA 47 04
                DEC SP                 ; @BT 3B
                LD C',D'               ; @BT 4A
                SET 3,D'               ; @BT CB DA
                LD (IY+((($6D-(7+1))+1)+7)),H' ; @BT FD 74 6D
                ADD A',IYL             ; @BT FD 85
                INC D'                 ; @BT 14
                XOR A',L'              ; @BT AD
                SET 5,A'               ; @BT CB EF
                ADC A',(IY+((($34-7)-9)+7+9)) ; @BT FD 8E 34
                SBC A',IYL             ; @BT FD 9D
                LD H',(HL')            ; @BT 66
                SET 4,B'               ; @BT CB E0
                ADD A',A'              ; @BT 87
                RES 2,D'               ; @BT CB 92
                SET 0,H'               ; @BT CB C4
                SRL B'                 ; @BT CB 38
                LD A',H'               ; @BT 7C
                SCF                    ; @BT 37
                INIR                   ; @BT ED B2
                ADC A',((($53-(5+1))+5)+1) ; @BT CE 53
                SL1 L'                 ; @BT CB 35
                LD (HL'),A'            ; @BT 77
                SRA D'                 ; @BT CB 2A
                LD A',A'               ; @BT 7F
                SET 2,(HL')            ; @BT CB D6
                RRD                    ; @BT ED 67
                XOR A',(IX+((($24-8)+(8-7))+7)) ; @BT DD AE 24
                EX (SP),IX             ; @BT DD E3
                XOR A',IYH             ; @BT FD AC
                SET 2,L'               ; @BT CB D5
                ADD IX,IX              ; @BT DD 29
                SET 1,A'               ; @BT CB CF
                LD A',((((T4_DEF_003-7)+(7-7))+7)) ; @BT 3A 66 26
                RET EQU                ; @BT C8
                SET 6,(HL')            ; @BT CB F6
                SET 2,E'               ; @BT CB D3
                RRC H'                 ; @BT CB 0C
                OUT (C'),A'            ; @BT ED 79
                LD B',A'               ; @BT 47
                INC C'                 ; @BT 0C
                SBC A',(((T4_DEF_004-3)+4)-4+3) ; @BT DE 82
                RET PO                 ; @BT E0
                INC A'                 ; @BT 3C
                SUB A',H'              ; @BT 94
                LD B',IYH              ; @BT FD 44
                LD (HL'),((($3E-(10+1))+10)+1) ; @BT 36 3E
                BIT 1,H'               ; @BT CB 4C
                LD H',B'               ; @BT 60
                RST (($08-(9+5))+9+5)  ; @BT CF
                LD IYH,A'              ; @BT FD 67
                BIT 4,E'               ; @BT CB 63
                OUTD                   ; @BT ED AB
                AND A',IYH             ; @BT FD A4
                RES 6,D'               ; @BT CB B2
                BIT 6,H'               ; @BT CB 74
                INC IXH                ; @BT DD 24
                PUSH BC'               ; @BT C5
                ADC HL',HL'            ; @BT ED 6A
                LD L',A'               ; @BT 6F
                BIT 0,(IY+$5F)         ; @BT FD CB 5F 46
                XOR A',(($48-(10+8))+10+8) ; @BT EE 48
                SET 3,L'               ; @BT CB DD
                INC H'                 ; @BT 24
                LD IYH,B'              ; @BT FD 60
                BIT 7,L'               ; @BT CB 7D
                RET POS                ; @BT F0
                CALL GTE,((($9BC5-5)-(2-1))+(2-1)+5) ; @BT D4 C5 9B
                IN B',(C')             ; @BT ED 40
                SET 4,H'               ; @BT CB E4
                SUB A',E'              ; @BT 93
                CP A',(HL')            ; @BT BE
                INC IYH                ; @BT FD 24
                SET 3,C'               ; @BT CB D9
                SET 3,A'               ; @BT CB DF
                CCF                    ; @BT 3F
                LD (((($D107-7)-6)+7+6)),BC' ; @BT ED 43 07 D1
                LD (HL'),H'            ; @BT 74
                BIT 3,L'               ; @BT CB 5D
                PUSH IY                ; @BT FD E5
                SL1 (HL')              ; @BT CB 36
                LD D',B'               ; @BT 50
                ADD IX,BC'             ; @BT DD 09
                RET PE                 ; @BT E8
                LD D',(IX+((($26-(5+1))+5)+1)) ; @BT DD 56 26
                LD B',(IX+(($43-(6+3))+6+3)) ; @BT DD 46 43
                SET 6,E'               ; @BT CB F3
                OUT (C'),C'            ; @BT ED 49
                OUT (C'),L'            ; @BT ED 69
                RL D'                  ; @BT CB 12
                ADC HL',DE'            ; @BT ED 5A
                LD IY,(((($B43C-(5+8))+(5+8)))) ; @BT FD 2A 3C B4
                SBC A',IXH             ; @BT DD 9C
                LD SP,IY               ; @BT FD F9
                BIT 5,E'               ; @BT CB 6B
                OR A',H'               ; @BT B4
                IM (((1-5)+(5-5))+5)   ; @BT ED 56
                SBC A',(IX+(($1B-(7+5))+7+5)) ; @BT DD 9E 1B
                RL L'                  ; @BT CB 15
                AND A',L'              ; @BT A5
                SL1 B'                 ; @BT CB 30
                SET 6,C'               ; @BT CB F1
                BIT 1,(HL')            ; @BT CB 4E
                LDI                    ; @BT ED A0
                BIT 5,C'               ; @BT CB 69
                LD C',IXL              ; @BT DD 4D
                LD IYL,C'              ; @BT FD 69
                ADC A',IXH             ; @BT DD 8C
                SUB A',A'              ; @BT 97
                BIT 6,B'               ; @BT CB 70
                RL (HL')               ; @BT CB 16
                LD D',IYH              ; @BT FD 54
                LD E',(HL')            ; @BT 5E
                LD A',E'               ; @BT 7B
                CALL POS,(((T4_DEF_005-(10+8))+(10+8))) ; @BT F4 E7 9D
                LD I,A'                ; @BT ED 47
                DEC DE'                ; @BT 1B
                LD A',((($6D-(9+1))+9)+1) ; @BT 3A 6D 00
                BIT 2,(HL')            ; @BT CB 56
                DEC (HL')              ; @BT 35
                LD E',(($5B-(10+6))+10+6) ; @BT 1E 5B
                JP (IY)                ; @BT FD E9
                ADD A',C'              ; @BT 81
                DEC (IY+((($1D-6)+1)-1+6)) ; @BT FD 35 1D
                SET 0,C'               ; @BT CB C1
                LD L',(IX+((($39-(7+1))+7)+1)) ; @BT DD 6E 39
                DEC B'                 ; @BT 05
                RL C'                  ; @BT CB 11
                RET NEQ                ; @BT C0
                LD B',IYL              ; @BT FD 45
                LD A',D'               ; @BT 7A
                LD IX,((((T4_DEF_006-(7+9))+9)+7)) ; @BT DD 2A 82 98
                BIT 3,H'               ; @BT CB 5C
                LD (IX+((($19-(8+1))+8)+1)),D' ; @BT DD 72 19
                LD IXL,B'              ; @BT DD 68
                BIT 4,L'               ; @BT CB 65
                DEC IYL                ; @BT FD 2D
                SRL A'                 ; @BT CB 3F
                LD E',E'               ; @BT 5B
                LD IYL,IYH             ; @BT FD 6C
                BIT 4,D'               ; @BT CB 62
                SUB A',(($EF-4)+4)     ; @BT D6 EF
                RRA                    ; @BT 1F
                SET 7,E'               ; @BT CB FB
                LD C',IYL              ; @BT FD 4D
                BIT 3,A'               ; @BT CB 5F
                SET 1,B'               ; @BT CB C8
                INC BC'                ; @BT 03
                RL A'                  ; @BT CB 17
                RES 2,H'               ; @BT CB 94
                SL1 H'                 ; @BT CB 34
                XOR A',B'              ; @BT A8
                SET 7,B'               ; @BT CB F8
                SBC HL',HL'            ; @BT ED 62
                BIT 4,(HL')            ; @BT CB 66
                RST ((($30-4)+1)-1+4)  ; @BT F7
                HALT                   ; @BT 76
                RES 0,C'               ; @BT CB 81
                RLD                    ; @BT ED 6F
                ADD IY,DE'             ; @BT FD 19
                CP A',D'               ; @BT BA
                JP EQU,(($099C-(7+7))+7+7) ; @BT CA 9C 09
                SLA C'                 ; @BT CB 21
                ADD IY,SP              ; @BT FD 39
                LD L',L'               ; @BT 6D
                LD ((((T4_DEF_017-(7+1))+7)+1)),IY ; @BT FD 22 4B D5
                RRC C'                 ; @BT CB 09
                JR GTE,((($5C-6)-9)+6+9) ; @BT 30 5C
                RES 6,E'               ; @BT CB B3
                RES 3,E'               ; @BT CB 9B
                LD HL',(((T4_DEF_013-7)+1)-1+7) ; @BT 2A BF 7B
                AND A',IXH             ; @BT DD A4
                AND A',E'              ; @BT A3
                CALL PE,(((T4_DEF_030-(3+3))+(3+3))) ; @BT EC 09 A0
                RST (($28-(4+9))+4+9)  ; @BT EF
                LD IYH,D'              ; @BT FD 62
                IN E',(C')             ; @BT ED 58
                RES 3,H'               ; @BT CB 9C
                SET 0,(HL')            ; @BT CB C6
                SET 5,E'               ; @BT CB EB
                LD IXH,E'              ; @BT DD 63
                JR NEQ,((($73-10)-(8-1))+(8-1)+10) ; @BT 20 73
                LD B',IXL              ; @BT DD 45
                RES 7,E'               ; @BT CB BB
                BIT 6,L'               ; @BT CB 75
                LD E',IXH              ; @BT DD 5C
                PUSH IX                ; @BT DD E5
                LD (IY+((($67-10)+5)-5+10)),D' ; @BT FD 72 67
                RLA                    ; @BT 17
                LD H',(IY+((($7D-(4+1))+4)+1)) ; @BT FD 66 7D
                LD A',I                ; @BT ED 57
                ADD A',(((T4_DEF_020-4)+1)-1+4) ; @BT C6 08
                LD E',D'               ; @BT 5A
                RES 0,A'               ; @BT CB 87
                OR A',L'               ; @BT B5
                AND A',(IX+((($35-5)-(8-1))+(8-1)+5)) ; @BT DD A6 35
                SLA (HL')              ; @BT CB 26
                OUT (C'),E'            ; @BT ED 59
                IN A',((((T4_DEF_001-(6+6))+(6+6)))) ; @BT DB 51
                LD (HL'),D'            ; @BT 72
                PUSH AF'               ; @BT F5
                INC SP                 ; @BT 33
                RES 4,D'               ; @BT CB A2
                RES 3,B'               ; @BT CB 98
                POP HL'                ; @BT E1
                LD C',C'               ; @BT 49
                LD (((T4_DEF_029-9)+9)),DE' ; @BT ED 53 18 D2
                SUB A',(HL')           ; @BT 96
                LD B',L'               ; @BT 45
                LD (HL'),C'            ; @BT 71
                LD B',IXH              ; @BT DD 44
                DEC IYH                ; @BT FD 25
                SRA L'                 ; @BT CB 2D
                LD DE',(((($510F-(7+9))+(7+9)))) ; @BT ED 5B 0F 51
                RLC B'                 ; @BT CB 00
                LD (BC'),A'            ; @BT 02
                RST ((($20-(6+1))+6)+1) ; @BT E7
                LD (DE'),A'            ; @BT 12
                IM ((0-11)+11)         ; @BT ED 46
                XOR A',E'              ; @BT AB
                OUT (((($09-(7+8))+(7+8)))),A' ; @BT D3 09
                RES 7,C'               ; @BT CB B9
                RES 5,L'               ; @BT CB AD
                BIT 7,H'               ; @BT CB 7C
                LD A',C'               ; @BT 79
                CP A',A'               ; @BT BF
                SET 5,D'               ; @BT CB EA
                EX DE',HL'             ; @BT EB
                LD E',L'               ; @BT 5D
                RES 0,D'               ; @BT CB 82
                SLA A'                 ; @BT CB 27
                LD IYH,IYH             ; @BT FD 64
                SRL D'                 ; @BT CB 3A
                OR A',(IY+(($41-4)+4)) ; @BT FD B6 41
                JP NEG,(($0558-6)+6)   ; @BT FA 58 05
                CPIR                   ; @BT ED B1
                RRC L'                 ; @BT CB 0D
                CP A',(((T4_DEF_018-5)+(5-4))+4) ; @BT FE A2
                ADC A',H'              ; @BT 8C
                OR A',A'               ; @BT B7
                IM (((2-(2+1))+2)+1)   ; @BT ED 5E
                SET 4,A'               ; @BT CB E7
                LD IXH,B'              ; @BT DD 60
                DAA                    ; @BT 27
                RLC (HL')              ; @BT CB 06
                LD C',H'               ; @BT 4C
                INC (IX+((($0F-11)+9)-9+11)) ; @BT DD 34 0F
                LD E',A'               ; @BT 5F
                SET 7,C'               ; @BT CB F9
                LD D',H'               ; @BT 54
                BIT 7,B'               ; @BT CB 78
                SUB A',D'              ; @BT 92
                SUB A',B'              ; @BT 90
                INC B'                 ; @BT 04
                RES 7,D'               ; @BT CB BA
                LD IXL,E'              ; @BT DD 6B
                XOR A',IXL             ; @BT DD AD
                BIT 6,D'               ; @BT CB 72
                ADD HL',BC'            ; @BT 09
                AND A',IXL             ; @BT DD A5
                AND A',(IY+((($3E-(7+1))+(7+1)))) ; @BT FD A6 3E
                LD C',IYH              ; @BT FD 4C
                LD IXL,(((T4_DEF_009-(2+1))+2)+1) ; @BT DD 2E 7A
                LD C',((($64-2)+(2-7))+7) ; @BT 0E 64
                BIT 0,E'               ; @BT CB 43
                ADC A',IYL             ; @BT FD 8D
                XOR A',IYL             ; @BT FD AD
                LD IYL,A'              ; @BT FD 6F
                LD C',E'               ; @BT 4B
                LD (HL'),E'            ; @BT 73
                LD (IX+((($1F-5)+3)-3+5)),H' ; @BT DD 74 1F
                JP PE,(((T4_DEF_024-(6+4))+(6+4))) ; @BT EA AD 0A
                LD (IY+((($70-8)+(8+7))-7)),L' ; @BT FD 75 70
                ADC HL',SP             ; @BT ED 7A
                RR L'                  ; @BT CB 1D
                LD A',(IX+((($07-2)+(2-8))+8)) ; @BT DD 7E 07
                EXX                    ; @BT D9
                RL H'                  ; @BT CB 14
                POP IX                 ; @BT DD E1
                LD A',IXH              ; @BT DD 7C
                LD ((((T4_DEF_016-7)-5)+7+5)),HL' ; @BT 22 29 D3
                SUB A',IXH             ; @BT DD 94
                SET 5,H'               ; @BT CB EC
                LD SP,((($9660-3)+6)-6+3) ; @BT ED 7B 60 96
                LD BC',(((T4_DEF_014-(6+4))+6+4)) ; @BT ED 4B CC 2B
                SUB A',IXL             ; @BT DD 95
                SL1 E'                 ; @BT CB 33
                SRL C'                 ; @BT CB 39
                SET 3,H'               ; @BT CB DC
                RES 6,A'               ; @BT CB B7
                RES 2,E'               ; @BT CB 93
                IN A',(C')             ; @BT ED 78
                ADC A',IYH             ; @BT FD 8C
                LD (IX+((($25-(4+7))+7)+4)),((($32-3)+(3-3))+3) ; @BT DD 36 25 32
                BIT 5,H'               ; @BT CB 6C
                JP PO,(((T4_DEF_025-(8+1))+8)+1) ; @BT E2 BE 0B
                OR A',((($0C-(8+9))+9)+8) ; @BT F6 0C
                RLC H'                 ; @BT CB 04
                SRA (HL')              ; @BT CB 2E
                AND A',B'              ; @BT A0
                LD IYL,((($D0-8)-(1-1))+(1-1)+8) ; @BT FD 2E D0
                SET 6,B'               ; @BT CB F0
                SET 7,H'               ; @BT CB FC
                LD B',D'               ; @BT 42
                RES 4,B'               ; @BT CB A0
                CPDR                   ; @BT ED B9
                LD A',IXL              ; @BT DD 7D
                BIT 7,(HL')            ; @BT CB 7E
                CP A',C'               ; @BT B9
                LD H',L'               ; @BT 65
                RES 0,H'               ; @BT CB 84
                CALL NEQ,((($9CD6-(10+1))+(10+1))) ; @BT C4 D6 9C
                EI                     ; @BT FB
                SET 4,E'               ; @BT CB E3
                LD H',H'               ; @BT 64
                CP A',H'               ; @BT BC
                LD HL',(((($7AA6-7)-(2-1))+(2-1)+7)) ; @BT 2A A6 7A
                SLA L'                 ; @BT CB 25
                ADD A',E'              ; @BT 83
                INC IXL                ; @BT DD 2C
                SET 5,B'               ; @BT CB E8
                AND A',(HL')           ; @BT A6
                DEC E'                 ; @BT 1D
                SET 6,H'               ; @BT CB F4
                AND A',(((T4_DEF_028-6)-1)+6+1) ; @BT E6 75
                LD C',(IX+((($70-(5+1))+5)+1)) ; @BT DD 4E 70
                INC (IY+((($1B-(2+1))+2)+1)) ; @BT FD 34 1B
                BIT 6,A'               ; @BT CB 77
                BIT 5,D'               ; @BT CB 6A
                RR D'                  ; @BT CB 1A
                ADD A',IYH             ; @BT FD 84
                RES 4,(HL')            ; @BT CB A6
                ADC A',A'              ; @BT 8F
                BIT 2,A'               ; @BT CB 57
                SBC HL',DE'            ; @BT ED 52
                LD L',D'               ; @BT 6A
                RL E'                  ; @BT CB 13
                JP (IX)                ; @BT DD E9
                LD C',(IY+((($79-(7+1))+7)+1)) ; @BT FD 4E 79
                LD IYL,E'              ; @BT FD 6B
                RES 1,(HL')            ; @BT CB 8E
                LD A',R                ; @BT ED 5F
                XOR A',(HL')           ; @BT AE
                ADD HL',DE'            ; @BT 19
                OR A',D'               ; @BT B2
                BIT 1,E'               ; @BT CB 4B
                RES 3,(HL')            ; @BT CB 9E
                POP IY                 ; @BT FD E1
                LD IXH,(($C2-(4+4))+4+4) ; @BT DD 26 C2
                ADD HL',SP             ; @BT 39
                SRA B'                 ; @BT CB 28
                BIT 0,A'               ; @BT CB 47
                CPI                    ; @BT ED A1
                ADD A',(IX+((($64-4)-(7-1))+(7-1)+4)) ; @BT DD 86 64
                LD BC',(((T4_DEF_012-(5+1))+5)+1) ; @BT ED 4B CD 2C
                SET 3,B'               ; @BT CB D8
                RES 4,A'               ; @BT CB A7
                SL1 D'                 ; @BT CB 32
                RST ((($10-7)+(7-1))+1) ; @BT D7
                INC HL'                ; @BT 23
                RST ((($18-(3+1))+3)+1) ; @BT DF
                SET 2,D'               ; @BT CB D2
                SUB A',L'              ; @BT 95
                RLC L'                 ; @BT CB 05
                INC IYL                ; @BT FD 2C
                POP DE'                ; @BT D1
                LD IYL,B'              ; @BT FD 68
                SET 4,(HL')            ; @BT CB E6
                OR A',IYH              ; @BT FD B4
                INI                    ; @BT ED A2
                DEC H'                 ; @BT 25
                CALL ((T4_DEF_007-(11+4))+11+4) ; @BT CD 2B A2
                ADD A',IXL             ; @BT DD 85
                RES 5,E'               ; @BT CB AB
                LD B',(((T4_DEF_011-(8+1))+1)+8) ; @BT 06 0B
                ADD A',H'              ; @BT 84
                LD IXH,IXH             ; @BT DD 64
                LD D',IYL              ; @BT FD 55
                SET 3,(HL')            ; @BT CB DE
                RET LSS                ; @BT D8
                RES 4,L'               ; @BT CB A5
                LD IXL,A'              ; @BT DD 6F
                SET 6,D'               ; @BT CB F2
                LD IXH,IXL             ; @BT DD 65
                LD IXH,C'              ; @BT DD 61
                LD D',(IY+(($2F-8)+8)) ; @BT FD 56 2F
                SL1 C'                 ; @BT CB 31
                LD L',E'               ; @BT 6B
                RES 7,B'               ; @BT CB B8
                LD B',H'               ; @BT 44
                XOR A',(IY+((($2D-(8+1))+8)+1)) ; @BT FD AE 2D
                RES 3,L'               ; @BT CB 9D
                DEC IXL                ; @BT DD 2D
                RES 5,C'               ; @BT CB A9
                SRL L'                 ; @BT CB 3D
                BIT 0,D'               ; @BT CB 42
                LD ((((T4_DEF_026-11)+5)-5+11)),A' ; @BT 32 F6 CF
                LD IYL,D'              ; @BT FD 6A
                LD (IY+((($61-(2+1))+2)+1)),B' ; @BT FD 70 61
                BIT 3,C'               ; @BT CB 59
                ADD A',IXH             ; @BT DD 84
                RES 4,C'               ; @BT CB A1
                LD A',(BC')            ; @BT 0A
                RLCA                   ; @BT 07
                RR E'                  ; @BT CB 1B
                RET NEG                ; @BT F8
                DEC BC'                ; @BT 0B
                JP ((($0CCF-10)-1)+10+1) ; @BT C3 CF 0C
                CP A',L'               ; @BT BD
                SUB A',(IY+((($7A-(3+8))+8)+3)) ; @BT FD 96 7A
                DJNZ ((($26-(11+7))+7)+11) ; @BT 10 26
                CPL                    ; @BT 2F
                DEC IY                 ; @BT FD 2B
                ADC A',(HL')           ; @BT 8E
                LD L',(IY+(($3C-11)+11)) ; @BT FD 6E 3C
                LD IXL,IXL             ; @BT DD 6D
                SET 1,L'               ; @BT CB CD
                LD (IY+((($73-6)+(6-2))+2)),(($88-(8+9))+8+9) ; @BT FD 36 73 88
                ADC A',B'              ; @BT 88
                OR A',C'               ; @BT B1
                RR (HL')               ; @BT CB 1E
                CP A',E'               ; @BT BB
                SLA E'                 ; @BT CB 23
                OR A',E'               ; @BT B3
                BIT 3,E'               ; @BT CB 5B
                INDR                   ; @BT ED BA
                LD ((((T4_DEF_033-9)-(7-1))+(7-1)+9)),IX ; @BT DD 22 5C D6
                JP GTE,((($0669-9)+(9+5))-5) ; @BT D2 69 06
                LD (IX+((($16-5)-2)+5+2)),C' ; @BT DD 71 16
                AND A',IYL             ; @BT FD A5
                SLA H'                 ; @BT CB 24
                BIT 6,(HL')            ; @BT CB 76
                SET 6,L'               ; @BT CB F5
                LD (HL'),L'            ; @BT 75
                RES 0,E'               ; @BT CB 83
                RRC A'                 ; @BT CB 0F
                ADC A',C'              ; @BT 89
                ADD HL',HL'            ; @BT 29
                INC IX                 ; @BT DD 23
                INC IY                 ; @BT FD 23
                SET 4,L'               ; @BT CB E5
                SET 2,C'               ; @BT CB D1
                LD A',(DE')            ; @BT 1A
                BIT 1,A'               ; @BT CB 4F
                CALL PO,((($A11A-6)+(6-4))+4) ; @BT E4 1A A1
                BIT 0,L'               ; @BT CB 45
                SET 3,E'               ; @BT CB DB
                OUT (C'),0             ; @BT ED 71
                RES 2,L'               ; @BT CB 95
                OR A',IXL              ; @BT DD B5
                LD IYH,E'              ; @BT FD 63
                BIT 1,L'               ; @BT CB 4D
                LD R,A'                ; @BT ED 4F
                RLC A'                 ; @BT CB 07
                SBC A',A'              ; @BT 9F
                SRA C'                 ; @BT CB 29
                RES 1,H'               ; @BT CB 8C
                SLA B'                 ; @BT CB 20
                IN C',(C')             ; @BT ED 48
                RES 6,L'               ; @BT CB B5
                RES 1,D'               ; @BT CB 8A
                RETN                   ; @BT ED 45
                JR LSS,(($45-4)+4)     ; @BT 38 45
                OR A',IYL              ; @BT FD B5
                LD IXH,D'              ; @BT DD 62
                LD D',L'               ; @BT 55
                ADD IY,BC'             ; @BT FD 09
                CP A',IYH              ; @BT FD BC
                LD B',E'               ; @BT 43
                RES 0,(HL')            ; @BT CB 86
                DEC IXH                ; @BT DD 25
                SBC A',IXL             ; @BT DD 9D
                BIT 5,A'               ; @BT CB 6F
                OTIR                   ; @BT ED B3
                SET 5,(HL')            ; @BT CB EE
                CP A',IYL              ; @BT FD BD
                LD C',IXH              ; @BT DD 4C
                SET 2,A'               ; @BT CB D7
                LD IYL,IYL             ; @BT FD 6D
                ADD A',(HL')           ; @BT 86
                LD E',C'               ; @BT 59
                EX (SP),HL'            ; @BT E3
                RETI                   ; @BT ED 4D
                EX (SP),IY             ; @BT FD E3
                RES 7,H'               ; @BT CB BC
                BIT 7,A'               ; @BT CB 7F
                SET 1,D'               ; @BT CB CA
                BIT 2,H'               ; @BT CB 54
                OUT (C'),D'            ; @BT ED 51
                RES 3,A'               ; @BT CB 9F
                SET 1,(HL')            ; @BT CB CE
                SET 0,E'               ; @BT CB C3
                BIT 2,C'               ; @BT CB 51
                RR C'                  ; @BT CB 19
                SUB A',IYL             ; @BT FD 95
                SUB A',(IX+((($77-7)-(8-1))+(8-1)+7)) ; @BT DD 96 77
                LD L',C'               ; @BT 69
                LD DE',((($5220-10)-9)+10+9) ; @BT ED 5B 20 52
                INC E'                 ; @BT 1C
                LD H',(IX+((($7A-(2+2))+(2+2)))) ; @BT DD 66 7A
                IN H',(C')             ; @BT ED 60
                IND                    ; @BT ED AA
                LD L',(HL')            ; @BT 6E
                LDD                    ; @BT ED A8

    .db "<04"               ; @BT END

    .BANK 4 SLOT 0
    .ORG 0

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////












    ; TEST-05 generated defines/functions
    .define T5_DEF_001 ((($51*10)/10)+(9-9)+(9-9))
    .define T5_DEF_002 (((T5_DEF_001)+($99A3)-($51))+(1-1))
    .define T5_DEF_003 ((($2667+6)-6)+(1-2))
    .define T5_DEF_004 ((($83)-($2667)+(T5_DEF_003))+(11-11)+(1-1))
    .define T5_DEF_005 ((($9DE7*2)-($9DE7))+($9DE7)+(9-9)+(1-1)-$9DE7)
    .define T5_DEF_006 ((($9882)+(T5_DEF_005)-($9DE7))+((4+6)-(4+6)))
    .define T5_DEF_007 ((($A22B+13)-(13+3)+3)+(2-2))
    .define T5_DEF_008 (((T5_DEF_007)+($088B)-($A22B))+(2-2))
    .define T5_DEF_009 ((($7A+7)-(7+1)+1)+(8-8))
    .define T5_DEF_010 ((($077A)+(T5_DEF_009)-($7A))+((10+7)-(10+7)))
    .define T5_DEF_011 ((($0B*2)-($0B))+($0B)+(12-12)+(4-4)-$0B)
    .define T5_DEF_012 (((T5_DEF_011)*2+($2CCD)-(($0B)*2))+(11-11))
    .define T5_DEF_013 ((($7BB7+3)-3)+(10-2))
    .define T5_DEF_014 (((T5_DEF_013)*2+($2BBC)-(($7BB7)*2))+(11-11))
    .define T5_DEF_015 ((($18-2)+2)-(7-2)+(7-2))
    .define T5_DEF_016 (((T5_DEF_015)*2+($D329)-(($18)*2))+(9-9))
    .define T5_DEF_017 ((($D54B+(8*2))-(8*2))+(9-9))
    .define T5_DEF_018 (((T5_DEF_017)*2+($A2)-(($D54B)*2))+(11-11))
    .define T5_DEF_019 ((($A1+12+8)-12-8)+(3-3))
    .define T5_DEF_020 ((($08)-($A1)+(T5_DEF_019))+(2-2)+(1-1))
    .define T5_DEF_021 ((($3B+11)-(11+5)+5)+(4-4))
    .define T5_DEF_022 ((($58)+(T5_DEF_021)-($3B))+((6+2)-(6+2)))
    .define T5_DEF_023 ((($9993+3+9)-3-9)+(7-7))
    .define T5_DEF_024 ((($0AAD)+(T5_DEF_023)-($9993))+((11+9)-(11+9)))
    .define T5_DEF_025 ((($0BBE+(9*1))-(9*1))+(9-9))
    .define T5_DEF_026 (((T5_DEF_025)*2+($CFF6)-(($0BBE)*2))+(10-10))
    .define T5_DEF_027 ((($9AB4-3)+3)-(3-8)+(3-8))
    .define T5_DEF_028 (((T5_DEF_027)+($75)-($9AB4))+(8-8))
    .define T5_DEF_029 ((($D218+2+8)-2-8)+(3-3))
    .define T5_DEF_030 (((T5_DEF_029)*2+($A009)-(($D218)*2))+(9-9))
    .define T5_DEF_031 ((($954F+8+3)-8-3)+(9-9))
    .define T5_DEF_032 (((T5_DEF_031)+($9EF8)-($954F))+(4-4))
    .define T5_DEF_033 ((($D65C-12)+12)-(11-2)+(11-2))

    ; TEST-05 generated functions
    .function F5_FN_001(a,b) (((a-b)+(8-8))+(T5_DEF_030-T5_DEF_030))
    .function F5_FN_002(a,b) (((F5_FN_001(a,b))+((5+1)-(5+1)))+(T5_DEF_002-T5_DEF_002))
    .function F5_FN_003(a,b,c) ((((a+b-c)+8)-8)+(T5_DEF_027-T5_DEF_027))
    .function F5_FN_004(a,b,c) (((F5_FN_003(a,b,c))+2-2)+(4-4)+(T5_DEF_007-T5_DEF_007))
    .function F5_FN_005(a,b,c,dv) ((((a+b-c-dv)+2)-2)+(T5_DEF_005-T5_DEF_005))
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04 with full TEST-05 randomization + varied function replacement
    ; GENERATED: seed = 1507128863
    ; GENERATED: replaced define refs = 5 / 10

                    BIT 1,E'           ; @BT CB 4B
                    JP POS,(((T5_DEF_008-(2+1))+2)+1) ; @BT F2 8B 08
                    RR H'              ; @BT CB 1C
                    LD E',IYL          ; @BT FD 5D
                    POP DE'            ; @BT D1
                    CALL PE,(((F5_FN_001(T5_DEF_030+15,15)-(3+3))+(3+3))) ; @BT EC 09 A0
                    LD (HL'),B'        ; @BT 70
                    RES 6,C'           ; @BT CB B1
                    PUSH BC'           ; @BT C5
                    JP PE,(((T5_DEF_024-(6+4))+(6+4))) ; @BT EA AD 0A
                    POP BC'            ; @BT C1
                    LD C',(IX+((($70-(5+1))+5)+1)) ; @BT DD 4E 70
                    SRA D'             ; @BT CB 2A
                    RRC B'             ; @BT CB 08
                    DEC SP             ; @BT 3B
                    INC C'             ; @BT 0C
                    ADC HL',SP         ; @BT ED 7A
                    RR B'              ; @BT CB 18
                    RES 2,A'           ; @BT CB 97
                    NEG                ; @BT ED 44
                    SET 2,A'           ; @BT CB D7
                    RL H'              ; @BT CB 14
                    LDDR               ; @BT ED B8
                    CP A',(HL')        ; @BT BE
                    RET NEQ            ; @BT C0
                    RST ((($10-7)+(7-1))+1) ; @BT D7
                    EX DE',HL'         ; @BT EB
                    OR A',IYL          ; @BT FD B5
                    RES 2,D'           ; @BT CB 92
                    LD IYH,IYH         ; @BT FD 64
                    LD IYL,((($D0-8)-(1-1))+(1-1)+8) ; @BT FD 2E D0
                    LD D',((($02-4)+(4-9))+9) ; @BT 16 02
                    INC IYL            ; @BT FD 2C
                    SET 1,A'           ; @BT CB CF
                    RES 2,L'           ; @BT CB 95
                    RES 5,E'           ; @BT CB AB
                    SLA (HL')          ; @BT CB 26
                    BIT 0,(HL')        ; @BT CB 46
                    RES 7,L'           ; @BT CB BD
                    ADD A',H'          ; @BT 84
                    XOR A',C'          ; @BT A9
                    DEC C'             ; @BT 0D
                    LD L',A'           ; @BT 6F
                    CP A',C'           ; @BT B9
                    SRL L'             ; @BT CB 3D
                    INC D'             ; @BT 14
                    LD IY,(((($B43C-(5+8))+(5+8)))) ; @BT FD 2A 3C B4
                    LD IYH,B'          ; @BT FD 60
                    BIT 0,(IY+$5F)     ; @BT FD CB 5F 46
                    RES 0,C'           ; @BT CB 81
                    IND                ; @BT ED AA
                    BIT 6,L'           ; @BT CB 75
                    SET 7,E'           ; @BT CB FB
                    CP A',IXH          ; @BT DD BC
                    OR A',H'           ; @BT B4
                    CPL                ; @BT 2F
                    IN D',(C')         ; @BT ED 50
                    ADD IX,SP          ; @BT DD 39
                    BIT 2,E'           ; @BT CB 53
                    LD B',L'           ; @BT 45
                    SUB A',C'          ; @BT 91
                    SBC A',L'          ; @BT 9D
                    SRL H'             ; @BT CB 3C
                    SBC A',(IY+((($24-(11+2))+(11+2)))) ; @BT FD 9E 24
                    ADC A',E'          ; @BT 8B
                    LD IX,((((T5_DEF_006-(7+9))+9)+7)) ; @BT DD 2A 82 98
                    INC IY             ; @BT FD 23
                    LD E',(IX+(($53-(9+5))+9+5)) ; @BT DD 5E 53
                    SCF                ; @BT 37
                    SRL B'             ; @BT CB 38
                    JP (IY)            ; @BT FD E9
                    RES 0,A'           ; @BT CB 87
                    LD IYL,A'          ; @BT FD 6F
                    SET 7,L'           ; @BT CB FD
                    LD SP,((($9660-3)+6)-6+3) ; @BT ED 7B 60 96
                    INDR               ; @BT ED BA
                    LD (IY+((($70-8)+(8+7))-7)),L' ; @BT FD 75 70
                    RES 0,D'           ; @BT CB 82
                    XOR A',(IY+((($2D-(8+1))+8)+1)) ; @BT FD AE 2D
                    LD D',IYL          ; @BT FD 55
                    SLA A'             ; @BT CB 27
                    BIT 1,D'           ; @BT CB 4A
                    LD (HL'),L'        ; @BT 75
                    LD C',IYH          ; @BT FD 4C
                    ADC A',D'          ; @BT 8A
                    SET 4,L'           ; @BT CB E5
                    IM (((1-5)+(5-5))+5) ; @BT ED 56
                    OUTD               ; @BT ED AB
                    SET 5,A'           ; @BT CB EF
                    LD A',B'           ; @BT 78
                    BIT 5,(HL')        ; @BT CB 6E
                    BIT 6,B'           ; @BT CB 70
                    SUB A',(($EF-4)+4) ; @BT D6 EF
                    LD IXL,(((T5_DEF_009-(2+1))+2)+1) ; @BT DD 2E 7A
                    BIT 4,E'           ; @BT CB 63
                    LD B',E'           ; @BT 43
                    BIT 5,A'           ; @BT CB 6F
                    LD IXH,(($C2-(4+4))+4+4) ; @BT DD 26 C2
                    CP A',A'           ; @BT BF
                    BIT 7,E'           ; @BT CB 7B
                    DEC IXL            ; @BT DD 2D
                    RES 5,C'           ; @BT CB A9
                    LD C',IXH          ; @BT DD 4C
                    DEC BC'            ; @BT 0B
                    ADD HL',DE'        ; @BT 19
                    LD L',(IX+((($39-(7+1))+7)+1)) ; @BT DD 6E 39
                    DEC D'             ; @BT 15
                    LD H',(HL')        ; @BT 66
                    RLA                ; @BT 17
                    SRL (HL')          ; @BT CB 3E
                    SBC A',C'          ; @BT 99
                    RL B'              ; @BT CB 10
                    SET 6,C'           ; @BT CB F1
                    XOR A',B'          ; @BT A8
                    JP NEQ,(((T5_DEF_010-9)-(5-1))+(5-1)+9) ; @BT C2 7A 07
                    XOR A',(HL')       ; @BT AE
                    LD L',C'           ; @BT 69
                    INIR               ; @BT ED B2
                    BIT 5,C'           ; @BT CB 69
                    SET 7,A'           ; @BT CB FF
                    OUT (C'),C'        ; @BT ED 49
                    LD C',H'           ; @BT 4C
                    SET 4,E'           ; @BT CB E3
                    LD E',L'           ; @BT 5D
                    LD A',H'           ; @BT 7C
                    BIT 5,L'           ; @BT CB 6D
                    LD E',(HL')        ; @BT 5E
                    CALL NEG,((F5_FN_003(T5_DEF_027+8,15,8+15)-(5+2))+5+2) ; @BT FC B4 9A
                    RRC A'             ; @BT CB 0F
                    INC L'             ; @BT 2C
                    LD ((((T5_DEF_033-9)-(7-1))+(7-1)+9)),IX ; @BT DD 22 5C D6
                    LD E',A'           ; @BT 5F
                    BIT 0,C'           ; @BT CB 41
                    ADD A',IYL         ; @BT FD 85
                    LD E',IYH          ; @BT FD 5C
                    LD L',(IY+(($3C-11)+11)) ; @BT FD 6E 3C
                    SET 2,B'           ; @BT CB D0
                    LD IYL,E'          ; @BT FD 6B
                    RES 4,C'           ; @BT CB A1
                    RES 7,E'           ; @BT CB BB
                    RL (HL')           ; @BT CB 16
                    INC H'             ; @BT 24
                    OR A',A'           ; @BT B7
                    IN B',(C')         ; @BT ED 40
                    DEC HL'            ; @BT 2B
                    RES 6,D'           ; @BT CB B2
                    LD ((((T5_DEF_026-11)+5)-5+11)),A' ; @BT 32 F6 CF
                    OUT (C'),L'        ; @BT ED 69
                    ADC HL',BC'        ; @BT ED 4A
                    RES 3,L'           ; @BT CB 9D
                    AND A',(((T5_DEF_028-6)-1)+6+1) ; @BT E6 75
                    SLA H'             ; @BT CB 24
                    RET                ; @BT C9
                    SET 4,D'           ; @BT CB E2
                    CP A',H'           ; @BT BC
                    LD C',A'           ; @BT 4F
                    NOP                ; @BT 00
                    JP ((($0CCF-10)-1)+10+1) ; @BT C3 CF 0C
                    RST ((($20-(6+1))+6)+1) ; @BT E7
                    BIT 2,D'           ; @BT CB 52
                    SBC A',IYH         ; @BT FD 9C
                    LD IYH,A'          ; @BT FD 67
                    INC B'             ; @BT 04
                    XOR A',IYL         ; @BT FD AD
                    AND A',B'          ; @BT A0
                    RES 7,B'           ; @BT CB B8
                    OR A',C'           ; @BT B1
                    LD A',(BC')        ; @BT 0A
                    ADD A',IXH         ; @BT DD 84
                    INC (HL')          ; @BT 34
                    SUB A',A'          ; @BT 97
                    DEC IYH            ; @BT FD 25
                    LD L',D'           ; @BT 6A
                    LD L',B'           ; @BT 68
                    BIT 5,B'           ; @BT CB 68
                    JP GTE,((($0669-9)+(9+5))-5) ; @BT D2 69 06
                    SET 5,D'           ; @BT CB EA
                    BIT 0,E'           ; @BT CB 43
                    RLC L'             ; @BT CB 05
                    SRL E'             ; @BT CB 3B
                    SBC A',A'          ; @BT 9F
                    XOR A',E'          ; @BT AB
                    ADD A',IYH         ; @BT FD 84
                    LD A',E'           ; @BT 7B
                    PUSH IX            ; @BT DD E5
                    LD B',(((T5_DEF_011-(8+1))+1)+8) ; @BT 06 0B
                    DEC (IY+((($1D-6)+1)-1+6)) ; @BT FD 35 1D
                    BIT 0,B'           ; @BT CB 40
                    LD A',D'           ; @BT 7A
                    SL1 C'             ; @BT CB 31
                    SET 6,E'           ; @BT CB F3
                    LD IYH,D'          ; @BT FD 62
                    BIT 4,B'           ; @BT CB 60
                    SBC A',IXL         ; @BT DD 9D
                    BIT 0,D'           ; @BT CB 42
                    BIT 5,D'           ; @BT CB 6A
                    DJNZ ((($26-(11+7))+7)+11) ; @BT 10 26
                    LD B',IYH          ; @BT FD 44
                    CALL EQU,(((T5_DEF_032-9)-4)+9+4) ; @BT CC F8 9E
                    LD IX,(((T5_DEF_023-7)+(7-3))+3) ; @BT DD 2A 93 99
                    ADD A',B'          ; @BT 80
                    RES 4,H'           ; @BT CB A4
                    SBC A',(HL')       ; @BT 9E
                    SET 0,D'           ; @BT CB C2
                    RES 6,H'           ; @BT CB B4
                    BIT 3,H'           ; @BT CB 5C
                    RET EQU            ; @BT C8
                    SET 7,B'           ; @BT CB F8
                    SBC A',IXH         ; @BT DD 9C
                    OR A',(IX+(($38-(8+8))+8+8)) ; @BT DD B6 38
                    RES 5,B'           ; @BT CB A8
                    OUT (C'),H'        ; @BT ED 61
                    SET 1,C'           ; @BT CB C9
                    AND A',IXH         ; @BT DD A4
                    LD E',H'           ; @BT 5C
                    ADD HL',SP         ; @BT 39
                    JP (HL')           ; @BT E9
                    DEC IYL            ; @BT FD 2D
                    BIT 7,L'           ; @BT CB 7D
                    DEC (HL')          ; @BT 35
                    SET 5,B'           ; @BT CB E8
                    INC A'             ; @BT 3C
                    SLA C'             ; @BT CB 21
                    LD H',(IY+((($7D-(4+1))+4)+1)) ; @BT FD 66 7D
                    RES 1,A'           ; @BT CB 8F
                    LD (IX+((($10-7)-9)+7+9)),A' ; @BT DD 77 10
                    RES 3,E'           ; @BT CB 9B
                    SET 0,A'           ; @BT CB C7
                    XOR A',IXL         ; @BT DD AD
                    SUB A',D'          ; @BT 92
                    BIT 5,E'           ; @BT CB 6B
                    SL1 D'             ; @BT CB 32
                    RES 6,E'           ; @BT CB B3
                    BIT 4,(HL')        ; @BT CB 66
                    BIT 2,A'           ; @BT CB 57
                    ADC A',A'          ; @BT 8F
                    OUT (C'),B'        ; @BT ED 41
                    SET 7,(HL')        ; @BT CB FE
                    LD L',E'           ; @BT 6B
                    LD SP,HL'          ; @BT F9
                    PUSH AF'           ; @BT F5
                    EX (SP),IY         ; @BT FD E3
                    RRD                ; @BT ED 67
                    ADD A',(IX+((($64-4)-(7-1))+(7-1)+4)) ; @BT DD 86 64
                    LD H',L'           ; @BT 65
                    IN C',(C')         ; @BT ED 48
                    RES 7,(HL')        ; @BT CB BE
                    LD B',A'           ; @BT 47
                    DEC A'             ; @BT 3D
                    ADC A',IYL         ; @BT FD 8D
                    SRA A'             ; @BT CB 2F
                    CPIR               ; @BT ED B1
                    SUB A',(IY+((($7A-(3+8))+8)+3)) ; @BT FD 96 7A
                    SUB A',E'          ; @BT 93
                    POP HL'            ; @BT E1
                    LD IYH,E'          ; @BT FD 63
                    LD C',L'           ; @BT 4D
                    SBC A',(((T5_DEF_004-3)+4)-4+3) ; @BT DE 82
                    CP A',IYH          ; @BT FD BC
                    RES 6,B'           ; @BT CB B0
                    LD (IY+((($64-11)-(8-1))+(8-1)+11)),C' ; @BT FD 71 64
                    OTIR               ; @BT ED B3
                    RL A'              ; @BT CB 17
                    RES 6,A'           ; @BT CB B7
                    CPDR               ; @BT ED B9
                    XOR A',A'          ; @BT AF
                    OUTI               ; @BT ED A3
                    INC (IX+((($0F-11)+9)-9+11)) ; @BT DD 34 0F
                    LD (IX+((($22-(2+1))+2)+1)),L' ; @BT DD 75 22
                    RES 1,D'           ; @BT CB 8A
                    LD B',(IY+((($4C-9)+2)-2+9)) ; @BT FD 46 4C
                    BIT 7,(HL')        ; @BT CB 7E
                    AND A',IXL         ; @BT DD A5
                    BIT 7,H'           ; @BT CB 7C
                    SET 5,L'           ; @BT CB ED
                    BIT 7,D'           ; @BT CB 7A
                    HALT               ; @BT 76
                    AND A',(IY+((($3E-(7+1))+(7+1)))) ; @BT FD A6 3E
                    CP A',B'           ; @BT B8
                    BIT 0,A'           ; @BT CB 47
                    LD IYH,C'          ; @BT FD 61
                    XOR A',(($48-(10+8))+10+8) ; @BT EE 48
                    LD A',(HL')        ; @BT 7E
                    LD A',(IX+((($07-2)+(2-8))+8)) ; @BT DD 7E 07
                    BIT 2,H'           ; @BT CB 54
                    LD D',E'           ; @BT 53
                    RRCA               ; @BT 0F
                    SL1 B'             ; @BT CB 30
                    BIT 6,D'           ; @BT CB 72
                    LD A',L'           ; @BT 7D
                    SL1 H'             ; @BT CB 34
                    LD IYH,IYL         ; @BT FD 65
                    SRA H'             ; @BT CB 2C
                    JP (IX)            ; @BT DD E9
                    CALL GTE,((($9BC5-5)-(2-1))+(2-1)+5) ; @BT D4 C5 9B
                    RET NEG            ; @BT F8
                    LD IYH,(((T5_DEF_015-(10+7))+(10+7))) ; @BT FD 26 18
                    RES 0,H'           ; @BT CB 84
                    CCF                ; @BT 3F
                    SUB A',(HL')       ; @BT 96
                    ADC A',B'          ; @BT 88
                    ADD HL',BC'        ; @BT 09
                    RLC C'             ; @BT CB 01
                    SET 3,H'           ; @BT CB DC
                    INC IXH            ; @BT DD 24
                    AND A',L'          ; @BT A5
                    DEC IX             ; @BT DD 2B
                    SET 2,C'           ; @BT CB D1
                    INC (IY+((($1B-(2+1))+2)+1)) ; @BT FD 34 1B
                    RST ((($30-4)+1)-1+4) ; @BT F7
                    ADD IX,DE'         ; @BT DD 19
                    LD C',(IY+((($79-(7+1))+7)+1)) ; @BT FD 4E 79
                    RST ((($38-9)+(9-8))+8) ; @BT FF
                    RES 4,A'           ; @BT CB A7
                    BIT 1,A'           ; @BT CB 4F
                    XOR A',H'          ; @BT AC
                    RR D'              ; @BT CB 1A
                    RES 7,A'           ; @BT CB BF
                    ADC A',(HL')       ; @BT 8E
                    LD C',E'           ; @BT 4B
                    SL1 (HL')          ; @BT CB 36
                    INC BC'            ; @BT 03
                    OR A',D'           ; @BT B2
                    RES 0,L'           ; @BT CB 85
                    BIT 4,H'           ; @BT CB 64
                    SET 5,(HL')        ; @BT CB EE
                    LD A',((($6D-(9+1))+9)+1) ; @BT 3A 6D 00
                    JR NEQ,((($73-10)-(8-1))+(8-1)+10) ; @BT 20 73
                    AND A',E'          ; @BT A3
                    BIT 6,A'           ; @BT CB 77
                    SRA C'             ; @BT CB 29
                    RRA                ; @BT 1F
                    RES 4,B'           ; @BT CB A0
                    LD (HL'),E'        ; @BT 73
                    BIT 0,L'           ; @BT CB 45
                    LD A',((((T5_DEF_003-7)+(7-7))+7)) ; @BT 3A 66 26
                    LD D',(HL')        ; @BT 56
                    SET 4,C'           ; @BT CB E1
                    IN H',(C')         ; @BT ED 60
                    SET 0,H'           ; @BT CB C4
                    LD IYL,IYH         ; @BT FD 6C
                    SET 6,H'           ; @BT CB F4
                    SET 3,B'           ; @BT CB D8
                    PUSH IY            ; @BT FD E5
                    ADC HL',HL'        ; @BT ED 6A
                    JP NEG,(($0558-6)+6) ; @BT FA 58 05
                    LD A',C'           ; @BT 79
                    RLC B'             ; @BT CB 00
                    SBC HL',SP         ; @BT ED 72
                    LD DE',((($5220-10)-9)+10+9) ; @BT ED 5B 20 52
                    RES 3,(HL')        ; @BT CB 9E
                    RES 7,D'           ; @BT CB BA
                    DEC IXH            ; @BT DD 25
                    RST ((($00-(10+1))+10)+1) ; @BT C7
                    LD HL',(((($7AA6-7)-(2-1))+(2-1)+7)) ; @BT 2A A6 7A
                    LD (IX+((($1F-5)+3)-3+5)),H' ; @BT DD 74 1F
                    RES 0,(HL')        ; @BT CB 86
                    ADC A',IYH         ; @BT FD 8C
                    SET 3,L'           ; @BT CB DD
                    AND A',(HL')       ; @BT A6
                    SET 7,H'           ; @BT CB FC
                    POP IY             ; @BT FD E1
                    PUSH DE'           ; @BT D5
                    SLA L'             ; @BT CB 25
                    DEC H'             ; @BT 25
                    SBC A',H'          ; @BT 9C
                    OUT (C'),0         ; @BT ED 71
                    LD A',(DE')        ; @BT 1A
                    LD A',A'           ; @BT 7F
                    RES 6,L'           ; @BT CB B5
                    OR A',L'           ; @BT B5
                    INC IXL            ; @BT DD 2C
                    ADC A',C'          ; @BT 89
                    CP A',(IY+(($4F-(5+6))+5+6)) ; @BT FD BE 4F
                    RETI               ; @BT ED 4D
                    LD (IX+((($25-(4+7))+7)+4)),((($32-3)+(3-3))+3) ; @BT DD 36 25 32
                    EI                 ; @BT FB
                    RRC D'             ; @BT CB 0A
                    RST (($28-(4+9))+4+9) ; @BT EF
                    RST (($08-(9+5))+9+5) ; @BT CF
                    RES 2,(HL')        ; @BT CB 96
                    BIT 3,C'           ; @BT CB 59
                    BIT 6,H'           ; @BT CB 74
                    RLC H'             ; @BT CB 04
                    RES 1,H'           ; @BT CB 8C
                    ADD A',E'          ; @BT 83
                    CP A',E'           ; @BT BB
                    RES 0,E'           ; @BT CB 83
                    RR E'              ; @BT CB 1B
                    LD R,A'            ; @BT ED 4F
                    RL C'              ; @BT CB 11
                    RES 1,B'           ; @BT CB 88
                    LD (HL'),((($3E-(10+1))+10)+1) ; @BT 36 3E
                    JR (((T5_DEF_019-10)+(10-5))+5) ; @BT 18 A1
                    LD E',B'           ; @BT 58
                    LD IYL,C'          ; @BT FD 69
                    SET 1,B'           ; @BT CB C8
                    SET 5,C'           ; @BT CB E9
                    ADC A',L'          ; @BT 8D
                    ADD A',C'          ; @BT 81
                    SET 6,A'           ; @BT CB F7
                    LD D',H'           ; @BT 54
                    JR GTE,((($5C-6)-9)+6+9) ; @BT 30 5C
                    BIT 3,B'           ; @BT CB 58
                    BIT 6,E'           ; @BT CB 73
                    SL1 E'             ; @BT CB 33
                    SET 2,H'           ; @BT CB D4
                    LD DE',(((($510F-(7+9))+(7+9)))) ; @BT ED 5B 0F 51
                    LD L',L'           ; @BT 6D
                    RES 5,H'           ; @BT CB AC
                    SLA E'             ; @BT CB 23
                    CP A',IXL          ; @BT DD BD
                    LD D',B'           ; @BT 50
                    RR L'              ; @BT CB 1D
                    XOR A',IXH         ; @BT DD AC
                    LD (IY+((($61-(2+1))+2)+1)),B' ; @BT FD 70 61
                    EX AF',AF          ; @BT 08
                    CALL NEQ,((($9CD6-(10+1))+(10+1))) ; @BT C4 D6 9C
                    SBC A',IYL         ; @BT FD 9D
                    LD (((($D43A-(5+5))+5)+5)),SP ; @BT ED 73 3A D4
                    LDI                ; @BT ED A0
                    RES 2,B'           ; @BT CB 90
                    RLD                ; @BT ED 6F
                    LD D',IXL          ; @BT DD 55
                    SRL A'             ; @BT CB 3F
                    SET 0,C'           ; @BT CB C1
                    SET 4,B'           ; @BT CB E0
                    AND A',(IX+((($35-5)-(8-1))+(8-1)+5)) ; @BT DD A6 35
                    SRA (HL')          ; @BT CB 2E
                    LD H',(IX+((($7A-(2+2))+(2+2)))) ; @BT DD 66 7A
                    LD B',IXL          ; @BT DD 45
                    RET GTE            ; @BT D0
                    BIT 2,B'           ; @BT CB 50
                    XOR A',IYH         ; @BT FD AC
                    LD I,A'            ; @BT ED 47
                    LD A',(IY+((($10-1)+(1-3))+3)) ; @BT FD 7E 10
                    LD IXL,C'          ; @BT DD 69
                    BIT 3,L'           ; @BT CB 5D
                    RES 2,H'           ; @BT CB 94
                    AND A',H'          ; @BT A4
                    SET 3,(HL')        ; @BT CB DE
                    SET 3,A'           ; @BT CB DF
                    BIT 4,A'           ; @BT CB 67
                    LD D',L'           ; @BT 55
                    LD IXH,E'          ; @BT DD 63
                    INC IX             ; @BT DD 23
                    SUB A',(IX+((($77-7)-(8-1))+(8-1)+7)) ; @BT DD 96 77
                    DEC DE'            ; @BT 1B
                    RES 3,H'           ; @BT CB 9C
                    SRL D'             ; @BT CB 3A
                    SUB A',IXH         ; @BT DD 94
                    BIT 5,H'           ; @BT CB 6C
                    LD A',IYL          ; @BT FD 7D
                    IN E',(C')         ; @BT ED 58
                    IN F',(C')         ; @BT ED 70
                    LD C',B'           ; @BT 48
                    ADC A',(IY+((($34-7)-9)+7+9)) ; @BT FD 8E 34
                    SET 4,(HL')        ; @BT CB E6
                    SET 0,L'           ; @BT CB C5
                    LD H',A'           ; @BT 67
                    DEC IY             ; @BT FD 2B
                    LD E',C'           ; @BT 59
                    IN L',(C')         ; @BT ED 68
                    BIT 2,(HL')        ; @BT CB 56
                    ADD A',D'          ; @BT 82
                    LD A',R            ; @BT ED 5F
                    LD B',(HL')        ; @BT 46
                    ADD IX,IX          ; @BT DD 29
                    JR EQU,((($8A-3)-8)+3+8) ; @BT 28 8A
                    LD L',H'           ; @BT 6C
                    OR A',E'           ; @BT B3
                    SET 6,L'           ; @BT CB F5
                    OR A',(IY+(($41-4)+4)) ; @BT FD B6 41
                    BIT 1,(HL')        ; @BT CB 4E
                    RLC D'             ; @BT CB 02
                    LD B',C'           ; @BT 41
                    LD (HL'),C'        ; @BT 71
                    BIT 1,H'           ; @BT CB 4C
                    LD IXH,IXH         ; @BT DD 64
                    SET 6,(HL')        ; @BT CB F6
                    RES 1,(HL')        ; @BT CB 8E
                    LD IXH,D'          ; @BT DD 62
                    BIT 4,L'           ; @BT CB 65
                    ADD IY,IY          ; @BT FD 29
                    CPD                ; @BT ED A9
                    RES 4,E'           ; @BT CB A3
                    RL D'              ; @BT CB 12
                    POP IX             ; @BT DD E1
                    ADC A',((($53-(5+1))+5)+1) ; @BT CE 53
                    LD BC',(((T5_DEF_012-(5+1))+5)+1) ; @BT ED 4B CD 2C
                    LD B',D'           ; @BT 42
                    RET PE             ; @BT E8
                    LD IXH,C'          ; @BT DD 61
                    CALL PO,((($A11A-6)+(6-4))+4) ; @BT E4 1A A1
                    SLA B'             ; @BT CB 20
                    LD IXL,IXH         ; @BT DD 6C
                    LD (HL'),H'        ; @BT 74
                    SET 2,E'           ; @BT CB D3
                    BIT 4,C'           ; @BT CB 61
                    RLC (HL')          ; @BT CB 06
                    RR C'              ; @BT CB 19
                    LD D',IYH          ; @BT FD 54
                    BIT 0,(IX+$5C)     ; @BT DD CB 5C 46
                    SRL C'             ; @BT CB 39
                    AND A',C'          ; @BT A1
                    ADD IY,SP          ; @BT FD 39
                    LD E',IXL          ; @BT DD 5D
                    ADD HL',HL'        ; @BT 29
                    BIT 4,D'           ; @BT CB 62
                    LD E',IXH          ; @BT DD 5C
                    JR LSS,(($45-4)+4) ; @BT 38 45
                    OUT (((($09-(7+8))+(7+8)))),A' ; @BT D3 09
                    INC HL'            ; @BT 23
                    LD IXH,IXL         ; @BT DD 65
                    XOR A',D'          ; @BT AA
                    SRA E'             ; @BT CB 2B
                    INC E'             ; @BT 1C
                    SET 5,E'           ; @BT CB EB
                    SBC HL',HL'        ; @BT ED 62
                    JP LSS,(($0447-11)+11) ; @BT DA 47 04
                    LD IXL,IXL         ; @BT DD 6D
                    LD IXL,D'          ; @BT DD 6A
                    EX (SP),HL'        ; @BT E3
                    LD IXH,B'          ; @BT DD 60
                    RRC (HL')          ; @BT CB 0E
                    SET 3,E'           ; @BT CB DB
                    SET 0,(HL')        ; @BT CB C6
                    CALL POS,(((F5_FN_005(T5_DEF_005+6+8,14,6,8+14)-(10+8))+(10+8))) ; @BT F4 E7 9D
                    BIT 3,A'           ; @BT CB 5F
                    LD (IX+((($13-(8+1))+8)+1)),B' ; @BT DD 70 13
                    LD H',(((T5_DEF_022-(3+1))+3)+1) ; @BT 26 58
                    LD D',(IY+(($2F-8)+8)) ; @BT FD 56 2F
                    OUT (C'),D'        ; @BT ED 51
                    SET 3,D'           ; @BT CB DA
                    LD BC',(((T5_DEF_014-(6+4))+6+4)) ; @BT ED 4B CC 2B
                    OUT (C'),A'        ; @BT ED 79
                    IN A',((((T5_DEF_001-(6+6))+(6+6)))) ; @BT DB 51
                    SET 7,D'           ; @BT CB FA
                    RR (HL')           ; @BT CB 1E
                    LD (IY+((($73-6)+(6-2))+2)),(($88-(8+9))+8+9) ; @BT FD 36 73 88
                    LD ((((T5_DEF_017-(7+1))+7)+1)),IY ; @BT FD 22 4B D5
                    LD IYL,B'          ; @BT FD 68
                    LDIR               ; @BT ED B0
                    LD C',IYL          ; @BT FD 4D
                    SET 4,A'           ; @BT CB E7
                    RES 7,C'           ; @BT CB B9
                    LD L',((T5_DEF_021-(11+7))+11+7) ; @BT 2E 3B
                    ADC A',IXL         ; @BT DD 8D
                    LD IXL,E'          ; @BT DD 6B
                    LD D',IXH          ; @BT DD 54
                    RLCA               ; @BT 07
                    SET 2,L'           ; @BT CB D5
                    LD A',IYH          ; @BT FD 7C
                    RES 2,E'           ; @BT CB 93
                    SET 1,H'           ; @BT CB CC
                    RRC E'             ; @BT CB 0B
                    OUT (C'),E'        ; @BT ED 59
                    LD B',IYL          ; @BT FD 45
                    ADD A',(((T5_DEF_020-4)+1)-1+4) ; @BT C6 08
                    LD L',(HL')        ; @BT 6E
                    EXX                ; @BT D9
                    BIT 2,L'           ; @BT CB 55
                    IN A',(C')         ; @BT ED 78
                    RES 5,A'           ; @BT CB AF
                    LD (IY+((($67-10)+5)-5+10)),D' ; @BT FD 72 67
                    ADC A',(IX+(($2B-4)+4)) ; @BT DD 8E 2B
                    IM ((0-11)+11)     ; @BT ED 46
                    INI                ; @BT ED A2
                    LD HL',(((T5_DEF_013-7)+1)-1+7) ; @BT 2A BF 7B
                    RES 3,A'           ; @BT CB 9F
                    SET 1,E'           ; @BT CB CB
                    DI                 ; @BT F3
                    SBC A',(IX+(($1B-(7+5))+7+5)) ; @BT DD 9E 1B
                    LD H',C'           ; @BT 61
                    CP A',(((T5_DEF_018-5)+(5-4))+4) ; @BT FE A2
                    SET 3,C'           ; @BT CB D9
                    RES 0,B'           ; @BT CB 80
                    LD B',H'           ; @BT 44
                    RL E'              ; @BT CB 13
                    ADD A',IXL         ; @BT DD 85
                    BIT 7,A'           ; @BT CB 7F
                    BIT 1,C'           ; @BT CB 49
                    AND A',IYH         ; @BT FD A4
                    SET 5,H'           ; @BT CB EC
                    OR A',IXH          ; @BT DD B4
                    ADD IY,DE'         ; @BT FD 19
                    BIT 6,(HL')        ; @BT CB 76
                    LD E',E'           ; @BT 5B
                    BIT 1,B'           ; @BT CB 48
                    LD A',IXL          ; @BT DD 7D
                    LD A',I            ; @BT ED 57
                    ADD IX,BC'         ; @BT DD 09
                    RES 4,D'           ; @BT CB A2
                    SET 1,D'           ; @BT CB CA
                    RES 3,B'           ; @BT CB 98
                    SET 2,(HL')        ; @BT CB D6
                    AND A',D'          ; @BT A2
                    RES 3,D'           ; @BT CB 9A
                    SBC A',D'          ; @BT 9A
                    RLC A'             ; @BT CB 07
                    EX (SP),IX         ; @BT DD E3
                    RES 1,C'           ; @BT CB 89
                    SET 1,(HL')        ; @BT CB CE
                    LD IXH,A'          ; @BT DD 67
                    SRA B'             ; @BT CB 28
                    SBC HL',BC'        ; @BT ED 42
                    RET PO             ; @BT E0
                    ADD A',A'          ; @BT 87
                    SLA D'             ; @BT CB 22
                    ADD A',(HL')       ; @BT 86
                    LD (DE'),A'        ; @BT 12
                    LD E',D'           ; @BT 5A
                    BIT 6,C'           ; @BT CB 71
                    CPI                ; @BT ED A1
                    SUB A',IYL         ; @BT FD 95
                    RL L'              ; @BT CB 15
                    LD (BC'),A'        ; @BT 02
                    RES 2,C'           ; @BT CB 91
                    LD C',((($64-2)+(2-7))+7) ; @BT 0E 64
                    OTDR               ; @BT ED BB
                    LD (IY+((($5E-3)+(3-6))+6)),A' ; @BT FD 77 5E
                    LD H',E'           ; @BT 63
                    ADC A',IXH         ; @BT DD 8C
                    LD E',(($5B-(10+6))+10+6) ; @BT 1E 5B
                    LD B',IXH          ; @BT DD 44
                    LDD                ; @BT ED A8
                    SET 1,L'           ; @BT CB CD
                    ADC HL',DE'        ; @BT ED 5A
                    LD IYL,D'          ; @BT FD 6A
                    LD SP,IY           ; @BT FD F9
                    LD IY,((($B54D-2)+9)-9+2) ; @BT FD 2A 4D B5
                    SBC HL',DE'        ; @BT ED 52
                    OR A',IYH          ; @BT FD B4
                    LD (((($D107-7)-6)+7+6)),BC' ; @BT ED 43 07 D1
                    LD IXL,A'          ; @BT DD 6F
                    DEC (IX+((($11-(5+1))+5)+1)) ; @BT DD 35 11
                    LD SP,((((T5_DEF_031-5)-3)+5+3)) ; @BT ED 7B 4F 95
                    LD E',(IY+((($5C-5)+(5+7))-7)) ; @BT FD 5E 5C
                    IM (((2-(2+1))+2)+1) ; @BT ED 5E
                    LD (IX+((($1C-7)+5)-5+7)),E' ; @BT DD 73 1C
                    RES 3,C'           ; @BT CB 99
                    LD IYL,IYL         ; @BT FD 6D
                    ADD A',L'          ; @BT 85
                    LD IXL,B'          ; @BT DD 68
                    LD H',D'           ; @BT 62
                    DEC E'             ; @BT 1D
                    CALL ((F5_FN_004(T5_DEF_007+6,5,6+5)-(11+4))+11+4) ; @BT CD 2B A2
                    RLC E'             ; @BT CB 03
                    LD (IY+((($6A-(8+1))+8)+1)),E' ; @BT FD 73 6A
                    LD (HL'),A'        ; @BT 77
                    RRC L'             ; @BT CB 0D
                    LD D',D'           ; @BT 52
                    LD C',C'           ; @BT 49
                    RES 4,L'           ; @BT CB A5
                    LD D',A'           ; @BT 57
                    LD H',H'           ; @BT 64
                    AND A',A'          ; @BT A7
                    LD C',IXL          ; @BT DD 4D
                    RETN               ; @BT ED 45
                    RES 6,(HL')        ; @BT CB B6
                    RET POS            ; @BT F0
                    BIT 3,D'           ; @BT CB 5A
                    SET 2,D'           ; @BT CB D2
                    RES 5,L'           ; @BT CB AD
                    LD ((((T5_DEF_016-7)-5)+7+5)),HL' ; @BT 22 29 D3
                    ADD A',(IY+((($6D-9)-5)+9+5)) ; @BT FD 86 6D
                    JP PO,(((T5_DEF_025-(8+1))+8)+1) ; @BT E2 BE 0B
                    OR A',(HL')        ; @BT B6
                    SET 6,D'           ; @BT CB F2
                    BIT 7,B'           ; @BT CB 78
                    ADC A',H'          ; @BT 8C
                    RES 5,D'           ; @BT CB AA
                    RRC H'             ; @BT CB 0C
                    SBC A',E'          ; @BT 9B
                    BIT 3,(HL')        ; @BT CB 5E
                    LD (((T5_DEF_029-9)+9)),DE' ; @BT ED 53 18 D2
                    RST ((($18-(3+1))+3)+1) ; @BT DF
                    LD (IX+((($19-(8+1))+8)+1)),D' ; @BT DD 72 19
                    DEC L'             ; @BT 2D
                    BIT 0,H'           ; @BT CB 44
                    SRA L'             ; @BT CB 2D
                    INC IYH            ; @BT FD 24
                    BIT 3,E'           ; @BT CB 5B
                    CALL LSS,((F5_FN_002(T5_DEF_002+13,13)-8)+8) ; @BT DC A3 99
                    RES 7,H'           ; @BT CB BC
                    LD C',(HL')        ; @BT 4E
                    SUB A',H'          ; @BT 94
                    LD A',IXH          ; @BT DD 7C
                    INC SP             ; @BT 33
                    SUB A',IYH         ; @BT FD 94
                    CP A',D'           ; @BT BA
                    BIT 2,C'           ; @BT CB 51
                    RES 1,L'           ; @BT CB 8D
                    LD B',B'           ; @BT 40
                    SET 7,C'           ; @BT CB F9
                    LD (IY+((($6D-(7+1))+1)+7)),H' ; @BT FD 74 6D
                    OR A',((($0C-(8+9))+9)+8) ; @BT F6 0C
                    OR A',B'           ; @BT B0
                    SET 4,H'           ; @BT CB E4
                    SL1 A'             ; @BT CB 37
                    RES 4,(HL')        ; @BT CB A6
                    SUB A',B'          ; @BT 90
                    LD B',(IX+(($43-(6+3))+6+3)) ; @BT DD 46 43
                    CP A',(IX+((($46-6)+(6-9))+9)) ; @BT DD BE 46
                    LD D',C'           ; @BT 51
                    SET 6,B'           ; @BT CB F0
                    RR A'              ; @BT CB 1F
                    SET 0,E'           ; @BT CB C3
                    LD H',B'           ; @BT 60
                    INC DE'            ; @BT 13
                    LD D',(IX+((($26-(5+1))+5)+1)) ; @BT DD 56 26
                    CP A',L'           ; @BT BD
                    SBC A',B'          ; @BT 98
                    SL1 L'             ; @BT CB 35
                    BIT 7,C'           ; @BT CB 79
                    XOR A',(IX+((($24-8)+(8-7))+7)) ; @BT DD AE 24
                    DEC B'             ; @BT 05
                    LD C',D'           ; @BT 4A
                    AND A',IYL         ; @BT FD A5
                    RET LSS            ; @BT D8
                    BIT 1,L'           ; @BT CB 4D
                    DAA                ; @BT 27
                    RES 5,(HL')        ; @BT CB AE
                    SUB A',L'          ; @BT 95
                    XOR A',L'          ; @BT AD
                    RES 1,E'           ; @BT CB 8B
                    LD (HL'),D'        ; @BT 72
                    LD SP,IX           ; @BT DD F9
                    CP A',IYL          ; @BT FD BD
                    SUB A',IXL         ; @BT DD 95
                    POP AF'            ; @BT F1
                    PUSH HL'           ; @BT E5
                    OR A',IXL          ; @BT DD B5
                    JP EQU,(($099C-(7+7))+7+7) ; @BT CA 9C 09
                    LD (IX+((($16-5)-2)+5+2)),C' ; @BT DD 71 16
                    RRC C'             ; @BT CB 09
                    SET 0,B'           ; @BT CB C0
                    ADD IY,BC'         ; @BT FD 09

    .db "<05"               ; @BT END
