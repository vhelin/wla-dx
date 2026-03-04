    .MEMORYMAP
        SLOTSIZE $8000
        DEFAULTSLOT 0
        SLOT 0 START $0000
    .ENDME

    .ROMBANKSIZE $8000
    .ROMBANKS 1

    .BANK 0 SLOT 0
    .ORG 0

    ; @BT linked.rom

    ; NOTE: z80n opcodes, as WLA DX understands them, can be found in /iz80n.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/z80n/z80n_instruction_set.html)
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
    ; Auto-generated from iz80n.c + cross-checked with external z80n ISA references

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
    ADD BC,A'                          ; @BT ED 33
    ADD BC',$3787                      ; @BT ED 36 87 37
    ADD DE,A'                          ; @BT ED 32
    ADD DE',$39A9                      ; @BT ED 35 A9 39
    ADD HL,A'                          ; @BT ED 31
    ADD HL',BC'                        ; @BT 09
    ADD HL',DE'                        ; @BT 19
    ADD HL',HL'                        ; @BT 29
    ADD HL',SP                         ; @BT 39
    ADD HL',$400F                      ; @BT ED 34 0F 40
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
    AND A',(IX+$47)                    ; @BT DD A6 47
    AND A',IYH                         ; @BT FD A4
    AND A',IYL                         ; @BT FD A5
    AND A',(IY+$50)                    ; @BT FD A6 50
    AND A',L'                          ; @BT A5
    AND A',$FF                         ; @BT E6 FF
    BIT 0,A'                           ; @BT CB 47
    BIT 0,B'                           ; @BT CB 40
    BIT 0,C'                           ; @BT CB 41
    BIT 0,D'                           ; @BT CB 42
    BIT 0,E'                           ; @BT CB 43
    BIT 0,H'                           ; @BT CB 44
    BIT 0,(HL')                        ; @BT CB 46
    BIT 0,(IX+$6E)                     ; @BT DD CB 6E 46
    BIT 0,(IY+$71)                     ; @BT FD CB 71 46
    BIT 0,L'                           ; @BT CB 45
    BIT 1,A'                           ; @BT CB 4F
    BIT 1,B'                           ; @BT CB 48
    BIT 1,C'                           ; @BT CB 49
    BIT 1,D'                           ; @BT CB 4A
    BIT 1,E'                           ; @BT CB 4B
    BIT 1,H'                           ; @BT CB 4C
    BIT 1,(HL')                        ; @BT CB 4E
    BIT 1,(IX+$0C)                     ; @BT DD CB 0C 4E
    BIT 1,(IY+$0F)                     ; @BT FD CB 0F 4E
    BIT 1,L'                           ; @BT CB 4D
    BIT 2,A'                           ; @BT CB 57
    BIT 2,B'                           ; @BT CB 50
    BIT 2,C'                           ; @BT CB 51
    BIT 2,D'                           ; @BT CB 52
    BIT 2,E'                           ; @BT CB 53
    BIT 2,H'                           ; @BT CB 54
    BIT 2,(HL')                        ; @BT CB 56
    BIT 2,(IX+$2A)                     ; @BT DD CB 2A 56
    BIT 2,(IY+$2D)                     ; @BT FD CB 2D 56
    BIT 2,L'                           ; @BT CB 55
    BIT 3,A'                           ; @BT CB 5F
    BIT 3,B'                           ; @BT CB 58
    BIT 3,C'                           ; @BT CB 59
    BIT 3,D'                           ; @BT CB 5A
    BIT 3,E'                           ; @BT CB 5B
    BIT 3,H'                           ; @BT CB 5C
    BIT 3,(HL')                        ; @BT CB 5E
    BIT 3,(IX+$48)                     ; @BT DD CB 48 5E
    BIT 3,(IY+$4B)                     ; @BT FD CB 4B 5E
    BIT 3,L'                           ; @BT CB 5D
    BIT 4,A'                           ; @BT CB 67
    BIT 4,B'                           ; @BT CB 60
    BIT 4,C'                           ; @BT CB 61
    BIT 4,D'                           ; @BT CB 62
    BIT 4,E'                           ; @BT CB 63
    BIT 4,H'                           ; @BT CB 64
    BIT 4,(HL')                        ; @BT CB 66
    BIT 4,(IX+$66)                     ; @BT DD CB 66 66
    BIT 4,(IY+$69)                     ; @BT FD CB 69 66
    BIT 4,L'                           ; @BT CB 65
    BIT 5,A'                           ; @BT CB 6F
    BIT 5,B'                           ; @BT CB 68
    BIT 5,C'                           ; @BT CB 69
    BIT 5,D'                           ; @BT CB 6A
    BIT 5,E'                           ; @BT CB 6B
    BIT 5,H'                           ; @BT CB 6C
    BIT 5,(HL')                        ; @BT CB 6E
    BIT 5,(IX+$04)                     ; @BT DD CB 04 6E
    BIT 5,(IY+$07)                     ; @BT FD CB 07 6E
    BIT 5,L'                           ; @BT CB 6D
    BIT 6,A'                           ; @BT CB 77
    BIT 6,B'                           ; @BT CB 70
    BIT 6,C'                           ; @BT CB 71
    BIT 6,D'                           ; @BT CB 72
    BIT 6,E'                           ; @BT CB 73
    BIT 6,H'                           ; @BT CB 74
    BIT 6,(HL')                        ; @BT CB 76
    BIT 6,(IX+$22)                     ; @BT DD CB 22 76
    BIT 6,(IY+$25)                     ; @BT FD CB 25 76
    BIT 6,L'                           ; @BT CB 75
    BIT 7,A'                           ; @BT CB 7F
    BIT 7,B'                           ; @BT CB 78
    BIT 7,C'                           ; @BT CB 79
    BIT 7,D'                           ; @BT CB 7A
    BIT 7,E'                           ; @BT CB 7B
    BIT 7,H'                           ; @BT CB 7C
    BIT 7,(HL')                        ; @BT CB 7E
    BIT 7,(IX+$40)                     ; @BT DD CB 40 7E
    BIT 7,(IY+$43)                     ; @BT FD CB 43 7E
    BIT 7,L'                           ; @BT CB 7D
    BRLC DE',B                         ; @BT ED 2C
    BSLA DE',B                         ; @BT ED 28
    BSRA DE',B                         ; @BT ED 29
    BSRL DE',B                         ; @BT ED 2A
    BSRF DE',B                         ; @BT ED 2B
    CALL LSS,$B44C                     ; @BT DC 4C B4
    CALL NEG,$B55D                     ; @BT FC 5D B5
    CALL GTE,$B66E                     ; @BT D4 6E B6
    CALL NEQ,$B77F                     ; @BT C4 7F B7
    CALL POS,$B890                     ; @BT F4 90 B8
    CALL EQU,$B9A1                     ; @BT CC A1 B9
    CALL PE,$BAB2                      ; @BT EC B2 BA
    CALL PO,$BBC3                      ; @BT E4 C3 BB
    CALL $BCD4                         ; @BT CD D4 BC
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
    CP A',(IX+$11)                     ; @BT DD BE 11
    CP A',IYH                          ; @BT FD BC
    CP A',IYL                          ; @BT FD BD
    CP A',(IY+$1A)                     ; @BT FD BE 1A
    CP A',L'                           ; @BT BD
    CP A',$E1                          ; @BT FE E1
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
    DEC (IX+$5C)                       ; @BT DD 35 5C
    DEC IY                             ; @BT FD 2B
    DEC IYH                            ; @BT FD 25
    DEC IYL                            ; @BT FD 2D
    DEC (IY+$68)                       ; @BT FD 35 68
    DEC L'                             ; @BT 2D
    DEC SP                             ; @BT 3B
    DI                                 ; @BT F3
    DJNZ $65                           ; @BT 10 65
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
    IN A',($90)                        ; @BT DB 90
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
    INC (IX+$5A)                       ; @BT DD 34 5A
    INC IY                             ; @BT FD 23
    INC IYH                            ; @BT FD 24
    INC IYL                            ; @BT FD 2C
    INC (IY+$66)                       ; @BT FD 34 66
    INC L'                             ; @BT 2C
    INC SP                             ; @BT 33
    INDR                               ; @BT ED BA
    IND                                ; @BT ED AA
    INIR                               ; @BT ED B2
    INI                                ; @BT ED A2
    JP (C')                            ; @BT ED 98
    JP (HL')                           ; @BT E9
    JP (IX)                            ; @BT DD E9
    JP (IY)                            ; @BT FD E9
    JP LSS,$2001                       ; @BT DA 01 20
    JP NEG,$2112                       ; @BT FA 12 21
    JP GTE,$2223                       ; @BT D2 23 22
    JP NEQ,$2334                       ; @BT C2 34 23
    JP POS,$2445                       ; @BT F2 45 24
    JP EQU,$2556                       ; @BT CA 56 25
    JP PE,$2667                        ; @BT EA 67 26
    JP PO,$2778                        ; @BT E2 78 27
    JP $2889                           ; @BT C3 89 28
    JR LSS,$9B                         ; @BT 38 9B
    JR GTE,$B2                         ; @BT 30 B2
    JR NEQ,$C9                         ; @BT 20 C9
    JR EQU,$E0                         ; @BT 28 E0
    JR $F7                             ; @BT 18 F7
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
    LD A',(IX+$55)                     ; @BT DD 7E 55
    LD A',IYH                          ; @BT FD 7C
    LD A',IYL                          ; @BT FD 7D
    LD A',(IY+$5E)                     ; @BT FD 7E 5E
    LD A',L'                           ; @BT 7D
    LD A',R                            ; @BT ED 5F
    LD A',($4221)                      ; @BT 3A 21 42
    LD A',$C3                          ; @BT 3E C3
    LD B',A'                           ; @BT 47
    LD B',B'                           ; @BT 40
    LD B',C'                           ; @BT 41
    LD BC',($4776)                     ; @BT ED 4B 76 47
    LD BC',$4887                       ; @BT 01 87 48
    LD (BC'),A'                        ; @BT 02
    LD B',D'                           ; @BT 42
    LD B',E'                           ; @BT 43
    LD B',H'                           ; @BT 44
    LD B',(HL')                        ; @BT 46
    LD B',IXH                          ; @BT DD 44
    LD B',IXL                          ; @BT DD 45
    LD B',(IX+$11)                     ; @BT DD 46 11
    LD B',IYH                          ; @BT FD 44
    LD B',IYL                          ; @BT FD 45
    LD B',(IY+$1A)                     ; @BT FD 46 1A
    LD B',L'                           ; @BT 45
    LD B',$61                          ; @BT 06 61
    LD C',A'                           ; @BT 4F
    LD C',B'                           ; @BT 48
    LD C',C'                           ; @BT 49
    LD C',D'                           ; @BT 4A
    LD C',E'                           ; @BT 4B
    LD C',H'                           ; @BT 4C
    LD C',(HL')                        ; @BT 4E
    LD C',IXH                          ; @BT DD 4C
    LD C',IXL                          ; @BT DD 4D
    LD C',(IX+$3E)                     ; @BT DD 4E 3E
    LD C',IYH                          ; @BT FD 4C
    LD C',IYL                          ; @BT FD 4D
    LD C',(IY+$47)                     ; @BT FD 4E 47
    LD C',L'                           ; @BT 4D
    LD C',$BA                          ; @BT 0E BA
    LD D',A'                           ; @BT 57
    LD D',B'                           ; @BT 50
    LD D',C'                           ; @BT 51
    LD D',D'                           ; @BT 52
    LD D',E'                           ; @BT 53
    LD DE',($6CC9)                     ; @BT ED 5B C9 6C
    LD DE',$6DDA                       ; @BT 11 DA 6D
    LD (DE'),A'                        ; @BT 12
    LD D',H'                           ; @BT 54
    LD D',(HL')                        ; @BT 56
    LD D',IXH                          ; @BT DD 54
    LD D',IXL                          ; @BT DD 55
    LD D',(IX+$74)                     ; @BT DD 56 74
    LD D',IYH                          ; @BT FD 54
    LD D',IYL                          ; @BT FD 55
    LD D',(IY+$7D)                     ; @BT FD 56 7D
    LD D',L'                           ; @BT 55
    LD D',$58                          ; @BT 16 58
    LD E',A'                           ; @BT 5F
    LD E',B'                           ; @BT 58
    LD E',C'                           ; @BT 59
    LD E',D'                           ; @BT 5A
    LD E',E'                           ; @BT 5B
    LD E',H'                           ; @BT 5C
    LD E',(HL')                        ; @BT 5E
    LD E',IXH                          ; @BT DD 5C
    LD E',IXL                          ; @BT DD 5D
    LD E',(IX+$21)                     ; @BT DD 5E 21
    LD E',IYH                          ; @BT FD 5C
    LD E',IYL                          ; @BT FD 5D
    LD E',(IY+$2A)                     ; @BT FD 5E 2A
    LD E',L'                           ; @BT 5D
    LD E',$B1                          ; @BT 1E B1
    LD H',A'                           ; @BT 67
    LD H',B'                           ; @BT 60
    LD H',C'                           ; @BT 61
    LD H',D'                           ; @BT 62
    LD H',E'                           ; @BT 63
    LD H',H'                           ; @BT 64
    LD H',(HL')                        ; @BT 66
    LD H',(IX+$48)                     ; @BT DD 66 48
    LD H',(IY+$4B)                     ; @BT FD 66 4B
    LD H',L'                           ; @BT 65
    LD H',$AE                          ; @BT 26 AE
    LD HL',($9660)                     ; @BT 2A 60 96
    LD HL',$9771                       ; @BT 21 71 97
    LD (HL'),A'                        ; @BT 77
    LD (HL'),B'                        ; @BT 70
    LD (HL'),C'                        ; @BT 71
    LD (HL'),D'                        ; @BT 72
    LD (HL'),E'                        ; @BT 73
    LD (HL'),H'                        ; @BT 74
    LD (HL'),L'                        ; @BT 75
    LD (HL'),$94                       ; @BT 36 94
    LD L',A'                           ; @BT 6F
    LD L',B'                           ; @BT 68
    LD L',C'                           ; @BT 69
    LD L',D'                           ; @BT 6A
    LD L',E'                           ; @BT 6B
    LD L',H'                           ; @BT 6C
    LD L',(HL')                        ; @BT 6E
    LD L',(IX+$07)                     ; @BT DD 6E 07
    LD L',(IY+$0A)                     ; @BT FD 6E 0A
    LD L',L'                           ; @BT 6D
    LD L',$91                          ; @BT 2E 91
    LD R,A'                            ; @BT ED 4F
    LD SP,HL'                          ; @BT F9
    LD SP,IX                           ; @BT DD F9
    LD SP,IY                           ; @BT FD F9
    LD SP,($B109)                      ; @BT ED 7B 09 B1
    LD SP,$B21A                        ; @BT 31 1A B2
    LD I,A'                            ; @BT ED 47
    LD IX,($B43C)                      ; @BT DD 2A 3C B4
    LD IX,$B54D                        ; @BT DD 21 4D B5
    LD IXH,A'                          ; @BT DD 67
    LD IXH,B'                          ; @BT DD 60
    LD IXH,C'                          ; @BT DD 61
    LD IXH,D'                          ; @BT DD 62
    LD IXH,E'                          ; @BT DD 63
    LD IXH,IXH                         ; @BT DD 64
    LD IXH,IXL                         ; @BT DD 65
    LD IXH,$18                         ; @BT DD 26 18
    LD IXL,A'                          ; @BT DD 6F
    LD IXL,B'                          ; @BT DD 68
    LD IXL,C'                          ; @BT DD 69
    LD IXL,D'                          ; @BT DD 6A
    LD IXL,E'                          ; @BT DD 6B
    LD IXL,IXH                         ; @BT DD 6C
    LD IXL,IXL                         ; @BT DD 6D
    LD IXL,$D0                         ; @BT DD 2E D0
    LD (IX+$5E),A'                     ; @BT DD 77 5E
    LD (IX+$61),B'                     ; @BT DD 70 61
    LD (IX+$64),C'                     ; @BT DD 71 64
    LD (IX+$67),D'                     ; @BT DD 72 67
    LD (IX+$6A),E'                     ; @BT DD 73 6A
    LD (IX+$6D),H'                     ; @BT DD 74 6D
    LD (IX+$70),L'                     ; @BT DD 75 70
    LD (IX+$73),$88                    ; @BT DD 36 73 88
    LD IY,($CFF6)                      ; @BT FD 2A F6 CF
    LD IY,$D107                        ; @BT FD 21 07 D1
    LD IYH,A'                          ; @BT FD 67
    LD IYH,B'                          ; @BT FD 60
    LD IYH,C'                          ; @BT FD 61
    LD IYH,D'                          ; @BT FD 62
    LD IYH,E'                          ; @BT FD 63
    LD IYH,IYH                         ; @BT FD 64
    LD IYH,IYL                         ; @BT FD 65
    LD IYH,$6E                         ; @BT FD 26 6E
    LD IYL,A'                          ; @BT FD 6F
    LD IYL,B'                          ; @BT FD 68
    LD IYL,C'                          ; @BT FD 69
    LD IYL,D'                          ; @BT FD 6A
    LD IYL,E'                          ; @BT FD 6B
    LD IYL,IYH                         ; @BT FD 6C
    LD IYL,IYL                         ; @BT FD 6D
    LD IYL,$26                         ; @BT FD 2E 26
    LD (IY+$2C),A'                     ; @BT FD 77 2C
    LD (IY+$2F),B'                     ; @BT FD 70 2F
    LD (IY+$32),C'                     ; @BT FD 71 32
    LD (IY+$35),D'                     ; @BT FD 72 35
    LD (IY+$38),E'                     ; @BT FD 73 38
    LD (IY+$3B),H'                     ; @BT FD 74 3B
    LD (IY+$3E),L'                     ; @BT FD 75 3E
    LD (IY+$41),$DE                    ; @BT FD 36 41 DE
    LD ($EBB0),A'                      ; @BT 32 B0 EB
    LD ($ECC1),BC'                     ; @BT ED 43 C1 EC
    LD ($EDD2),DE'                     ; @BT ED 53 D2 ED
    LD ($EEE3),HL'                     ; @BT 22 E3 EE
    LD ($EFF4),SP                      ; @BT ED 73 F4 EF
    LD ($F105),IY                      ; @BT FD 22 05 F1
    LD ($F216),IX                      ; @BT DD 22 16 F2
    LD A',RES 0,(IX+$59)               ; @BT DD CB 59 87
    LD A',RES 0,(IY+$5C)               ; @BT FD CB 5C 87
    LD A',RES 1,(IX+$5F)               ; @BT DD CB 5F 8F
    LD A',RES 1,(IY+$62)               ; @BT FD CB 62 8F
    LD A',RES 2,(IX+$65)               ; @BT DD CB 65 97
    LD A',RES 2,(IY+$68)               ; @BT FD CB 68 97
    LD A',RES 3,(IX+$6B)               ; @BT DD CB 6B 9F
    LD A',RES 3,(IY+$6E)               ; @BT FD CB 6E 9F
    LD A',RES 4,(IX+$71)               ; @BT DD CB 71 A7
    LD A',RES 4,(IY+$74)               ; @BT FD CB 74 A7
    LD A',RES 5,(IX+$77)               ; @BT DD CB 77 AF
    LD A',RES 5,(IY+$7A)               ; @BT FD CB 7A AF
    LD A',RES 6,(IX+$7D)               ; @BT DD CB 7D B7
    LD A',RES 6,(IY+$00)               ; @BT FD CB 00 B7
    LD A',RES 7,(IX+$03)               ; @BT DD CB 03 BF
    LD A',RES 7,(IY+$06)               ; @BT FD CB 06 BF
    LD A',RLC (IX+$09)                 ; @BT DD CB 09 07
    LD A',RLC (IY+$0C)                 ; @BT FD CB 0C 07
    LD A',RL (IX+$0F)                  ; @BT DD CB 0F 17
    LD A',RL (IY+$12)                  ; @BT FD CB 12 17
    LD A',RRC (IX+$15)                 ; @BT DD CB 15 0F
    LD A',RRC (IY+$18)                 ; @BT FD CB 18 0F
    LD A',RR (IX+$1B)                  ; @BT DD CB 1B 1F
    LD A',RR (IY+$1E)                  ; @BT FD CB 1E 1F
    LD A',SET 0,(IX+$21)               ; @BT DD CB 21 C7
    LD A',SET 0,(IY+$24)               ; @BT FD CB 24 C7
    LD A',SET 1,(IX+$27)               ; @BT DD CB 27 CF
    LD A',SET 1,(IY+$2A)               ; @BT FD CB 2A CF
    LD A',SET 2,(IX+$2D)               ; @BT DD CB 2D D7
    LD A',SET 2,(IY+$30)               ; @BT FD CB 30 D7
    LD A',SET 3,(IX+$33)               ; @BT DD CB 33 DF
    LD A',SET 3,(IY+$36)               ; @BT FD CB 36 DF
    LD A',SET 4,(IX+$39)               ; @BT DD CB 39 E7
    LD A',SET 4,(IY+$3C)               ; @BT FD CB 3C E7
    LD A',SET 5,(IX+$3F)               ; @BT DD CB 3F EF
    LD A',SET 5,(IY+$42)               ; @BT FD CB 42 EF
    LD A',SET 6,(IX+$45)               ; @BT DD CB 45 F7
    LD A',SET 6,(IY+$48)               ; @BT FD CB 48 F7
    LD A',SET 7,(IX+$4B)               ; @BT DD CB 4B FF
    LD A',SET 7,(IY+$4E)               ; @BT FD CB 4E FF
    LD A',SLA (IX+$51)                 ; @BT DD CB 51 27
    LD A',SLA (IY+$54)                 ; @BT FD CB 54 27
    LD A',SLL (IX+$57)                 ; @BT DD CB 57 37
    LD A',SLL (IY+$5A)                 ; @BT FD CB 5A 37
    LD A',SRA (IX+$5D)                 ; @BT DD CB 5D 2F
    LD A',SRA (IY+$60)                 ; @BT FD CB 60 2F
    LD A',SRL (IX+$63)                 ; @BT DD CB 63 3F
    LD A',SRL (IY+$66)                 ; @BT FD CB 66 3F
    LD B',RES 0,(IX+$69)               ; @BT DD CB 69 80
    LD B',RES 0,(IY+$6C)               ; @BT FD CB 6C 80
    LD B',RES 1,(IX+$6F)               ; @BT DD CB 6F 88
    LD B',RES 1,(IY+$72)               ; @BT FD CB 72 88
    LD B',RES 2,(IX+$75)               ; @BT DD CB 75 90
    LD B',RES 2,(IY+$78)               ; @BT FD CB 78 90
    LD B',RES 3,(IX+$7B)               ; @BT DD CB 7B 98
    LD B',RES 3,(IY+$7E)               ; @BT FD CB 7E 98
    LD B',RES 4,(IX+$01)               ; @BT DD CB 01 A0
    LD B',RES 4,(IY+$04)               ; @BT FD CB 04 A0
    LD B',RES 5,(IX+$07)               ; @BT DD CB 07 A8
    LD B',RES 5,(IY+$0A)               ; @BT FD CB 0A A8
    LD B',RES 6,(IX+$0D)               ; @BT DD CB 0D B0
    LD B',RES 6,(IY+$10)               ; @BT FD CB 10 B0
    LD B',RES 7,(IX+$13)               ; @BT DD CB 13 B8
    LD B',RES 7,(IY+$16)               ; @BT FD CB 16 B8
    LD B',RLC (IX+$19)                 ; @BT DD CB 19 00
    LD B',RLC (IY+$1C)                 ; @BT FD CB 1C 00
    LD B',RL (IX+$1F)                  ; @BT DD CB 1F 10
    LD B',RL (IY+$22)                  ; @BT FD CB 22 10
    LD B',RRC (IX+$25)                 ; @BT DD CB 25 08
    LD B',RRC (IY+$28)                 ; @BT FD CB 28 08
    LD B',RR (IX+$2B)                  ; @BT DD CB 2B 18
    LD B',RR (IY+$2E)                  ; @BT FD CB 2E 18
    LD B',SET 0,(IX+$31)               ; @BT DD CB 31 C0
    LD B',SET 0,(IY+$34)               ; @BT FD CB 34 C0
    LD B',SET 1,(IX+$37)               ; @BT DD CB 37 C8
    LD B',SET 1,(IY+$3A)               ; @BT FD CB 3A C8
    LD B',SET 2,(IX+$3D)               ; @BT DD CB 3D D0
    LD B',SET 2,(IY+$40)               ; @BT FD CB 40 D0
    LD B',SET 3,(IX+$43)               ; @BT DD CB 43 D8
    LD B',SET 3,(IY+$46)               ; @BT FD CB 46 D8
    LD B',SET 4,(IX+$49)               ; @BT DD CB 49 E0
    LD B',SET 4,(IY+$4C)               ; @BT FD CB 4C E0
    LD B',SET 5,(IX+$4F)               ; @BT DD CB 4F E8
    LD B',SET 5,(IY+$52)               ; @BT FD CB 52 E8
    LD B',SET 6,(IX+$55)               ; @BT DD CB 55 F0
    LD B',SET 6,(IY+$58)               ; @BT FD CB 58 F0
    LD B',SET 7,(IX+$5B)               ; @BT DD CB 5B F8
    LD B',SET 7,(IY+$5E)               ; @BT FD CB 5E F8
    LD B',SLA (IX+$61)                 ; @BT DD CB 61 20
    LD B',SLA (IY+$64)                 ; @BT FD CB 64 20
    LD B',SLL (IX+$67)                 ; @BT DD CB 67 30
    LD B',SLL (IY+$6A)                 ; @BT FD CB 6A 30
    LD B',SRA (IX+$6D)                 ; @BT DD CB 6D 28
    LD B',SRA (IY+$70)                 ; @BT FD CB 70 28
    LD B',SRL (IX+$73)                 ; @BT DD CB 73 38
    LD B',SRL (IY+$76)                 ; @BT FD CB 76 38
    LD C',RES 0,(IX+$79)               ; @BT DD CB 79 81
    LD C',RES 0,(IY+$7C)               ; @BT FD CB 7C 81
    LD C',RES 1,(IX+$7F)               ; @BT DD CB 7F 89
    LD C',RES 1,(IY+$02)               ; @BT FD CB 02 89
    LD C',RES 2,(IX+$05)               ; @BT DD CB 05 91
    LD C',RES 2,(IY+$08)               ; @BT FD CB 08 91
    LD C',RES 3,(IX+$0B)               ; @BT DD CB 0B 99
    LD C',RES 3,(IY+$0E)               ; @BT FD CB 0E 99
    LD C',RES 4,(IX+$11)               ; @BT DD CB 11 A1
    LD C',RES 4,(IY+$14)               ; @BT FD CB 14 A1
    LD C',RES 5,(IX+$17)               ; @BT DD CB 17 A9
    LD C',RES 5,(IY+$1A)               ; @BT FD CB 1A A9
    LD C',RES 6,(IX+$1D)               ; @BT DD CB 1D B1
    LD C',RES 6,(IY+$20)               ; @BT FD CB 20 B1
    LD C',RES 7,(IX+$23)               ; @BT DD CB 23 B9
    LD C',RES 7,(IY+$26)               ; @BT FD CB 26 B9
    LD C',RLC (IX+$29)                 ; @BT DD CB 29 01
    LD C',RLC (IY+$2C)                 ; @BT FD CB 2C 01
    LD C',RL (IX+$2F)                  ; @BT DD CB 2F 11
    LD C',RL (IY+$32)                  ; @BT FD CB 32 11
    LD C',RRC (IX+$35)                 ; @BT DD CB 35 09
    LD C',RRC (IY+$38)                 ; @BT FD CB 38 09
    LD C',RR (IX+$3B)                  ; @BT DD CB 3B 19
    LD C',RR (IY+$3E)                  ; @BT FD CB 3E 19
    LD C',SET 0,(IX+$41)               ; @BT DD CB 41 C1
    LD C',SET 0,(IY+$44)               ; @BT FD CB 44 C1
    LD C',SET 1,(IX+$47)               ; @BT DD CB 47 C9
    LD C',SET 1,(IY+$4A)               ; @BT FD CB 4A C9
    LD C',SET 2,(IX+$4D)               ; @BT DD CB 4D D1
    LD C',SET 2,(IY+$50)               ; @BT FD CB 50 D1
    LD C',SET 3,(IX+$53)               ; @BT DD CB 53 D9
    LD C',SET 3,(IY+$56)               ; @BT FD CB 56 D9
    LD C',SET 4,(IX+$59)               ; @BT DD CB 59 E1
    LD C',SET 4,(IY+$5C)               ; @BT FD CB 5C E1
    LD C',SET 5,(IX+$5F)               ; @BT DD CB 5F E9
    LD C',SET 5,(IY+$62)               ; @BT FD CB 62 E9
    LD C',SET 6,(IX+$65)               ; @BT DD CB 65 F1
    LD C',SET 6,(IY+$68)               ; @BT FD CB 68 F1
    LD C',SET 7,(IX+$6B)               ; @BT DD CB 6B F9
    LD C',SET 7,(IY+$6E)               ; @BT FD CB 6E F9
    LD C',SLA (IX+$71)                 ; @BT DD CB 71 21
    LD C',SLA (IY+$74)                 ; @BT FD CB 74 21
    LD C',SLL (IX+$77)                 ; @BT DD CB 77 31
    LD C',SLL (IY+$7A)                 ; @BT FD CB 7A 31
    LD C',SRA (IX+$7D)                 ; @BT DD CB 7D 29
    LD C',SRA (IY+$00)                 ; @BT FD CB 00 29
    LD C',SRL (IX+$03)                 ; @BT DD CB 03 39
    LD C',SRL (IY+$06)                 ; @BT FD CB 06 39
    LD D',RES 0,(IX+$09)               ; @BT DD CB 09 82
    LD D',RES 0,(IY+$0C)               ; @BT FD CB 0C 82
    LD D',RES 1,(IX+$0F)               ; @BT DD CB 0F 8A
    LD D',RES 1,(IY+$12)               ; @BT FD CB 12 8A
    LD D',RES 2,(IX+$15)               ; @BT DD CB 15 92
    LD D',RES 2,(IY+$18)               ; @BT FD CB 18 92
    LD D',RES 3,(IX+$1B)               ; @BT DD CB 1B 9A
    LD D',RES 3,(IY+$1E)               ; @BT FD CB 1E 9A
    LD D',RES 4,(IX+$21)               ; @BT DD CB 21 A2
    LD D',RES 4,(IY+$24)               ; @BT FD CB 24 A2
    LD D',RES 5,(IX+$27)               ; @BT DD CB 27 AA
    LD D',RES 5,(IY+$2A)               ; @BT FD CB 2A AA
    LD D',RES 6,(IX+$2D)               ; @BT DD CB 2D B2
    LD D',RES 6,(IY+$30)               ; @BT FD CB 30 B2
    LD D',RES 7,(IX+$33)               ; @BT DD CB 33 BA
    LD D',RES 7,(IY+$36)               ; @BT FD CB 36 BA
    LD D',RLC (IX+$39)                 ; @BT DD CB 39 02
    LD D',RLC (IY+$3C)                 ; @BT FD CB 3C 02
    LD D',RL (IX+$3F)                  ; @BT DD CB 3F 12
    LD D',RL (IY+$42)                  ; @BT FD CB 42 12
    LD D',RRC (IX+$45)                 ; @BT DD CB 45 0A
    LD D',RRC (IY+$48)                 ; @BT FD CB 48 0A
    LD D',RR (IX+$4B)                  ; @BT DD CB 4B 1A
    LD D',RR (IY+$4E)                  ; @BT FD CB 4E 1A
    LD D',SET 0,(IX+$51)               ; @BT DD CB 51 C2
    LD D',SET 0,(IY+$54)               ; @BT FD CB 54 C2
    LD D',SET 1,(IX+$57)               ; @BT DD CB 57 CA
    LD D',SET 1,(IY+$5A)               ; @BT FD CB 5A CA
    LD D',SET 2,(IX+$5D)               ; @BT DD CB 5D D2
    LD D',SET 2,(IY+$60)               ; @BT FD CB 60 D2
    LD D',SET 3,(IX+$63)               ; @BT DD CB 63 DA
    LD D',SET 3,(IY+$66)               ; @BT FD CB 66 DA
    LD D',SET 4,(IX+$69)               ; @BT DD CB 69 E2
    LD D',SET 4,(IY+$6C)               ; @BT FD CB 6C E2
    LD D',SET 5,(IX+$6F)               ; @BT DD CB 6F EA
    LD D',SET 5,(IY+$72)               ; @BT FD CB 72 EA
    LD D',SET 6,(IX+$75)               ; @BT DD CB 75 F2
    LD D',SET 6,(IY+$78)               ; @BT FD CB 78 F2
    LD D',SET 7,(IX+$7B)               ; @BT DD CB 7B FA
    LD D',SET 7,(IY+$7E)               ; @BT FD CB 7E FA
    LD D',SLA (IX+$01)                 ; @BT DD CB 01 22
    LD D',SLA (IY+$04)                 ; @BT FD CB 04 22
    LD D',SLL (IX+$07)                 ; @BT DD CB 07 32
    LD D',SLL (IY+$0A)                 ; @BT FD CB 0A 32
    LD D',SRA (IX+$0D)                 ; @BT DD CB 0D 2A
    LD D',SRA (IY+$10)                 ; @BT FD CB 10 2A
    LD D',SRL (IX+$13)                 ; @BT DD CB 13 3A
    LD D',SRL (IY+$16)                 ; @BT FD CB 16 3A
    LD E',RES 0,(IX+$19)               ; @BT DD CB 19 83
    LD E',RES 0,(IY+$1C)               ; @BT FD CB 1C 83
    LD E',RES 1,(IX+$1F)               ; @BT DD CB 1F 8B
    LD E',RES 1,(IY+$22)               ; @BT FD CB 22 8B
    LD E',RES 2,(IX+$25)               ; @BT DD CB 25 93
    LD E',RES 2,(IY+$28)               ; @BT FD CB 28 93
    LD E',RES 3,(IX+$2B)               ; @BT DD CB 2B 9B
    LD E',RES 3,(IY+$2E)               ; @BT FD CB 2E 9B
    LD E',RES 4,(IX+$31)               ; @BT DD CB 31 A3
    LD E',RES 4,(IY+$34)               ; @BT FD CB 34 A3
    LD E',RES 5,(IX+$37)               ; @BT DD CB 37 AB
    LD E',RES 5,(IY+$3A)               ; @BT FD CB 3A AB
    LD E',RES 6,(IX+$3D)               ; @BT DD CB 3D B3
    LD E',RES 6,(IY+$40)               ; @BT FD CB 40 B3
    LD E',RES 7,(IX+$43)               ; @BT DD CB 43 BB
    LD E',RES 7,(IY+$46)               ; @BT FD CB 46 BB
    LD E',RLC (IX+$49)                 ; @BT DD CB 49 03
    LD E',RLC (IY+$4C)                 ; @BT FD CB 4C 03
    LD E',RL (IX+$4F)                  ; @BT DD CB 4F 13
    LD E',RL (IY+$52)                  ; @BT FD CB 52 13
    LD E',RRC (IX+$55)                 ; @BT DD CB 55 0B
    LD E',RRC (IY+$58)                 ; @BT FD CB 58 0B
    LD E',RR (IX+$5B)                  ; @BT DD CB 5B 1B
    LD E',RR (IY+$5E)                  ; @BT FD CB 5E 1B
    LD E',SET 0,(IX+$61)               ; @BT DD CB 61 C3
    LD E',SET 0,(IY+$64)               ; @BT FD CB 64 C3
    LD E',SET 1,(IX+$67)               ; @BT DD CB 67 CB
    LD E',SET 1,(IY+$6A)               ; @BT FD CB 6A CB
    LD E',SET 2,(IX+$6D)               ; @BT DD CB 6D D3
    LD E',SET 2,(IY+$70)               ; @BT FD CB 70 D3
    LD E',SET 3,(IX+$73)               ; @BT DD CB 73 DB
    LD E',SET 3,(IY+$76)               ; @BT FD CB 76 DB
    LD E',SET 4,(IX+$79)               ; @BT DD CB 79 E3
    LD E',SET 4,(IY+$7C)               ; @BT FD CB 7C E3
    LD E',SET 5,(IX+$7F)               ; @BT DD CB 7F EB
    LD E',SET 5,(IY+$02)               ; @BT FD CB 02 EB
    LD E',SET 6,(IX+$05)               ; @BT DD CB 05 F3
    LD E',SET 6,(IY+$08)               ; @BT FD CB 08 F3
    LD E',SET 7,(IX+$0B)               ; @BT DD CB 0B FB
    LD E',SET 7,(IY+$0E)               ; @BT FD CB 0E FB
    LD E',SLA (IX+$11)                 ; @BT DD CB 11 23
    LD E',SLA (IY+$14)                 ; @BT FD CB 14 23
    LD E',SLL (IX+$17)                 ; @BT DD CB 17 33
    LD E',SLL (IY+$1A)                 ; @BT FD CB 1A 33
    LD E',SRA (IX+$1D)                 ; @BT DD CB 1D 2B
    LD E',SRA (IY+$20)                 ; @BT FD CB 20 2B
    LD E',SRL (IX+$23)                 ; @BT DD CB 23 3B
    LD E',SRL (IY+$26)                 ; @BT FD CB 26 3B
    LD H',RES 0,(IX+$29)               ; @BT DD CB 29 84
    LD H',RES 0,(IY+$2C)               ; @BT FD CB 2C 84
    LD H',RES 1,(IX+$2F)               ; @BT DD CB 2F 8C
    LD H',RES 1,(IY+$32)               ; @BT FD CB 32 8C
    LD H',RES 2,(IX+$35)               ; @BT DD CB 35 94
    LD H',RES 2,(IY+$38)               ; @BT FD CB 38 94
    LD H',RES 3,(IX+$3B)               ; @BT DD CB 3B 9C
    LD H',RES 3,(IY+$3E)               ; @BT FD CB 3E 9C
    LD H',RES 4,(IX+$41)               ; @BT DD CB 41 A4
    LD H',RES 4,(IY+$44)               ; @BT FD CB 44 A4
    LD H',RES 5,(IX+$47)               ; @BT DD CB 47 AC
    LD H',RES 5,(IY+$4A)               ; @BT FD CB 4A AC
    LD H',RES 6,(IX+$4D)               ; @BT DD CB 4D B4
    LD H',RES 6,(IY+$50)               ; @BT FD CB 50 B4
    LD H',RES 7,(IX+$53)               ; @BT DD CB 53 BC
    LD H',RES 7,(IY+$56)               ; @BT FD CB 56 BC
    LD H',RLC (IX+$59)                 ; @BT DD CB 59 04
    LD H',RLC (IY+$5C)                 ; @BT FD CB 5C 04
    LD H',RL (IX+$5F)                  ; @BT DD CB 5F 14
    LD H',RL (IY+$62)                  ; @BT FD CB 62 14
    LD H',RRC (IX+$65)                 ; @BT DD CB 65 0C
    LD H',RRC (IY+$68)                 ; @BT FD CB 68 0C
    LD H',RR (IX+$6B)                  ; @BT DD CB 6B 1C
    LD H',RR (IY+$6E)                  ; @BT FD CB 6E 1C
    LD H',SET 0,(IX+$71)               ; @BT DD CB 71 C4
    LD H',SET 0,(IY+$74)               ; @BT FD CB 74 C4
    LD H',SET 1,(IX+$77)               ; @BT DD CB 77 CC
    LD H',SET 1,(IY+$7A)               ; @BT FD CB 7A CC
    LD H',SET 2,(IX+$7D)               ; @BT DD CB 7D D4
    LD H',SET 2,(IY+$00)               ; @BT FD CB 00 D4
    LD H',SET 3,(IX+$03)               ; @BT DD CB 03 DC
    LD H',SET 3,(IY+$06)               ; @BT FD CB 06 DC
    LD H',SET 4,(IX+$09)               ; @BT DD CB 09 E4
    LD H',SET 4,(IY+$0C)               ; @BT FD CB 0C E4
    LD H',SET 5,(IX+$0F)               ; @BT DD CB 0F EC
    LD H',SET 5,(IY+$12)               ; @BT FD CB 12 EC
    LD H',SET 6,(IX+$15)               ; @BT DD CB 15 F4
    LD H',SET 6,(IY+$18)               ; @BT FD CB 18 F4
    LD H',SET 7,(IX+$1B)               ; @BT DD CB 1B FC
    LD H',SET 7,(IY+$1E)               ; @BT FD CB 1E FC
    LD H',SLA (IX+$21)                 ; @BT DD CB 21 24
    LD H',SLA (IY+$24)                 ; @BT FD CB 24 24
    LD H',SLL (IX+$27)                 ; @BT DD CB 27 34
    LD H',SLL (IY+$2A)                 ; @BT FD CB 2A 34
    LD H',SRA (IX+$2D)                 ; @BT DD CB 2D 2C
    LD H',SRA (IY+$30)                 ; @BT FD CB 30 2C
    LD H',SRL (IX+$33)                 ; @BT DD CB 33 3C
    LD H',SRL (IY+$36)                 ; @BT FD CB 36 3C
    LD L',RES 0,(IX+$39)               ; @BT DD CB 39 85
    LD L',RES 0,(IY+$3C)               ; @BT FD CB 3C 85
    LD L',RES 1,(IX+$3F)               ; @BT DD CB 3F 8D
    LD L',RES 1,(IY+$42)               ; @BT FD CB 42 8D
    LD L',RES 2,(IX+$45)               ; @BT DD CB 45 95
    LD L',RES 2,(IY+$48)               ; @BT FD CB 48 95
    LD L',RES 3,(IX+$4B)               ; @BT DD CB 4B 9D
    LD L',RES 3,(IY+$4E)               ; @BT FD CB 4E 9D
    LD L',RES 4,(IX+$51)               ; @BT DD CB 51 A5
    LD L',RES 4,(IY+$54)               ; @BT FD CB 54 A5
    LD L',RES 5,(IX+$57)               ; @BT DD CB 57 AD
    LD L',RES 5,(IY+$5A)               ; @BT FD CB 5A AD
    LD L',RES 6,(IX+$5D)               ; @BT DD CB 5D B5
    LD L',RES 6,(IY+$60)               ; @BT FD CB 60 B5
    LD L',RES 7,(IX+$63)               ; @BT DD CB 63 BD
    LD L',RES 7,(IY+$66)               ; @BT FD CB 66 BD
    LD L',RLC (IX+$69)                 ; @BT DD CB 69 05
    LD L',RLC (IY+$6C)                 ; @BT FD CB 6C 05
    LD L',RL (IX+$6F)                  ; @BT DD CB 6F 15
    LD L',RL (IY+$72)                  ; @BT FD CB 72 15
    LD L',RRC (IX+$75)                 ; @BT DD CB 75 0D
    LD L',RRC (IY+$78)                 ; @BT FD CB 78 0D
    LD L',RR (IX+$7B)                  ; @BT DD CB 7B 1D
    LD L',RR (IY+$7E)                  ; @BT FD CB 7E 1D
    LD L',SET 0,(IX+$01)               ; @BT DD CB 01 C5
    LD L',SET 0,(IY+$04)               ; @BT FD CB 04 C5
    LD L',SET 1,(IX+$07)               ; @BT DD CB 07 CD
    LD L',SET 1,(IY+$0A)               ; @BT FD CB 0A CD
    LD L',SET 2,(IX+$0D)               ; @BT DD CB 0D D5
    LD L',SET 2,(IY+$10)               ; @BT FD CB 10 D5
    LD L',SET 3,(IX+$13)               ; @BT DD CB 13 DD
    LD L',SET 3,(IY+$16)               ; @BT FD CB 16 DD
    LD L',SET 4,(IX+$19)               ; @BT DD CB 19 E5
    LD L',SET 4,(IY+$1C)               ; @BT FD CB 1C E5
    LD L',SET 5,(IX+$1F)               ; @BT DD CB 1F ED
    LD L',SET 5,(IY+$22)               ; @BT FD CB 22 ED
    LD L',SET 6,(IX+$25)               ; @BT DD CB 25 F5
    LD L',SET 6,(IY+$28)               ; @BT FD CB 28 F5
    LD L',SET 7,(IX+$2B)               ; @BT DD CB 2B FD
    LD L',SET 7,(IY+$2E)               ; @BT FD CB 2E FD
    LD L',SLA (IX+$31)                 ; @BT DD CB 31 25
    LD L',SLA (IY+$34)                 ; @BT FD CB 34 25
    LD L',SLL (IX+$37)                 ; @BT DD CB 37 35
    LD L',SLL (IY+$3A)                 ; @BT FD CB 3A 35
    LD L',SRA (IX+$3D)                 ; @BT DD CB 3D 2D
    LD L',SRA (IY+$40)                 ; @BT FD CB 40 2D
    LD L',SRL (IX+$43)                 ; @BT DD CB 43 3D
    LD L',SRL (IY+$46)                 ; @BT FD CB 46 3D
    LDIX                               ; @BT ED A4
    LDWS                               ; @BT ED A5
    LDDRX                              ; @BT ED BC
    LDDR                               ; @BT ED B8
    LDDX                               ; @BT ED AC
    LDD                                ; @BT ED A8
    LDIRX                              ; @BT ED B4
    LDIR                               ; @BT ED B0
    LDI                                ; @BT ED A0
    LDPIRX                             ; @BT ED B7
    MIRROR A'                          ; @BT ED 24
    MUL D',E                           ; @BT ED 30
    NEXTREG $DA,A'                     ; @BT ED 92 DA
    NEXTREG $F1,$F1                    ; @BT ED 91 F1 F1
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
    OR A',(IX+$14)                     ; @BT DD B6 14
    OR A',IYH                          ; @BT FD B4
    OR A',IYL                          ; @BT FD B5
    OR A',(IY+$1D)                     ; @BT FD B6 1D
    OR A',L'                           ; @BT B5
    OR A',$78                          ; @BT F6 78
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
    OUT ($75),A'                       ; @BT D3 75
    OUTD                               ; @BT ED AB
    OUTI                               ; @BT ED A3
    OUTINB                             ; @BT ED 90
    PIXELDN                            ; @BT ED 93
    PIXELAD                            ; @BT ED 94
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
    PUSH $9862                         ; @BT ED 8A 98 62
    RES 0,A'                           ; @BT CB 87
    RES 0,B'                           ; @BT CB 80
    RES 0,C'                           ; @BT CB 81
    RES 0,D'                           ; @BT CB 82
    RES 0,E'                           ; @BT CB 83
    RES 0,H'                           ; @BT CB 84
    RES 0,(HL')                        ; @BT CB 86
    RES 0,(IX+$12)                     ; @BT DD CB 12 86
    RES 0,(IY+$15)                     ; @BT FD CB 15 86
    RES 0,L'                           ; @BT CB 85
    RES 1,A'                           ; @BT CB 8F
    RES 1,B'                           ; @BT CB 88
    RES 1,C'                           ; @BT CB 89
    RES 1,D'                           ; @BT CB 8A
    RES 1,E'                           ; @BT CB 8B
    RES 1,H'                           ; @BT CB 8C
    RES 1,(HL')                        ; @BT CB 8E
    RES 1,(IX+$30)                     ; @BT DD CB 30 8E
    RES 1,(IY+$33)                     ; @BT FD CB 33 8E
    RES 1,L'                           ; @BT CB 8D
    RES 2,A'                           ; @BT CB 97
    RES 2,B'                           ; @BT CB 90
    RES 2,C'                           ; @BT CB 91
    RES 2,D'                           ; @BT CB 92
    RES 2,E'                           ; @BT CB 93
    RES 2,H'                           ; @BT CB 94
    RES 2,(HL')                        ; @BT CB 96
    RES 2,(IX+$4E)                     ; @BT DD CB 4E 96
    RES 2,(IY+$51)                     ; @BT FD CB 51 96
    RES 2,L'                           ; @BT CB 95
    RES 3,A'                           ; @BT CB 9F
    RES 3,B'                           ; @BT CB 98
    RES 3,C'                           ; @BT CB 99
    RES 3,D'                           ; @BT CB 9A
    RES 3,E'                           ; @BT CB 9B
    RES 3,H'                           ; @BT CB 9C
    RES 3,(HL')                        ; @BT CB 9E
    RES 3,(IX+$6C)                     ; @BT DD CB 6C 9E
    RES 3,(IY+$6F)                     ; @BT FD CB 6F 9E
    RES 3,L'                           ; @BT CB 9D
    RES 4,A'                           ; @BT CB A7
    RES 4,B'                           ; @BT CB A0
    RES 4,C'                           ; @BT CB A1
    RES 4,D'                           ; @BT CB A2
    RES 4,E'                           ; @BT CB A3
    RES 4,H'                           ; @BT CB A4
    RES 4,(HL')                        ; @BT CB A6
    RES 4,(IX+$0A)                     ; @BT DD CB 0A A6
    RES 4,(IY+$0D)                     ; @BT FD CB 0D A6
    RES 4,L'                           ; @BT CB A5
    RES 5,A'                           ; @BT CB AF
    RES 5,B'                           ; @BT CB A8
    RES 5,C'                           ; @BT CB A9
    RES 5,D'                           ; @BT CB AA
    RES 5,E'                           ; @BT CB AB
    RES 5,H'                           ; @BT CB AC
    RES 5,(HL')                        ; @BT CB AE
    RES 5,(IX+$28)                     ; @BT DD CB 28 AE
    RES 5,(IY+$2B)                     ; @BT FD CB 2B AE
    RES 5,L'                           ; @BT CB AD
    RES 6,A'                           ; @BT CB B7
    RES 6,B'                           ; @BT CB B0
    RES 6,C'                           ; @BT CB B1
    RES 6,D'                           ; @BT CB B2
    RES 6,E'                           ; @BT CB B3
    RES 6,H'                           ; @BT CB B4
    RES 6,(HL')                        ; @BT CB B6
    RES 6,(IX+$46)                     ; @BT DD CB 46 B6
    RES 6,(IY+$49)                     ; @BT FD CB 49 B6
    RES 6,L'                           ; @BT CB B5
    RES 7,A'                           ; @BT CB BF
    RES 7,B'                           ; @BT CB B8
    RES 7,C'                           ; @BT CB B9
    RES 7,D'                           ; @BT CB BA
    RES 7,E'                           ; @BT CB BB
    RES 7,H'                           ; @BT CB BC
    RES 7,(HL')                        ; @BT CB BE
    RES 7,(IX+$64)                     ; @BT DD CB 64 BE
    RES 7,(IY+$67)                     ; @BT FD CB 67 BE
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
    RL (IX+$23)                        ; @BT DD CB 23 16
    RL (IY+$26)                        ; @BT FD CB 26 16
    RL L'                              ; @BT CB 15
    RLC A'                             ; @BT CB 07
    RLC B'                             ; @BT CB 00
    RLC C'                             ; @BT CB 01
    RLC D'                             ; @BT CB 02
    RLC E'                             ; @BT CB 03
    RLC H'                             ; @BT CB 04
    RLC (HL')                          ; @BT CB 06
    RLC (IX+$41)                       ; @BT DD CB 41 06
    RLC (IY+$44)                       ; @BT FD CB 44 06
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
    RR (IX+$68)                        ; @BT DD CB 68 1E
    RR (IY+$6B)                        ; @BT FD CB 6B 1E
    RR L'                              ; @BT CB 1D
    RRA                                ; @BT 1F
    RRC A'                             ; @BT CB 0F
    RRC B'                             ; @BT CB 08
    RRC C'                             ; @BT CB 09
    RRC D'                             ; @BT CB 0A
    RRC E'                             ; @BT CB 0B
    RRC H'                             ; @BT CB 0C
    RRC (HL')                          ; @BT CB 0E
    RRC (IX+$09)                       ; @BT DD CB 09 0E
    RRC (IY+$0C)                       ; @BT FD CB 0C 0E
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
    RST $00                            ; @BT C7
    RST                                ; @BT C7
    SBC A',A'                          ; @BT 9F
    SBC A',B'                          ; @BT 98
    SBC A',C'                          ; @BT 99
    SBC A',D'                          ; @BT 9A
    SBC A',E'                          ; @BT 9B
    SBC A',H'                          ; @BT 9C
    SBC A',(HL')                       ; @BT 9E
    SBC A',IXH                         ; @BT DD 9C
    SBC A',IXL                         ; @BT DD 9D
    SBC A',(IX+$51)                    ; @BT DD 9E 51
    SBC A',IYH                         ; @BT FD 9C
    SBC A',IYL                         ; @BT FD 9D
    SBC A',(IY+$5A)                    ; @BT FD 9E 5A
    SBC A',L'                          ; @BT 9D
    SBC A',$A1                         ; @BT DE A1
    SBC HL',BC'                        ; @BT ED 42
    SBC HL',DE'                        ; @BT ED 52
    SBC HL',HL'                        ; @BT ED 62
    SBC HL',SP                         ; @BT ED 72
    SCF                                ; @BT 37
    SETAE                              ; @BT ED 95
    SET 0,A'                           ; @BT CB C7
    SET 0,B'                           ; @BT CB C0
    SET 0,C'                           ; @BT CB C1
    SET 0,D'                           ; @BT CB C2
    SET 0,E'                           ; @BT CB C3
    SET 0,H'                           ; @BT CB C4
    SET 0,(HL')                        ; @BT CB C6
    SET 0,(IX+$0A)                     ; @BT DD CB 0A C6
    SET 0,(IY+$0D)                     ; @BT FD CB 0D C6
    SET 0,L'                           ; @BT CB C5
    SET 1,A'                           ; @BT CB CF
    SET 1,B'                           ; @BT CB C8
    SET 1,C'                           ; @BT CB C9
    SET 1,D'                           ; @BT CB CA
    SET 1,E'                           ; @BT CB CB
    SET 1,H'                           ; @BT CB CC
    SET 1,(HL')                        ; @BT CB CE
    SET 1,(IX+$28)                     ; @BT DD CB 28 CE
    SET 1,(IY+$2B)                     ; @BT FD CB 2B CE
    SET 1,L'                           ; @BT CB CD
    SET 2,A'                           ; @BT CB D7
    SET 2,B'                           ; @BT CB D0
    SET 2,C'                           ; @BT CB D1
    SET 2,D'                           ; @BT CB D2
    SET 2,E'                           ; @BT CB D3
    SET 2,H'                           ; @BT CB D4
    SET 2,(HL')                        ; @BT CB D6
    SET 2,(IX+$46)                     ; @BT DD CB 46 D6
    SET 2,(IY+$49)                     ; @BT FD CB 49 D6
    SET 2,L'                           ; @BT CB D5
    SET 3,A'                           ; @BT CB DF
    SET 3,B'                           ; @BT CB D8
    SET 3,C'                           ; @BT CB D9
    SET 3,D'                           ; @BT CB DA
    SET 3,E'                           ; @BT CB DB
    SET 3,H'                           ; @BT CB DC
    SET 3,(HL')                        ; @BT CB DE
    SET 3,(IX+$64)                     ; @BT DD CB 64 DE
    SET 3,(IY+$67)                     ; @BT FD CB 67 DE
    SET 3,L'                           ; @BT CB DD
    SET 4,A'                           ; @BT CB E7
    SET 4,B'                           ; @BT CB E0
    SET 4,C'                           ; @BT CB E1
    SET 4,D'                           ; @BT CB E2
    SET 4,E'                           ; @BT CB E3
    SET 4,H'                           ; @BT CB E4
    SET 4,(HL')                        ; @BT CB E6
    SET 4,(IX+$02)                     ; @BT DD CB 02 E6
    SET 4,(IY+$05)                     ; @BT FD CB 05 E6
    SET 4,L'                           ; @BT CB E5
    SET 5,A'                           ; @BT CB EF
    SET 5,B'                           ; @BT CB E8
    SET 5,C'                           ; @BT CB E9
    SET 5,D'                           ; @BT CB EA
    SET 5,E'                           ; @BT CB EB
    SET 5,H'                           ; @BT CB EC
    SET 5,(HL')                        ; @BT CB EE
    SET 5,(IX+$20)                     ; @BT DD CB 20 EE
    SET 5,(IY+$23)                     ; @BT FD CB 23 EE
    SET 5,L'                           ; @BT CB ED
    SET 6,A'                           ; @BT CB F7
    SET 6,B'                           ; @BT CB F0
    SET 6,C'                           ; @BT CB F1
    SET 6,D'                           ; @BT CB F2
    SET 6,E'                           ; @BT CB F3
    SET 6,H'                           ; @BT CB F4
    SET 6,(HL')                        ; @BT CB F6
    SET 6,(IX+$3E)                     ; @BT DD CB 3E F6
    SET 6,(IY+$41)                     ; @BT FD CB 41 F6
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
    SL1 (IX+$7A)                       ; @BT DD CB 7A 36
    SL1 (IY+$7D)                       ; @BT FD CB 7D 36
    SL1 L'                             ; @BT CB 35
    SLA A'                             ; @BT CB 27
    SLA B'                             ; @BT CB 20
    SLA C'                             ; @BT CB 21
    SLA D'                             ; @BT CB 22
    SLA E'                             ; @BT CB 23
    SLA H'                             ; @BT CB 24
    SLA (HL')                          ; @BT CB 26
    SLA (IX+$18)                       ; @BT DD CB 18 26
    SLA (IY+$1B)                       ; @BT FD CB 1B 26
    SLA L'                             ; @BT CB 25
    SRA A'                             ; @BT CB 2F
    SRA B'                             ; @BT CB 28
    SRA C'                             ; @BT CB 29
    SRA D'                             ; @BT CB 2A
    SRA E'                             ; @BT CB 2B
    SRA H'                             ; @BT CB 2C
    SRA (HL')                          ; @BT CB 2E
    SRA (IX+$36)                       ; @BT DD CB 36 2E
    SRA (IY+$39)                       ; @BT FD CB 39 2E
    SRA L'                             ; @BT CB 2D
    SRL A'                             ; @BT CB 3F
    SRL B'                             ; @BT CB 38
    SRL C'                             ; @BT CB 39
    SRL D'                             ; @BT CB 3A
    SRL E'                             ; @BT CB 3B
    SRL H'                             ; @BT CB 3C
    SRL (HL')                          ; @BT CB 3E
    SRL (IX+$54)                       ; @BT DD CB 54 3E
    SRL (IY+$57)                       ; @BT FD CB 57 3E
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
    SUB A',(IX+$78)                    ; @BT DD 96 78
    SUB A',(IY+$7B)                    ; @BT FD 96 7B
    SUB A',(HL')                       ; @BT 96
    SUB A',IYH                         ; @BT FD 94
    SUB A',IYL                         ; @BT FD 95
    SUB A',$4C                         ; @BT D6 4C
    SWAPNIB                            ; @BT ED 23
    TEST $7A                           ; @BT ED 27 7A
    XOR A',A'                          ; @BT AF
    XOR A',B'                          ; @BT A8
    XOR A',C'                          ; @BT A9
    XOR A',D'                          ; @BT AA
    XOR A',E'                          ; @BT AB
    XOR A',H'                          ; @BT AC
    XOR A',(HL')                       ; @BT AE
    XOR A',IXH                         ; @BT DD AC
    XOR A',IXL                         ; @BT DD AD
    XOR A',(IX+$2B)                    ; @BT DD AE 2B
    XOR A',IYH                         ; @BT FD AC
    XOR A',IYL                         ; @BT FD AD
    XOR A',(IY+$34)                    ; @BT FD AE 34
    XOR A',L'                          ; @BT AD
    XOR A',$D3                         ; @BT EE D3

    ; Explicit bare indexed-CB forms (no LD r, destination)
    RES 0,(IX+$12)                     ; @BT DD CB 12 86
    RES 0,(IY+$15)                     ; @BT FD CB 15 86
    SET 0,(IX+$0A)                     ; @BT DD CB 0A C6
    SET 0,(IY+$0D)                     ; @BT FD CB 0D C6
    RLC (IX+$41)                       ; @BT DD CB 41 06
    RLC (IY+$44)                       ; @BT FD CB 44 06

    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 286982118

        LD C',B'                       ; @BT 48
        LD A',RR (IX+$1B)              ; @BT DD CB 1B 1F
        LD A',SRA (IY+$60)             ; @BT FD CB 60 2F
        RES 5,C'                       ; @BT CB A9
        BIT 0,L'                       ; @BT CB 45
        LD L',RES 0,(IX+$39)           ; @BT DD CB 39 85
        RET POS                        ; @BT F0
        LD L',L'                       ; @BT 6D
        BIT 5,D'                       ; @BT CB 6A
        ADD A',A'                      ; @BT 87
        LD A',SET 6,(IX+$45)           ; @BT DD CB 45 F7
        ADC A',IYH                     ; @BT FD 8C
        RES 7,D'                       ; @BT CB BA
        SET 0,(IX+$0A)                 ; @BT DD CB 0A C6
        LD L',E'                       ; @BT 6B
        RLC E'                         ; @BT CB 03
        SL1 L'                         ; @BT CB 35
        LD (HL'),L'                    ; @BT 75
        RST $08                        ; @BT CF
        LD B',RR (IY+$2E)              ; @BT FD CB 2E 18
        LD H',SET 0,(IY+$74)           ; @BT FD CB 74 C4
        BIT 6,A'                       ; @BT CB 77
        RL (IY+$26)                    ; @BT FD CB 26 16
        LD B',RL (IY+$22)              ; @BT FD CB 22 10
        LD L',D'                       ; @BT 6A
        CALL POS,$B890                 ; @BT F4 90 B8
        BIT 7,(IX+$40)                 ; @BT DD CB 40 7E
        LD A',SET 1,(IY+$2A)           ; @BT FD CB 2A CF
        LD D',RES 5,(IY+$2A)           ; @BT FD CB 2A AA
        PUSH IX                        ; @BT DD E5
        DEC IX                         ; @BT DD 2B
        SET 1,B'                       ; @BT CB C8
        RRC A'                         ; @BT CB 0F
        RLA                            ; @BT 17
        RES 5,(IY+$2B)                 ; @BT FD CB 2B AE
        IN A',($90)                    ; @BT DB 90
        LD A',RES 3,(IX+$6B)           ; @BT DD CB 6B 9F
        ADC A',(IY+$34)                ; @BT FD 8E 34
        LD H',SET 3,(IX+$03)           ; @BT DD CB 03 DC
        SET 5,A'                       ; @BT CB EF
        SET 4,(IX+$02)                 ; @BT DD CB 02 E6
        RR (IY+$6B)                    ; @BT FD CB 6B 1E
        LD D',RLC (IY+$3C)             ; @BT FD CB 3C 02
        SET 4,A'                       ; @BT CB E7
        SUB A',$4C                     ; @BT D6 4C
        RL (IX+$23)                    ; @BT DD CB 23 16
        LD (IY+$3B),H'                 ; @BT FD 74 3B
        LD L',SET 7,(IY+$2E)           ; @BT FD CB 2E FD
        JP (HL')                       ; @BT E9
        LD (IY+$2F),B'                 ; @BT FD 70 2F
        LD D',IXH                      ; @BT DD 54
        LD E',B'                       ; @BT 58
        LD D',H'                       ; @BT 54
        LD BC',($4776)                 ; @BT ED 4B 76 47
        LD IYL,E'                      ; @BT FD 6B
        LD A',SET 4,(IY+$3C)           ; @BT FD CB 3C E7
        LD C',SET 2,(IY+$50)           ; @BT FD CB 50 D1
        LD A',RES 4,(IX+$71)           ; @BT DD CB 71 A7
        RES 4,A'                       ; @BT CB A7
        LD D',RES 1,(IX+$0F)           ; @BT DD CB 0F 8A
        XOR A',(IX+$2B)                ; @BT DD AE 2B
        LD ($EDD2),DE'                 ; @BT ED 53 D2 ED
        INC (HL')                      ; @BT 34
        SET 1,E'                       ; @BT CB CB
        JR EQU,$E0                     ; @BT 28 E0
        LD A',RES 5,(IX+$77)           ; @BT DD CB 77 AF
        RES 0,(IX+$12)                 ; @BT DD CB 12 86
        CALL EQU,$B9A1                 ; @BT CC A1 B9
        ADC HL',HL'                    ; @BT ED 6A
        AND A',A'                      ; @BT A7
        RLC L'                         ; @BT CB 05
        LD SP,IX                       ; @BT DD F9
        RST $38                        ; @BT FF
        XOR A',IYL                     ; @BT FD AD
        LD C',IYH                      ; @BT FD 4C
        LD A',RES 7,(IY+$06)           ; @BT FD CB 06 BF
        LD B',H'                       ; @BT 44
        BIT 4,D'                       ; @BT CB 62
        SBC A',E'                      ; @BT 9B
        LD B',SET 3,(IY+$46)           ; @BT FD CB 46 D8
        SBC A',(IY+$5A)                ; @BT FD 9E 5A
        INIR                           ; @BT ED B2
        SUB A',IYL                     ; @BT FD 95
        RES 7,(HL')                    ; @BT CB BE
        LD E',SET 6,(IY+$08)           ; @BT FD CB 08 F3
        LD (BC'),A'                    ; @BT 02
        JP (IX)                        ; @BT DD E9
        LD C',RES 7,(IY+$26)           ; @BT FD CB 26 B9
        LD D',SET 5,(IX+$6F)           ; @BT DD CB 6F EA
        BIT 4,E'                       ; @BT CB 63
        RES 0,C'                       ; @BT CB 81
        BIT 2,E'                       ; @BT CB 53
        SUB A',E'                      ; @BT 93
        SET 5,H'                       ; @BT CB EC
        LD A',(IX+$55)                 ; @BT DD 7E 55
        LD E',IYL                      ; @BT FD 5D
        LD B',SET 0,(IX+$31)           ; @BT DD CB 31 C0
        INC (IY+$66)                   ; @BT FD 34 66
        LD L',RRC (IX+$75)             ; @BT DD CB 75 0D
        OR A',(IY+$1D)                 ; @BT FD B6 1D
        IN H',(C')                     ; @BT ED 60
        LD D',SET 7,(IY+$7E)           ; @BT FD CB 7E FA
        LD B',E'                       ; @BT 43
        LD B',SET 4,(IY+$4C)           ; @BT FD CB 4C E0
        SET 6,E'                       ; @BT CB F3
        SRL A'                         ; @BT CB 3F
        SET 0,B'                       ; @BT CB C0
        LD IXH,B'                      ; @BT DD 60
        LD A',SET 1,(IX+$27)           ; @BT DD CB 27 CF
        INC IYH                        ; @BT FD 24
        LD B',SLA (IY+$64)             ; @BT FD CB 64 20
        LD E',SLA (IY+$14)             ; @BT FD CB 14 23
        SET 0,(HL')                    ; @BT CB C6
        RL H'                          ; @BT CB 14
        JP GTE,$2223                   ; @BT D2 23 22
        LD B',IXL                      ; @BT DD 45
        RRC D'                         ; @BT CB 0A
        LD IXH,IXL                     ; @BT DD 65
        RES 3,E'                       ; @BT CB 9B
        LD DE',($6CC9)                 ; @BT ED 5B C9 6C
        LD (IX+$73),$88                ; @BT DD 36 73 88
        CALL GTE,$B66E                 ; @BT D4 6E B6
        LD I,A'                        ; @BT ED 47
        LD D',RES 7,(IX+$33)           ; @BT DD CB 33 BA
        LD E',SET 0,(IY+$64)           ; @BT FD CB 64 C3
        LD A',I                        ; @BT ED 57
        LD (DE'),A'                    ; @BT 12
        LD (IX+$6A),E'                 ; @BT DD 73 6A
        RES 5,H'                       ; @BT CB AC
        LD L',RES 6,(IX+$5D)           ; @BT DD CB 5D B5
        INC H'                         ; @BT 24
        RET NEQ                        ; @BT C0
        RR H'                          ; @BT CB 1C
        JP (C')                        ; @BT ED 98
        INDR                           ; @BT ED BA
        LD E',IYH                      ; @BT FD 5C
        SBC A',H'                      ; @BT 9C
        LD D',$58                      ; @BT 16 58
        LD B',D'                       ; @BT 42
        DI                             ; @BT F3
        LD B',SRA (IY+$70)             ; @BT FD CB 70 28
        RRC (IX+$09)                   ; @BT DD CB 09 0E
        LD L',SET 4,(IX+$19)           ; @BT DD CB 19 E5
        SL1 E'                         ; @BT CB 33
        SUB A',(IX+$78)                ; @BT DD 96 78
        LD B',SLL (IY+$6A)             ; @BT FD CB 6A 30
        SET 3,L'                       ; @BT CB DD
        LD DE',$6DDA                   ; @BT 11 DA 6D
        SL1 H'                         ; @BT CB 34
        LD IYL,D'                      ; @BT FD 6A
        LD E',RES 6,(IX+$3D)           ; @BT DD CB 3D B3
        IN L',(C')                     ; @BT ED 68
        PIXELDN                        ; @BT ED 93
        LD C',H'                       ; @BT 4C
        LD B',RES 4,(IY+$04)           ; @BT FD CB 04 A0
        RES 0,(IY+$15)                 ; @BT FD CB 15 86
        LD ($F216),IX                  ; @BT DD 22 16 F2
        SET 7,(IY+$5F)                 ; @BT FD CB 5F FE
        RES 3,C'                       ; @BT CB 99
        SET 0,E'                       ; @BT CB C3
        LD E',SET 5,(IX+$7F)           ; @BT DD CB 7F EB
        ADD DE,A'                      ; @BT ED 32
        RES 1,(IX+$30)                 ; @BT DD CB 30 8E
        BIT 1,B'                       ; @BT CB 48
        JP $2889                       ; @BT C3 89 28
        LD IXL,D'                      ; @BT DD 6A
        ADC HL',DE'                    ; @BT ED 5A
        XOR A',C'                      ; @BT A9
        LD L',C'                       ; @BT 69
        LD C',SLL (IY+$7A)             ; @BT FD CB 7A 31
        OUTD                           ; @BT ED AB
        AND A',E'                      ; @BT A3
        LD C',RES 0,(IX+$79)           ; @BT DD CB 79 81
        LD D',RES 4,(IX+$21)           ; @BT DD CB 21 A2
        ADD A',D'                      ; @BT 82
        SET 2,A'                       ; @BT CB D7
        RES 6,B'                       ; @BT CB B0
        LD B',RLC (IX+$19)             ; @BT DD CB 19 00
        SET 1,(IX+$28)                 ; @BT DD CB 28 CE
        SUB A',A'                      ; @BT 97
        MIRROR A'                      ; @BT ED 24
        IND                            ; @BT ED AA
        SET 4,E'                       ; @BT CB E3
        LD D',RES 6,(IY+$30)           ; @BT FD CB 30 B2
        ADC HL',BC'                    ; @BT ED 4A
        RES 2,A'                       ; @BT CB 97
        RET PO                         ; @BT E0
        LD A',(BC')                    ; @BT 0A
        LD D',SET 5,(IY+$72)           ; @BT FD CB 72 EA
        NEXTREG $F1,$F1                ; @BT ED 91 F1 F1
        LD B',SET 5,(IX+$4F)           ; @BT DD CB 4F E8
        LD L',SET 1,(IX+$07)           ; @BT DD CB 07 CD
        LD (HL'),A'                    ; @BT 77
        XOR A',E'                      ; @BT AB
        RST $00                        ; @BT C7
        LD D',RES 4,(IY+$24)           ; @BT FD CB 24 A2
        SET 3,(HL')                    ; @BT CB DE
        LD D',A'                       ; @BT 57
        RL C'                          ; @BT CB 11
        OR A',E'                       ; @BT B3
        LD H',SLA (IX+$21)             ; @BT DD CB 21 24
        LD L',SET 2,(IY+$10)           ; @BT FD CB 10 D5
        LD B',RES 3,(IY+$7E)           ; @BT FD CB 7E 98
        SRA H'                         ; @BT CB 2C
        LD B',RES 6,(IX+$0D)           ; @BT DD CB 0D B0
        RLC (IY+$44)                   ; @BT FD CB 44 06
        LD E',RES 1,(IX+$1F)           ; @BT DD CB 1F 8B
        LD E',SET 4,(IY+$7C)           ; @BT FD CB 7C E3
        LD L',SRL (IY+$46)             ; @BT FD CB 46 3D
        LD IXH,D'                      ; @BT DD 62
        CP A',D'                       ; @BT BA
        LD D',SLA (IY+$04)             ; @BT FD CB 04 22
        LD L',SET 6,(IX+$25)           ; @BT DD CB 25 F5
        RR A'                          ; @BT CB 1F
        LD B',(IX+$11)                 ; @BT DD 46 11
        AND A',IXL                     ; @BT DD A5
        LD D',SRA (IY+$10)             ; @BT FD CB 10 2A
        ADD A',(HL')                   ; @BT 86
        LD A',E'                       ; @BT 7B
        BIT 6,C'                       ; @BT CB 71
        SRL (HL')                      ; @BT CB 3E
        SUB A',IXL                     ; @BT DD 95
        LD H',L'                       ; @BT 65
        BIT 1,L'                       ; @BT CB 4D
        CALL PE,$BAB2                  ; @BT EC B2 BA
        LD E',C'                       ; @BT 59
        LD B',SET 1,(IX+$37)           ; @BT DD CB 37 C8
        PUSH DE'                       ; @BT D5
        RET                            ; @BT C9
        ADD A',IXH                     ; @BT DD 84
        IN A',(C')                     ; @BT ED 78
        SLA L'                         ; @BT CB 25
        LD H',RRC (IY+$68)             ; @BT FD CB 68 0C
        SET 4,H'                       ; @BT CB E4
        RL (HL')                       ; @BT CB 16
        RES 2,(IY+$51)                 ; @BT FD CB 51 96
        EX DE',HL'                     ; @BT EB
        RETI                           ; @BT ED 4D
        LD A',SET 5,(IY+$42)           ; @BT FD CB 42 EF
        DEC IY                         ; @BT FD 2B
        RES 6,A'                       ; @BT CB B7
        XOR A',IXL                     ; @BT DD AD
        LD B',SET 4,(IX+$49)           ; @BT DD CB 49 E0
        DEC D'                         ; @BT 15
        LD D',SET 3,(IX+$63)           ; @BT DD CB 63 DA
        BSRL DE',B                     ; @BT ED 2A
        LD (HL'),B'                    ; @BT 70
        XOR A',$D3                     ; @BT EE D3
        LD SP,($B109)                  ; @BT ED 7B 09 B1
        RES 3,D'                       ; @BT CB 9A
        DEC SP                         ; @BT 3B
        SET 2,B'                       ; @BT CB D0
        JP EQU,$2556                   ; @BT CA 56 25
        LD A',SRL (IY+$66)             ; @BT FD CB 66 3F
        RST $00                        ; @BT C7
        SET 6,(IX+$3E)                 ; @BT DD CB 3E F6
        LD L',SLA (IY+$34)             ; @BT FD CB 34 25
        BIT 3,E'                       ; @BT CB 5B
        IN B',(C')                     ; @BT ED 40
        SET 3,E'                       ; @BT CB DB
        RLC H'                         ; @BT CB 04
        ADD IY,IY                      ; @BT FD 29
        SUB A',(IY+$7B)                ; @BT FD 96 7B
        RES 7,B'                       ; @BT CB B8
        LD L',RES 1,(IY+$42)           ; @BT FD CB 42 8D
        SLA H'                         ; @BT CB 24
        LD E',RR (IY+$5E)              ; @BT FD CB 5E 1B
        RES 5,(HL')                    ; @BT CB AE
        LD D',RL (IY+$42)              ; @BT FD CB 42 12
        CP A',IXH                      ; @BT DD BC
        LD A',RLC (IX+$09)             ; @BT DD CB 09 07
        SLA B'                         ; @BT CB 20
        LD A',IXL                      ; @BT DD 7D
        RES 0,(HL')                    ; @BT CB 86
        BIT 7,E'                       ; @BT CB 7B
        LD IX,$B54D                    ; @BT DD 21 4D B5
        LD C',RES 4,(IY+$14)           ; @BT FD CB 14 A1
        LD B',B'                       ; @BT 40
        LD D',SET 0,(IY+$54)           ; @BT FD CB 54 C2
        JR GTE,$B2                     ; @BT 30 B2
        RES 3,(HL')                    ; @BT CB 9E
        LD E',RES 4,(IY+$34)           ; @BT FD CB 34 A3
        SET 6,L'                       ; @BT CB F5
        SBC HL',SP                     ; @BT ED 72
        LD D',SET 6,(IX+$75)           ; @BT DD CB 75 F2
        XOR A',(IY+$34)                ; @BT FD AE 34
        OR A',IYH                      ; @BT FD B4
        LD IXH,IXH                     ; @BT DD 64
        LD B',RES 1,(IY+$72)           ; @BT FD CB 72 88
        RES 5,A'                       ; @BT CB AF
        LD A',D'                       ; @BT 7A
        LD H',SET 3,(IY+$06)           ; @BT FD CB 06 DC
        RES 6,(IY+$49)                 ; @BT FD CB 49 B6
        LD H',SET 4,(IY+$0C)           ; @BT FD CB 0C E4
        CALL LSS,$B44C                 ; @BT DC 4C B4
        RL E'                          ; @BT CB 13
        LD H',SET 6,(IX+$15)           ; @BT DD CB 15 F4
        LD E',E'                       ; @BT 5B
        LD L',SRA (IY+$40)             ; @BT FD CB 40 2D
        LD D',RR (IX+$4B)              ; @BT DD CB 4B 1A
        LD L',RL (IX+$6F)              ; @BT DD CB 6F 15
        LD A',C'                       ; @BT 79
        SET 6,D'                       ; @BT CB F2
        LD L',RES 5,(IX+$57)           ; @BT DD CB 57 AD
        LD C',RES 1,(IY+$02)           ; @BT FD CB 02 89
        IN D',(C')                     ; @BT ED 50
        CP A',IXL                      ; @BT DD BD
        RETN                           ; @BT ED 45
        LD H',RES 7,(IX+$53)           ; @BT DD CB 53 BC
        EXX                            ; @BT D9
        ADD A',(IX+$64)                ; @BT DD 86 64
        LD D',SET 7,(IX+$7B)           ; @BT DD CB 7B FA
        LD B',SET 5,(IY+$52)           ; @BT FD CB 52 E8
        CPD                            ; @BT ED A9
        LD E',SET 5,(IY+$02)           ; @BT FD CB 02 EB
        LD D',SLL (IY+$0A)             ; @BT FD CB 0A 32
        RLC (HL')                      ; @BT CB 06
        IN C',(C')                     ; @BT ED 48
        SET 4,(IY+$05)                 ; @BT FD CB 05 E6
        LD IYH,B'                      ; @BT FD 60
        SRL H'                         ; @BT CB 3C
        BIT 5,E'                       ; @BT CB 6B
        LD IYL,IYL                     ; @BT FD 6D
        ADD IX,DE'                     ; @BT DD 19
        LD D',IYL                      ; @BT FD 55
        JP NEG,$2112                   ; @BT FA 12 21
        LD D',RR (IY+$4E)              ; @BT FD CB 4E 1A
        LD H',SET 7,(IY+$1E)           ; @BT FD CB 1E FC
        CPIR                           ; @BT ED B1
        INC BC'                        ; @BT 03
        LD H',RES 1,(IX+$2F)           ; @BT DD CB 2F 8C
        LD L',RES 1,(IX+$3F)           ; @BT DD CB 3F 8D
        BIT 5,A'                       ; @BT CB 6F
        LD L',RES 4,(IY+$54)           ; @BT FD CB 54 A5
        SL1 (IY+$7D)                   ; @BT FD CB 7D 36
        DEC (HL')                      ; @BT 35
        LD E',RES 7,(IY+$46)           ; @BT FD CB 46 BB
        RES 5,D'                       ; @BT CB AA
        LD B',$61                      ; @BT 06 61
        INC A'                         ; @BT 3C
        SET 5,(HL')                    ; @BT CB EE
        SET 3,C'                       ; @BT CB D9
        OR A',H'                       ; @BT B4
        LD H',SET 6,(IY+$18)           ; @BT FD CB 18 F4
        LD B',RES 7,(IX+$13)           ; @BT DD CB 13 B8
        BIT 0,E'                       ; @BT CB 43
        LD C',SET 3,(IY+$56)           ; @BT FD CB 56 D9
        LD E',RRC (IY+$58)             ; @BT FD CB 58 0B
        RRD                            ; @BT ED 67
        LD A',SET 6,(IY+$48)           ; @BT FD CB 48 F7
        LD A',$C3                      ; @BT 3E C3
        SET 5,C'                       ; @BT CB E9
        RRC H'                         ; @BT CB 0C
        BIT 4,(HL')                    ; @BT CB 66
        POP IY                         ; @BT FD E1
        RES 6,E'                       ; @BT CB B3
        SRL B'                         ; @BT CB 38
        ADC A',(HL')                   ; @BT 8E
        LD C',SET 6,(IX+$65)           ; @BT DD CB 65 F1
        RR L'                          ; @BT CB 1D
        LD (IX+$5E),A'                 ; @BT DD 77 5E
        BIT 6,(IY+$25)                 ; @BT FD CB 25 76
        LD D',RES 0,(IX+$09)           ; @BT DD CB 09 82
        LD E',RES 2,(IY+$28)           ; @BT FD CB 28 93
        RES 1,(HL')                    ; @BT CB 8E
        ADD A',C'                      ; @BT 81
        SET 4,L'                       ; @BT CB E5
        BIT 1,(HL')                    ; @BT CB 4E
        RES 4,D'                       ; @BT CB A2
        LD A',SRL (IX+$63)             ; @BT DD CB 63 3F
        RES 3,H'                       ; @BT CB 9C
        BIT 1,C'                       ; @BT CB 49
        BIT 7,(HL')                    ; @BT CB 7E
        SET 2,L'                       ; @BT CB D5
        LD B',SRL (IX+$73)             ; @BT DD CB 73 38
        LD HL',$9771                   ; @BT 21 71 97
        LD D',RES 6,(IX+$2D)           ; @BT DD CB 2D B2
        SWAPNIB                        ; @BT ED 23
        LD E',SET 1,(IX+$67)           ; @BT DD CB 67 CB
        INC C'                         ; @BT 0C
        DEC H'                         ; @BT 25
        LD L',RES 2,(IY+$48)           ; @BT FD CB 48 95
        LD IYH,E'                      ; @BT FD 63
        SET 6,(HL')                    ; @BT CB F6
        RL A'                          ; @BT CB 17
        LD C',SRL (IX+$03)             ; @BT DD CB 03 39
        ADD HL',SP                     ; @BT 39
        LD D',SET 3,(IY+$66)           ; @BT FD CB 66 DA
        JP NEQ,$2334                   ; @BT C2 34 23
        POP HL'                        ; @BT E1
        ADC A',A'                      ; @BT 8F
        BIT 3,A'                       ; @BT CB 5F
        ADD A',$08                     ; @BT C6 08
        RES 0,L'                       ; @BT CB 85
        LD L',SET 5,(IX+$1F)           ; @BT DD CB 1F ED
        SBC HL',BC'                    ; @BT ED 42
        BIT 5,(HL')                    ; @BT CB 6E
        LD C',RES 3,(IY+$0E)           ; @BT FD CB 0E 99
        LD C',IXL                      ; @BT DD 4D
        SET 5,E'                       ; @BT CB EB
        RES 0,E'                       ; @BT CB 83
        LD A',SET 7,(IX+$4B)           ; @BT DD CB 4B FF
        SL1 C'                         ; @BT CB 31
        ADD IX,IX                      ; @BT DD 29
        LD ($EBB0),A'                  ; @BT 32 B0 EB
        LD C',(HL')                    ; @BT 4E
        AND A',(IX+$47)                ; @BT DD A6 47
        LD H',RES 2,(IY+$38)           ; @BT FD CB 38 94
        BIT 6,B'                       ; @BT CB 70
        RES 0,A'                       ; @BT CB 87
        LD C',RES 3,(IX+$0B)           ; @BT DD CB 0B 99
        LD A',SLL (IY+$5A)             ; @BT FD CB 5A 37
        DAA                            ; @BT 27
        XOR A',(HL')                   ; @BT AE
        OR A',(IX+$14)                 ; @BT DD B6 14
        LD A',($4221)                  ; @BT 3A 21 42
        CP A',(HL')                    ; @BT BE
        LD A',RRC (IX+$15)             ; @BT DD CB 15 0F
        LD L',RES 6,(IY+$60)           ; @BT FD CB 60 B5
        LD IXL,IXL                     ; @BT DD 6D
        SRL E'                         ; @BT CB 3B
        PIXELAD                        ; @BT ED 94
        ADC A',IXL                     ; @BT DD 8D
        RET NEG                        ; @BT F8
        RES 1,(IY+$33)                 ; @BT FD CB 33 8E
        LD C',SET 4,(IX+$59)           ; @BT DD CB 59 E1
        LD H',SRL (IX+$33)             ; @BT DD CB 33 3C
        PUSH HL'                       ; @BT E5
        SET 7,(IX+$5C)                 ; @BT DD CB 5C FE
        SET 3,B'                       ; @BT CB D8
        LD B',SET 6,(IX+$55)           ; @BT DD CB 55 F0
        CP A',IYH                      ; @BT FD BC
        ADD IX,BC'                     ; @BT DD 09
        LD (IY+$35),D'                 ; @BT FD 72 35
        BIT 5,H'                       ; @BT CB 6C
        LD C',E'                       ; @BT 4B
        LD A',SET 3,(IX+$33)           ; @BT DD CB 33 DF
        BIT 3,H'                       ; @BT CB 5C
        SET 1,H'                       ; @BT CB CC
        SET 0,(IY+$0D)                 ; @BT FD CB 0D C6
        RET GTE                        ; @BT D0
        INC B'                         ; @BT 04
        LD D',L'                       ; @BT 55
        LDPIRX                         ; @BT ED B7
        OR A',L'                       ; @BT B5
        BIT 6,H'                       ; @BT CB 74
        LD D',SLL (IX+$07)             ; @BT DD CB 07 32
        LDDRX                          ; @BT ED BC
        OUT ($75),A'                   ; @BT D3 75
        LD A',SET 2,(IX+$2D)           ; @BT DD CB 2D D7
        RES 1,D'                       ; @BT CB 8A
        OUT (C'),A'                    ; @BT ED 79
        LD E',SET 3,(IY+$76)           ; @BT FD CB 76 DB
        LD (HL'),E'                    ; @BT 73
        BIT 3,C'                       ; @BT CB 59
        LD B',SLA (IX+$61)             ; @BT DD CB 61 20
        LD A',L'                       ; @BT 7D
        SET 6,B'                       ; @BT CB F0
        LD IYL,B'                      ; @BT FD 68
        SL1 D'                         ; @BT CB 32
        BIT 7,L'                       ; @BT CB 7D
        LDDX                           ; @BT ED AC
        SRA D'                         ; @BT CB 2A
        LD IYL,C'                      ; @BT FD 69
        SBC HL',DE'                    ; @BT ED 52
        SLA C'                         ; @BT CB 21
        ADD A',(IY+$6D)                ; @BT FD 86 6D
        LD E',RR (IX+$5B)              ; @BT DD CB 5B 1B
        LD C',SRL (IY+$06)             ; @BT FD CB 06 39
        BIT 3,(HL')                    ; @BT CB 5E
        RLD                            ; @BT ED 6F
        SRA C'                         ; @BT CB 29
        LD E',RL (IX+$4F)              ; @BT DD CB 4F 13
        SET 5,B'                       ; @BT CB E8
        SRL (IY+$57)                   ; @BT FD CB 57 3E
        RES 4,E'                       ; @BT CB A3
        SET 1,(IY+$2B)                 ; @BT FD CB 2B CE
        OUT (C'),E'                    ; @BT ED 59
        LD D',RES 3,(IY+$1E)           ; @BT FD CB 1E 9A
        ADC A',L'                      ; @BT 8D
        LD D',RES 5,(IX+$27)           ; @BT DD CB 27 AA
        LD D',C'                       ; @BT 51
        LD E',SET 7,(IX+$0B)           ; @BT DD CB 0B FB
        LD (IY+$38),E'                 ; @BT FD 73 38
        POP AF'                        ; @BT F1
        RR (HL')                       ; @BT CB 1E
        LDDR                           ; @BT ED B8
        LD B',RES 7,(IY+$16)           ; @BT FD CB 16 B8
        LD B',RES 5,(IY+$0A)           ; @BT FD CB 0A A8
        LD E',RES 3,(IX+$2B)           ; @BT DD CB 2B 9B
        BRLC DE',B                     ; @BT ED 2C
        AND A',D'                      ; @BT A2
        BIT 0,C'                       ; @BT CB 41
        INC SP                         ; @BT 33
        LD C',IXH                      ; @BT DD 4C
        POP DE'                        ; @BT D1
        LDIX                           ; @BT ED A4
        LD B',RES 1,(IX+$6F)           ; @BT DD CB 6F 88
        LD D',E'                       ; @BT 53
        SET 0,L'                       ; @BT CB C5
        LD A',SET 3,(IY+$36)           ; @BT FD CB 36 DF
        RES 2,C'                       ; @BT CB 91
        LD H',C'                       ; @BT 61
        LD E',SET 0,(IX+$61)           ; @BT DD CB 61 C3
        CPL                            ; @BT 2F
        LD H',SLL (IX+$27)             ; @BT DD CB 27 34
        BIT 4,L'                       ; @BT CB 65
        LD B',RES 2,(IX+$75)           ; @BT DD CB 75 90
        ADC A',D'                      ; @BT 8A
        LD B',RLC (IY+$1C)             ; @BT FD CB 1C 00
        LD B',IYH                      ; @BT FD 44
        LD ($EFF4),SP                  ; @BT ED 73 F4 EF
        LD L',RES 7,(IY+$66)           ; @BT FD CB 66 BD
        LD A',(DE')                    ; @BT 1A
        BSRF DE',B                     ; @BT ED 2B
        RLC (IY+$44)                   ; @BT FD CB 44 06
        BIT 6,L'                       ; @BT CB 75
        LD B',SET 6,(IY+$58)           ; @BT FD CB 58 F0
        LD C',RES 6,(IY+$20)           ; @BT FD CB 20 B1
        SL1 (HL')                      ; @BT CB 36
        LD A',R                        ; @BT ED 5F
        RST                            ; @BT C7
        SRA (IY+$39)                   ; @BT FD CB 39 2E
        LD C',A'                       ; @BT 4F
        RES 5,B'                       ; @BT CB A8
        LD D',RLC (IX+$39)             ; @BT DD CB 39 02
        LD IXL,A'                      ; @BT DD 6F
        DEC (IX+$5C)                   ; @BT DD 35 5C
        RRCA                           ; @BT 0F
        LD IYH,A'                      ; @BT FD 67
        SET 2,(HL')                    ; @BT CB D6
        LD E',RES 2,(IX+$25)           ; @BT DD CB 25 93
        LD B',(HL')                    ; @BT 46
        LD B',A'                       ; @BT 47
        LD H',A'                       ; @BT 67
        EI                             ; @BT FB
        LD C',SET 6,(IY+$68)           ; @BT FD CB 68 F1
        SUB A',B'                      ; @BT 90
        LD L',RLC (IY+$6C)             ; @BT FD CB 6C 05
        SBC A',$A1                     ; @BT DE A1
        LD B',RRC (IY+$28)             ; @BT FD CB 28 08
        DEC C'                         ; @BT 0D
        BIT 7,B'                       ; @BT CB 78
        LD H',RES 4,(IX+$41)           ; @BT DD CB 41 A4
        LD R,A'                        ; @BT ED 4F
        RES 1,A'                       ; @BT CB 8F
        LD H',RES 5,(IY+$4A)           ; @BT FD CB 4A AC
        LD H',RL (IY+$62)              ; @BT FD CB 62 14
        LD B',IXH                      ; @BT DD 44
        SET 1,C'                       ; @BT CB C9
        SET 0,(IX+$0A)                 ; @BT DD CB 0A C6
        DEC L'                         ; @BT 2D
        RES 2,(HL')                    ; @BT CB 96
        ADC A',IYL                     ; @BT FD 8D
        JR LSS,$9B                     ; @BT 38 9B
        RES 3,(IX+$6C)                 ; @BT DD CB 6C 9E
        LD H',SRA (IX+$2D)             ; @BT DD CB 2D 2C
        CP A',(IX+$11)                 ; @BT DD BE 11
        LD H',RLC (IX+$59)             ; @BT DD CB 59 04
        LD H',RES 0,(IX+$29)           ; @BT DD CB 29 84
        LD C',SET 5,(IY+$62)           ; @BT FD CB 62 E9
        OUTINB                         ; @BT ED 90
        LD E',SRL (IX+$23)             ; @BT DD CB 23 3B
        LD IX,($B43C)                  ; @BT DD 2A 3C B4
        ADD A',IYH                     ; @BT FD 84
        RES 2,D'                       ; @BT CB 92
        LD H',RES 4,(IY+$44)           ; @BT FD CB 44 A4
        OTDR                           ; @BT ED BB
        LD E',SET 2,(IY+$70)           ; @BT FD CB 70 D3
        LD D',RRC (IY+$48)             ; @BT FD CB 48 0A
        LD D',IYH                      ; @BT FD 54
        LD C',SLA (IY+$74)             ; @BT FD CB 74 21
        EX (SP),IX                     ; @BT DD E3
        LD L',RES 4,(IX+$51)           ; @BT DD CB 51 A5
        LD C',SET 7,(IY+$6E)           ; @BT FD CB 6E F9
        SBC A',IXL                     ; @BT DD 9D
        RLC D'                         ; @BT CB 02
        LD E',RES 6,(IY+$40)           ; @BT FD CB 40 B3
        NEXTREG $DA,A'                 ; @BT ED 92 DA
        LD L',SRA (IX+$3D)             ; @BT DD CB 3D 2D
        LD E',(HL')                    ; @BT 5E
        OR A',D'                       ; @BT B2
        LD D',RL (IX+$3F)              ; @BT DD CB 3F 12
        RES 4,L'                       ; @BT CB A5
        LD C',SET 3,(IX+$53)           ; @BT DD CB 53 D9
        LD H',RL (IX+$5F)              ; @BT DD CB 5F 14
        INC E'                         ; @BT 1C
        BSRA DE',B                     ; @BT ED 29
        BSLA DE',B                     ; @BT ED 28
        LD L',SET 4,(IY+$1C)           ; @BT FD CB 1C E5
        LD L',RR (IX+$7B)              ; @BT DD CB 7B 1D
        LD E',IXL                      ; @BT DD 5D
        ADD IY,SP                      ; @BT FD 39
        LD A',A'                       ; @BT 7F
        CP A',$E1                      ; @BT FE E1
        LD H',SET 0,(IX+$71)           ; @BT DD CB 71 C4
        SET 0,C'                       ; @BT CB C1
        PUSH IY                        ; @BT FD E5
        LD A',RES 3,(IY+$6E)           ; @BT FD CB 6E 9F
        ADD HL',$400F                  ; @BT ED 34 0F 40
        LD IYH,$6E                     ; @BT FD 26 6E
        LD C',RES 4,(IX+$11)           ; @BT DD CB 11 A1
        LD L',RRC (IY+$78)             ; @BT FD CB 78 0D
        LD C',(IY+$47)                 ; @BT FD 4E 47
        LD E',SET 1,(IY+$6A)           ; @BT FD CB 6A CB
        LD E',SLL (IY+$1A)             ; @BT FD CB 1A 33
        RES 4,(IY+$0D)                 ; @BT FD CB 0D A6
        LD A',IYL                      ; @BT FD 7D
        LD IYH,C'                      ; @BT FD 61
        LD H',RES 0,(IY+$2C)           ; @BT FD CB 2C 84
        BIT 2,(IY+$2D)                 ; @BT FD CB 2D 56
        LD A',SET 7,(IY+$4E)           ; @BT FD CB 4E FF
        SET 1,A'                       ; @BT CB CF
        JP PO,$2778                    ; @BT E2 78 27
        BIT 3,L'                       ; @BT CB 5D
        XOR A',L'                      ; @BT AD
        DEC IXH                        ; @BT DD 25
        LD A',RES 2,(IY+$68)           ; @BT FD CB 68 97
        BIT 2,C'                       ; @BT CB 51
        DEC E'                         ; @BT 1D
        BIT 5,(IY+$07)                 ; @BT FD CB 07 6E
        LD E',RLC (IX+$49)             ; @BT DD CB 49 03
        OR A',IYL                      ; @BT FD B5
        LD A',RRC (IY+$18)             ; @BT FD CB 18 0F
        LD A',IXH                      ; @BT DD 7C
        ADD A',B'                      ; @BT 80
        LD SP,IY                       ; @BT FD F9
        HALT                           ; @BT 76
        RES 0,D'                       ; @BT CB 82
        LD H',SET 2,(IX+$7D)           ; @BT DD CB 7D D4
        RES 4,C'                       ; @BT CB A1
        LD L',SET 2,(IX+$0D)           ; @BT DD CB 0D D5
        LD C',RES 6,(IX+$1D)           ; @BT DD CB 1D B1
        SET 1,L'                       ; @BT CB CD
        LD C',IYL                      ; @BT FD 4D
        LD H',RR (IY+$6E)              ; @BT FD CB 6E 1C
        SBC A',IYL                     ; @BT FD 9D
        LD A',SLA (IY+$54)             ; @BT FD CB 54 27
        CALL PO,$BBC3                  ; @BT E4 C3 BB
        LD H',SET 1,(IX+$77)           ; @BT DD CB 77 CC
        BIT 3,D'                       ; @BT CB 5A
        TEST $7A                       ; @BT ED 27 7A
        LD C',RR (IX+$3B)              ; @BT DD CB 3B 19
        BIT 4,C'                       ; @BT CB 61
        DEC HL'                        ; @BT 2B
        RES 1,H'                       ; @BT CB 8C
        RES 7,(IX+$64)                 ; @BT DD CB 64 BE
        BIT 2,B'                       ; @BT CB 50
        IM 1                           ; @BT ED 56
        LD D',RES 2,(IY+$18)           ; @BT FD CB 18 92
        ADC A',C'                      ; @BT 89
        LD C',SET 4,(IY+$5C)           ; @BT FD CB 5C E1
        LD L',RES 7,(IX+$63)           ; @BT DD CB 63 BD
        RES 6,C'                       ; @BT CB B1
        RES 4,B'                       ; @BT CB A0
        RES 1,C'                       ; @BT CB 89
        CPI                            ; @BT ED A1
        ADC A',E'                      ; @BT 8B
        SET 2,(IY+$49)                 ; @BT FD CB 49 D6
        AND A',(HL')                   ; @BT A6
        LD H',SLL (IY+$2A)             ; @BT FD CB 2A 34
        SET 6,A'                       ; @BT CB F7
        ADC A',(IX+$2B)                ; @BT DD 8E 2B
        RES 6,H'                       ; @BT CB B4
        PUSH $9862                     ; @BT ED 8A 98 62
        RR D'                          ; @BT CB 1A
        LD L',$91                      ; @BT 2E 91
        ADD HL',DE'                    ; @BT 19
        IN E',(C')                     ; @BT ED 58
        LD C',SET 7,(IX+$6B)           ; @BT DD CB 6B F9
        LD B',SET 2,(IY+$40)           ; @BT FD CB 40 D0
        BIT 2,H'                       ; @BT CB 54
        RES 1,E'                       ; @BT CB 8B
        ADD A',H'                      ; @BT 84
        ADD HL',HL'                    ; @BT 29
        BIT 7,D'                       ; @BT CB 7A
        SBC A',(IX+$51)                ; @BT DD 9E 51
        RES 0,B'                       ; @BT CB 80
        RR (IX+$68)                    ; @BT DD CB 68 1E
        LD B',C'                       ; @BT 41
        LD L',RES 5,(IY+$5A)           ; @BT FD CB 5A AD
        ADD A',E'                      ; @BT 83
        EX (SP),HL'                    ; @BT E3
        LD ($F105),IY                  ; @BT FD 22 05 F1
        RES 2,E'                       ; @BT CB 93
        LD C',SLL (IX+$77)             ; @BT DD CB 77 31
        LD L',SET 0,(IY+$04)           ; @BT FD CB 04 C5
        LD L',SET 6,(IY+$28)           ; @BT FD CB 28 F5
        LD B',SET 1,(IY+$3A)           ; @BT FD CB 3A C8
        DEC (IY+$68)                   ; @BT FD 35 68
        LD D',RRC (IX+$45)             ; @BT DD CB 45 0A
        LD B',SRA (IX+$6D)             ; @BT DD CB 6D 28
        INC HL'                        ; @BT 23
        LDIRX                          ; @BT ED B4
        LD E',SET 7,(IY+$0E)           ; @BT FD CB 0E FB
        LD IY,$D107                    ; @BT FD 21 07 D1
        RES 2,L'                       ; @BT CB 95
        LD IXL,$D0                     ; @BT DD 2E D0
        LD C',SRA (IX+$7D)             ; @BT DD CB 7D 29
        LD H',RES 2,(IX+$35)           ; @BT DD CB 35 94
        SBC A',C'                      ; @BT 99
        LD E',SRL (IY+$26)             ; @BT FD CB 26 3B
        LD E',SLA (IX+$11)             ; @BT DD CB 11 23
        CP A',E'                       ; @BT BB
        LD C',$BA                      ; @BT 0E BA
        OR A',B'                       ; @BT B0
        OR A',A'                       ; @BT B7
        CALL NEG,$B55D                 ; @BT FC 5D B5
        LD L',RL (IY+$72)              ; @BT FD CB 72 15
        LD E',RES 4,(IX+$31)           ; @BT DD CB 31 A3
        RST $18                        ; @BT DF
        SET 1,D'                       ; @BT CB CA
        NEG                            ; @BT ED 44
        RRC L'                         ; @BT CB 0D
        LD B',RES 3,(IX+$7B)           ; @BT DD CB 7B 98
        LD B',RES 6,(IY+$10)           ; @BT FD CB 10 B0
        LD E',SRA (IX+$1D)             ; @BT DD CB 1D 2B
        DEC DE'                        ; @BT 1B
        LD H',(IY+$4B)                 ; @BT FD 66 4B
        INC D'                         ; @BT 14
        SBC A',D'                      ; @BT 9A
        AND A',L'                      ; @BT A5
        LD H',SET 4,(IX+$09)           ; @BT DD CB 09 E4
        LD B',RES 4,(IX+$01)           ; @BT DD CB 01 A0
        LD H',RES 6,(IX+$4D)           ; @BT DD CB 4D B4
        BIT 4,(IY+$69)                 ; @BT FD CB 69 66
        RES 3,B'                       ; @BT CB 98
        RET PE                         ; @BT E8
        LD D',RES 7,(IY+$36)           ; @BT FD CB 36 BA
        CALL NEQ,$B77F                 ; @BT C4 7F B7
        LD A',RES 0,(IY+$5C)           ; @BT FD CB 5C 87
        LD H',B'                       ; @BT 60
        LD L',RR (IY+$7E)              ; @BT FD CB 7E 1D
        SBC HL',HL'                    ; @BT ED 62
        SET 4,(HL')                    ; @BT CB E6
        BIT 2,A'                       ; @BT CB 57
        DEC B'                         ; @BT 05
        LD L',H'                       ; @BT 6C
        SLA D'                         ; @BT CB 22
        LD IYH,IYH                     ; @BT FD 64
        LD IXL,C'                      ; @BT DD 69
        SLA A'                         ; @BT CB 27
        AND A',B'                      ; @BT A0
        BIT 1,(IX+$0C)                 ; @BT DD CB 0C 4E
        RES 7,A'                       ; @BT CB BF
        LD L',(IY+$0A)                 ; @BT FD 6E 0A
        CP A',B'                       ; @BT B8
        AND A',IXH                     ; @BT DD A4
        LD C',RLC (IX+$29)             ; @BT DD CB 29 01
        OR A',$78                      ; @BT F6 78
        IM 0                           ; @BT ED 46
        RST $10                        ; @BT D7
        LD E',RES 0,(IY+$1C)           ; @BT FD CB 1C 83
        LD D',(HL')                    ; @BT 56
        DJNZ $65                       ; @BT 10 65
        LD E',SLL (IX+$17)             ; @BT DD CB 17 33
        LD E',RES 5,(IX+$37)           ; @BT DD CB 37 AB
        RES 7,L'                       ; @BT CB BD
        BIT 1,E'                       ; @BT CB 4B
        BIT 0,(IY+$71)                 ; @BT FD CB 71 46
        LD A',RES 2,(IX+$65)           ; @BT DD CB 65 97
        SBC A',(HL')                   ; @BT 9E
        BIT 6,E'                       ; @BT CB 73
        POP BC'                        ; @BT C1
        BIT 1,H'                       ; @BT CB 4C
        ADD IX,SP                      ; @BT DD 39
        LD B',SET 2,(IX+$3D)           ; @BT DD CB 3D D0
        LD IXL,IXH                     ; @BT DD 6C
        PUSH BC'                       ; @BT C5
        SLA (IX+$18)                   ; @BT DD CB 18 26
        RES 0,H'                       ; @BT CB 84
        SUB A',IXH                     ; @BT DD 94
        SET 6,(IY+$41)                 ; @BT FD CB 41 F6
        SLA (IY+$1B)                   ; @BT FD CB 1B 26
        SET 0,(IY+$0D)                 ; @BT FD CB 0D C6
        LD B',RES 0,(IY+$6C)           ; @BT FD CB 6C 80
        AND A',$FF                     ; @BT E6 FF
        SET 3,(IX+$64)                 ; @BT DD CB 64 DE
        RES 7,H'                       ; @BT CB BC
        LD (HL'),H'                    ; @BT 74
        SET 2,(IX+$46)                 ; @BT DD CB 46 D6
        INI                            ; @BT ED A2
        LD L',SET 7,(IX+$2B)           ; @BT DD CB 2B FD
        LD D',B'                       ; @BT 50
        SET 7,E'                       ; @BT CB FB
        INC DE'                        ; @BT 13
        SRA E'                         ; @BT CB 2B
        LD IXL,B'                      ; @BT DD 68
        LD (IY+$32),C'                 ; @BT FD 71 32
        LD E',SET 4,(IX+$79)           ; @BT DD CB 79 E3
        LD C',C'                       ; @BT 49
        RES 2,(IX+$4E)                 ; @BT DD CB 4E 96
        RES 4,(HL')                    ; @BT CB A6
        XOR A',H'                      ; @BT AC
        SET 1,(HL')                    ; @BT CB CE
        LD A',IYH                      ; @BT FD 7C
        LD C',RRC (IX+$35)             ; @BT DD CB 35 09
        OUT (C'),D'                    ; @BT ED 51
        SET 7,C'                       ; @BT CB F9
        LD C',RES 5,(IX+$17)           ; @BT DD CB 17 A9
        LD (IX+$6D),H'                 ; @BT DD 74 6D
        LD D',SET 1,(IX+$57)           ; @BT DD CB 57 CA
        ADD IY,DE'                     ; @BT FD 19
        LD H',E'                       ; @BT 63
        BIT 5,B'                       ; @BT CB 68
        LD D',SRL (IX+$13)             ; @BT DD CB 13 3A
        LD C',SRA (IY+$00)             ; @BT FD CB 00 29
        IN F',(C')                     ; @BT ED 70
        LD A',B'                       ; @BT 78
        RES 3,A'                       ; @BT CB 9F
        IM 2                           ; @BT ED 5E
        LD C',(IX+$3E)                 ; @BT DD 4E 3E
        LD L',SRL (IX+$43)             ; @BT DD CB 43 3D
        MUL D',E                       ; @BT ED 30
        RES 4,H'                       ; @BT CB A4
        LD B',SET 7,(IX+$5B)           ; @BT DD CB 5B F8
        RES 6,(HL')                    ; @BT CB B6
        SRA A'                         ; @BT CB 2F
        LD D',RES 1,(IY+$12)           ; @BT FD CB 12 8A
        LD L',RES 3,(IY+$4E)           ; @BT FD CB 4E 9D
        SL1 (IX+$7A)                   ; @BT DD CB 7A 36
        ADD BC,A'                      ; @BT ED 33
        OR A',(HL')                    ; @BT B6
        RES 6,D'                       ; @BT CB B2
        LD (IX+$67),D'                 ; @BT DD 72 67
        LD C',RES 0,(IY+$7C)           ; @BT FD CB 7C 81
        BIT 1,D'                       ; @BT CB 4A
        RET LSS                        ; @BT D8
        LD L',RLC (IX+$69)             ; @BT DD CB 69 05
        LD ($EEE3),HL'                 ; @BT 22 E3 EE
        BIT 5,C'                       ; @BT CB 69
        SET 0,H'                       ; @BT CB C4
        LD IXH,C'                      ; @BT DD 61
        RES 1,L'                       ; @BT CB 8D
        BIT 4,H'                       ; @BT CB 64
        RES 6,L'                       ; @BT CB B5
        RST $20                        ; @BT E7
        LDI                            ; @BT ED A0
        LD IY,($CFF6)                  ; @BT FD 2A F6 CF
        LD E',SRA (IY+$20)             ; @BT FD CB 20 2B
        ADC A',IXH                     ; @BT DD 8C
        LD H',SRA (IY+$30)             ; @BT FD CB 30 2C
        LD IXH,A'                      ; @BT DD 67
        SRA B'                         ; @BT CB 28
        RES 5,(IX+$28)                 ; @BT DD CB 28 AE
        RRC C'                         ; @BT CB 09
        BIT 1,A'                       ; @BT CB 4F
        SUB A',L'                      ; @BT 95
        LD C',RR (IY+$3E)              ; @BT FD CB 3E 19
        LD H',SET 5,(IX+$0F)           ; @BT DD CB 0F EC
        SLA E'                         ; @BT CB 23
        AND A',C'                      ; @BT A1
        SUB A',IYH                     ; @BT FD 94
        RLCA                           ; @BT 07
        SET 5,(IY+$23)                 ; @BT FD CB 23 EE
        LD L',RES 3,(IX+$4B)           ; @BT DD CB 4B 9D
        LD A',RES 1,(IY+$62)           ; @BT FD CB 62 8F
        LD D',SET 2,(IY+$60)           ; @BT FD CB 60 D2
        LD H',D'                       ; @BT 62
        LD A',RLC (IY+$0C)             ; @BT FD CB 0C 07
        OUT (C'),H'                    ; @BT ED 61
        SL1 B'                         ; @BT CB 30
        LD A',(HL')                    ; @BT 7E
        LD IXH,$18                     ; @BT DD 26 18
        RES 7,E'                       ; @BT CB BB
        LD B',RL (IX+$1F)              ; @BT DD CB 1F 10
        SET 2,E'                       ; @BT CB D3
        BIT 0,(HL')                    ; @BT CB 46
        LD A',H'                       ; @BT 7C
        LD ($ECC1),BC'                 ; @BT ED 43 C1 EC
        DEC IYL                        ; @BT FD 2D
        LD B',RES 5,(IX+$07)           ; @BT DD CB 07 A8
        LD A',RES 4,(IY+$74)           ; @BT FD CB 74 A7
        SET 2,D'                       ; @BT CB D2
        RRA                            ; @BT 1F
        LD L',SLL (IY+$3A)             ; @BT FD CB 3A 35
        AND A',IYL                     ; @BT FD A5
        SRA (HL')                      ; @BT CB 2E
        LD (IX+$61),B'                 ; @BT DD 70 61
        LD H',RR (IX+$6B)              ; @BT DD CB 6B 1C
        JP LSS,$2001                   ; @BT DA 01 20
        BIT 4,B'                       ; @BT CB 60
        OTIR                           ; @BT ED B3
        RLC A'                         ; @BT CB 07
        BIT 3,(IX+$48)                 ; @BT DD CB 48 5E
        BIT 7,A'                       ; @BT CB 7F
        INC IX                         ; @BT DD 23
        SET 2,H'                       ; @BT CB D4
        LD H',SET 7,(IX+$1B)           ; @BT DD CB 1B FC
        LD H',RLC (IY+$5C)             ; @BT FD CB 5C 04
        AND A',H'                      ; @BT A4
        SET 6,C'                       ; @BT CB F1
        LD SP,HL'                      ; @BT F9
        SBC A',B'                      ; @BT 98
        LD A',SLL (IX+$57)             ; @BT DD CB 57 37
        LD D',D'                       ; @BT 52
        OR A',IXH                      ; @BT DD B4
        LD IYL,A'                      ; @BT FD 6F
        LD A',RES 7,(IX+$03)           ; @BT DD CB 03 BF
        SLA (HL')                      ; @BT CB 26
        LD (IX+$70),L'                 ; @BT DD 75 70
        RLC B'                         ; @BT CB 00
        CP A',L'                       ; @BT BD
        BIT 0,A'                       ; @BT CB 47
        LD D',SRL (IY+$16)             ; @BT FD CB 16 3A
        CP A',A'                       ; @BT BF
        RES 5,L'                       ; @BT CB AD
        LD L',RES 2,(IX+$45)           ; @BT DD CB 45 95
        LD E',RLC (IY+$4C)             ; @BT FD CB 4C 03
        LD IYL,IYH                     ; @BT FD 6C
        LD B',SET 3,(IX+$43)           ; @BT DD CB 43 D8
        LD D',(IX+$74)                 ; @BT DD 56 74
        INC (IX+$5A)                   ; @BT DD 34 5A
        RR B'                          ; @BT CB 18
        LD B',SET 0,(IY+$34)           ; @BT FD CB 34 C0
        ADD A',L'                      ; @BT 85
        BIT 7,(IY+$43)                 ; @BT FD CB 43 7E
        LD B',(IY+$1A)                 ; @BT FD 46 1A
        LD L',SET 3,(IY+$16)           ; @BT FD CB 16 DD
        ADD IY,BC'                     ; @BT FD 09
        BIT 6,(HL')                    ; @BT CB 76
        LD C',RES 2,(IY+$08)           ; @BT FD CB 08 91
        LD E',SET 2,(IX+$6D)           ; @BT DD CB 6D D3
        LD H',(IX+$48)                 ; @BT DD 66 48
        LD L',SET 1,(IY+$0A)           ; @BT FD CB 0A CD
        BIT 0,B'                       ; @BT CB 40
        SET 0,D'                       ; @BT CB C2
        SET 2,C'                       ; @BT CB D1
        LD C',SET 2,(IX+$4D)           ; @BT DD CB 4D D1
        SET 4,D'                       ; @BT CB E2
        CALL $BCD4                     ; @BT CD D4 BC
        LD (HL'),D'                    ; @BT 72
        BIT 3,B'                       ; @BT CB 58
        BIT 3,(IY+$4B)                 ; @BT FD CB 4B 5E
        LD C',RL (IY+$32)              ; @BT FD CB 32 11
        SRL L'                         ; @BT CB 3D
        LD H',RES 3,(IX+$3B)           ; @BT DD CB 3B 9C
        ADD HL,A'                      ; @BT ED 31
        RRC (IY+$0C)                   ; @BT FD CB 0C 0E
        LD D',RES 0,(IY+$0C)           ; @BT FD CB 0C 82
        BIT 5,L'                       ; @BT CB 6D
        LD H',SET 5,(IY+$12)           ; @BT FD CB 12 EC
        LDD                            ; @BT ED A8
        LD E',A'                       ; @BT 5F
        LD D',SET 2,(IX+$5D)           ; @BT DD CB 5D D2
        LD A',RES 6,(IX+$7D)           ; @BT DD CB 7D B7
        LD E',RRC (IX+$55)             ; @BT DD CB 55 0B
        RRC (HL')                      ; @BT CB 0E
        BIT 1,(IY+$0F)                 ; @BT FD CB 0F 4E
        LD C',RL (IX+$2F)              ; @BT DD CB 2F 11
        RRC E'                         ; @BT CB 0B
        LD H',RES 6,(IY+$50)           ; @BT FD CB 50 B4
        SET 5,(IX+$20)                 ; @BT DD CB 20 EE
        LD B',L'                       ; @BT 45
        RR E'                          ; @BT CB 1B
        RST $28                        ; @BT EF
        LD A',SET 5,(IX+$3F)           ; @BT DD CB 3F EF
        SET 4,C'                       ; @BT CB E1
        SUB A',C'                      ; @BT 91
        LD L',(HL')                    ; @BT 6E
        LD A',RR (IY+$1E)              ; @BT FD CB 1E 1F
        XOR A',A'                      ; @BT AF
        LD L',SET 0,(IX+$01)           ; @BT DD CB 01 C5
        BIT 0,H'                       ; @BT CB 44
        SET 3,A'                       ; @BT CB DF
        SET 3,(IY+$67)                 ; @BT FD CB 67 DE
        LD D',SET 1,(IY+$5A)           ; @BT FD CB 5A CA
        LD C',RES 1,(IX+$7F)           ; @BT DD CB 7F 89
        LD L',SET 5,(IY+$22)           ; @BT FD CB 22 ED
        LD D',RES 2,(IX+$15)           ; @BT DD CB 15 92
        LD D',IXL                      ; @BT DD 55
        OUT (C'),L'                    ; @BT ED 69
        LD E',D'                       ; @BT 5A
        LD (IY+$41),$DE                ; @BT FD 36 41 DE
        SET 7,D'                       ; @BT CB FA
        LD H',RES 3,(IY+$3E)           ; @BT FD CB 3E 9C
        SBC A',L'                      ; @BT 9D
        LD BC',$4887                   ; @BT 01 87 48
        LD A',RES 5,(IY+$7A)           ; @BT FD CB 7A AF
        LD A',SLA (IX+$51)             ; @BT DD CB 51 27
        LD E',RL (IY+$52)              ; @BT FD CB 52 13
        BIT 0,D'                       ; @BT CB 42
        LD C',RRC (IY+$38)             ; @BT FD CB 38 09
        SRA (IX+$36)                   ; @BT DD CB 36 2E
        RL L'                          ; @BT CB 15
        SRL C'                         ; @BT CB 39
        SET 7,H'                       ; @BT CB FC
        LD L',SLL (IX+$37)             ; @BT DD CB 37 35
        BIT 6,D'                       ; @BT CB 72
        LD E',(IY+$2A)                 ; @BT FD 5E 2A
        LD E',L'                       ; @BT 5D
        RES 7,(IY+$67)                 ; @BT FD CB 67 BE
        LD E',IXH                      ; @BT DD 5C
        LD A',SET 4,(IX+$39)           ; @BT DD CB 39 E7
        LD IYL,$26                     ; @BT FD 2E 26
        RES 2,H'                       ; @BT CB 94
        LD B',RR (IX+$2B)              ; @BT DD CB 2B 18
        AND A',(IY+$50)                ; @BT FD A6 50
        LD B',RES 0,(IX+$69)           ; @BT DD CB 69 80
        LD C',D'                       ; @BT 4A
        INC IXH                        ; @BT DD 24
        SET 5,D'                       ; @BT CB EA
        DEC IYH                        ; @BT FD 25
        LD H',(HL')                    ; @BT 66
        SET 0,A'                       ; @BT CB C7
        LD L',(IX+$07)                 ; @BT DD 6E 07
        SUB A',(HL')                   ; @BT 96
        ADD HL',BC'                    ; @BT 09
        LD (IY+$3E),L'                 ; @BT FD 75 3E
        BIT 2,D'                       ; @BT CB 52
        LD L',A'                       ; @BT 6F
        LD (IY+$2C),A'                 ; @BT FD 77 2C
        BIT 7,C'                       ; @BT CB 79
        LD E',H'                       ; @BT 5C
        LD A',SRA (IX+$5D)             ; @BT DD CB 5D 2F
        LD B',SET 7,(IY+$5E)           ; @BT FD CB 5E F8
        CP A',C'                       ; @BT B9
        RES 3,(IY+$6F)                 ; @BT FD CB 6F 9E
        POP IX                         ; @BT DD E1
        INC IXL                        ; @BT DD 2C
        LD H',SRL (IY+$36)             ; @BT FD CB 36 3C
        LD IYH,IYL                     ; @BT FD 65
        SCF                            ; @BT 37
        LD A',RES 6,(IY+$00)           ; @BT FD CB 00 B7
        SET 6,H'                       ; @BT CB F4
        BIT 2,(IX+$2A)                 ; @BT DD CB 2A 56
        SRL (IX+$54)                   ; @BT DD CB 54 3E
        BIT 5,(IX+$04)                 ; @BT DD CB 04 6E
        CP A',H'                       ; @BT BC
        LD D',SRA (IX+$0D)             ; @BT DD CB 0D 2A
        SETAE                          ; @BT ED 95
        LD C',SET 1,(IX+$47)           ; @BT DD CB 47 C9
        LD IXH,E'                      ; @BT DD 63
        LD D',SET 4,(IY+$6C)           ; @BT FD CB 6C E2
        XOR A',IXH                     ; @BT DD AC
        LD E',RES 5,(IY+$3A)           ; @BT FD CB 3A AB
        DEC IXL                        ; @BT DD 2D
        SET 3,D'                       ; @BT CB DA
        JP (IY)                        ; @BT FD E9
        SET 3,H'                       ; @BT CB DC
        RES 4,(IX+$0A)                 ; @BT DD CB 0A A6
        NOP                            ; @BT 00
        LD L',SLA (IX+$31)             ; @BT DD CB 31 25
        LD A',RL (IY+$12)              ; @BT FD CB 12 17
        LD A',(IY+$5E)                 ; @BT FD 7E 5E
        OUT (C'),B'                    ; @BT ED 41
        LD C',SET 0,(IX+$41)           ; @BT DD CB 41 C1
        LD C',RES 5,(IY+$1A)           ; @BT FD CB 1A A9
        LD C',RES 7,(IX+$23)           ; @BT DD CB 23 B9
        SUB A',D'                      ; @BT 92
        SRL D'                         ; @BT CB 3A
        SET 7,(HL')                    ; @BT CB FE
        LD E',RES 0,(IX+$19)           ; @BT DD CB 19 83
        LD C',L'                       ; @BT 4D
        JP PE,$2667                    ; @BT EA 67 26
        RET EQU                        ; @BT C8
        LD SP,$B21A                    ; @BT 31 1A B2
        LD IXL,E'                      ; @BT DD 6B
        SBC A',IXH                     ; @BT DD 9C
        LD H',RES 7,(IY+$56)           ; @BT FD CB 56 BC
        LD H',RES 5,(IX+$47)           ; @BT DD CB 47 AC
        RLC (IX+$41)                   ; @BT DD CB 41 06
        SL1 A'                         ; @BT CB 37
        RRC B'                         ; @BT CB 08
        OUTI                           ; @BT ED A3
        LD H',H'                       ; @BT 64
        RES 7,C'                       ; @BT CB B9
        ADD A',IYL                     ; @BT FD 85
        RES 0,(IX+$12)                 ; @BT DD CB 12 86
        SET 7,L'                       ; @BT CB FD
        LD H',SLA (IY+$24)             ; @BT FD CB 24 24
        RES 2,B'                       ; @BT CB 90
        LD A',SET 0,(IY+$24)           ; @BT FD CB 24 C7
        LD A',SET 2,(IY+$30)           ; @BT FD CB 30 D7
        BIT 2,(HL')                    ; @BT CB 56
        LD H',RRC (IX+$65)             ; @BT DD CB 65 0C
        RES 0,(IY+$15)                 ; @BT FD CB 15 86
        LD L',B'                       ; @BT 68
        SBC A',IYH                     ; @BT FD 9C
        SET 7,A'                       ; @BT CB FF
        CP A',(IY+$1A)                 ; @BT FD BE 1A
        RL D'                          ; @BT CB 12
        RST $30                        ; @BT F7
        LD D',(IY+$7D)                 ; @BT FD 56 7D
        SRA L'                         ; @BT CB 2D
        ADC A',$53                     ; @BT CE 53
        EX AF',AF                      ; @BT 08
        BIT 4,A'                       ; @BT CB 67
        ADD DE',$39A9                  ; @BT ED 35 A9 39
        RES 1,B'                       ; @BT CB 88
        INC IY                         ; @BT FD 23
        LD D',SET 6,(IY+$78)           ; @BT FD CB 78 F2
        JR $F7                         ; @BT 18 F7
        LD (IX+$64),C'                 ; @BT DD 71 64
        BIT 4,(IX+$66)                 ; @BT DD CB 66 66
        LD E',RES 3,(IY+$2E)           ; @BT FD CB 2E 9B
        XOR A',IYH                     ; @BT FD AC
        LD A',RL (IX+$0F)              ; @BT DD CB 0F 17
        LD D',SET 4,(IX+$69)           ; @BT DD CB 69 E2
        LD E',SET 6,(IX+$05)           ; @BT DD CB 05 F3
        LD E',$B1                      ; @BT 1E B1
        AND A',IYH                     ; @BT FD A4
        BIT 0,(IX+$6E)                 ; @BT DD CB 6E 46
        LD E',RES 7,(IX+$43)           ; @BT DD CB 43 BB
        LD H',RES 1,(IY+$32)           ; @BT FD CB 32 8C
        LD D',SLA (IX+$01)             ; @BT DD CB 01 22
        LD E',SET 3,(IX+$73)           ; @BT DD CB 73 DB
        RES 3,L'                       ; @BT CB 9D
        OR A',IXL                      ; @BT DD B5
        LD A',SET 0,(IX+$21)           ; @BT DD CB 21 C7
        ADD A',IXL                     ; @BT DD 85
        RL B'                          ; @BT CB 10
        INC L'                         ; @BT 2C
        ADD BC',$3787                  ; @BT ED 36 87 37
        LD H',SET 2,(IY+$00)           ; @BT FD CB 00 D4
        LD L',RES 0,(IY+$3C)           ; @BT FD CB 3C 85
        SBC A',A'                      ; @BT 9F
        LD C',RES 2,(IX+$05)           ; @BT DD CB 05 91
        ADC HL',SP                     ; @BT ED 7A
        SUB A',H'                      ; @BT 94
        LD C',SET 5,(IX+$5F)           ; @BT DD CB 5F E9
        RR C'                          ; @BT CB 19
        DEC A'                         ; @BT 3D
        SET 4,B'                       ; @BT CB E0
        LD B',RES 2,(IY+$78)           ; @BT FD CB 78 90
        ADC A',H'                      ; @BT 8C
        OUT (C'),0                     ; @BT ED 71
        LD E',(IX+$21)                 ; @BT DD 5E 21
        BIT 2,L'                       ; @BT CB 55
        RLC C'                         ; @BT CB 01
        OUT (C'),C'                    ; @BT ED 49
        XOR A',B'                      ; @BT A8
        LD (HL'),$94                   ; @BT 36 94
        CP A',IYL                      ; @BT FD BD
        LD B',RRC (IX+$25)             ; @BT DD CB 25 08
        PUSH AF'                       ; @BT F5
        LD B',SRL (IY+$76)             ; @BT FD CB 76 38
        SET 5,L'                       ; @BT CB ED
        LD B',SLL (IX+$67)             ; @BT DD CB 67 30
        LD D',RES 3,(IX+$1B)           ; @BT DD CB 1B 9A
        LD B',IYL                      ; @BT FD 45
        LD D',SET 0,(IX+$51)           ; @BT DD CB 51 C2
        LDIR                           ; @BT ED B0
        INC IYL                        ; @BT FD 2C
        RES 6,(IX+$46)                 ; @BT DD CB 46 B6
        RES 5,E'                       ; @BT CB AB
        DEC BC'                        ; @BT 0B
        BIT 6,(IX+$22)                 ; @BT DD CB 22 76
        LDWS                           ; @BT ED A5
        LD A',RES 1,(IX+$5F)           ; @BT DD CB 5F 8F
        CPDR                           ; @BT ED B9
        JR NEQ,$C9                     ; @BT 20 C9
        LD C',SLA (IX+$71)             ; @BT DD CB 71 21
        OR A',C'                       ; @BT B1
        XOR A',D'                      ; @BT AA
        LD H',$AE                      ; @BT 26 AE
        RLC (IX+$41)                   ; @BT DD CB 41 06
        LD H',SET 1,(IY+$7A)           ; @BT FD CB 7A CC
        LD (HL'),C'                    ; @BT 71
        LD C',SET 0,(IY+$44)           ; @BT FD CB 44 C1
        EX (SP),IY                     ; @BT FD E3
        LD C',RLC (IY+$2C)             ; @BT FD CB 2C 01
        LD IYH,D'                      ; @BT FD 62
        ADC A',B'                      ; @BT 88
        CCF                            ; @BT 3F
        BIT 7,H'                       ; @BT CB 7C
        LD L',SET 3,(IX+$13)           ; @BT DD CB 13 DD
        LD E',RES 1,(IY+$22)           ; @BT FD CB 22 8B
        LD HL',($9660)                 ; @BT 2A 60 96
        LD C',SET 1,(IY+$4A)           ; @BT FD CB 4A C9
        SET 7,B'                       ; @BT CB F8
        JP POS,$2445                   ; @BT F2 45 24
        LD A',RES 0,(IX+$59)           ; @BT DD CB 59 87

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with full TEST-03 randomization + varied arithmetic expressions
    ; GENERATED: seed = 1408561867

            RES 5,(IY+$2B)             ; @BT FD CB 2B AE
            LD H',RES 5,(IX+$47)       ; @BT DD CB 47 AC
            SBC A',E'                  ; @BT 9B
            LD B',RES 7,(IX+$13)       ; @BT DD CB 13 B8
            CPI                        ; @BT ED A1
            LD ((($EEE3-4)+4)),HL'     ; @BT 22 E3 EE
            LD A',IXH                  ; @BT DD 7C
            RES 0,L'                   ; @BT CB 85
            RES 3,B'                   ; @BT CB 98
            XOR A',C'                  ; @BT A9
            LD A',SET 1,(IY+$2A)       ; @BT FD CB 2A CF
            BIT 3,E'                   ; @BT CB 5B
            JP LSS,((($2001-(8+1))+8)+1) ; @BT DA 01 20
            CP A',(IX+((($11-4)-(7-1))+(7-1)+4)) ; @BT DD BE 11
            LD C',RES 0,(IX+$79)       ; @BT DD CB 79 81
            RES 0,A'                   ; @BT CB 87
            DEC C'                     ; @BT 0D
            SUB A',(IY+((($7B-(11+9))+(11+9)))) ; @BT FD 96 7B
            INC IXH                    ; @BT DD 24
            BIT 6,L'                   ; @BT CB 75
            RRC E'                     ; @BT CB 0B
            LD A',RRC (IX+(($15-(5+8))+5+8)) ; @BT DD CB 15 0F
            CALL (($BCD4-6)+6)         ; @BT CD D4 BC
            LD A',RRC (IY+((($18-7)+(7-10))+10)) ; @BT FD CB 18 0F
            LD L',SRL (IY+(($46-4)+4)) ; @BT FD CB 46 3D
            LD L',RES 6,(IX+$5D)       ; @BT DD CB 5D B5
            LD IYH,C'                  ; @BT FD 61
            LD IYH,D'                  ; @BT FD 62
            LD C',RES 1,(IX+$7F)       ; @BT DD CB 7F 89
            LD L',SET 1,(IY+$0A)       ; @BT FD CB 0A CD
            RRC A'                     ; @BT CB 0F
            LD L',RLC (IX+((($69-(6+1))+6)+1)) ; @BT DD CB 69 05
            RES 3,(IY+$6F)             ; @BT FD CB 6F 9E
            LD H',B'                   ; @BT 60
            LD L',SET 1,(IX+$07)       ; @BT DD CB 07 CD
            LD L',SET 5,(IY+$22)       ; @BT FD CB 22 ED
            LD B',RES 0,(IX+$69)       ; @BT DD CB 69 80
            LD C',SET 6,(IX+$65)       ; @BT DD CB 65 F1
            LD IXL,B'                  ; @BT DD 68
            SL1 B'                     ; @BT CB 30
            LD A',SET 5,(IY+$42)       ; @BT FD CB 42 EF
            ADD A',IXH                 ; @BT DD 84
            LD L',SRA (IY+(($40-(9+4))+9+4)) ; @BT FD CB 40 2D
            XOR A',(IX+((($2B-5)+(5-9))+9)) ; @BT DD AE 2B
            BIT 5,L'                   ; @BT CB 6D
            RR E'                      ; @BT CB 1B
            SLA E'                     ; @BT CB 23
            LD (DE'),A'                ; @BT 12
            LDI                        ; @BT ED A0
            RRC D'                     ; @BT CB 0A
            LD D',SRA (IY+((($10-5)+6)-6+5)) ; @BT FD CB 10 2A
            INC (HL')                  ; @BT 34
            LD H',SLA (IX+((($21-11)-(8-1))+(8-1)+11)) ; @BT DD CB 21 24
            LD E',SET 0,(IY+$64)       ; @BT FD CB 64 C3
            ADC HL',HL'                ; @BT ED 6A
            LD B',SET 5,(IX+$4F)       ; @BT DD CB 4F E8
            RST ((($30-6)+4)-4+6)      ; @BT F7
            LD L',SET 7,(IX+$2B)       ; @BT DD CB 2B FD
            ADC A',D'                  ; @BT 8A
            BIT 6,B'                   ; @BT CB 70
            LD C',L'                   ; @BT 4D
            LD E',SET 1,(IX+$67)       ; @BT DD CB 67 CB
            LD A',SET 7,(IY+$4E)       ; @BT FD CB 4E FF
            ADD A',H'                  ; @BT 84
            LD L',RES 4,(IY+$54)       ; @BT FD CB 54 A5
            RES 3,(HL')                ; @BT CB 9E
            LD (((($EBB0-(5+5))+(5+5)))),A' ; @BT 32 B0 EB
            JP NEG,((($2112-9)+(9-4))+4) ; @BT FA 12 21
            LD IX,((($B54D-(9+5))+5)+9) ; @BT DD 2A 4D B5
            PUSH AF'                   ; @BT F5
            BIT 5,D'                   ; @BT CB 6A
            POP BC'                    ; @BT C1
            LD (IY+((($3E-9)-5)+9+5)),L' ; @BT FD 75 3E
            LD A',SET 0,(IY+$24)       ; @BT FD CB 24 C7
            RES 1,A'                   ; @BT CB 8F
            RETI                       ; @BT ED 4D
            LD IYL,A'                  ; @BT FD 6F
            LD L',(HL')                ; @BT 6E
            LD E',RES 1,(IY+$22)       ; @BT FD CB 22 8B
            OUTD                       ; @BT ED AB
            SET 0,C'                   ; @BT CB C1
            RES 5,D'                   ; @BT CB AA
            EX (SP),IX                 ; @BT DD E3
            SET 5,E'                   ; @BT CB EB
            ADC A',(HL')               ; @BT 8E
            BIT 3,H'                   ; @BT CB 5C
            LD A',SLA (IX+((($51-6)+(6-6))+6)) ; @BT DD CB 51 27
            LD (IY+((($38-(4+8))+(4+8)))),E' ; @BT FD 73 38
            JP (C')                    ; @BT ED 98
            SLA H'                     ; @BT CB 24
            SET 7,(IX+$5C)             ; @BT DD CB 5C FE
            LD R,A'                    ; @BT ED 4F
            BIT 5,(IX+$04)             ; @BT DD CB 04 6E
            RES 6,(IY+$49)             ; @BT FD CB 49 B6
            RST ((($10-4)+(4-8))+8)    ; @BT D7
            LD B',RLC (IX+((($19-9)+7)-7+9)) ; @BT DD CB 19 00
            BIT 3,B'                   ; @BT CB 58
            LD E',(IY+((($2A-(6+2))+(6+2)))) ; @BT FD 5E 2A
            XOR A',(IY+((($34-(1+1))+1)+1)) ; @BT FD AE 34
            DEC D'                     ; @BT 15
            RST (($08-(10+2))+10+2)    ; @BT CF
            SUB A',IXH                 ; @BT DD 94
            CPD                        ; @BT ED A9
            AND A',E'                  ; @BT A3
            LD B',L'                   ; @BT 45
            RRC L'                     ; @BT CB 0D
            OUT (((($75-9)+(9+7))-7)),A' ; @BT D3 75
            LD A',I                    ; @BT ED 57
            LD D',RES 7,(IX+$33)       ; @BT DD CB 33 BA
            RL H'                      ; @BT CB 14
            BIT 6,C'                   ; @BT CB 71
            SWAPNIB                    ; @BT ED 23
            BIT 7,(HL')                ; @BT CB 7E
            LD C',RLC (IX+((($29-(7+6))+6)+7)) ; @BT DD CB 29 01
            RR B'                      ; @BT CB 18
            OR A',IYL                  ; @BT FD B5
            SET 1,B'                   ; @BT CB C8
            LD H',SET 4,(IY+$0C)       ; @BT FD CB 0C E4
            INI                        ; @BT ED A2
            SET 7,H'                   ; @BT CB FC
            LD H',RL (IX+(($5F-(10+9))+10+9)) ; @BT DD CB 5F 14
            LD E',RES 6,(IX+$3D)       ; @BT DD CB 3D B3
            LD B',SRL (IY+((($76-(6+1))+6)+1)) ; @BT FD CB 76 38
            RES 3,A'                   ; @BT CB 9F
            LD C',SRL (IX+(($03-9)+9)) ; @BT DD CB 03 39
            LD (HL'),((($94-8)+7)-7+8) ; @BT 36 94
            AND A',(IY+(($50-(2+4))+2+4)) ; @BT FD A6 50
            BIT 0,E'                   ; @BT CB 43
            RES 0,C'                   ; @BT CB 81
            RLC (IY+(($44-(7+7))+7+7)) ; @BT FD CB 44 06
            LD A',A'                   ; @BT 7F
            OR A',IYH                  ; @BT FD B4
            LD B',SET 4,(IX+$49)       ; @BT DD CB 49 E0
            LD A',RR (IY+((($1E-10)-9)+10+9)) ; @BT FD CB 1E 1F
            LD E',RLC (IY+((($4C-5)+(5-9))+9)) ; @BT FD CB 4C 03
            DAA                        ; @BT 27
            LD A',RL (IX+((($0F-(3+1))+3)+1)) ; @BT DD CB 0F 17
            SRL B'                     ; @BT CB 38
            LD D',(IY+(($7D-(9+2))+9+2)) ; @BT FD 56 7D
            RES 6,L'                   ; @BT CB B5
            LD L',C'                   ; @BT 69
            LDD                        ; @BT ED A8
            LD E',SET 0,(IX+$61)       ; @BT DD CB 61 C3
            RES 5,B'                   ; @BT CB A8
            LD D',SRL (IY+(($16-(5+2))+5+2)) ; @BT FD CB 16 3A
            ADD A',((($08-3)+(3-4))+4) ; @BT C6 08
            LD B',H'                   ; @BT 44
            ADD BC',((($3787-7)+(7+5))-5) ; @BT ED 36 87 37
            LD L',SLA (IY+((($34-11)-(1-1))+(1-1)+11)) ; @BT FD CB 34 25
            LD B',RES 7,(IY+$16)       ; @BT FD CB 16 B8
            CP A',E'                   ; @BT BB
            LD B',SET 7,(IY+$5E)       ; @BT FD CB 5E F8
            LD A',SET 2,(IX+$2D)       ; @BT DD CB 2D D7
            INC L'                     ; @BT 2C
            RES 5,(IX+$28)             ; @BT DD CB 28 AE
            LDDX                       ; @BT ED AC
            JR EQU,((($E0-6)+(6-2))+2) ; @BT 28 E0
            SUB A',(IX+((($78-(9+6))+(9+6)))) ; @BT DD 96 78
            LD B',SET 2,(IY+$40)       ; @BT FD CB 40 D0
            SET 0,B'                   ; @BT CB C0
            LD E',RES 4,(IY+$34)       ; @BT FD CB 34 A3
            NEG                        ; @BT ED 44
            LD B',RES 3,(IY+$7E)       ; @BT FD CB 7E 98
            LD L',((($91-(5+1))+5)+1)  ; @BT 2E 91
            LD A',SRL (IX+(($63-(5+8))+5+8)) ; @BT DD CB 63 3F
            LD H',SET 4,(IX+$09)       ; @BT DD CB 09 E4
            LD C',SET 7,(IY+$6E)       ; @BT FD CB 6E F9
            ADC HL',DE'                ; @BT ED 5A
            LD D',SRL (IX+(($13-6)+6)) ; @BT DD CB 13 3A
            BIT 5,B'                   ; @BT CB 68
            LD H',SLA (IY+((($24-7)-4)+7+4)) ; @BT FD CB 24 24
            LD A',RES 7,(IY+$06)       ; @BT FD CB 06 BF
            LD L',SET 2,(IY+$10)       ; @BT FD CB 10 D5
            LD D',RR (IX+(($4B-(3+3))+3+3)) ; @BT DD CB 4B 1A
            LD C',E'                   ; @BT 4B
            LD C',IXH                  ; @BT DD 4C
            SET 1,D'                   ; @BT CB CA
            RES 1,C'                   ; @BT CB 89
            RES 7,C'                   ; @BT CB B9
            XOR A',((($D3-(9+8))+(9+8))) ; @BT EE D3
            LD L',RES 7,(IY+$66)       ; @BT FD CB 66 BD
            SET 2,(IX+$46)             ; @BT DD CB 46 D6
            RRA                        ; @BT 1F
            SUB A',D'                  ; @BT 92
            PUSH IX                    ; @BT DD E5
            SET 0,(IY+$0D)             ; @BT FD CB 0D C6
            SET 1,(IX+$28)             ; @BT DD CB 28 CE
            RES 3,(IX+$6C)             ; @BT DD CB 6C 9E
            RET POS                    ; @BT F0
            LD H',H'                   ; @BT 64
            LD D',SET 4,(IY+$6C)       ; @BT FD CB 6C E2
            CP A',IYH                  ; @BT FD BC
            SLA B'                     ; @BT CB 20
            OR A',L'                   ; @BT B5
            SET 5,D'                   ; @BT CB EA
            RES 2,L'                   ; @BT CB 95
            LD D',RLC (IX+((($39-(10+4))+4)+10)) ; @BT DD CB 39 02
            ADD IY,BC'                 ; @BT FD 09
            LD E',RES 5,(IY+$3A)       ; @BT FD CB 3A AB
            ADD A',C'                  ; @BT 81
            LD D',SLL (IY+((($0A-3)+(3+5))-5)) ; @BT FD CB 0A 32
            LD B',SET 1,(IY+$3A)       ; @BT FD CB 3A C8
            LD H',SET 6,(IY+$18)       ; @BT FD CB 18 F4
            LD E',RLC (IX+((($49-4)-(3-1))+(3-1)+4)) ; @BT DD CB 49 03
            BIT 7,H'                   ; @BT CB 7C
            LD B',B'                   ; @BT 40
            LD A',SRL (IY+((($66-11)+(11+2))-2)) ; @BT FD CB 66 3F
            LD (HL'),L'                ; @BT 75
            LD (((($ECC1-(5+7))+7)+5)),BC' ; @BT ED 43 C1 EC
            ADC A',A'                  ; @BT 8F
            BIT 1,L'                   ; @BT CB 4D
            LD L',SLA (IX+((($31-3)+9)-9+3)) ; @BT DD CB 31 25
            LD C',SET 4,(IY+$5C)       ; @BT FD CB 5C E1
            LD IXH,B'                  ; @BT DD 60
            LD IX,(((($B43C-8)+6)-6+8)) ; @BT DD 2A 3C B4
            RES 4,A'                   ; @BT CB A7
            LD B',SLA (IY+((($64-5)+2)-2+5)) ; @BT FD CB 64 20
            LD BC',(((($4776-(9+8))+8)+9)) ; @BT ED 4B 76 47
            BIT 0,(IY+$71)             ; @BT FD CB 71 46
            CPL                        ; @BT 2F
            LD E',SET 5,(IX+$7F)       ; @BT DD CB 7F EB
            XOR A',L'                  ; @BT AD
            JP (IY)                    ; @BT FD E9
            CALL PO,((($BBC3-7)-(1-1))+(1-1)+7) ; @BT E4 C3 BB
            SCF                        ; @BT 37
            CP A',L'                   ; @BT BD
            LD C',RES 2,(IX+$05)       ; @BT DD CB 05 91
            LD H',SET 5,(IY+$12)       ; @BT FD CB 12 EC
            RES 6,A'                   ; @BT CB B7
            LD H',RES 0,(IY+$2C)       ; @BT FD CB 2C 84
            LD H',RES 6,(IY+$50)       ; @BT FD CB 50 B4
            SRA E'                     ; @BT CB 2B
            ADC A',(IY+((($34-(11+3))+3)+11)) ; @BT FD 8E 34
            ADD BC,A'                  ; @BT ED 33
            LD L',RES 2,(IY+$48)       ; @BT FD CB 48 95
            SET 6,B'                   ; @BT CB F0
            LD C',RES 7,(IX+$23)       ; @BT DD CB 23 B9
            LD H',SLL (IX+((($27-6)-2)+6+2)) ; @BT DD CB 27 34
            DEC (HL')                  ; @BT 35
            LD H',SLL (IY+((($2A-8)+(8-3))+3)) ; @BT FD CB 2A 34
            ADD HL',DE'                ; @BT 19
            LD C',RES 1,(IY+$02)       ; @BT FD CB 02 89
            LD L',SRA (IX+((($3D-(11+3))+(11+3)))) ; @BT DD CB 3D 2D
            ADC A',(IX+((($2B-9)+(9-2))+2)) ; @BT DD 8E 2B
            POP IX                     ; @BT DD E1
            LD E',IYH                  ; @BT FD 5C
            SET 0,H'                   ; @BT CB C4
            SRL D'                     ; @BT CB 3A
            BIT 5,E'                   ; @BT CB 6B
            LD L',RES 1,(IX+$3F)       ; @BT DD CB 3F 8D
            NEXTREG ((($DA-(4+3))+3)+4),A' ; @BT ED 92 DA
            SET 7,A'                   ; @BT CB FF
            EX AF',AF                  ; @BT 08
            LD A',((($C3-(3+9))+9)+3)  ; @BT 3A C3 00
            SLA (IY+((($1B-4)+(4-2))+2)) ; @BT FD CB 1B 26
            LD D',SET 2,(IX+$5D)       ; @BT DD CB 5D D2
            ADC A',H'                  ; @BT 8C
            LD A',IYL                  ; @BT FD 7D
            RES 3,H'                   ; @BT CB 9C
            LD E',IXL                  ; @BT DD 5D
            RES 6,D'                   ; @BT CB B2
            RES 1,H'                   ; @BT CB 8C
            LD A',SET 0,(IX+$21)       ; @BT DD CB 21 C7
            LD C',RL (IY+((($32-(4+1))+4)+1)) ; @BT FD CB 32 11
            LD (IY+((($2C-2)+(2-5))+5)),A' ; @BT FD 77 2C
            LD A',E'                   ; @BT 7B
            RET                        ; @BT C9
            SRA B'                     ; @BT CB 28
            RRC (HL')                  ; @BT CB 0E
            LD A',SET 6,(IX+$45)       ; @BT DD CB 45 F7
            BIT 0,C'                   ; @BT CB 41
            BIT 4,E'                   ; @BT CB 63
            ADD IY,SP                  ; @BT FD 39
            RLC B'                     ; @BT CB 00
            SET 2,L'                   ; @BT CB D5
            RRC B'                     ; @BT CB 08
            LD D',RRC (IY+((($48-4)+(4-3))+3)) ; @BT FD CB 48 0A
            JP ((($2889-(10+1))+10)+1) ; @BT C3 89 28
            DEC IY                     ; @BT FD 2B
            OUT (C'),L'                ; @BT ED 69
            SBC HL',HL'                ; @BT ED 62
            SET 7,(HL')                ; @BT CB FE
            SRL A'                     ; @BT CB 3F
            RES 5,(HL')                ; @BT CB AE
            LD IXH,IXL                 ; @BT DD 65
            SRA L'                     ; @BT CB 2D
            LD L',SET 3,(IX+$13)       ; @BT DD CB 13 DD
            SET 2,(HL')                ; @BT CB D6
            LD D',IXL                  ; @BT DD 55
            BIT 3,(IX+$48)             ; @BT DD CB 48 5E
            LD D',RES 5,(IY+$2A)       ; @BT FD CB 2A AA
            LD D',((($58-(7+1))+7)+1)  ; @BT 16 58
            LD D',E'                   ; @BT 53
            LD B',IYH                  ; @BT FD 44
            LD E',RES 3,(IX+$2B)       ; @BT DD CB 2B 9B
            RES 1,(IY+$33)             ; @BT FD CB 33 8E
            ADD A',(IY+((($6D-11)-(1-1))+(1-1)+11)) ; @BT FD 86 6D
            LD E',SET 2,(IY+$70)       ; @BT FD CB 70 D3
            XOR A',IXH                 ; @BT DD AC
            BIT 3,L'                   ; @BT CB 5D
            LD H',RES 4,(IY+$44)       ; @BT FD CB 44 A4
            LD E',RES 0,(IX+$19)       ; @BT DD CB 19 83
            RES 4,H'                   ; @BT CB A4
            LD C',RL (IX+((($2F-10)+(10+8))-8)) ; @BT DD CB 2F 11
            RES 2,C'                   ; @BT CB 91
            LD D',SLA (IY+((($04-10)+(10+7))-7)) ; @BT FD CB 04 22
            RES 2,A'                   ; @BT CB 97
            LD (HL'),E'                ; @BT 73
            LD C',SET 7,(IX+$6B)       ; @BT DD CB 6B F9
            LD SP,((($B21A-(2+1))+2)+1) ; @BT ED 7B 1A B2
            LD D',C'                   ; @BT 51
            ADC HL',BC'                ; @BT ED 4A
            LD B',SRA (IX+((($6D-5)+3)-3+5)) ; @BT DD CB 6D 28
            DEC (IY+(($68-(7+4))+7+4)) ; @BT FD 35 68
            SET 7,C'                   ; @BT CB F9
            SET 4,L'                   ; @BT CB E5
            ADD HL',SP                 ; @BT 39
            JP POS,((($2445-10)-8)+10+8) ; @BT F2 45 24
            LD IXH,((($18-9)+4)-4+9)   ; @BT DD 26 18
            RRCA                       ; @BT 0F
            SUB A',IXL                 ; @BT DD 95
            OR A',IXL                  ; @BT DD B5
            IM (((1-6)+(6-4))+4)       ; @BT ED 56
            SET 3,L'                   ; @BT CB DD
            LD H',RES 2,(IX+$35)       ; @BT DD CB 35 94
            AND A',H'                  ; @BT A4
            BIT 2,C'                   ; @BT CB 51
            LD D',H'                   ; @BT 54
            LD SP,(((($B109-8)+(8-3))+3)) ; @BT ED 7B 09 B1
            LD C',SET 1,(IY+$4A)       ; @BT FD CB 4A C9
            LD D',RES 5,(IX+$27)       ; @BT DD CB 27 AA
            RL D'                      ; @BT CB 12
            RES 4,(IY+$0D)             ; @BT FD CB 0D A6
            RR (IX+((($68-9)-(9-1))+(9-1)+9)) ; @BT DD CB 68 1E
            JR LSS,((($9B-4)-3)+4+3)   ; @BT 38 9B
            SET 5,H'                   ; @BT CB EC
            SRA A'                     ; @BT CB 2F
            LD H',(HL')                ; @BT 66
            RET LSS                    ; @BT D8
            BRLC DE',B                 ; @BT ED 2C
            LD C',SET 3,(IY+$56)       ; @BT FD CB 56 D9
            RST ((($28-(7+4))+(7+4)))  ; @BT EF
            LD D',SRA (IX+((($0D-6)+(6-2))+2)) ; @BT DD CB 0D 2A
            LD B',SLL (IX+((($67-6)+5)-5+6)) ; @BT DD CB 67 30
            IN A',((($90-(8+6))+8+6))  ; @BT DB 90
            SET 5,A'                   ; @BT CB EF
            RR A'                      ; @BT CB 1F
            SBC A',C'                  ; @BT 99
            SUB A',H'                  ; @BT 94
            RES 0,E'                   ; @BT CB 83
            BIT 2,E'                   ; @BT CB 53
            LD D',SET 3,(IX+$63)       ; @BT DD CB 63 DA
            LD B',RES 3,(IX+$7B)       ; @BT DD CB 7B 98
            HALT                       ; @BT 76
            BIT 4,C'                   ; @BT CB 61
            OTIR                       ; @BT ED B3
            LDWS                       ; @BT ED A5
            OUT (C'),C'                ; @BT ED 49
            LD C',RRC (IX+((($35-10)+1)-1+10)) ; @BT DD CB 35 09
            SBC A',IYL                 ; @BT FD 9D
            LD IYH,((($6E-8)+(8-11))+11) ; @BT FD 26 6E
            LD E',H'                   ; @BT 5C
            LD D',SET 0,(IX+$51)       ; @BT DD CB 51 C2
            DEC BC'                    ; @BT 0B
            PUSH DE'                   ; @BT D5
            LD E',SET 1,(IY+$6A)       ; @BT FD CB 6A CB
            LD ((($F216-9)+9)),IX      ; @BT DD 22 16 F2
            LD I,A'                    ; @BT ED 47
            LD B',RR (IX+(($2B-(5+1))+5+1)) ; @BT DD CB 2B 18
            LD L',SET 7,(IY+$2E)       ; @BT FD CB 2E FD
            CP A',C'                   ; @BT B9
            LD H',SET 7,(IX+$1B)       ; @BT DD CB 1B FC
            LD B',(HL')                ; @BT 46
            CP A',H'                   ; @BT BC
            LD C',RES 4,(IX+$11)       ; @BT DD CB 11 A1
            CALL NEQ,((($B77F-(11+9))+9)+11) ; @BT C4 7F B7
            LD C',RR (IX+((($3B-10)+(10-4))+4)) ; @BT DD CB 3B 19
            LD (HL'),C'                ; @BT 71
            OR A',E'                   ; @BT B3
            AND A',A'                  ; @BT A7
            CP A',(IY+((($1A-(8+1))+8)+1)) ; @BT FD BE 1A
            LD L',SET 0,(IY+$04)       ; @BT FD CB 04 C5
            LD A',C'                   ; @BT 79
            SUB A',((($4C-4)-(5-1))+(5-1)+4) ; @BT D6 4C
            LD A',(HL')                ; @BT 7E
            LD IY,(($D107-3)+3)        ; @BT FD 2A 07 D1
            SET 1,(HL')                ; @BT CB CE
            INC IX                     ; @BT DD 23
            IND                        ; @BT ED AA
            LD B',SLL (IY+((($6A-8)-(6-1))+(6-1)+8)) ; @BT FD CB 6A 30
            BIT 2,A'                   ; @BT CB 57
            RES 1,E'                   ; @BT CB 8B
            BIT 3,(HL')                ; @BT CB 5E
            BIT 1,(IX+$0C)             ; @BT DD CB 0C 4E
            LD A',SET 1,(IX+$27)       ; @BT DD CB 27 CF
            LD B',RRC (IY+((($28-(6+1))+6)+1)) ; @BT FD CB 28 08
            XOR A',IXL                 ; @BT DD AD
            SET 7,B'                   ; @BT CB F8
            OR A',(HL')                ; @BT B6
            LD E',SRL (IY+((($26-(10+1))+10)+1)) ; @BT FD CB 26 3B
            BIT 0,(HL')                ; @BT CB 46
            LD H',RL (IY+(($62-(11+1))+11+1)) ; @BT FD CB 62 14
            LD A',RLC (IX+((($09-(4+3))+3)+4)) ; @BT DD CB 09 07
            RES 4,(HL')                ; @BT CB A6
            RES 2,H'                   ; @BT CB 94
            LD L',SLL (IX+((($37-6)-(5-1))+(5-1)+6)) ; @BT DD CB 37 35
            SET 0,D'                   ; @BT CB C2
            LD D',D'                   ; @BT 52
            LD B',RES 4,(IY+$04)       ; @BT FD CB 04 A0
            INC D'                     ; @BT 14
            LD (IX+((($6A-5)-(1-1))+(1-1)+5)),E' ; @BT DD 73 6A
            CALL PE,(($BAB2-10)+10)    ; @BT EC B2 BA
            AND A',IYL                 ; @BT FD A5
            LD A',SET 4,(IX+$39)       ; @BT DD CB 39 E7
            LD (((($EDD2-(2+1))+2)+1)),DE' ; @BT ED 53 D2 ED
            LDIX                       ; @BT ED A4
            LD E',SET 2,(IX+$6D)       ; @BT DD CB 6D D3
            RLC E'                     ; @BT CB 03
            PIXELDN                    ; @BT ED 93
            LD D',RES 3,(IX+$1B)       ; @BT DD CB 1B 9A
            LD D',SET 1,(IX+$57)       ; @BT DD CB 57 CA
            LD (HL'),A'                ; @BT 77
            LD L',RES 0,(IX+$39)       ; @BT DD CB 39 85
            LD B',(($61-(4+3))+4+3)    ; @BT 06 61
            RRC (IY+((($0C-(3+1))+3)+1)) ; @BT FD CB 0C 0E
            LD C',SET 2,(IX+$4D)       ; @BT DD CB 4D D1
            LD (IX+((($6D-(9+1))+9)+1)),H' ; @BT DD 74 6D
            LD D',SET 3,(IY+$66)       ; @BT FD CB 66 DA
            LD E',SET 6,(IY+$08)       ; @BT FD CB 08 F3
            DI                         ; @BT F3
            LD H',SRL (IY+((($36-8)+5)-5+8)) ; @BT FD CB 36 3C
            RLC (IY+((($44-2)+4)-4+2)) ; @BT FD CB 44 06
            LD IYL,C'                  ; @BT FD 69
            LD A',SET 6,(IY+$48)       ; @BT FD CB 48 F7
            PUSH IY                    ; @BT FD E5
            EXX                        ; @BT D9
            LD D',IYH                  ; @BT FD 54
            LD A',RES 1,(IY+$62)       ; @BT FD CB 62 8F
            BIT 3,C'                   ; @BT CB 59
            EX DE',HL'                 ; @BT EB
            LD L',RES 3,(IX+$4B)       ; @BT DD CB 4B 9D
            ADD A',IYL                 ; @BT FD 85
            RES 7,(IY+$67)             ; @BT FD CB 67 BE
            SET 1,A'                   ; @BT CB CF
            LD A',RES 3,(IY+$6E)       ; @BT FD CB 6E 9F
            SET 0,(IY+$0D)             ; @BT FD CB 0D C6
            LD IXL,IXH                 ; @BT DD 6C
            DEC H'                     ; @BT 25
            LD H',RES 1,(IX+$2F)       ; @BT DD CB 2F 8C
            LD H',A'                   ; @BT 67
            LD B',SET 7,(IX+$5B)       ; @BT DD CB 5B F8
            LD L',SET 5,(IX+$1F)       ; @BT DD CB 1F ED
            BIT 6,(IX+$22)             ; @BT DD CB 22 76
            SET 3,(IY+$67)             ; @BT FD CB 67 DE
            LD D',L'                   ; @BT 55
            LD IYL,IYL                 ; @BT FD 6D
            LD C',SET 3,(IX+$53)       ; @BT DD CB 53 D9
            IN D',(C')                 ; @BT ED 50
            BIT 5,C'                   ; @BT CB 69
            LD H',RES 3,(IX+$3B)       ; @BT DD CB 3B 9C
            RR (HL')                   ; @BT CB 1E
            RR D'                      ; @BT CB 1A
            SL1 E'                     ; @BT CB 33
            LD L',RR (IY+((($7E-9)+(9-8))+8)) ; @BT FD CB 7E 1D
            LD A',RES 7,(IX+$03)       ; @BT DD CB 03 BF
            ADD HL',BC'                ; @BT 09
            LD IYH,B'                  ; @BT FD 60
            LD IXL,C'                  ; @BT DD 69
            LD C',SRA (IX+((($7D-2)-9)+2+9)) ; @BT DD CB 7D 29
            LD IYL,E'                  ; @BT FD 6B
            EX (SP),HL'                ; @BT E3
            RES 7,L'                   ; @BT CB BD
            LD B',SLA (IX+((($61-(2+1))+2)+1)) ; @BT DD CB 61 20
            LD A',L'                   ; @BT 7D
            BIT 7,(IY+$43)             ; @BT FD CB 43 7E
            LD E',SET 6,(IX+$05)       ; @BT DD CB 05 F3
            OR A',IXH                  ; @BT DD B4
            LD E',D'                   ; @BT 5A
            LD H',SET 3,(IY+$06)       ; @BT FD CB 06 DC
            OR A',C'                   ; @BT B1
            LD IYH,E'                  ; @BT FD 63
            DEC DE'                    ; @BT 1B
            LD D',RES 3,(IY+$1E)       ; @BT FD CB 1E 9A
            RES 0,(IX+$12)             ; @BT DD CB 12 86
            LD A',RES 0,(IY+$5C)       ; @BT FD CB 5C 87
            LD A',SRA (IY+((($60-(2+4))+(2+4)))) ; @BT FD CB 60 2F
            SBC A',(IX+(($51-(7+2))+7+2)) ; @BT DD 9E 51
            LD B',SET 3,(IX+$43)       ; @BT DD CB 43 D8
            PUSH HL'                   ; @BT E5
            LD D',RES 6,(IY+$30)       ; @BT FD CB 30 B2
            LD A',SET 2,(IY+$30)       ; @BT FD CB 30 D7
            ADD A',B'                  ; @BT 80
            DEC B'                     ; @BT 05
            LD H',RES 3,(IY+$3E)       ; @BT FD CB 3E 9C
            LD D',RRC (IX+(($45-(8+7))+8+7)) ; @BT DD CB 45 0A
            LD A',(IY+((($5E-(4+6))+(4+6)))) ; @BT FD 7E 5E
            AND A',IYH                 ; @BT FD A4
            SLA L'                     ; @BT CB 25
            LD E',RRC (IX+((($55-(4+1))+(4+1)))) ; @BT DD CB 55 0B
            INC IXL                    ; @BT DD 2C
            BIT 2,B'                   ; @BT CB 50
            LD A',RES 5,(IY+$7A)       ; @BT FD CB 7A AF
            LD D',SET 0,(IY+$54)       ; @BT FD CB 54 C2
            OUT (C'),B'                ; @BT ED 41
            LD E',RES 5,(IX+$37)       ; @BT DD CB 37 AB
            ADD IX,DE'                 ; @BT DD 19
            LD E',RES 1,(IX+$1F)       ; @BT DD CB 1F 8B
            RR L'                      ; @BT CB 1D
            ADC A',L'                  ; @BT 8D
            BIT 1,D'                   ; @BT CB 4A
            LD IYH,IYL                 ; @BT FD 65
            LD E',IYL                  ; @BT FD 5D
            LD C',D'                   ; @BT 4A
            LD B',(IX+((($11-(6+5))+5)+6)) ; @BT DD 46 11
            LD B',RES 5,(IY+$0A)       ; @BT FD CB 0A A8
            XOR A',A'                  ; @BT AF
            ADD A',L'                  ; @BT 85
            MUL D',E                   ; @BT ED 30
            RES 1,L'                   ; @BT CB 8D
            RL L'                      ; @BT CB 15
            CP A',IXH                  ; @BT DD BC
            CALL NEG,(($B55D-(10+5))+10+5) ; @BT FC 5D B5
            AND A',(HL')               ; @BT A6
            LD C',SET 2,(IY+$50)       ; @BT FD CB 50 D1
            LD C',B'                   ; @BT 48
            ADD A',E'                  ; @BT 83
            NOP                        ; @BT 00
            BIT 1,(IY+$0F)             ; @BT FD CB 0F 4E
            ADD HL',HL'                ; @BT 29
            OR A',(IX+((($14-9)-9)+9+9)) ; @BT DD B6 14
            BIT 0,A'                   ; @BT CB 47
            LD A',SRA (IX+((($5D-6)+(6-9))+9)) ; @BT DD CB 5D 2F
            AND A',IXL                 ; @BT DD A5
            SET 2,H'                   ; @BT CB D4
            LD A',B'                   ; @BT 78
            RES 1,B'                   ; @BT CB 88
            SET 3,B'                   ; @BT CB D8
            RES 4,E'                   ; @BT CB A3
            LD C',RES 5,(IX+$17)       ; @BT DD CB 17 A9
            LD E',(HL')                ; @BT 5E
            SET 6,H'                   ; @BT CB F4
            LD C',SLA (IY+((($74-10)+6)-6+10)) ; @BT FD CB 74 21
            LD E',L'                   ; @BT 5D
            SBC A',A'                  ; @BT 9F
            LD A',RES 4,(IY+$74)       ; @BT FD CB 74 A7
            LD H',SET 2,(IX+$7D)       ; @BT DD CB 7D D4
            LD D',RL (IX+((($3F-3)+(3-9))+9)) ; @BT DD CB 3F 12
            LD B',RL (IY+((($22-5)+(5-7))+7)) ; @BT FD CB 22 10
            LD H',RES 7,(IX+$53)       ; @BT DD CB 53 BC
            LD D',SET 2,(IY+$60)       ; @BT FD CB 60 D2
            LD E',SET 3,(IX+$73)       ; @BT DD CB 73 DB
            RET NEQ                    ; @BT C0
            SBC A',IYH                 ; @BT FD 9C
            LD C',SLA (IX+((($71-4)-2)+4+2)) ; @BT DD CB 71 21
            RR C'                      ; @BT CB 19
            LD A',IXL                  ; @BT DD 7D
            LD L',RES 1,(IY+$42)       ; @BT FD CB 42 8D
            LD IYH,A'                  ; @BT FD 67
            LD A',RLC (IY+(($0C-(6+9))+6+9)) ; @BT FD CB 0C 07
            JP (IX)                    ; @BT DD E9
            LD C',SLL (IX+((($77-(2+3))+(2+3)))) ; @BT DD CB 77 31
            LD C',RES 6,(IX+$1D)       ; @BT DD CB 1D B1
            LD H',((($AE-10)-(4-1))+(4-1)+10) ; @BT 26 AE
            CP A',B'                   ; @BT B8
            SET 0,A'                   ; @BT CB C7
            ADD A',(HL')               ; @BT 86
            ADC A',B'                  ; @BT 88
            LD E',RES 7,(IY+$46)       ; @BT FD CB 46 BB
            LD (IX+((($61-6)+(6-3))+3)),B' ; @BT DD 70 61
            CALL LSS,((($B44C-(10+1))+10)+1) ; @BT DC 4C B4
            RRC (IX+((($09-(7+9))+(7+9)))) ; @BT DD CB 09 0E
            LD BC',((($4887-4)-4)+4+4) ; @BT ED 4B 87 48
            SLA D'                     ; @BT CB 22
            LD E',RES 0,(IY+$1C)       ; @BT FD CB 1C 83
            LD (IX+((($64-7)+(7-4))+4)),C' ; @BT DD 71 64
            SET 6,C'                   ; @BT CB F1
            LD D',SET 7,(IY+$7E)       ; @BT FD CB 7E FA
            LD D',A'                   ; @BT 57
            RES 4,C'                   ; @BT CB A1
            LD A',(IX+((($55-9)+(9-3))+3)) ; @BT DD 7E 55
            SRL (IY+((($57-6)-(1-1))+(1-1)+6)) ; @BT FD CB 57 3E
            XOR A',IYH                 ; @BT FD AC
            ADD DE,A'                  ; @BT ED 32
            INC A'                     ; @BT 3C
            RES 4,D'                   ; @BT CB A2
            LD A',RES 3,(IX+$6B)       ; @BT DD CB 6B 9F
            SET 3,(IX+$64)             ; @BT DD CB 64 DE
            INC HL'                    ; @BT 23
            LD IYL,D'                  ; @BT FD 6A
            SET 3,(HL')                ; @BT CB DE
            LD L',RLC (IY+((($6C-(9+8))+(9+8)))) ; @BT FD CB 6C 05
            LD E',RR (IX+((($5B-4)-8)+4+8)) ; @BT DD CB 5B 1B
            JR ((($F7-11)+(11-1))+1)   ; @BT 18 F7
            LD H',D'                   ; @BT 62
            RES 6,C'                   ; @BT CB B1
            SET 1,H'                   ; @BT CB CC
            LD H',SET 5,(IX+$0F)       ; @BT DD CB 0F EC
            LD B',IXL                  ; @BT DD 45
            BIT 2,D'                   ; @BT CB 52
            LD L',(IX+((($07-3)+2)-2+3)) ; @BT DD 6E 07
            INC H'                     ; @BT 24
            LD D',RES 0,(IX+$09)       ; @BT DD CB 09 82
            LD (IX+((($70-(7+6))+(7+6)))),L' ; @BT DD 75 70
            LD C',SET 0,(IX+$41)       ; @BT DD CB 41 C1
            RES 5,C'                   ; @BT CB A9
            BSRF DE',B                 ; @BT ED 2B
            LD D',RES 1,(IX+$0F)       ; @BT DD CB 0F 8A
            OTDR                       ; @BT ED BB
            IM (((0-(5+2))+(5+2)))     ; @BT ED 46
            BIT 7,B'                   ; @BT CB 78
            LD H',SET 1,(IX+$77)       ; @BT DD CB 77 CC
            RES 3,D'                   ; @BT CB 9A
            LD B',RLC (IY+((($1C-(4+7))+7)+4)) ; @BT FD CB 1C 00
            LD D',RLC (IY+((($3C-11)-(5-1))+(5-1)+11)) ; @BT FD CB 3C 02
            SET 6,A'                   ; @BT CB F7
            LD C',SET 5,(IX+$5F)       ; @BT DD CB 5F E9
            LD B',(IY+((($1A-(10+3))+3)+10)) ; @BT FD 46 1A
            JR NEQ,((($C9-(8+1))+8)+1) ; @BT 20 C9
            AND A',L'                  ; @BT A5
            LD L',RES 4,(IX+$51)       ; @BT DD CB 51 A5
            PUSH ((($9862-(7+1))+7)+1) ; @BT ED 8A 98 62
            BIT 1,C'                   ; @BT CB 49
            SL1 D'                     ; @BT CB 32
            LD D',RES 4,(IX+$21)       ; @BT DD CB 21 A2
            LD DE',((($6DDA-6)+(6-4))+4) ; @BT ED 5B DA 6D
            NEXTREG (($F1-(5+9))+5+9),((($F1-4)+(4-8))+8) ; @BT ED 91 F1 F1
            LD C',C'                   ; @BT 49
            LD D',SET 5,(IX+$6F)       ; @BT DD CB 6F EA
            LD H',C'                   ; @BT 61
            LD E',SLL (IY+((($1A-(1+1))+1)+1)) ; @BT FD CB 1A 33
            RES 5,E'                   ; @BT CB AB
            RST ((($18-5)+(5-3))+3)    ; @BT DF
            PUSH BC'                   ; @BT C5
            JR GTE,((($B2-(11+8))+8)+11) ; @BT 30 B2
            BIT 3,A'                   ; @BT CB 5F
            LD H',RR (IX+((($6B-3)+(3-3))+3)) ; @BT DD CB 6B 1C
            BIT 0,B'                   ; @BT CB 40
            LD B',RES 2,(IX+$75)       ; @BT DD CB 75 90
            RES 7,E'                   ; @BT CB BB
            LD B',SET 1,(IX+$37)       ; @BT DD CB 37 C8
            SRA (HL')                  ; @BT CB 2E
            SUB A',C'                  ; @BT 91
            LD H',(IY+((($4B-(7+5))+5)+7)) ; @BT FD 66 4B
            SET 6,(IX+$3E)             ; @BT DD CB 3E F6
            RL (IY+((($26-(5+1))+5)+1)) ; @BT FD CB 26 16
            RET NEG                    ; @BT F8
            BIT 0,D'                   ; @BT CB 42
            SET 4,(IX+$02)             ; @BT DD CB 02 E6
            BIT 6,(HL')                ; @BT CB 76
            JP PO,((($2778-2)+(2-6))+6) ; @BT E2 78 27
            LD D',SET 6,(IY+$78)       ; @BT FD CB 78 F2
            LD D',SET 4,(IX+$69)       ; @BT DD CB 69 E2
            LD H',RR (IY+((($6E-(7+4))+(7+4)))) ; @BT FD CB 6E 1C
            LD H',RES 5,(IY+$4A)       ; @BT FD CB 4A AC
            INC BC'                    ; @BT 03
            JP (HL')                   ; @BT E9
            LD D',IYL                  ; @BT FD 55
            LD HL',((($9660-4)+4))     ; @BT 2A 60 96
            LD D',B'                   ; @BT 50
            LD B',RES 5,(IX+$07)       ; @BT DD CB 07 A8
            LD C',RES 5,(IY+$1A)       ; @BT FD CB 1A A9
            SRL C'                     ; @BT CB 39
            LD A',SLA (IY+((($54-9)+2)-2+9)) ; @BT FD CB 54 27
            LD L',SET 0,(IX+$01)       ; @BT DD CB 01 C5
            LD H',RES 6,(IX+$4D)       ; @BT DD CB 4D B4
            LD H',RES 0,(IX+$29)       ; @BT DD CB 29 84
            RLC (HL')                  ; @BT CB 06
            XOR A',H'                  ; @BT AC
            SET 4,(IY+$05)             ; @BT FD CB 05 E6
            EX (SP),IY                 ; @BT FD E3
            INC (IX+((($5A-3)+(3+2))-2)) ; @BT DD 34 5A
            RES 5,H'                   ; @BT CB AC
            DEC IYH                    ; @BT FD 25
            RLC C'                     ; @BT CB 01
            LD (IY+((($2F-(8+5))+5)+8)),B' ; @BT FD 70 2F
            SL1 (IX+((($7A-5)+6)-6+5)) ; @BT DD CB 7A 36
            LD A',RES 2,(IX+$65)       ; @BT DD CB 65 97
            IM ((2-(7+2))+7+2)         ; @BT ED 5E
            RES 1,D'                   ; @BT CB 8A
            RES 6,(IX+$46)             ; @BT DD CB 46 B6
            SET 6,L'                   ; @BT CB F5
            LD IXL,E'                  ; @BT DD 6B
            RES 3,C'                   ; @BT CB 99
            SBC A',D'                  ; @BT 9A
            LD D',SLA (IX+((($01-2)-3)+2+3)) ; @BT DD CB 01 22
            RES 7,A'                   ; @BT CB BF
            SET 4,H'                   ; @BT CB E4
            SET 5,(HL')                ; @BT CB EE
            SET 4,C'                   ; @BT CB E1
            BIT 2,(IY+$2D)             ; @BT FD CB 2D 56
            LD IXH,D'                  ; @BT DD 62
            LD L',SET 3,(IY+$16)       ; @BT FD CB 16 DD
            LD H',RES 1,(IY+$32)       ; @BT FD CB 32 8C
            LD A',RR (IX+((($1B-(11+7))+(11+7)))) ; @BT DD CB 1B 1F
            LD H',RLC (IX+((($59-2)+(2+9))-9)) ; @BT DD CB 59 04
            XOR A',E'                  ; @BT AB
            BIT 7,L'                   ; @BT CB 7D
            LD L',RRC (IY+(($78-(4+7))+4+7)) ; @BT FD CB 78 0D
            BIT 0,L'                   ; @BT CB 45
            BIT 1,E'                   ; @BT CB 4B
            LD C',IYH                  ; @BT FD 4C
            TEST (($7A-(9+2))+9+2)     ; @BT ED 27 7A
            BIT 5,(IY+$07)             ; @BT FD CB 07 6E
            SET 3,A'                   ; @BT CB DF
            SBC A',B'                  ; @BT 98
            LD B',RES 6,(IY+$10)       ; @BT FD CB 10 B0
            LD E',SET 4,(IX+$79)       ; @BT DD CB 79 E3
            LD IXL,IXL                 ; @BT DD 6D
            SRA (IY+((($39-6)+8)-8+6)) ; @BT FD CB 39 2E
            SET 1,E'                   ; @BT CB CB
            SL1 C'                     ; @BT CB 31
            SET 7,(IY+$5F)             ; @BT FD CB 5F FE
            SET 6,E'                   ; @BT CB F3
            RES 0,(IX+$12)             ; @BT DD CB 12 86
            LD B',SET 3,(IY+$46)       ; @BT FD CB 46 D8
            LD H',SRA (IY+((($30-6)+(6-4))+4)) ; @BT FD CB 30 2C
            SET 0,E'                   ; @BT CB C3
            XOR A',D'                  ; @BT AA
            LD SP,HL'                  ; @BT F9
            LD D',(IX+((($74-(2+7))+(2+7)))) ; @BT DD 56 74
            DEC SP                     ; @BT 3B
            LD C',RES 3,(IY+$0E)       ; @BT FD CB 0E 99
            RES 7,(IX+$64)             ; @BT DD CB 64 BE
            SET 1,L'                   ; @BT CB CD
            DEC HL'                    ; @BT 2B
            LD A',SLL (IX+(($57-(9+3))+9+3)) ; @BT DD CB 57 37
            LD E',RES 4,(IX+$31)       ; @BT DD CB 31 A3
            LD H',SET 0,(IY+$74)       ; @BT FD CB 74 C4
            SBC A',IXH                 ; @BT DD 9C
            BSRA DE',B                 ; @BT ED 29
            BIT 4,H'                   ; @BT CB 64
            RLD                        ; @BT ED 6F
            SET 1,C'                   ; @BT CB C9
            LD B',RL (IX+((($1F-(8+8))+8)+8)) ; @BT DD CB 1F 10
            LD E',((($B1-5)+(5-5))+5)  ; @BT 1E B1
            LD H',RRC (IY+((($68-(10+8))+8)+10)) ; @BT FD CB 68 0C
            LD E',SET 7,(IY+$0E)       ; @BT FD CB 0E FB
            LD B',RES 0,(IY+$6C)       ; @BT FD CB 6C 80
            LD C',SET 5,(IY+$62)       ; @BT FD CB 62 E9
            RES 3,L'                   ; @BT CB 9D
            SRA H'                     ; @BT CB 2C
            SET 2,D'                   ; @BT CB D2
            LD IYL,IYH                 ; @BT FD 6C
            ADC A',((($53-(7+7))+(7+7))) ; @BT CE 53
            LD L',(IY+((($0A-3)+5)-5+3)) ; @BT FD 6E 0A
            SBC HL',SP                 ; @BT ED 72
            ADC A',IYL                 ; @BT FD 8D
            RLA                        ; @BT 17
            LD D',RES 2,(IY+$18)       ; @BT FD CB 18 92
            ADD A',IXL                 ; @BT DD 85
            SLA (HL')                  ; @BT CB 26
            AND A',D'                  ; @BT A2
            RES 1,(HL')                ; @BT CB 8E
            LD (IX+((($67-5)+(5-9))+9)),D' ; @BT DD 72 67
            LD D',RES 0,(IY+$0C)       ; @BT FD CB 0C 82
            BIT 7,C'                   ; @BT CB 79
            LD C',RRC (IY+((($38-4)+(4-8))+8)) ; @BT FD CB 38 09
            RLC H'                     ; @BT CB 04
            RET PE                     ; @BT E8
            LD D',RES 6,(IX+$2D)       ; @BT DD CB 2D B2
            LD E',RES 6,(IY+$40)       ; @BT FD CB 40 B3
            LD H',E'                   ; @BT 63
            RES 6,H'                   ; @BT CB B4
            DEC E'                     ; @BT 1D
            RET EQU                    ; @BT C8
            LD IXL,((($D0-11)+9)-9+11) ; @BT DD 2E D0
            LD B',E'                   ; @BT 43
            RES 4,L'                   ; @BT CB A5
            BIT 1,(HL')                ; @BT CB 4E
            LD H',RES 4,(IX+$41)       ; @BT DD CB 41 A4
            LD C',RES 0,(IY+$7C)       ; @BT FD CB 7C 81
            SET 0,(IX+$0A)             ; @BT DD CB 0A C6
            RLCA                       ; @BT 07
            LD E',SRA (IY+((($20-(3+6))+6)+3)) ; @BT FD CB 20 2B
            CALL POS,((($B890-(4+4))+4)+4) ; @BT F4 90 B8
            SET 0,L'                   ; @BT CB C5
            INC DE'                    ; @BT 13
            LD A',SET 3,(IY+$36)       ; @BT FD CB 36 DF
            SL1 A'                     ; @BT CB 37
            LD E',SRL (IX+((($23-3)-(9-1))+(9-1)+3)) ; @BT DD CB 23 3B
            INC (IY+((($66-10)+(10+6))-6)) ; @BT FD 34 66
            ADD HL,A'                  ; @BT ED 31
            LD E',IXH                  ; @BT DD 5C
            LD A',R                    ; @BT ED 5F
            EI                         ; @BT FB
            RLC L'                     ; @BT CB 05
            BIT 4,A'                   ; @BT CB 67
            LD B',SET 2,(IX+$3D)       ; @BT DD CB 3D D0
            RES 0,(IY+$15)             ; @BT FD CB 15 86
            LD C',RLC (IY+((($2C-9)+(9+1))-1)) ; @BT FD CB 2C 01
            SET 2,E'                   ; @BT CB D3
            BIT 4,(IY+$69)             ; @BT FD CB 69 66
            ADC A',IXH                 ; @BT DD 8C
            LD B',IYL                  ; @BT FD 45
            SET 5,(IX+$20)             ; @BT DD CB 20 EE
            OUTINB                     ; @BT ED 90
            CALL GTE,((($B66E-(1+1))+1)+1) ; @BT D4 6E B6
            SET 3,H'                   ; @BT CB DC
            LD D',SET 7,(IX+$7B)       ; @BT DD CB 7B FA
            BIT 3,(IY+$4B)             ; @BT FD CB 4B 5E
            LD D',RES 7,(IY+$36)       ; @BT FD CB 36 BA
            SBC A',H'                  ; @BT 9C
            SBC HL',DE'                ; @BT ED 52
            LD L',RES 0,(IY+$3C)       ; @BT FD CB 3C 85
            CP A',IYL                  ; @BT FD BD
            RL (HL')                   ; @BT CB 16
            LD A',SLL (IY+((($5A-3)+(3+5))-5)) ; @BT FD CB 5A 37
            LD D',RES 1,(IY+$12)       ; @BT FD CB 12 8A
            BIT 4,(IX+$66)             ; @BT DD CB 66 66
            RES 0,(IY+$15)             ; @BT FD CB 15 86
            LD C',RR (IY+((($3E-(4+1))+4)+1)) ; @BT FD CB 3E 19
            CP A',D'                   ; @BT BA
            LD A',SET 5,(IX+$3F)       ; @BT DD CB 3F EF
            RES 7,D'                   ; @BT CB BA
            SLA A'                     ; @BT CB 27
            LD E',RES 7,(IX+$43)       ; @BT DD CB 43 BB
            LD L',D'                   ; @BT 6A
            JP NEQ,((($2334-4)-1)+4+1) ; @BT C2 34 23
            RRC C'                     ; @BT CB 09
            LD (HL'),D'                ; @BT 72
            POP HL'                    ; @BT E1
            LD D',IXH                  ; @BT DD 54
            LD A',RES 5,(IX+$77)       ; @BT DD CB 77 AF
            LD L',E'                   ; @BT 6B
            SETAE                      ; @BT ED 95
            LD L',RES 5,(IY+$5A)       ; @BT FD CB 5A AD
            LD L',RL (IY+((($72-(8+9))+(8+9)))) ; @BT FD CB 72 15
            LD H',SET 1,(IY+$7A)       ; @BT FD CB 7A CC
            LD E',RL (IY+((($52-(1+1))+1)+1)) ; @BT FD CB 52 13
            LD (IY+((($35-4)+4)-4+4)),D' ; @BT FD 72 35
            IN F',(C')                 ; @BT ED 70
            LD E',SET 5,(IY+$02)       ; @BT FD CB 02 EB
            SET 3,C'                   ; @BT CB D9
            CP A',(HL')                ; @BT BE
            BIT 1,H'                   ; @BT CB 4C
            BIT 7,A'                   ; @BT CB 7F
            PIXELAD                    ; @BT ED 94
            ADD A',IYH                 ; @BT FD 84
            LD B',IXH                  ; @BT DD 44
            LD L',SRL (IX+((($43-(6+1))+(6+1)))) ; @BT DD CB 43 3D
            SET 0,(IX+$0A)             ; @BT DD CB 0A C6
            LD E',SRA (IX+((($1D-2)+(2-4))+4)) ; @BT DD CB 1D 2B
            SET 2,B'                   ; @BT CB D0
            LD B',RES 1,(IY+$72)       ; @BT FD CB 72 88
            LD B',RES 6,(IX+$0D)       ; @BT DD CB 0D B0
            CP A',((($E1-(10+6))+(10+6))) ; @BT FE E1
            RES 5,A'                   ; @BT CB AF
            POP AF'                    ; @BT F1
            LD (IX+(($5E-(3+2))+3+2)),A' ; @BT DD 77 5E
            LD A',RES 6,(IX+$7D)       ; @BT DD CB 7D B7
            SET 5,C'                   ; @BT CB E9
            INC IY                     ; @BT FD 23
            OR A',((($78-(11+4))+(11+4))) ; @BT F6 78
            RL A'                      ; @BT CB 17
            SBC A',(HL')               ; @BT 9E
            LD C',(IY+((($47-3)-(1-1))+(1-1)+3)) ; @BT FD 4E 47
            LD B',SRA (IY+(($70-11)+11)) ; @BT FD CB 70 28
            LD A',H'                   ; @BT 7C
            LD (IY+((($32-8)-(9-1))+(9-1)+8)),C' ; @BT FD 71 32
            RST ((($00-(10+2))+2)+10)  ; @BT C7
            CP A',IXL                  ; @BT DD BD
            LD L',RRC (IX+((($75-7)-8)+7+8)) ; @BT DD CB 75 0D
            BIT 7,D'                   ; @BT CB 7A
            DEC (IX+(($5C-3)+3))       ; @BT DD 35 5C
            SET 5,L'                   ; @BT CB ED
            ADC A',C'                  ; @BT 89
            RL C'                      ; @BT CB 11
            JP GTE,((($2223-2)+(2-9))+9) ; @BT D2 23 22
            ADD A',A'                  ; @BT 87
            IN E',(C')                 ; @BT ED 58
            SUB A',(HL')               ; @BT 96
            LD HL',(($9771-4)+4)       ; @BT 2A 71 97
            LD L',L'                   ; @BT 6D
            LD A',RES 0,(IX+$59)       ; @BT DD CB 59 87
            LD E',SET 3,(IY+$76)       ; @BT FD CB 76 DB
            INDR                       ; @BT ED BA
            SL1 H'                     ; @BT CB 34
            LD B',RES 1,(IX+$6F)       ; @BT DD CB 6F 88
            OUT (C'),A'                ; @BT ED 79
            LD H',SET 6,(IX+$15)       ; @BT DD CB 15 F4
            SL1 L'                     ; @BT CB 35
            LD C',RES 3,(IX+$0B)       ; @BT DD CB 0B 99
            LD L',SET 4,(IX+$19)       ; @BT DD CB 19 E5
            RES 4,(IX+$0A)             ; @BT DD CB 0A A6
            RES 5,L'                   ; @BT CB AD
            SET 6,D'                   ; @BT CB F2
            SRA (IX+((($36-9)+6)-6+9)) ; @BT DD CB 36 2E
            LD C',SET 6,(IY+$68)       ; @BT FD CB 68 F1
            INC C'                     ; @BT 0C
            LD C',SET 1,(IX+$47)       ; @BT DD CB 47 C9
            LD D',RES 2,(IX+$15)       ; @BT DD CB 15 92
            SET 2,C'                   ; @BT CB D1
            POP IY                     ; @BT FD E1
            SET 5,B'                   ; @BT CB E8
            LD IXH,A'                  ; @BT DD 67
            IN H',(C')                 ; @BT ED 60
            LD B',SET 4,(IY+$4C)       ; @BT FD CB 4C E0
            SUB A',IYH                 ; @BT FD 94
            LD E',A'                   ; @BT 5F
            LD D',SET 5,(IY+$72)       ; @BT FD CB 72 EA
            LD IXL,A'                  ; @BT DD 6F
            SUB A',E'                  ; @BT 93
            LD L',RL (IX+((($6F-(4+1))+4)+1)) ; @BT DD CB 6F 15
            RET GTE                    ; @BT D0
            LD E',C'                   ; @BT 59
            LD E',RES 2,(IX+$25)       ; @BT DD CB 25 93
            BIT 3,D'                   ; @BT CB 5A
            RLC A'                     ; @BT CB 07
            LD D',RR (IY+((($4E-6)+6)-6+6)) ; @BT FD CB 4E 1A
            LD (HL'),B'                ; @BT 70
            BIT 2,(IX+$2A)             ; @BT DD CB 2A 56
            ADD IX,IX                  ; @BT DD 29
            LD C',RES 4,(IY+$14)       ; @BT FD CB 14 A1
            AND A',IXH                 ; @BT DD A4
            OR A',D'                   ; @BT B2
            DEC IXH                    ; @BT DD 25
            LD C',(($BA-(10+8))+10+8)  ; @BT 0E BA
            SRL (HL')                  ; @BT CB 3E
            LD B',A'                   ; @BT 47
            BIT 5,H'                   ; @BT CB 6C
            RES 0,D'                   ; @BT CB 82
            LD B',C'                   ; @BT 41
            LD IXH,E'                  ; @BT DD 63
            SET 3,D'                   ; @BT CB DA
            LD A',RES 4,(IX+$71)       ; @BT DD CB 71 A7
            LD L',RES 6,(IY+$60)       ; @BT FD CB 60 B5
            RLC (IX+((($41-4)+(4-2))+2)) ; @BT DD CB 41 06
            LD IXH,IXH                 ; @BT DD 64
            LD A',IYH                  ; @BT FD 7C
            LD C',IXL                  ; @BT DD 4D
            BIT 1,A'                   ; @BT CB 4F
            LD A',RES 6,(IY+$00)       ; @BT FD CB 00 B7
            LD IYL,B'                  ; @BT FD 68
            ADD IY,IY                  ; @BT FD 29
            RR H'                      ; @BT CB 1C
            BIT 4,(HL')                ; @BT CB 66
            INC E'                     ; @BT 1C
            LD C',H'                   ; @BT 4C
            LD L',B'                   ; @BT 68
            RST ((($00-(11+4))+4)+11)  ; @BT C7
            LD E',RR (IY+((($5E-3)+(3+8))-8)) ; @BT FD CB 5E 1B
            LD E',RL (IX+((($4F-(9+9))+9)+9)) ; @BT DD CB 4F 13
            LD L',RES 7,(IX+$63)       ; @BT DD CB 63 BD
            LD D',(HL')                ; @BT 56
            LD (((($F105-8)+7)-7+8)),IY ; @BT FD 22 05 F1
            LDDR                       ; @BT ED B8
            LD IYH,IYH                 ; @BT FD 64
            CALL EQU,((($B9A1-5)+1)-1+5) ; @BT CC A1 B9
            RL E'                      ; @BT CB 13
            LD H',SRL (IX+((($33-2)+(2-2))+2)) ; @BT DD CB 33 3C
            SET 1,(IY+$2B)             ; @BT FD CB 2B CE
            LD DE',(((($6CC9-(4+4))+(4+4)))) ; @BT ED 5B C9 6C
            SBC A',L'                  ; @BT 9D
            LD L',SET 2,(IX+$0D)       ; @BT DD CB 0D D5
            LD E',RES 2,(IY+$28)       ; @BT FD CB 28 93
            LD H',RES 2,(IY+$38)       ; @BT FD CB 38 94
            DEC A'                     ; @BT 3D
            RES 7,H'                   ; @BT CB BC
            LD L',SET 4,(IY+$1C)       ; @BT FD CB 1C E5
            INC B'                     ; @BT 04
            LD B',RRC (IX+((($25-10)+9)-9+10)) ; @BT DD CB 25 08
            BIT 2,H'                   ; @BT CB 54
            LD H',SET 3,(IX+$03)       ; @BT DD CB 03 DC
            RES 6,B'                   ; @BT CB B0
            LD B',D'                   ; @BT 42
            LD L',RR (IX+(($7B-(8+9))+8+9)) ; @BT DD CB 7B 1D
            DEC IX                     ; @BT DD 2B
            LD C',SET 4,(IX+$59)       ; @BT DD CB 59 E1
            RET PO                     ; @BT E0
            LD B',SET 0,(IX+$31)       ; @BT DD CB 31 C0
            LD C',SRL (IY+((($06-8)+(8-10))+10)) ; @BT FD CB 06 39
            LD B',SET 6,(IX+$55)       ; @BT DD CB 55 F0
            ADC HL',SP                 ; @BT ED 7A
            BIT 4,L'                   ; @BT CB 65
            LD E',SET 4,(IY+$7C)       ; @BT FD CB 7C E3
            SRL L'                     ; @BT CB 3D
            SET 7,L'                   ; @BT CB FD
            RES 2,B'                   ; @BT CB 90
            BIT 4,D'                   ; @BT CB 62
            OUT (C'),0                 ; @BT ED 71
            LD H',SET 2,(IY+$00)       ; @BT FD CB 00 D4
            BSRL DE',B                 ; @BT ED 2A
            POP DE'                    ; @BT D1
            RES 2,(IX+$4E)             ; @BT DD CB 4E 96
            LD E',SET 7,(IX+$0B)       ; @BT DD CB 0B FB
            MIRROR A'                  ; @BT ED 24
            SET 4,(HL')                ; @BT CB E6
            JP EQU,((($2556-9)-(5-1))+(5-1)+9) ; @BT CA 56 25
            LD L',SET 6,(IY+$28)       ; @BT FD CB 28 F5
            LD L',RES 2,(IX+$45)       ; @BT DD CB 45 95
            AND A',(IX+((($47-1)+(1-3))+3)) ; @BT DD A6 47
            LD D',SET 6,(IX+$75)       ; @BT DD CB 75 F2
            BIT 6,D'                   ; @BT CB 72
            IN B',(C')                 ; @BT ED 40
            CPDR                       ; @BT ED B9
            LD D',SLL (IX+((($07-(3+1))+3)+1)) ; @BT DD CB 07 32
            BIT 6,A'                   ; @BT CB 77
            LD A',SET 3,(IX+$33)       ; @BT DD CB 33 DF
            LD H',(IX+(($48-(11+9))+11+9)) ; @BT DD 66 48
            ADD A',D'                  ; @BT 82
            ADC A',E'                  ; @BT 8B
            LD B',RES 2,(IY+$78)       ; @BT FD CB 78 90
            IN L',(C')                 ; @BT ED 68
            XOR A',IYL                 ; @BT FD AD
            OUT (C'),E'                ; @BT ED 59
            IN C',(C')                 ; @BT ED 48
            SLA (IX+((($18-(6+9))+(6+9)))) ; @BT DD CB 18 26
            LD L',H'                   ; @BT 6C
            RST ((($38-2)+(2-3))+3)    ; @BT FF
            DEC L'                     ; @BT 2D
            SLA C'                     ; @BT CB 21
            LD A',RL (IY+(($12-(3+9))+3+9)) ; @BT FD CB 12 17
            LD B',RES 4,(IX+$01)       ; @BT DD CB 01 A0
            IN A',(C')                 ; @BT ED 78
            LD A',D'                   ; @BT 7A
            LD A',RES 2,(IY+$68)       ; @BT FD CB 68 97
            LD D',SET 1,(IY+$5A)       ; @BT FD CB 5A CA
            LD B',RR (IY+((($2E-(1+1))+1)+1)) ; @BT FD CB 2E 18
            AND A',((($FF-8)+(8-3))+3) ; @BT E6 FF
            BIT 6,E'                   ; @BT CB 73
            OR A',(IY+((($1D-2)+(2-11))+11)) ; @BT FD B6 1D
            LDDRX                      ; @BT ED BC
            SET 0,(HL')                ; @BT CB C6
            LD IXH,C'                  ; @BT DD 61
            RETN                       ; @BT ED 45
            LD C',(HL')                ; @BT 4E
            INIR                       ; @BT ED B2
            SET 4,D'                   ; @BT CB E2
            RR (IY+((($6B-(5+7))+7)+5)) ; @BT FD CB 6B 1E
            SBC A',(($A1-8)+8)         ; @BT DE A1
            LD E',RES 3,(IY+$2E)       ; @BT FD CB 2E 9B
            RES 2,(IY+$51)             ; @BT FD CB 51 96
            SET 2,A'                   ; @BT CB D7
            LDIRX                      ; @BT ED B4
            SET 4,E'                   ; @BT CB E3
            LD A',(BC')                ; @BT 0A
            SET 2,(IY+$49)             ; @BT FD CB 49 D6
            LD H',SET 7,(IY+$1E)       ; @BT FD CB 1E FC
            LD IY,(((($CFF6-(8+7))+7)+8)) ; @BT FD 2A F6 CF
            ADD IY,DE'                 ; @BT FD 19
            RST                        ; @BT C7
            SUB A',B'                  ; @BT 90
            LD SP,IY                   ; @BT FD F9
            BIT 2,(HL')                ; @BT CB 56
            LD H',L'                   ; @BT 65
            LD E',E'                   ; @BT 5B
            BIT 7,(IX+$40)             ; @BT DD CB 40 7E
            LD H',SET 0,(IX+$71)       ; @BT DD CB 71 C4
            DEC IYL                    ; @BT FD 2D
            SRL (IX+((($54-1)+(1-2))+2)) ; @BT DD CB 54 3E
            LD (IY+((($3B-9)+5)-5+9)),H' ; @BT FD 74 3B
            AND A',B'                  ; @BT A0
            RLC (IX+((($41-(3+1))+3)+1)) ; @BT DD CB 41 06
            BSLA DE',B                 ; @BT ED 28
            RES 2,E'                   ; @BT CB 93
            RES 0,H'                   ; @BT CB 84
            INC IYH                    ; @BT FD 24
            ADD A',(IX+((($64-7)-(2-1))+(2-1)+7)) ; @BT DD 86 64
            RST ((($20-5)-(2-1))+(2-1)+5) ; @BT E7
            SBC HL',BC'                ; @BT ED 42
            SL1 (HL')                  ; @BT CB 36
            ADC A',IXL                 ; @BT DD 8D
            ADC A',IYH                 ; @BT FD 8C
            JP PE,((($2667-1)+(1-11))+11) ; @BT EA 67 26
            SBC A',IXL                 ; @BT DD 9D
            LD B',SRL (IX+(($73-(7+5))+7+5)) ; @BT DD CB 73 38
            CPIR                       ; @BT ED B1
            LD H',RES 7,(IY+$56)       ; @BT FD CB 56 BC
            LD A',(DE')                ; @BT 1A
            RES 0,B'                   ; @BT CB 80
            OR A',H'                   ; @BT B4
            SUB A',IYL                 ; @BT FD 95
            BIT 1,B'                   ; @BT CB 48
            LD L',RES 3,(IY+$4E)       ; @BT FD CB 4E 9D
            RES 6,(HL')                ; @BT CB B6
            SET 5,(IY+$23)             ; @BT FD CB 23 EE
            DJNZ ((($65-(3+6))+(3+6))) ; @BT 10 65
            RES 7,(HL')                ; @BT CB BE
            LD H',RRC (IX+((($65-(7+8))+(7+8)))) ; @BT DD CB 65 0C
            BIT 7,E'                   ; @BT CB 7B
            CCF                        ; @BT 3F
            LD C',RES 6,(IY+$20)       ; @BT FD CB 20 B1
            LD L',A'                   ; @BT 6F
            AND A',C'                  ; @BT A1
            LD D',RES 4,(IY+$24)       ; @BT FD CB 24 A2
            LD D',RL (IY+((($42-(11+7))+(11+7)))) ; @BT FD CB 42 12
            LD E',SLL (IX+((($17-6)-(4-1))+(4-1)+6)) ; @BT DD CB 17 33
            SET 4,B'                   ; @BT CB E0
            SBC A',(IY+((($5A-(5+6))+6)+5)) ; @BT FD 9E 5A
            BIT 6,(IY+$25)             ; @BT FD CB 25 76
            RES 6,E'                   ; @BT CB B3
            LD (IY+(($41-11)+11)),((($DE-8)+8)-8+8) ; @BT FD 36 41 DE
            OR A',A'                   ; @BT B7
            LDPIRX                     ; @BT ED B7
            SET 6,(HL')                ; @BT CB F6
            LD (IX+((($73-5)+4)-4+5)),(($88-6)+6) ; @BT DD 36 73 88
            SRL H'                     ; @BT CB 3C
            BIT 5,(HL')                ; @BT CB 6E
            OR A',B'                   ; @BT B0
            RES 2,D'                   ; @BT CB 92
            SRA D'                     ; @BT CB 2A
            XOR A',B'                  ; @BT A8
            OUT (C'),H'                ; @BT ED 61
            INC IYL                    ; @BT FD 2C
            LD SP,IX                   ; @BT DD F9
            OUT (C'),D'                ; @BT ED 51
            SRA C'                     ; @BT CB 29
            RES 4,B'                   ; @BT CB A0
            SET 3,E'                   ; @BT CB DB
            LD B',SET 0,(IY+$34)       ; @BT FD CB 34 C0
            LD L',SET 6,(IX+$25)       ; @BT DD CB 25 F5
            LD E',SLA (IY+((($14-(4+8))+8)+4)) ; @BT FD CB 14 23
            RRD                        ; @BT ED 67
            LD IXL,D'                  ; @BT DD 6A
            BIT 5,A'                   ; @BT CB 6F
            LD (((($EFF4-9)+3)-3+9)),SP ; @BT ED 73 F4 EF
            LD A',RES 1,(IX+$5F)       ; @BT DD CB 5F 8F
            BIT 6,H'                   ; @BT CB 74
            LD C',A'                   ; @BT 4F
            SET 4,A'                   ; @BT CB E7
            LD A',SET 4,(IY+$3C)       ; @BT FD CB 3C E7
            LD E',RRC (IY+(($58-(3+2))+3+2)) ; @BT FD CB 58 0B
            LD (BC'),A'                ; @BT 02
            SET 7,E'                   ; @BT CB FB
            LD E',B'                   ; @BT 58
            LD C',IYL                  ; @BT FD 4D
            ADD IX,SP                  ; @BT DD 39
            LD C',SRA (IY+(($00-6)+6)) ; @BT FD CB 00 29
            LDIR                       ; @BT ED B0
            LD E',(IX+((($21-(6+4))+(6+4)))) ; @BT DD 5E 21
            LD C',RES 2,(IY+$08)       ; @BT FD CB 08 91
            LD C',SLL (IY+((($7A-8)+(8-9))+9)) ; @BT FD CB 7A 31
            OUTI                       ; @BT ED A3
            RES 1,(IX+$30)             ; @BT DD CB 30 8E
            LD A',SET 7,(IX+$4B)       ; @BT DD CB 4B FF
            DEC IXL                    ; @BT DD 2D
            RES 2,(HL')                ; @BT CB 96
            LD (HL'),H'                ; @BT 74
            LD IYL,((($26-9)+(9-5))+5) ; @BT FD 2E 26
            LD H',RLC (IY+((($5C-2)+(2+5))-5)) ; @BT FD CB 5C 04
            LD C',SET 0,(IY+$44)       ; @BT FD CB 44 C1
            BIT 0,H'                   ; @BT CB 44
            LD B',SET 6,(IY+$58)       ; @BT FD CB 58 F0
            LD L',SLL (IY+((($3A-(8+1))+8)+1)) ; @BT FD CB 3A 35
            RES 3,E'                   ; @BT CB 9B
            BIT 4,B'                   ; @BT CB 60
            RRC H'                     ; @BT CB 0C
            LD C',(IX+((($3E-2)+(2-9))+9)) ; @BT DD 4E 3E
            ADD IX,BC'                 ; @BT DD 09
            LD A',(((($4221-10)-(1-1))+(1-1)+10)) ; @BT 3A 21 42
            BIT 0,(IX+$6E)             ; @BT DD CB 6E 46
            RES 0,(HL')                ; @BT CB 86
            RL B'                      ; @BT CB 10
            INC SP                     ; @BT 33
            LD E',SLA (IX+((($11-6)+(6-6))+6)) ; @BT DD CB 11 23
            SRL E'                     ; @BT CB 3B
            LD H',SRA (IX+((($2D-5)-(6-1))+(6-1)+5)) ; @BT DD CB 2D 2C
            LD C',RES 7,(IY+$26)       ; @BT FD CB 26 B9
            RLC D'                     ; @BT CB 02
            XOR A',(HL')               ; @BT AE
            BIT 2,L'                   ; @BT CB 55
            LD B',SET 5,(IY+$52)       ; @BT FD CB 52 E8
            LD L',RES 5,(IX+$57)       ; @BT DD CB 57 AD
            RES 7,B'                   ; @BT CB B8
            SL1 (IY+((($7D-7)+(7-8))+8)) ; @BT FD CB 7D 36
            SET 6,(IY+$41)             ; @BT FD CB 41 F6
            RL (IX+((($23-6)+(6-3))+3)) ; @BT DD CB 23 16
            SUB A',L'                  ; @BT 95
            ADD HL',((($400F-11)+(11-5))+5) ; @BT ED 34 0F 40
            CP A',A'                   ; @BT BF
            SET 7,D'                   ; @BT CB FA
            SUB A',A'                  ; @BT 97
            ADD DE',((($39A9-3)+(3+7))-7) ; @BT ED 35 A9 39

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////







    ; TEST-04 generated defines
    .define T4_DEF_001 ((($18+9+3)-9-3)+(2-2))
    .define T4_DEF_002 (((T4_DEF_001)*2+($D0)-(($18)*2))+(11-11))
    .define T4_DEF_003 ((($39A9+6+6)-6-6)+(6-6))
    .define T4_DEF_004 (((T4_DEF_003)+($ECC1)-($39A9))+(5-5))
    .define T4_DEF_005 ((($6E+7)-(7+1)+1)+(1-1))
    .define T4_DEF_006 ((($F216)+(T4_DEF_005)-($6E))+((7+1)-(7+1)))
    .define T4_DEF_007 ((($B21A+6+2)-6-2)+(5-5))
    .define T4_DEF_008 ((T4_DEF_007)+(($D3)-($B21A))+(9-9))
    .define T4_DEF_009 ((($B2*2)-($B2))+($B2)+(5-5)+(4-4)-$B2)
    .define T4_DEF_010 ((($4887)-($B2)+(T4_DEF_009))+(7-7)+(6-6))
    .define T4_DEF_011 ((($B109*2)/2)+(10-10)+(2-2))
    .define T4_DEF_012 ((T4_DEF_011)+(($2889)-($B109))+(6-6))
    .define T4_DEF_013 ((($7A+13)-(13+2)+2)+(9-9))
    .define T4_DEF_014 ((($F105)+(T4_DEF_013)-($7A))+((11+8)-(11+8)))
    .define T4_DEF_015 ((($BBC3+(13*11))-(13*11))+(6-6))
    .define T4_DEF_016 (((T4_DEF_015)+($C3)-($BBC3))+(2-2))
    .define T4_DEF_017 ((($9862+2)-2)+(10-2))
    .define T4_DEF_018 ((($B54D)+(T4_DEF_017)-($9862))+((3+1)-(3+1)))
    .define T4_DEF_019 ((($26+12+4)-12-4)+(4-4))
    .define T4_DEF_020 (((T4_DEF_019)+($2334)-($26))+(3-3))
    .define T4_DEF_021 ((($3787+(3*4))-(3*4))+(2-2))
    .define T4_DEF_022 ((($9660)-($3787)+(T4_DEF_021))+(7-7)+(7-7))
    .define T4_DEF_023 ((($E1+3)-3)+(7-6))
    .define T4_DEF_024 ((T4_DEF_023)+(($90)-($E1))+(11-11))
    .define T4_DEF_025 ((($BAB2*3)/3)+(1-1)+(4-4))
    .define T4_DEF_026 ((T4_DEF_025)+(($C9)-($BAB2))+(5-5))
    .define T4_DEF_027 ((($78*2)-($78))+($78)+(6-6)+(3-3)-$78)
    .define T4_DEF_028 (((T4_DEF_027)*2+($08)-(($78)*2))+(7-7))
    .define T4_DEF_029 ((($D107+8+9)-8-9)+(3-3))
    .define T4_DEF_030 ((($B43C)+(T4_DEF_029)-($D107))+((8+9)-(8+9)))
    .define T4_DEF_031 ((($2556*9)/9)+(11-11)+(9-9))
    .define T4_DEF_032 ((($94)-($2556)+(T4_DEF_031))+(9-9)+(4-4))
    .define T4_DEF_033 ((($EFF4+12+2)-12-2)+(6-6))
    .define T4_DEF_034 ((($53)+(T4_DEF_033)-($EFF4))+((9+8)-(9+8)))
    .define T4_DEF_035 ((($6CC9*12)/12)+(6-6)+(6-6))
    .define T4_DEF_036 ((($75)-($6CC9)+(T4_DEF_035))+(9-9)+(1-1))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with full TEST-04 randomization + varied define replacement
    ; GENERATED: seed = 1425775533
    ; GENERATED: replaced operands = 36 / 73

                LD L',SET 4,(IX+$19)   ; @BT DD CB 19 E5
                LD E',RES 3,(IY+$2E)   ; @BT FD CB 2E 9B
                BIT 4,L'               ; @BT CB 65
                JP (IX)                ; @BT DD E9
                CP A',IXL              ; @BT DD BD
                ADD DE',(((T4_DEF_003-3)+(3+7))-7) ; @BT ED 35 A9 39
                LD C',L'               ; @BT 4D
                ADD BC,A'              ; @BT ED 33
                LD B',SRL (IX+(($73-(7+5))+7+5)) ; @BT DD CB 73 38
                RES 1,E'               ; @BT CB 8B
                LD L',C'               ; @BT 69
                DEC IXH                ; @BT DD 25
                LD A',(((($4221-10)-(1-1))+(1-1)+10)) ; @BT 3A 21 42
                DEC (IX+(($5C-3)+3))   ; @BT DD 35 5C
                LD H',SET 7,(IX+$1B)   ; @BT DD CB 1B FC
                LD L',SRL (IY+(($46-4)+4)) ; @BT FD CB 46 3D
                LD E',L'               ; @BT 5D
                RR H'                  ; @BT CB 1C
                LD A',SRA (IY+((($60-(2+4))+(2+4)))) ; @BT FD CB 60 2F
                LD (IY+((($2F-(8+5))+5)+8)),B' ; @BT FD 70 2F
                SUB A',A'              ; @BT 97
                SET 7,H'               ; @BT CB FC
                RRC H'                 ; @BT CB 0C
                LD H',RRC (IY+((($68-(10+8))+8)+10)) ; @BT FD CB 68 0C
                LD (IY+((($32-8)-(9-1))+(9-1)+8)),C' ; @BT FD 71 32
                BIT 2,B'               ; @BT CB 50
                AND A',IYL             ; @BT FD A5
                RLC (IX+((($41-(3+1))+3)+1)) ; @BT DD CB 41 06
                LD A',SRL (IY+((($66-11)+(11+2))-2)) ; @BT FD CB 66 3F
                MIRROR A'              ; @BT ED 24
                RES 1,C'               ; @BT CB 89
                RRC B'                 ; @BT CB 08
                SET 2,H'               ; @BT CB D4
                SET 6,L'               ; @BT CB F5
                LD H',SET 0,(IY+$74)   ; @BT FD CB 74 C4
                LD IXL,C'              ; @BT DD 69
                BIT 1,D'               ; @BT CB 4A
                LD L',(IY+((($0A-3)+5)-5+3)) ; @BT FD 6E 0A
                POP BC'                ; @BT C1
                DEC DE'                ; @BT 1B
                RES 0,C'               ; @BT CB 81
                BSRF DE',B             ; @BT ED 2B
                RRC (IX+((($09-(7+9))+(7+9)))) ; @BT DD CB 09 0E
                SBC HL',DE'            ; @BT ED 52
                SET 1,D'               ; @BT CB CA
                LD A',IXH              ; @BT DD 7C
                LD E',SET 6,(IX+$05)   ; @BT DD CB 05 F3
                RES 5,L'               ; @BT CB AD
                LD IXL,E'              ; @BT DD 6B
                LD A',RES 2,(IY+$68)   ; @BT FD CB 68 97
                BSRA DE',B             ; @BT ED 29
                LD B',SET 7,(IX+$5B)   ; @BT DD CB 5B F8
                LD H',SLA (IX+((($21-11)-(8-1))+(8-1)+11)) ; @BT DD CB 21 24
                LD B',RLC (IY+((($1C-(4+7))+7)+4)) ; @BT FD CB 1C 00
                LD L',SRL (IX+((($43-(6+1))+(6+1)))) ; @BT DD CB 43 3D
                RR (IY+((($6B-(5+7))+7)+5)) ; @BT FD CB 6B 1E
                OUT (C'),L'            ; @BT ED 69
                LD E',SLL (IX+((($17-6)-(4-1))+(4-1)+6)) ; @BT DD CB 17 33
                ADD DE,A'              ; @BT ED 32
                LD (HL'),H'            ; @BT 74
                RES 5,C'               ; @BT CB A9
                XOR A',H'              ; @BT AC
                RRA                    ; @BT 1F
                SET 0,(HL')            ; @BT CB C6
                SET 3,(IY+$67)         ; @BT FD CB 67 DE
                INI                    ; @BT ED A2
                LD C',SRL (IX+(($03-9)+9)) ; @BT DD CB 03 39
                LD L',SET 6,(IY+$28)   ; @BT FD CB 28 F5
                SET 1,(IX+$28)         ; @BT DD CB 28 CE
                INC DE'                ; @BT 13
                LD L',RES 0,(IX+$39)   ; @BT DD CB 39 85
                RES 7,A'               ; @BT CB BF
                LD L',RES 3,(IX+$4B)   ; @BT DD CB 4B 9D
                LD (IY+((($35-4)+4)-4+4)),D' ; @BT FD 72 35
                SRL (IX+((($54-1)+(1-2))+2)) ; @BT DD CB 54 3E
                RL (IX+((($23-6)+(6-3))+3)) ; @BT DD CB 23 16
                LD H',SET 5,(IY+$12)   ; @BT FD CB 12 EC
                LD E',IXH              ; @BT DD 5C
                LD B',RES 7,(IX+$13)   ; @BT DD CB 13 B8
                SET 1,L'               ; @BT CB CD
                RES 0,L'               ; @BT CB 85
                LD D',L'               ; @BT 55
                ADC A',(((T4_DEF_034-(7+7))+(7+7))) ; @BT CE 53
                LD E',RR (IX+((($5B-4)-8)+4+8)) ; @BT DD CB 5B 1B
                LD A',RL (IY+(($12-(3+9))+3+9)) ; @BT FD CB 12 17
                SBC A',B'              ; @BT 98
                LD D',A'               ; @BT 57
                IM ((2-(7+2))+7+2)     ; @BT ED 5E
                JP (((T4_DEF_012-(10+1))+10)+1) ; @BT C3 89 28
                JR GTE,(((T4_DEF_009-(11+8))+8)+11) ; @BT 30 B2
                LD C',D'               ; @BT 4A
                RES 4,D'               ; @BT CB A2
                BIT 7,A'               ; @BT CB 7F
                LD IXH,A'              ; @BT DD 67
                LD B',RES 5,(IX+$07)   ; @BT DD CB 07 A8
                JP EQU,(((T4_DEF_031-9)-(5-1))+(5-1)+9) ; @BT CA 56 25
                LD L',(HL')            ; @BT 6E
                RES 5,B'               ; @BT CB A8
                LD B',SLL (IY+((($6A-8)-(6-1))+(6-1)+8)) ; @BT FD CB 6A 30
                SL1 H'                 ; @BT CB 34
                RES 5,(IY+$2B)         ; @BT FD CB 2B AE
                LDDRX                  ; @BT ED BC
                LD H',SRL (IX+((($33-2)+(2-2))+2)) ; @BT DD CB 33 3C
                LD E',RES 6,(IY+$40)   ; @BT FD CB 40 B3
                LD D',H'               ; @BT 54
                POP HL'                ; @BT E1
                AND A',A'              ; @BT A7
                LD (DE'),A'            ; @BT 12
                LD C',SLL (IY+((($7A-8)+(8-9))+9)) ; @BT FD CB 7A 31
                RES 5,H'               ; @BT CB AC
                SET 1,E'               ; @BT CB CB
                CP A',(((T4_DEF_023-(10+6))+(10+6))) ; @BT FE E2
                LD B',SET 3,(IY+$46)   ; @BT FD CB 46 D8
                LD A',RES 1,(IY+$62)   ; @BT FD CB 62 8F
                RRD                    ; @BT ED 67
                LDD                    ; @BT ED A8
                CP A',B'               ; @BT B8
                LD A',D'               ; @BT 7A
                LD E',H'               ; @BT 5C
                RST ((($28-(7+4))+(7+4))) ; @BT EF
                RR C'                  ; @BT CB 19
                SET 3,E'               ; @BT CB DB
                LD B',RES 6,(IY+$10)   ; @BT FD CB 10 B0
                LD L',SLL (IX+((($37-6)-(5-1))+(5-1)+6)) ; @BT DD CB 37 35
                LD B',SET 2,(IX+$3D)   ; @BT DD CB 3D D0
                RES 2,A'               ; @BT CB 97
                BIT 0,L'               ; @BT CB 45
                RLC D'                 ; @BT CB 02
                LD L',SET 1,(IY+$0A)   ; @BT FD CB 0A CD
                LD C',RES 1,(IX+$7F)   ; @BT DD CB 7F 89
                LD D',SET 7,(IX+$7B)   ; @BT DD CB 7B FA
                LD D',RES 4,(IX+$21)   ; @BT DD CB 21 A2
                LD C',SLA (IY+((($74-10)+6)-6+10)) ; @BT FD CB 74 21
                INC (HL')              ; @BT 34
                SET 4,A'               ; @BT CB E7
                LD L',RL (IY+((($72-(8+9))+(8+9)))) ; @BT FD CB 72 15
                SET 7,B'               ; @BT CB F8
                LD (IX+((($6A-5)-(1-1))+(1-1)+5)),E' ; @BT DD 73 6A
                RLC H'                 ; @BT CB 04
                LD D',RRC (IY+((($48-4)+(4-3))+3)) ; @BT FD CB 48 0A
                SET 0,E'               ; @BT CB C3
                INC (IX+((($5A-3)+(3+2))-2)) ; @BT DD 34 5A
                DEC E'                 ; @BT 1D
                LD B',RLC (IX+((($19-9)+7)-7+9)) ; @BT DD CB 19 00
                RES 3,L'               ; @BT CB 9D
                CP A',L'               ; @BT BD
                LD C',SET 1,(IY+$4A)   ; @BT FD CB 4A C9
                LD H',RES 1,(IY+$32)   ; @BT FD CB 32 8C
                RRC A'                 ; @BT CB 0F
                SET 1,H'               ; @BT CB CC
                LD L',RES 3,(IY+$4E)   ; @BT FD CB 4E 9D
                LD B',RR (IX+(($2B-(5+1))+5+1)) ; @BT DD CB 2B 18
                CALL NEG,(($B55D-(10+5))+10+5) ; @BT FC 5D B5
                OUT (C'),0             ; @BT ED 71
                DAA                    ; @BT 27
                LD D',RES 4,(IY+$24)   ; @BT FD CB 24 A2
                LD (HL'),(((T4_DEF_032-8)+7)-7+8) ; @BT 36 94
                SETAE                  ; @BT ED 95
                LD D',RES 6,(IY+$30)   ; @BT FD CB 30 B2
                DEC SP                 ; @BT 3B
                BIT 5,E'               ; @BT CB 6B
                LD E',RLC (IY+((($4C-5)+(5-9))+9)) ; @BT FD CB 4C 03
                RET                    ; @BT C9
                LD ((((T4_DEF_033-9)+3)-3+9)),SP ; @BT ED 73 F4 EF
                LD D',E'               ; @BT 53
                BIT 1,C'               ; @BT CB 49
                LD H',A'               ; @BT 67
                SET 0,(IY+$0D)         ; @BT FD CB 0D C6
                LD L',SET 5,(IY+$22)   ; @BT FD CB 22 ED
                BIT 4,E'               ; @BT CB 63
                LD E',((($B1-5)+(5-5))+5) ; @BT 1E B1
                LD SP,((((T4_DEF_011-8)+(8-3))+3)) ; @BT ED 7B 09 B1
                LD D',RES 0,(IY+$0C)   ; @BT FD CB 0C 82
                LD L',SLA (IX+((($31-3)+9)-9+3)) ; @BT DD CB 31 25
                LD C',C'               ; @BT 49
                LD D',SET 1,(IX+$57)   ; @BT DD CB 57 CA
                BIT 7,L'               ; @BT CB 7D
                LD B',RES 6,(IX+$0D)   ; @BT DD CB 0D B0
                LD IXL,IXH             ; @BT DD 6C
                LD L',RES 6,(IX+$5D)   ; @BT DD CB 5D B5
                CPL                    ; @BT 2F
                LD SP,(((T4_DEF_007-(2+1))+2)+1) ; @BT ED 7B 1A B2
                LD SP,IY               ; @BT FD F9
                RES 1,B'               ; @BT CB 88
                LD B',RES 0,(IX+$69)   ; @BT DD CB 69 80
                LD D',SET 5,(IX+$6F)   ; @BT DD CB 6F EA
                RL A'                  ; @BT CB 17
                JR ((($F7-11)+(11-1))+1) ; @BT 18 F7
                SLA (IX+((($18-(6+9))+(6+9)))) ; @BT DD CB 18 26
                LD L',H'               ; @BT 6C
                ADD A',B'              ; @BT 80
                SET 6,(IX+$3E)         ; @BT DD CB 3E F6
                RLC (HL')              ; @BT CB 06
                LD H',(IX+(($48-(11+9))+11+9)) ; @BT DD 66 48
                LD B',RR (IY+((($2E-(1+1))+1)+1)) ; @BT FD CB 2E 18
                DEC BC'                ; @BT 0B
                LD D',SRL (IY+(($16-(5+2))+5+2)) ; @BT FD CB 16 3A
                LD A',SET 6,(IX+$45)   ; @BT DD CB 45 F7
                SET 0,C'               ; @BT CB C1
                NEXTREG ((($DA-(4+3))+3)+4),A' ; @BT ED 92 DA
                BIT 6,(IY+$25)         ; @BT FD CB 25 76
                SBC HL',BC'            ; @BT ED 42
                LD H',RES 4,(IY+$44)   ; @BT FD CB 44 A4
                SUB A',D'              ; @BT 92
                LD C',E'               ; @BT 4B
                LD (IX+((($70-(7+6))+(7+6)))),L' ; @BT DD 75 70
                LD A',I                ; @BT ED 57
                INC IXH                ; @BT DD 24
                LD ((((T4_DEF_004-(5+7))+7)+5)),BC' ; @BT ED 43 C1 EC
                RES 4,L'               ; @BT CB A5
                RES 3,(HL')            ; @BT CB 9E
                LD H',RES 7,(IY+$56)   ; @BT FD CB 56 BC
                LD B',SRL (IY+((($76-(6+1))+6)+1)) ; @BT FD CB 76 38
                ADC A',(HL')           ; @BT 8E
                LD C',RES 3,(IY+$0E)   ; @BT FD CB 0E 99
                LD (IY+((($38-(4+8))+(4+8)))),E' ; @BT FD 73 38
                LD L',RLC (IX+((($69-(6+1))+6)+1)) ; @BT DD CB 69 05
                LD A',IXL              ; @BT DD 7D
                RLD                    ; @BT ED 6F
                INC C'                 ; @BT 0C
                LD C',SET 0,(IY+$44)   ; @BT FD CB 44 C1
                SL1 E'                 ; @BT CB 33
                OR A',B'               ; @BT B0
                ADD HL',SP             ; @BT 39
                LD H',SET 3,(IY+$06)   ; @BT FD CB 06 DC
                RES 0,(IX+$12)         ; @BT DD CB 12 86
                RES 2,L'               ; @BT CB 95
                DEC IY                 ; @BT FD 2B
                SET 0,D'               ; @BT CB C2
                SRA B'                 ; @BT CB 28
                LD (HL'),L'            ; @BT 75
                BIT 4,C'               ; @BT CB 61
                SET 5,L'               ; @BT CB ED
                LD H',RR (IY+((($6E-(7+4))+(7+4)))) ; @BT FD CB 6E 1C
                LD H',RES 1,(IX+$2F)   ; @BT DD CB 2F 8C
                LD H',((($AE-10)-(4-1))+(4-1)+10) ; @BT 26 AE
                BIT 2,E'               ; @BT CB 53
                LD A',RLC (IY+(($0C-(6+9))+6+9)) ; @BT FD CB 0C 07
                LD E',RES 0,(IY+$1C)   ; @BT FD CB 1C 83
                LD (IY+((($3B-9)+5)-5+9)),H' ; @BT FD 74 3B
                RLC (IX+((($41-4)+(4-2))+2)) ; @BT DD CB 41 06
                RETI                   ; @BT ED 4D
                LD C',RLC (IX+((($29-(7+6))+6)+7)) ; @BT DD CB 29 01
                SBC A',D'              ; @BT 9A
                LD E',SET 2,(IX+$6D)   ; @BT DD CB 6D D3
                IN E',(C')             ; @BT ED 58
                LD E',SRL (IX+((($23-3)-(9-1))+(9-1)+3)) ; @BT DD CB 23 3B
                INC A'                 ; @BT 3C
                LD D',IXL              ; @BT DD 55
                LD H',RRC (IX+((($65-(7+8))+(7+8)))) ; @BT DD CB 65 0C
                BIT 4,(IY+$69)         ; @BT FD CB 69 66
                LD L',RES 5,(IY+$5A)   ; @BT FD CB 5A AD
                LD D',SRA (IX+((($0D-6)+(6-2))+2)) ; @BT DD CB 0D 2A
                LD DE',((($6DDA-6)+(6-4))+4) ; @BT ED 5B DA 6D
                LD E',RR (IY+((($5E-3)+(3+8))-8)) ; @BT FD CB 5E 1B
                BIT 1,(IX+$0C)         ; @BT DD CB 0C 4E
                SBC A',(($A1-8)+8)     ; @BT DE A1
                LD A',SET 7,(IY+$4E)   ; @BT FD CB 4E FF
                INDR                   ; @BT ED BA
                LD D',(IX+((($74-(2+7))+(2+7)))) ; @BT DD 56 74
                BIT 7,(IY+$43)         ; @BT FD CB 43 7E
                LD H',SLL (IX+((($27-6)-2)+6+2)) ; @BT DD CB 27 34
                JR NEQ,(((T4_DEF_026-(8+1))+8)+1) ; @BT 20 C9
                LD E',(IX+((($21-(6+4))+(6+4)))) ; @BT DD 5E 21
                LD A',RRC (IX+(($15-(5+8))+5+8)) ; @BT DD CB 15 0F
                BIT 6,B'               ; @BT CB 70
                INC BC'                ; @BT 03
                SRL D'                 ; @BT CB 3A
                SET 4,L'               ; @BT CB E5
                LD A',SET 2,(IX+$2D)   ; @BT DD CB 2D D7
                LD IX,((((T4_DEF_030-8)+6)-6+8)) ; @BT DD 2A 3C B4
                SRA L'                 ; @BT CB 2D
                LD IYH,B'              ; @BT FD 60
                SBC A',E'              ; @BT 9B
                LD H',RES 2,(IX+$35)   ; @BT DD CB 35 94
                LD (IX+(($5E-(3+2))+3+2)),A' ; @BT DD 77 5E
                LDDX                   ; @BT ED AC
                AND A',(IX+((($47-1)+(1-3))+3)) ; @BT DD A6 47
                RES 7,C'               ; @BT CB B9
                XOR A',L'              ; @BT AD
                DEC C'                 ; @BT 0D
                LD L',RES 2,(IY+$48)   ; @BT FD CB 48 95
                LD B',RES 5,(IY+$0A)   ; @BT FD CB 0A A8
                DEC (HL')              ; @BT 35
                LD H',SET 4,(IY+$0C)   ; @BT FD CB 0C E4
                ADD IX,IX              ; @BT DD 29
                LD IYH,A'              ; @BT FD 67
                RES 4,H'               ; @BT CB A4
                CP A',IXH              ; @BT DD BC
                LD H',H'               ; @BT 64
                CP A',A'               ; @BT BF
                ADC A',A'              ; @BT 8F
                CP A',(IY+((($1A-(8+1))+8)+1)) ; @BT FD BE 1A
                SBC A',(IY+((($5A-(5+6))+6)+5)) ; @BT FD 9E 5A
                SET 2,B'               ; @BT CB D0
                SET 3,(IX+$64)         ; @BT DD CB 64 DE
                RES 5,D'               ; @BT CB AA
                LD A',SRA (IX+((($5D-6)+(6-9))+9)) ; @BT DD CB 5D 2F
                JP POS,((($2445-10)-8)+10+8) ; @BT F2 45 24
                LD C',SET 7,(IY+$6E)   ; @BT FD CB 6E F9
                IN D',(C')             ; @BT ED 50
                LD E',RES 7,(IY+$46)   ; @BT FD CB 46 BB
                SBC A',L'              ; @BT 9D
                LD B',E'               ; @BT 43
                OR A',A'               ; @BT B7
                LD IXL,(((T4_DEF_002-11)+9)-9+11) ; @BT DD 2E D0
                XOR A',(HL')           ; @BT AE
                LD H',RES 3,(IY+$3E)   ; @BT FD CB 3E 9C
                LD C',RES 5,(IY+$1A)   ; @BT FD CB 1A A9
                SLA (IY+((($1B-4)+(4-2))+2)) ; @BT FD CB 1B 26
                LD A',RES 3,(IX+$6B)   ; @BT DD CB 6B 9F
                LD B',C'               ; @BT 41
                LD C',SET 2,(IX+$4D)   ; @BT DD CB 4D D1
                LD L',SET 2,(IY+$10)   ; @BT FD CB 10 D5
                LD HL',(($9771-4)+4)   ; @BT 2A 71 97
                IND                    ; @BT ED AA
                OR A',L'               ; @BT B5
                BSLA DE',B             ; @BT ED 28
                LD D',RES 0,(IX+$09)   ; @BT DD CB 09 82
                LD A',R                ; @BT ED 5F
                SET 2,(IY+$49)         ; @BT FD CB 49 D6
                LD H',RES 2,(IY+$38)   ; @BT FD CB 38 94
                EX (SP),IX             ; @BT DD E3
                LD H',SRL (IY+((($36-8)+5)-5+8)) ; @BT FD CB 36 3C
                IN B',(C')             ; @BT ED 40
                DJNZ ((($65-(3+6))+(3+6))) ; @BT 10 65
                RET POS                ; @BT F0
                RET NEG                ; @BT F8
                LD H',SET 3,(IX+$03)   ; @BT DD CB 03 DC
                LD H',SET 6,(IY+$18)   ; @BT FD CB 18 F4
                LD L',B'               ; @BT 68
                LD C',SET 6,(IY+$68)   ; @BT FD CB 68 F1
                XOR A',IYH             ; @BT FD AC
                RLC A'                 ; @BT CB 07
                LD H',SLA (IY+((($24-7)-4)+7+4)) ; @BT FD CB 24 24
                ADC HL',HL'            ; @BT ED 6A
                LD E',IYL              ; @BT FD 5D
                RLC E'                 ; @BT CB 03
                LD L',SLA (IY+((($34-11)-(1-1))+(1-1)+11)) ; @BT FD CB 34 25
                LD C',(IX+((($3E-2)+(2-9))+9)) ; @BT DD 4E 3E
                SET 3,D'               ; @BT CB DA
                SUB A',C'              ; @BT 91
                RL D'                  ; @BT CB 12
                LD C',RL (IY+((($32-(4+1))+4)+1)) ; @BT FD CB 32 11
                BIT 6,L'               ; @BT CB 75
                SUB A',((($4C-4)-(5-1))+(5-1)+4) ; @BT D6 4C
                LD H',SRA (IY+((($30-6)+(6-4))+4)) ; @BT FD CB 30 2C
                LD L',RES 5,(IX+$57)   ; @BT DD CB 57 AD
                LD IXL,A'              ; @BT DD 6F
                CP A',(HL')            ; @BT BE
                LD D',RES 7,(IX+$33)   ; @BT DD CB 33 BA
                AND A',D'              ; @BT A2
                LD L',RES 7,(IY+$66)   ; @BT FD CB 66 BD
                LD A',H'               ; @BT 7C
                LD E',SLA (IX+((($11-6)+(6-6))+6)) ; @BT DD CB 11 23
                LD A',SET 4,(IY+$3C)   ; @BT FD CB 3C E7
                LD L',A'               ; @BT 6F
                SBC HL',HL'            ; @BT ED 62
                SET 5,(HL')            ; @BT CB EE
                LD C',RRC (IY+((($38-4)+(4-8))+8)) ; @BT FD CB 38 09
                BIT 0,B'               ; @BT CB 40
                RES 7,E'               ; @BT CB BB
                SUB A',H'              ; @BT 94
                LD C',RES 0,(IY+$7C)   ; @BT FD CB 7C 81
                SET 0,H'               ; @BT CB C4
                LD H',RES 0,(IX+$29)   ; @BT DD CB 29 84
                RL B'                  ; @BT CB 10
                BIT 2,(HL')            ; @BT CB 56
                LD L',((($91-(5+1))+5)+1) ; @BT 2E 91
                LD C',RR (IX+((($3B-10)+(10-4))+4)) ; @BT DD CB 3B 19
                RES 6,B'               ; @BT CB B0
                LD H',RLC (IX+((($59-2)+(2+9))-9)) ; @BT DD CB 59 04
                LD A',RLC (IX+((($09-(4+3))+3)+4)) ; @BT DD CB 09 07
                LD B',IYH              ; @BT FD 44
                OR A',(IX+((($14-9)-9)+9+9)) ; @BT DD B6 14
                LD BC',(((($4776-(9+8))+8)+9)) ; @BT ED 4B 76 47
                LD D',RES 3,(IX+$1B)   ; @BT DD CB 1B 9A
                LD B',SET 4,(IX+$49)   ; @BT DD CB 49 E0
                RES 4,B'               ; @BT CB A0
                LD IXH,C'              ; @BT DD 61
                RES 6,A'               ; @BT CB B7
                LD B',RES 4,(IX+$01)   ; @BT DD CB 01 A0
                OTDR                   ; @BT ED BB
                SL1 (IX+((($7A-5)+6)-6+5)) ; @BT DD CB 7A 36
                DI                     ; @BT F3
                LD C',RES 7,(IX+$23)   ; @BT DD CB 23 B9
                LD E',RL (IX+((($4F-(9+9))+9)+9)) ; @BT DD CB 4F 13
                INC H'                 ; @BT 24
                LD D',RLC (IY+((($3C-11)-(5-1))+(5-1)+11)) ; @BT FD CB 3C 02
                SET 5,(IX+$20)         ; @BT DD CB 20 EE
                OUTINB                 ; @BT ED 90
                CALL LSS,((($B44C-(10+1))+10)+1) ; @BT DC 4C B4
                LD D',C'               ; @BT 51
                LD A',RES 5,(IX+$77)   ; @BT DD CB 77 AF
                SET 4,H'               ; @BT CB E4
                LD H',SET 4,(IX+$09)   ; @BT DD CB 09 E4
                OR A',D'               ; @BT B2
                SET 5,B'               ; @BT CB E8
                LD H',B'               ; @BT 60
                LD E',E'               ; @BT 5B
                RRC (IY+((($0C-(3+1))+3)+1)) ; @BT FD CB 0C 0E
                PUSH IY                ; @BT FD E5
                BIT 3,E'               ; @BT CB 5B
                BIT 1,B'               ; @BT CB 48
                LD B',(($61-(4+3))+4+3) ; @BT 06 61
                SET 1,(HL')            ; @BT CB CE
                ADD A',C'              ; @BT 81
                LD B',SET 2,(IY+$40)   ; @BT FD CB 40 D0
                BIT 4,(IX+$66)         ; @BT DD CB 66 66
                SRA A'                 ; @BT CB 2F
                PIXELAD                ; @BT ED 94
                AND A',IXL             ; @BT DD A5
                AND A',IYH             ; @BT FD A4
                LD A',SET 4,(IX+$39)   ; @BT DD CB 39 E7
                LD IXH,E'              ; @BT DD 63
                SET 6,C'               ; @BT CB F1
                CALL GTE,((($B66E-(1+1))+1)+1) ; @BT D4 6E B6
                SET 6,H'               ; @BT CB F4
                LD D',IYH              ; @BT FD 54
                BIT 3,C'               ; @BT CB 59
                LD (IX+((($67-5)+(5-9))+9)),D' ; @BT DD 72 67
                ADD A',L'              ; @BT 85
                LD E',D'               ; @BT 5A
                LD (IY+(($41-11)+11)),((($DE-8)+8)-8+8) ; @BT FD 36 41 DE
                XOR A',E'              ; @BT AB
                JP GTE,((($2223-2)+(2-9))+9) ; @BT D2 23 22
                LD E',RES 6,(IX+$3D)   ; @BT DD CB 3D B3
                SET 1,B'               ; @BT CB C8
                INIR                   ; @BT ED B2
                IN A',(C')             ; @BT ED 78
                LD C',RLC (IY+((($2C-9)+(9+1))-1)) ; @BT FD CB 2C 01
                LD A',B'               ; @BT 78
                SLA C'                 ; @BT CB 21
                LDIRX                  ; @BT ED B4
                XOR A',IXH             ; @BT DD AC
                LD B',SET 5,(IX+$4F)   ; @BT DD CB 4F E8
                BIT 6,A'               ; @BT CB 77
                LD A',IYH              ; @BT FD 7C
                CP A',IYH              ; @BT FD BC
                BIT 1,(HL')            ; @BT CB 4E
                INC E'                 ; @BT 1C
                SET 5,H'               ; @BT CB EC
                SL1 C'                 ; @BT CB 31
                JP (HL')               ; @BT E9
                CALL PE,((T4_DEF_025-10)+10) ; @BT EC B2 BA
                LD A',RL (IX+((($0F-(3+1))+3)+1)) ; @BT DD CB 0F 17
                INC IYL                ; @BT FD 2C
                RES 5,A'               ; @BT CB AF
                LD C',(($BA-(10+8))+10+8) ; @BT 0E BA
                SET 4,(IX+$02)         ; @BT DD CB 02 E6
                OUT ((((T4_DEF_036-9)+(9+7))-7)),A' ; @BT D3 75
                EX AF',AF              ; @BT 08
                RST ((($18-5)+(5-3))+3) ; @BT DF
                SBC A',(HL')           ; @BT 9E
                LD D',IXH              ; @BT DD 54
                LD E',(HL')            ; @BT 5E
                LD IXH,(((T4_DEF_001-9)+4)-4+9) ; @BT DD 26 18
                LD H',SET 7,(IY+$1E)   ; @BT FD CB 1E FC
                LD H',C'               ; @BT 61
                LD E',RRC (IY+(($58-(3+2))+3+2)) ; @BT FD CB 58 0B
                LD E',RES 1,(IX+$1F)   ; @BT DD CB 1F 8B
                LD (HL'),A'            ; @BT 77
                LD L',SET 1,(IX+$07)   ; @BT DD CB 07 CD
                XOR A',(IX+((($2B-5)+(5-9))+9)) ; @BT DD AE 2B
                LD B',SET 6,(IX+$55)   ; @BT DD CB 55 F0
                LD C',SET 5,(IY+$62)   ; @BT FD CB 62 E9
                JP LSS,((($2001-(8+1))+8)+1) ; @BT DA 01 20
                LD (HL'),E'            ; @BT 73
                BIT 4,H'               ; @BT CB 64
                XOR A',(IY+((($34-(1+1))+1)+1)) ; @BT FD AE 34
                SLA L'                 ; @BT CB 25
                SET 0,(IY+$0D)         ; @BT FD CB 0D C6
                LD D',SRA (IY+((($10-5)+6)-6+5)) ; @BT FD CB 10 2A
                OUT (C'),H'            ; @BT ED 61
                LD H',SET 1,(IY+$7A)   ; @BT FD CB 7A CC
                LD C',RES 6,(IX+$1D)   ; @BT DD CB 1D B1
                LD A',IYL              ; @BT FD 7D
                LD A',RES 3,(IY+$6E)   ; @BT FD CB 6E 9F
                LD L',RL (IX+((($6F-(4+1))+4)+1)) ; @BT DD CB 6F 15
                LD D',SET 5,(IY+$72)   ; @BT FD CB 72 EA
                LD C',IYL              ; @BT FD 4D
                LD B',IXL              ; @BT DD 45
                LD D',RES 7,(IY+$36)   ; @BT FD CB 36 BA
                LD L',RLC (IY+((($6C-(9+8))+(9+8)))) ; @BT FD CB 6C 05
                IN L',(C')             ; @BT ED 68
                RES 1,(IY+$33)         ; @BT FD CB 33 8E
                LD D',SLA (IY+((($04-10)+(10+7))-7)) ; @BT FD CB 04 22
                SRA (HL')              ; @BT CB 2E
                LD B',RES 4,(IY+$04)   ; @BT FD CB 04 A0
                BIT 5,(HL')            ; @BT CB 6E
                RES 7,(IX+$64)         ; @BT DD CB 64 BE
                LD L',RR (IY+((($7E-9)+(9-8))+8)) ; @BT FD CB 7E 1D
                LD A',(HL')            ; @BT 7E
                INC IYH                ; @BT FD 24
                RST ((($30-6)+4)-4+6)  ; @BT F7
                RRC D'                 ; @BT CB 0A
                LD HL',(((T4_DEF_022-4)+4)) ; @BT 2A 60 96
                LDWS                   ; @BT ED A5
                LDDR                   ; @BT ED B8
                RES 4,(IY+$0D)         ; @BT FD CB 0D A6
                SET 2,(HL')            ; @BT CB D6
                LD E',SET 4,(IY+$7C)   ; @BT FD CB 7C E3
                LD C',B'               ; @BT 48
                CALL EQU,((($B9A1-5)+1)-1+5) ; @BT CC A1 B9
                BIT 6,C'               ; @BT CB 71
                BIT 6,H'               ; @BT CB 74
                LD A',SET 7,(IX+$4B)   ; @BT DD CB 4B FF
                SET 2,C'               ; @BT CB D1
                BIT 3,H'               ; @BT CB 5C
                INC IY                 ; @BT FD 23
                LD SP,HL'              ; @BT F9
                INC IXL                ; @BT DD 2C
                LD E',SLL (IY+((($1A-(1+1))+1)+1)) ; @BT FD CB 1A 33
                ADC A',B'              ; @BT 88
                LD (IX+((($73-5)+4)-4+5)),(($88-6)+6) ; @BT DD 36 73 88
                LD IYL,IYH             ; @BT FD 6C
                LD (IX+((($61-6)+(6-3))+3)),B' ; @BT DD 70 61
                RETN                   ; @BT ED 45
                BIT 2,(IX+$2A)         ; @BT DD CB 2A 56
                LD (((($EBB0-(5+5))+(5+5)))),A' ; @BT 32 B0 EB
                DEC B'                 ; @BT 05
                DEC D'                 ; @BT 15
                SET 5,A'               ; @BT CB EF
                LD E',SET 2,(IY+$70)   ; @BT FD CB 70 D3
                LD (IY+((($3E-9)-5)+9+5)),L' ; @BT FD 75 3E
                RES 3,B'               ; @BT CB 98
                ADD A',(IY+((($6D-11)-(1-1))+(1-1)+11)) ; @BT FD 86 6D
                RL L'                  ; @BT CB 15
                BIT 6,(HL')            ; @BT CB 76
                SBC A',IYH             ; @BT FD 9C
                SRA D'                 ; @BT CB 2A
                RES 1,D'               ; @BT CB 8A
                LD IYL,E'              ; @BT FD 6B
                RR A'                  ; @BT CB 1F
                OUTI                   ; @BT ED A3
                LD BC',(((T4_DEF_010-4)-4)+4+4) ; @BT ED 4B 87 48
                BIT 2,A'               ; @BT CB 57
                LD D',SET 0,(IY+$54)   ; @BT FD CB 54 C2
                RES 0,A'               ; @BT CB 87
                BIT 3,D'               ; @BT CB 5A
                SET 7,D'               ; @BT CB FA
                LD C',SET 3,(IX+$53)   ; @BT DD CB 53 D9
                ADD HL,A'              ; @BT ED 31
                BIT 7,B'               ; @BT CB 78
                RES 2,C'               ; @BT CB 91
                LD D',RLC (IX+((($39-(10+4))+4)+10)) ; @BT DD CB 39 02
                SET 3,H'               ; @BT CB DC
                LD H',(IY+((($4B-(7+5))+5)+7)) ; @BT FD 66 4B
                SET 3,B'               ; @BT CB D8
                SET 1,(IY+$2B)         ; @BT FD CB 2B CE
                RR (IX+((($68-9)-(9-1))+(9-1)+9)) ; @BT DD CB 68 1E
                CPIR                   ; @BT ED B1
                LD E',RES 2,(IY+$28)   ; @BT FD CB 28 93
                SBC A',IYL             ; @BT FD 9D
                BIT 1,H'               ; @BT CB 4C
                BIT 5,(IX+$04)         ; @BT DD CB 04 6E
                SET 0,(IX+$0A)         ; @BT DD CB 0A C6
                XOR A',IYL             ; @BT FD AD
                RST (($08-(10+2))+10+2) ; @BT CF
                LD L',SET 3,(IY+$16)   ; @BT FD CB 16 DD
                RR L'                  ; @BT CB 1D
                SET 4,D'               ; @BT CB E2
                OR A',(((T4_DEF_027-(11+4))+(11+4))) ; @BT F6 78
                HALT                   ; @BT 76
                LD L',RES 2,(IX+$45)   ; @BT DD CB 45 95
                BIT 5,H'               ; @BT CB 6C
                LD H',D'               ; @BT 62
                BRLC DE',B             ; @BT ED 2C
                RES 2,(IY+$51)         ; @BT FD CB 51 96
                INC L'                 ; @BT 2C
                LD B',SLL (IX+((($67-6)+5)-5+6)) ; @BT DD CB 67 30
                CALL POS,((($B890-(4+4))+4)+4) ; @BT F4 90 B8
                RES 6,(IY+$49)         ; @BT FD CB 49 B6
                RR B'                  ; @BT CB 18
                AND A',C'              ; @BT A1
                LD A',(((T4_DEF_016-(3+9))+9)+3) ; @BT 3A C3 00
                BIT 2,D'               ; @BT CB 52
                ADD IX,BC'             ; @BT DD 09
                RST                    ; @BT C7
                BIT 5,A'               ; @BT CB 6F
                RLC B'                 ; @BT CB 00
                LD H',SET 2,(IX+$7D)   ; @BT DD CB 7D D4
                LD (((T4_DEF_006-9)+9)),IX ; @BT DD 22 16 F2
                LD B',SRA (IY+(($70-11)+11)) ; @BT FD CB 70 28
                LD A',SET 5,(IY+$42)   ; @BT FD CB 42 EF
                SCF                    ; @BT 37
                SRA (IX+((($36-9)+6)-6+9)) ; @BT DD CB 36 2E
                ADD IY,SP              ; @BT FD 39
                LD D',RES 1,(IX+$0F)   ; @BT DD CB 0F 8A
                LD B',RES 3,(IY+$7E)   ; @BT FD CB 7E 98
                LD D',(HL')            ; @BT 56
                ADD IY,IY              ; @BT FD 29
                LD C',IYH              ; @BT FD 4C
                SBC A',IXL             ; @BT DD 9D
                OR A',(IY+((($1D-2)+(2-11))+11)) ; @BT FD B6 1D
                BIT 0,(IX+$6E)         ; @BT DD CB 6E 46
                SET 3,A'               ; @BT CB DF
                LD C',IXL              ; @BT DD 4D
                IN C',(C')             ; @BT ED 48
                SUB A',E'              ; @BT 93
                OR A',IYL              ; @BT FD B5
                LD H',RL (IX+(($5F-(10+9))+10+9)) ; @BT DD CB 5F 14
                BIT 5,(IY+$07)         ; @BT FD CB 07 6E
                LD IYH,D'              ; @BT FD 62
                LD E',RRC (IX+((($55-(4+1))+(4+1)))) ; @BT DD CB 55 0B
                LD C',RES 5,(IX+$17)   ; @BT DD CB 17 A9
                LD C',SRL (IY+((($06-8)+(8-10))+10)) ; @BT FD CB 06 39
                TEST ((T4_DEF_013-(9+2))+9+2) ; @BT ED 27 7A
                LD A',RES 5,(IY+$7A)   ; @BT FD CB 7A AF
                RRC L'                 ; @BT CB 0D
                LD B',IXH              ; @BT DD 44
                LD B',RES 3,(IX+$7B)   ; @BT DD CB 7B 98
                BIT 7,D'               ; @BT CB 7A
                RES 7,B'               ; @BT CB B8
                BIT 1,E'               ; @BT CB 4B
                LDIR                   ; @BT ED B0
                RLC C'                 ; @BT CB 01
                LD C',H'               ; @BT 4C
                SLA H'                 ; @BT CB 24
                LD E',C'               ; @BT 59
                JP PE,((($2667-1)+(1-11))+11) ; @BT EA 67 26
                SRA (IY+((($39-6)+8)-8+6)) ; @BT FD CB 39 2E
                PUSH AF'               ; @BT F5
                RL E'                  ; @BT CB 13
                BIT 3,(IX+$48)         ; @BT DD CB 48 5E
                RR E'                  ; @BT CB 1B
                LD L',RES 1,(IX+$3F)   ; @BT DD CB 3F 8D
                BIT 0,D'               ; @BT CB 42
                SBC A',A'              ; @BT 9F
                LD L',RES 6,(IY+$60)   ; @BT FD CB 60 B5
                LD D',RES 2,(IX+$15)   ; @BT DD CB 15 92
                LD H',L'               ; @BT 65
                LD A',SET 3,(IY+$36)   ; @BT FD CB 36 DF
                RES 6,C'               ; @BT CB B1
                SET 0,A'               ; @BT CB C7
                LD L',SET 5,(IX+$1F)   ; @BT DD CB 1F ED
                RRC (HL')              ; @BT CB 0E
                LD C',RRC (IX+((($35-10)+1)-1+10)) ; @BT DD CB 35 09
                LD D',RES 5,(IY+$2A)   ; @BT FD CB 2A AA
                LD A',SET 0,(IX+$21)   ; @BT DD CB 21 C7
                LD D',RL (IY+((($42-(11+7))+(11+7)))) ; @BT FD CB 42 12
                RES 0,E'               ; @BT CB 83
                LD E',RES 4,(IY+$34)   ; @BT FD CB 34 A3
                BIT 7,(HL')            ; @BT CB 7E
                BIT 3,B'               ; @BT CB 58
                LD E',RES 4,(IX+$31)   ; @BT DD CB 31 A3
                RL (IY+((($26-(5+1))+5)+1)) ; @BT FD CB 26 16
                SRL A'                 ; @BT CB 3F
                IN F',(C')             ; @BT ED 70
                BIT 4,(HL')            ; @BT CB 66
                SUB A',B'              ; @BT 90
                LD D',SET 2,(IX+$5D)   ; @BT DD CB 5D D2
                ADC A',(IX+((($2B-9)+(9-2))+2)) ; @BT DD 8E 2B
                LD IYL,A'              ; @BT FD 6F
                SET 5,C'               ; @BT CB E9
                LD B',RES 1,(IY+$72)   ; @BT FD CB 72 88
                SET 3,C'               ; @BT CB D9
                LD IYH,E'              ; @BT FD 63
                LD A',SET 1,(IY+$2A)   ; @BT FD CB 2A CF
                LD H',RES 6,(IY+$50)   ; @BT FD CB 50 B4
                LD A',RES 6,(IX+$7D)   ; @BT DD CB 7D B7
                RES 1,H'               ; @BT CB 8C
                LD C',RL (IX+((($2F-10)+(10+8))-8)) ; @BT DD CB 2F 11
                RST ((($10-4)+(4-8))+8) ; @BT D7
                BIT 1,L'               ; @BT CB 4D
                SET 2,(IX+$46)         ; @BT DD CB 46 D6
                LD B',SET 5,(IY+$52)   ; @BT FD CB 52 E8
                LD SP,IX               ; @BT DD F9
                LD H',RES 5,(IX+$47)   ; @BT DD CB 47 AC
                RES 3,(IX+$6C)         ; @BT DD CB 6C 9E
                OUT (C'),A'            ; @BT ED 79
                LD D',SLL (IY+((($0A-3)+(3+5))-5)) ; @BT FD CB 0A 32
                LD A',RR (IX+((($1B-(11+7))+(11+7)))) ; @BT DD CB 1B 1F
                OUTD                   ; @BT ED AB
                LD D',SRL (IX+(($13-6)+6)) ; @BT DD CB 13 3A
                LD B',A'               ; @BT 47
                LD A',RES 1,(IX+$5F)   ; @BT DD CB 5F 8F
                SL1 (HL')              ; @BT CB 36
                LDIX                   ; @BT ED A4
                RL (HL')               ; @BT CB 16
                SL1 B'                 ; @BT CB 30
                CALL PO,(((T4_DEF_015-7)-(1-1))+(1-1)+7) ; @BT E4 C3 BB
                RES 4,A'               ; @BT CB A7
                BIT 4,B'               ; @BT CB 60
                SRL L'                 ; @BT CB 3D
                RES 3,(IY+$6F)         ; @BT FD CB 6F 9E
                XOR A',B'              ; @BT A8
                SET 7,(HL')            ; @BT CB FE
                LD E',SET 0,(IY+$64)   ; @BT FD CB 64 C3
                LD L',RRC (IY+(($78-(4+7))+4+7)) ; @BT FD CB 78 0D
                RES 3,A'               ; @BT CB 9F
                AND A',H'              ; @BT A4
                RES 4,(IX+$0A)         ; @BT DD CB 0A A6
                ADD A',H'              ; @BT 84
                LD B',(HL')            ; @BT 46
                LD C',RES 3,(IX+$0B)   ; @BT DD CB 0B 99
                SET 2,D'               ; @BT CB D2
                SBC A',H'              ; @BT 9C
                CCF                    ; @BT 3F
                ADD A',(HL')           ; @BT 86
                LD C',SET 6,(IX+$65)   ; @BT DD CB 65 F1
                LD (((($EDD2-(2+1))+2)+1)),DE' ; @BT ED 53 D2 ED
                RES 7,H'               ; @BT CB BC
                SET 4,B'               ; @BT CB E0
                ADC A',IXL             ; @BT DD 8D
                LD D',SET 4,(IX+$69)   ; @BT DD CB 69 E2
                RLA                    ; @BT 17
                ADC A',C'              ; @BT 89
                LD ((($EEE3-4)+4)),HL' ; @BT 22 E3 EE
                RLC L'                 ; @BT CB 05
                RES 2,B'               ; @BT CB 90
                AND A',(IY+(($50-(2+4))+2+4)) ; @BT FD A6 50
                RRCA                   ; @BT 0F
                BIT 7,C'               ; @BT CB 79
                ADD A',E'              ; @BT 83
                RES 6,L'               ; @BT CB B5
                SET 2,A'               ; @BT CB D7
                ADC HL',BC'            ; @BT ED 4A
                LD L',L'               ; @BT 6D
                LD D',RRC (IX+(($45-(8+7))+8+7)) ; @BT DD CB 45 0A
                ADC A',H'              ; @BT 8C
                RES 0,D'               ; @BT CB 82
                ADC A',L'              ; @BT 8D
                LD L',SLL (IY+((($3A-(8+1))+8)+1)) ; @BT FD CB 3A 35
                POP DE'                ; @BT D1
                JR LSS,((($9B-4)-3)+4+3) ; @BT 38 9B
                BIT 2,L'               ; @BT CB 55
                PUSH (((T4_DEF_017-(7+1))+7)+1) ; @BT ED 8A 98 6A
                SET 7,L'               ; @BT CB FD
                RES 3,D'               ; @BT CB 9A
                LD D',RR (IX+(($4B-(3+3))+3+3)) ; @BT DD CB 4B 1A
                SLA (HL')              ; @BT CB 26
                ADC A',E'              ; @BT 8B
                SET 2,E'               ; @BT CB D3
                CP A',(IX+((($11-4)-(7-1))+(7-1)+4)) ; @BT DD BE 11
                LD (IX+((($64-7)+(7-4))+4)),C' ; @BT DD 71 64
                LD E',SET 1,(IX+$67)   ; @BT DD CB 67 CB
                LD E',RES 5,(IX+$37)   ; @BT DD CB 37 AB
                SET 7,(IX+$5C)         ; @BT DD CB 5C FE
                EX (SP),IY             ; @BT FD E3
                ADC HL',DE'            ; @BT ED 5A
                RES 6,E'               ; @BT CB B3
                XOR A',C'              ; @BT A9
                LD IYH,IYH             ; @BT FD 64
                SL1 L'                 ; @BT CB 35
                LD E',SET 0,(IX+$61)   ; @BT DD CB 61 C3
                OR A',IXH              ; @BT DD B4
                LD (HL'),D'            ; @BT 72
                RST ((($00-(11+4))+4)+11) ; @BT C7
                CP A',E'               ; @BT BB
                LD C',RES 4,(IX+$11)   ; @BT DD CB 11 A1
                POP IY                 ; @BT FD E1
                LD IYL,B'              ; @BT FD 68
                LD L',RES 7,(IX+$63)   ; @BT DD CB 63 BD
                BIT 1,(IY+$0F)         ; @BT FD CB 0F 4E
                LD L',SET 3,(IX+$13)   ; @BT DD CB 13 DD
                BIT 7,E'               ; @BT CB 7B
                LD C',SET 4,(IY+$5C)   ; @BT FD CB 5C E1
                LD H',RES 6,(IX+$4D)   ; @BT DD CB 4D B4
                LD L',SET 0,(IX+$01)   ; @BT DD CB 01 C5
                LD D',SET 3,(IY+$66)   ; @BT FD CB 66 DA
                LD A',SET 0,(IY+$24)   ; @BT FD CB 24 C7
                PIXELDN                ; @BT ED 93
                LD B',RES 2,(IX+$75)   ; @BT DD CB 75 90
                LD B',SET 3,(IX+$43)   ; @BT DD CB 43 D8
                ADD A',D'              ; @BT 82
                LD L',SET 0,(IY+$04)   ; @BT FD CB 04 C5
                BIT 2,C'               ; @BT CB 51
                LD E',A'               ; @BT 5F
                LD A',(IY+((($5E-(4+6))+(4+6)))) ; @BT FD 7E 5E
                RL H'                  ; @BT CB 14
                LD D',D'               ; @BT 52
                LD B',IYL              ; @BT FD 45
                LD IY,((T4_DEF_029-3)+3) ; @BT FD 2A 07 D1
                ADD HL',DE'            ; @BT 19
                SRA C'                 ; @BT CB 29
                LD DE',((((T4_DEF_035-(4+4))+(4+4)))) ; @BT ED 5B C9 6C
                PUSH IX                ; @BT DD E5
                LD D',RES 1,(IY+$12)   ; @BT FD CB 12 8A
                SET 0,L'               ; @BT CB C5
                OUT (C'),B'            ; @BT ED 41
                LD E',SET 3,(IX+$73)   ; @BT DD CB 73 DB
                RST ((($38-2)+(2-3))+3) ; @BT FF
                LD L',RES 1,(IY+$42)   ; @BT FD CB 42 8D
                SET 0,(IX+$0A)         ; @BT DD CB 0A C6
                SET 5,E'               ; @BT CB EB
                LD L',SET 7,(IX+$2B)   ; @BT DD CB 2B FD
                LD A',SLL (IY+((($5A-3)+(3+5))-5)) ; @BT FD CB 5A 37
                LD C',SLA (IX+((($71-4)-2)+4+2)) ; @BT DD CB 71 21
                RET PE                 ; @BT E8
                BIT 6,(IX+$22)         ; @BT DD CB 22 76
                SET 7,A'               ; @BT CB FF
                LD IXH,IXH             ; @BT DD 64
                BIT 5,L'               ; @BT CB 6D
                RET GTE                ; @BT D0
                LD E',SRA (IY+((($20-(3+6))+6)+3)) ; @BT FD CB 20 2B
                LD ((((T4_DEF_014-8)+7)-7+8)),IY ; @BT FD 22 05 F1
                LD B',SET 1,(IY+$3A)   ; @BT FD CB 3A C8
                DEC IYH                ; @BT FD 25
                SRL B'                 ; @BT CB 38
                CALL NEQ,((($B77F-(11+9))+9)+11) ; @BT C4 7F B7
                LD IXH,D'              ; @BT DD 62
                LD (IY+((($2C-2)+(2-5))+5)),A' ; @BT FD 77 2C
                ADC A',D'              ; @BT 8A
                LD B',RL (IY+((($22-5)+(5-7))+7)) ; @BT FD CB 22 10
                RES 5,(HL')            ; @BT CB AE
                LD A',RES 7,(IY+$06)   ; @BT FD CB 06 BF
                LD H',RES 5,(IY+$4A)   ; @BT FD CB 4A AC
                LD A',SET 5,(IX+$3F)   ; @BT DD CB 3F EF
                LD B',H'               ; @BT 44
                LD C',SET 4,(IX+$59)   ; @BT DD CB 59 E1
                ADD BC',(((T4_DEF_021-7)+(7+5))-5) ; @BT ED 36 87 37
                LD B',SET 6,(IY+$58)   ; @BT FD CB 58 F0
                SET 6,D'               ; @BT CB F2
                CPDR                   ; @BT ED B9
                IN A',(((T4_DEF_024-(8+6))+8+6)) ; @BT DB 91
                DEC IX                 ; @BT DD 2B
                BIT 5,B'               ; @BT CB 68
                BIT 3,L'               ; @BT CB 5D
                JP NEQ,(((T4_DEF_020-4)-1)+4+1) ; @BT C2 34 23
                SUB A',(IY+((($7B-(11+9))+(11+9)))) ; @BT FD 96 7B
                ADD A',IXH             ; @BT DD 84
                LD C',RES 7,(IY+$26)   ; @BT FD CB 26 B9
                INC IX                 ; @BT DD 23
                AND A',E'              ; @BT A3
                LD E',SET 6,(IY+$08)   ; @BT FD CB 08 F3
                RES 6,H'               ; @BT CB B4
                NEG                    ; @BT ED 44
                JP (IY)                ; @BT FD E9
                BIT 3,(IY+$4B)         ; @BT FD CB 4B 5E
                ADD IY,DE'             ; @BT FD 19
                LD E',SRA (IX+((($1D-2)+(2-4))+4)) ; @BT DD CB 1D 2B
                INC SP                 ; @BT 33
                XOR A',IXL             ; @BT DD AD
                BIT 7,H'               ; @BT CB 7C
                SLA B'                 ; @BT CB 20
                BIT 2,H'               ; @BT CB 54
                RST ((($20-5)-(2-1))+(2-1)+5) ; @BT E7
                LD IYL,IYL             ; @BT FD 6D
                LD B',SET 1,(IX+$37)   ; @BT DD CB 37 C8
                LD E',(IY+((($2A-(6+2))+(6+2)))) ; @BT FD 5E 2A
                NOP                    ; @BT 00
                LD H',RLC (IY+((($5C-2)+(2+5))-5)) ; @BT FD CB 5C 04
                LD C',SRA (IY+(($00-6)+6)) ; @BT FD CB 00 29
                LD A',RES 0,(IY+$5C)   ; @BT FD CB 5C 87
                CALL (($BCD4-6)+6)     ; @BT CD D4 BC
                LD B',RL (IX+((($1F-(8+8))+8)+8)) ; @BT DD CB 1F 10
                LD L',SRA (IX+((($3D-(11+3))+(11+3)))) ; @BT DD CB 3D 2D
                LD E',SET 7,(IY+$0E)   ; @BT FD CB 0E FB
                JP (C')                ; @BT ED 98
                LD C',RR (IY+((($3E-(4+1))+4)+1)) ; @BT FD CB 3E 19
                RES 3,C'               ; @BT CB 99
                SET 7,C'               ; @BT CB F9
                BIT 1,A'               ; @BT CB 4F
                DEC IYL                ; @BT FD 2D
                RES 6,D'               ; @BT CB B2
                SET 3,L'               ; @BT CB DD
                LD L',SET 2,(IX+$0D)   ; @BT DD CB 0D D5
                SUB A',IYL             ; @BT FD 95
                SET 7,E'               ; @BT CB FB
                AND A',(HL')           ; @BT A6
                RES 7,D'               ; @BT CB BA
                SET 4,C'               ; @BT CB E1
                LD C',SET 0,(IX+$41)   ; @BT DD CB 41 C1
                LD B',SLA (IY+((($64-5)+2)-2+5)) ; @BT FD CB 64 20
                LD D',SET 6,(IY+$78)   ; @BT FD CB 78 F2
                LD C',SET 5,(IX+$5F)   ; @BT DD CB 5F E9
                LD IXH,B'              ; @BT DD 60
                RES 1,A'               ; @BT CB 8F
                LD H',RR (IX+((($6B-3)+(3-3))+3)) ; @BT DD CB 6B 1C
                OR A',(HL')            ; @BT B6
                LD D',RES 3,(IY+$1E)   ; @BT FD CB 1E 9A
                BIT 5,C'               ; @BT CB 69
                LD L',RRC (IX+((($75-7)-8)+7+8)) ; @BT DD CB 75 0D
                SUB A',(IX+((($78-(9+6))+(9+6)))) ; @BT DD 96 78
                LD IXL,B'              ; @BT DD 68
                PUSH DE'               ; @BT D5
                RES 0,B'               ; @BT CB 80
                LD H',RES 7,(IX+$53)   ; @BT DD CB 53 BC
                LD IYL,(((T4_DEF_019-9)+(9-5))+5) ; @BT FD 2E 26
                LD A',SET 1,(IX+$27)   ; @BT DD CB 27 CF
                ADC A',(IY+((($34-(11+3))+3)+11)) ; @BT FD 8E 34
                INC (IY+((($66-10)+(10+6))-6)) ; @BT FD 34 66
                LD E',RES 5,(IY+$3A)   ; @BT FD CB 3A AB
                LD D',SET 2,(IY+$60)   ; @BT FD CB 60 D2
                SUB A',IXL             ; @BT DD 95
                SET 6,E'               ; @BT CB F3
                RLCA                   ; @BT 07
                RES 4,(HL')            ; @BT CB A6
                LD D',((($58-(7+1))+7)+1) ; @BT 16 58
                LD B',SET 7,(IY+$5E)   ; @BT FD CB 5E F8
                CP A',H'               ; @BT BC
                LD L',SET 7,(IY+$2E)   ; @BT FD CB 2E FD
                RES 0,H'               ; @BT CB 84
                RES 0,(IY+$15)         ; @BT FD CB 15 86
                LD C',RES 2,(IX+$05)   ; @BT DD CB 05 91
                LD E',SET 1,(IY+$6A)   ; @BT FD CB 6A CB
                SBC A',C'              ; @BT 99
                RES 2,H'               ; @BT CB 94
                SBC A',(IX+(($51-(7+2))+7+2)) ; @BT DD 9E 51
                RES 6,(HL')            ; @BT CB B6
                SL1 D'                 ; @BT CB 32
                LD E',SET 7,(IX+$0B)   ; @BT DD CB 0B FB
                LD E',SRL (IY+((($26-(10+1))+10)+1)) ; @BT FD CB 26 3B
                RES 0,(IX+$12)         ; @BT DD CB 12 86
                LD L',SET 4,(IY+$1C)   ; @BT FD CB 1C E5
                SET 1,A'               ; @BT CB CF
                ADD A',IYL             ; @BT FD 85
                LD (IX+((($6D-(9+1))+9)+1)),H' ; @BT DD 74 6D
                LD H',SET 1,(IX+$77)   ; @BT DD CB 77 CC
                BIT 5,D'               ; @BT CB 6A
                CPD                    ; @BT ED A9
                OR A',IYH              ; @BT FD B4
                LD B',RES 2,(IY+$78)   ; @BT FD CB 78 90
                LD (HL'),B'            ; @BT 70
                LD A',RES 7,(IX+$03)   ; @BT DD CB 03 BF
                ADC HL',SP             ; @BT ED 7A
                SET 7,(IY+$5F)         ; @BT FD CB 5F FE
                LD D',SLA (IX+((($01-2)-3)+2+3)) ; @BT DD CB 01 22
                LD D',SET 6,(IX+$75)   ; @BT DD CB 75 F2
                JP PO,((($2778-2)+(2-6))+6) ; @BT E2 78 27
                SRA E'                 ; @BT CB 2B
                SET 4,(IY+$05)         ; @BT FD CB 05 E6
                LD B',SRA (IX+((($6D-5)+3)-3+5)) ; @BT DD CB 6D 28
                LD C',RES 0,(IX+$79)   ; @BT DD CB 79 81
                SRL H'                 ; @BT CB 3C
                LDPIRX                 ; @BT ED B7
                BIT 0,A'               ; @BT CB 47
                RST ((($00-(10+2))+2)+10) ; @BT C7
                LD D',RR (IY+((($4E-6)+6)-6+6)) ; @BT FD CB 4E 1A
                LD D',SET 3,(IX+$63)   ; @BT DD CB 63 DA
                LD E',RES 2,(IX+$25)   ; @BT DD CB 25 93
                SRL C'                 ; @BT CB 39
                LD C',SLL (IX+((($77-(2+3))+(2+3)))) ; @BT DD CB 77 31
                SBC HL',SP             ; @BT ED 72
                LD E',IYH              ; @BT FD 5C
                BIT 4,A'               ; @BT CB 67
                SET 4,(HL')            ; @BT CB E6
                LD H',RES 4,(IX+$41)   ; @BT DD CB 41 A4
                SET 6,B'               ; @BT CB F0
                RLC (IY+(($44-(7+7))+7+7)) ; @BT FD CB 44 06
                RES 0,(HL')            ; @BT CB 86
                LD R,A'                ; @BT ED 4F
                LD C',(HL')            ; @BT 4E
                LD L',SRA (IY+(($40-(9+4))+9+4)) ; @BT FD CB 40 2D
                RES 7,(IY+$67)         ; @BT FD CB 67 BE
                LD IYH,IYL             ; @BT FD 65
                BIT 7,(IX+$40)         ; @BT DD CB 40 7E
                SRL (HL')              ; @BT CB 3E
                LD A',SET 3,(IX+$33)   ; @BT DD CB 33 DF
                LD B',RES 1,(IX+$6F)   ; @BT DD CB 6F 88
                RES 1,(HL')            ; @BT CB 8E
                RET PO                 ; @BT E0
                RRC C'                 ; @BT CB 09
                LD A',SET 2,(IY+$30)   ; @BT FD CB 30 D7
                LD IYH,C'              ; @BT FD 61
                JP NEG,((($2112-9)+(9-4))+4) ; @BT FA 12 21
                LD A',SLA (IY+((($54-9)+2)-2+9)) ; @BT FD CB 54 27
                RR D'                  ; @BT CB 1A
                LD A',L'               ; @BT 7D
                LD D',(IY+(($7D-(9+2))+9+2)) ; @BT FD 56 7D
                RET LSS                ; @BT D8
                LD B',D'               ; @BT 42
                LD H',SET 0,(IX+$71)   ; @BT DD CB 71 C4
                LD C',RES 2,(IY+$08)   ; @BT FD CB 08 91
                LD D',IYL              ; @BT FD 55
                RET NEQ                ; @BT C0
                ADD IX,DE'             ; @BT DD 19
                LD D',SLL (IX+((($07-(3+1))+3)+1)) ; @BT DD CB 07 32
                CP A',C'               ; @BT B9
                LD B',B'               ; @BT 40
                SWAPNIB                ; @BT ED 23
                LD (HL'),C'            ; @BT 71
                LD A',SRL (IX+(($63-(5+8))+5+8)) ; @BT DD CB 63 3F
                LD L',(IX+((($07-3)+2)-2+3)) ; @BT DD 6E 07
                LD E',SLA (IY+((($14-(4+8))+8)+4)) ; @BT FD CB 14 23
                OTIR                   ; @BT ED B3
                LD D',B'               ; @BT 50
                ADD A',(((T4_DEF_028-3)+(3-4))+4) ; @BT C6 08
                DEC HL'                ; @BT 2B
                RES 2,(IX+$4E)         ; @BT DD CB 4E 96
                DEC IXL                ; @BT DD 2D
                LD B',SET 4,(IY+$4C)   ; @BT FD CB 4C E0
                LD C',IXH              ; @BT DD 4C
                RES 4,E'               ; @BT CB A3
                SET 6,(HL')            ; @BT CB F6
                LD D',RES 5,(IX+$27)   ; @BT DD CB 27 AA
                LD E',RL (IY+((($52-(1+1))+1)+1)) ; @BT FD CB 52 13
                ADC A',IYH             ; @BT FD 8C
                LD A',SLA (IX+((($51-6)+(6-6))+6)) ; @BT DD CB 51 27
                SRL E'                 ; @BT CB 3B
                RES 7,L'               ; @BT CB BD
                LD C',SRA (IX+((($7D-2)-9)+2+9)) ; @BT DD CB 7D 29
                BIT 0,H'               ; @BT CB 44
                INC B'                 ; @BT 04
                LD L',RES 4,(IX+$51)   ; @BT DD CB 51 A5
                AND A',B'              ; @BT A0
                LD H',SET 2,(IY+$00)   ; @BT FD CB 00 D4
                RES 7,(HL')            ; @BT CB BE
                ADD HL',HL'            ; @BT 29
                LD A',SLL (IX+(($57-(9+3))+9+3)) ; @BT DD CB 57 37
                LD D',RES 6,(IX+$2D)   ; @BT DD CB 2D B2
                RRC E'                 ; @BT CB 0B
                BIT 4,D'               ; @BT CB 62
                LD A',RES 0,(IX+$59)   ; @BT DD CB 59 87
                RES 5,E'               ; @BT CB AB
                ADC A',IYL             ; @BT FD 8D
                LD E',RES 1,(IY+$22)   ; @BT FD CB 22 8B
                RET EQU                ; @BT C8
                ADD HL',((($400F-11)+(11-5))+5) ; @BT ED 34 0F 40
                SLA E'                 ; @BT CB 23
                BIT 3,A'               ; @BT CB 5F
                LD A',(BC')            ; @BT 0A
                LD A',(IX+((($55-9)+(9-3))+3)) ; @BT DD 7E 55
                SL1 (IY+((($7D-7)+(7-8))+8)) ; @BT FD CB 7D 36
                IM (((0-(5+2))+(5+2))) ; @BT ED 46
                LD B',SET 0,(IY+$34)   ; @BT FD CB 34 C0
                LD B',SLA (IX+((($61-(2+1))+2)+1)) ; @BT DD CB 61 20
                LD H',(HL')            ; @BT 66
                LD H',SET 6,(IX+$15)   ; @BT DD CB 15 F4
                SET 1,C'               ; @BT CB C9
                BIT 6,D'               ; @BT CB 72
                LD E',SET 3,(IY+$76)   ; @BT FD CB 76 DB
                ADD A',(IX+((($64-7)-(2-1))+(2-1)+7)) ; @BT DD 86 64
                LDI                    ; @BT ED A0
                SUB A',(HL')           ; @BT 96
                POP IX                 ; @BT DD E1
                LD A',RRC (IY+((($18-7)+(7-10))+10)) ; @BT FD CB 18 0F
                LD D',SET 7,(IY+$7E)   ; @BT FD CB 7E FA
                LD D',RL (IX+((($3F-3)+(3-9))+9)) ; @BT DD CB 3F 12
                PUSH BC'               ; @BT C5
                SUB A',IYH             ; @BT FD 94
                LD D',SET 0,(IX+$51)   ; @BT DD CB 51 C2
                INC D'                 ; @BT 14
                LD A',(DE')            ; @BT 1A
                LD H',RES 0,(IY+$2C)   ; @BT FD CB 2C 84
                OR A',C'               ; @BT B1
                SET 4,E'               ; @BT CB E3
                RES 1,(IX+$30)         ; @BT DD CB 30 8E
                IN H',(C')             ; @BT ED 60
                INC HL'                ; @BT 23
                LD B',RES 7,(IY+$16)   ; @BT FD CB 16 B8
                LD H',E'               ; @BT 63
                LD IYH,(((T4_DEF_005-8)+(8-11))+11) ; @BT FD 26 6E
                RR (HL')               ; @BT CB 1E
                LD C',SET 3,(IY+$56)   ; @BT FD CB 56 D9
                LD L',SET 6,(IX+$25)   ; @BT DD CB 25 F5
                RES 5,(IX+$28)         ; @BT DD CB 28 AE
                LD E',RES 7,(IX+$43)   ; @BT DD CB 43 BB
                LD E',SET 5,(IY+$02)   ; @BT FD CB 02 EB
                LD L',RES 0,(IY+$3C)   ; @BT FD CB 3C 85
                LD I,A'                ; @BT ED 47
                RLC (IY+((($44-2)+4)-4+2)) ; @BT FD CB 44 06
                SET 6,A'               ; @BT CB F7
                LD L',D'               ; @BT 6A
                LD H',RES 3,(IX+$3B)   ; @BT DD CB 3B 9C
                LD A',SET 6,(IY+$48)   ; @BT FD CB 48 F7
                LD C',SET 2,(IY+$50)   ; @BT FD CB 50 D1
                CP A',IYL              ; @BT FD BD
                LD E',B'               ; @BT 58
                RES 0,(IY+$15)         ; @BT FD CB 15 86
                DEC A'                 ; @BT 3D
                XOR A',D'              ; @BT AA
                LD B',RRC (IY+((($28-(6+1))+6)+1)) ; @BT FD CB 28 08
                LD B',(IY+((($1A-(10+3))+3)+10)) ; @BT FD 46 1A
                LD D',SET 4,(IY+$6C)   ; @BT FD CB 6C E2
                SUB A',IXH             ; @BT DD 94
                CPI                    ; @BT ED A1
                OUT (C'),D'            ; @BT ED 51
                PUSH HL'               ; @BT E5
                ADC A',IXH             ; @BT DD 8C
                LD B',RRC (IX+((($25-10)+9)-9+10)) ; @BT DD CB 25 08
                LD (BC'),A'            ; @BT 02
                SET 2,L'               ; @BT CB D5
                SL1 A'                 ; @BT CB 37
                EI                     ; @BT FB
                LD IY,(((($CFF6-(8+7))+7)+8)) ; @BT FD 2A F6 CF
                BIT 0,(IY+$71)         ; @BT FD CB 71 46
                EX DE',HL'             ; @BT EB
                SET 6,(IY+$41)         ; @BT FD CB 41 F6
                BIT 2,(IY+$2D)         ; @BT FD CB 2D 56
                LD E',RLC (IX+((($49-4)-(3-1))+(3-1)+4)) ; @BT DD CB 49 03
                AND A',IXH             ; @BT DD A4
                AND A',((($FF-8)+(8-3))+3) ; @BT E6 FF
                LD E',RES 0,(IX+$19)   ; @BT DD CB 19 83
                LD IXL,D'              ; @BT DD 6A
                SET 3,(HL')            ; @BT CB DE
                SET 0,B'               ; @BT CB C0
                SRL (IY+((($57-6)-(1-1))+(1-1)+6)) ; @BT FD CB 57 3E
                IM (((1-6)+(6-4))+4)   ; @BT ED 56
                RES 3,E'               ; @BT CB 9B
                LD H',SRA (IX+((($2D-5)-(6-1))+(6-1)+5)) ; @BT DD CB 2D 2C
                EXX                    ; @BT D9
                LD L',E'               ; @BT 6B
                OR A',E'               ; @BT B3
                JR EQU,((($E0-6)+(6-2))+2) ; @BT 28 E0
                DEC L'                 ; @BT 2D
                LD C',RES 1,(IY+$02)   ; @BT FD CB 02 89
                ADD IX,SP              ; @BT DD 39
                LD C',RES 4,(IY+$14)   ; @BT FD CB 14 A1
                BIT 6,E'               ; @BT CB 73
                LD E',SET 5,(IX+$7F)   ; @BT DD CB 7F EB
                ADD A',IXL             ; @BT DD 85
                LD E',IXL              ; @BT DD 5D
                XOR A',A'              ; @BT AF
                CP A',D'               ; @BT BA
                LD A',E'               ; @BT 7B
                RES 3,H'               ; @BT CB 9C
                LD B',SET 0,(IX+$31)   ; @BT DD CB 31 C0
                AND A',L'              ; @BT A5
                LD H',RL (IY+(($62-(11+1))+11+1)) ; @BT FD CB 62 14
                ADD A',IYH             ; @BT FD 84
                OUT (C'),C'            ; @BT ED 49
                MUL D',E               ; @BT ED 30
                SRA H'                 ; @BT CB 2C
                LD D',RES 2,(IY+$18)   ; @BT FD CB 18 92
                RES 2,D'               ; @BT CB 92
                LD IYL,D'              ; @BT FD 6A
                OR A',H'               ; @BT B4
                LD C',A'               ; @BT 4F
                SLA A'                 ; @BT CB 27
                LD IYL,C'              ; @BT FD 69
                EX (SP),HL'            ; @BT E3
                LD A',A'               ; @BT 7F
                NEXTREG (($F1-(5+9))+5+9),((($F1-4)+(4-8))+8) ; @BT ED 91 F1 F1
                LD L',RES 4,(IY+$54)   ; @BT FD CB 54 A5
                LD A',RES 6,(IY+$00)   ; @BT FD CB 00 B7
                BIT 0,E'               ; @BT CB 43
                SBC A',IXH             ; @BT DD 9C
                ADD IY,BC'             ; @BT FD 09
                XOR A',(((T4_DEF_008-(9+8))+(9+8))) ; @BT EE D3
                RES 2,E'               ; @BT CB 93
                SLA D'                 ; @BT CB 22
                LD B',(IX+((($11-(6+5))+5)+6)) ; @BT DD 46 11
                LD A',RES 2,(IX+$65)   ; @BT DD CB 65 97
                LD H',SLL (IY+((($2A-8)+(8-3))+3)) ; @BT FD CB 2A 34
                POP AF'                ; @BT F1
                SET 5,D'               ; @BT CB EA
                DEC H'                 ; @BT 25
                BIT 3,(HL')            ; @BT CB 5E
                RL C'                  ; @BT CB 11
                LD A',C'               ; @BT 79
                LD IX,(((T4_DEF_018-(9+5))+5)+9) ; @BT DD 2A 55 B5
                LD A',RR (IY+((($1E-10)-9)+10+9)) ; @BT FD CB 1E 1F
                SET 5,(IY+$23)         ; @BT FD CB 23 EE
                LD C',(IY+((($47-3)-(1-1))+(1-1)+3)) ; @BT FD 4E 47
                OR A',IXL              ; @BT DD B5
                LD B',RES 0,(IY+$6C)   ; @BT FD CB 6C 80
                LD IXL,IXL             ; @BT DD 6D
                LD C',RES 6,(IY+$20)   ; @BT FD CB 20 B1
                RES 2,(HL')            ; @BT CB 96
                LD E',RES 3,(IX+$2B)   ; @BT DD CB 2B 9B
                RES 4,C'               ; @BT CB A1
                LD C',SET 7,(IX+$6B)   ; @BT DD CB 6B F9
                OUT (C'),E'            ; @BT ED 59
                RES 6,(IX+$46)         ; @BT DD CB 46 B6
                BIT 0,(HL')            ; @BT CB 46
                BSRL DE',B             ; @BT ED 2A
                LD D',SET 1,(IY+$5A)   ; @BT FD CB 5A CA
                LD B',L'               ; @BT 45
                BIT 0,C'               ; @BT CB 41
                LD L',RR (IX+(($7B-(8+9))+8+9)) ; @BT DD CB 7B 1D
                ADD A',A'              ; @BT 87
                SUB A',L'              ; @BT 95
                LD A',RES 4,(IX+$71)   ; @BT DD CB 71 A7
                LD H',SET 5,(IX+$0F)   ; @BT DD CB 0F EC
                LD C',SET 1,(IX+$47)   ; @BT DD CB 47 C9
                RES 1,L'               ; @BT CB 8D
                LD A',RES 4,(IY+$74)   ; @BT FD CB 74 A7
                DEC (IY+(($68-(7+4))+7+4)) ; @BT FD 35 68
                LD E',SET 4,(IX+$79)   ; @BT DD CB 79 E3
                LD IXH,IXL             ; @BT DD 65
                ADD HL',BC'            ; @BT 09

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////






    ; TEST-05 generated defines/functions
    .define T5_DEF_001 ((($18+9+3)-9-3)+(2-2))
    .define T5_DEF_002 (((T5_DEF_001)*2+($D0)-(($18)*2))+(11-11))
    .define T5_DEF_003 ((($39A9+6+6)-6-6)+(6-6))
    .define T5_DEF_004 (((T5_DEF_003)+($ECC1)-($39A9))+(5-5))
    .define T5_DEF_005 ((($6E+7)-(7+1)+1)+(1-1))
    .define T5_DEF_006 ((($F216)+(T5_DEF_005)-($6E))+((7+1)-(7+1)))
    .define T5_DEF_007 ((($B21A+6+2)-6-2)+(5-5))
    .define T5_DEF_008 ((T5_DEF_007)+(($D3)-($B21A))+(9-9))
    .define T5_DEF_009 ((($B2*2)-($B2))+($B2)+(5-5)+(4-4)-$B2)
    .define T5_DEF_010 ((($4887)-($B2)+(T5_DEF_009))+(7-7)+(6-6))
    .define T5_DEF_011 ((($B109*2)/2)+(10-10)+(2-2))
    .define T5_DEF_012 ((T5_DEF_011)+(($2889)-($B109))+(6-6))
    .define T5_DEF_013 ((($7A+13)-(13+2)+2)+(9-9))
    .define T5_DEF_014 ((($F105)+(T5_DEF_013)-($7A))+((11+8)-(11+8)))
    .define T5_DEF_015 ((($BBC3+(13*11))-(13*11))+(6-6))
    .define T5_DEF_016 (((T5_DEF_015)+($C3)-($BBC3))+(2-2))
    .define T5_DEF_017 ((($9862+2)-2)+(10-2))
    .define T5_DEF_018 ((($B54D)+(T5_DEF_017)-($9862))+((3+1)-(3+1)))
    .define T5_DEF_019 ((($26+12+4)-12-4)+(4-4))
    .define T5_DEF_020 (((T5_DEF_019)+($2334)-($26))+(3-3))
    .define T5_DEF_021 ((($3787+(3*4))-(3*4))+(2-2))
    .define T5_DEF_022 ((($9660)-($3787)+(T5_DEF_021))+(7-7)+(7-7))
    .define T5_DEF_023 ((($E1+3)-3)+(7-6))
    .define T5_DEF_024 ((T5_DEF_023)+(($90)-($E1))+(11-11))
    .define T5_DEF_025 ((($BAB2*3)/3)+(1-1)+(4-4))
    .define T5_DEF_026 ((T5_DEF_025)+(($C9)-($BAB2))+(5-5))
    .define T5_DEF_027 ((($78*2)-($78))+($78)+(6-6)+(3-3)-$78)
    .define T5_DEF_028 (((T5_DEF_027)*2+($08)-(($78)*2))+(7-7))
    .define T5_DEF_029 ((($D107+8+9)-8-9)+(3-3))
    .define T5_DEF_030 ((($B43C)+(T5_DEF_029)-($D107))+((8+9)-(8+9)))
    .define T5_DEF_031 ((($2556*9)/9)+(11-11)+(9-9))
    .define T5_DEF_032 ((($94)-($2556)+(T5_DEF_031))+(9-9)+(4-4))
    .define T5_DEF_033 ((($EFF4+12+2)-12-2)+(6-6))
    .define T5_DEF_034 ((($53)+(T5_DEF_033)-($EFF4))+((9+8)-(9+8)))
    .define T5_DEF_035 ((($6CC9*12)/12)+(6-6)+(6-6))
    .define T5_DEF_036 ((($75)-($6CC9)+(T5_DEF_035))+(9-9)+(1-1))

    ; TEST-05 generated functions
    .function F5_FN_001(a) (((a+3)-3)+(T5_DEF_001-T5_DEF_001))
    .function F5_FN_002(a,b) (((a*6)/6)+(T5_DEF_002-T5_DEF_002))
    .function F5_FN_003(a,b,c) ((((a+4)*6)/6-4)+(T5_DEF_003-T5_DEF_003))
    .function F5_FN_004(a,b,c,d) (((a+(9+7))-(9+7))+(T5_DEF_004-T5_DEF_004))
    .function F5_FN_005(a,b,c,d,e) (((a+(3*2))-(3*2))+(T5_DEF_005-T5_DEF_005))
    .function F5_FN_006(a,b,c,d,e,f) (((a+b)-b)+(T5_DEF_006-T5_DEF_006))
    .function F5_FN_007(a) (((a*6)/6)+(T5_DEF_007-T5_DEF_007))
    .function F5_FN_008(a,b) (((a+(b+7))-(b+7))+(T5_DEF_008-T5_DEF_008))
    .function F5_FN_009(a,b,c) (((a+b+c)-b-c)+(T5_DEF_009-T5_DEF_009))
    .function F5_FN_010(a,b,c,d) (((a+(b*5)+(c*3))-(b*5)-(c*3))+(T5_DEF_010-T5_DEF_010))
    .function F5_FN_011(a,b,c,d,e) (((F5_FN_005(a,b,c,d,e)+(6*10))-(6*10))+(T5_DEF_011-T5_DEF_011))
    .function F5_FN_012(a,b,c,d,e,f) (((F5_FN_006(a,b,c,d,e,f)+6+8+8)-6-8-8)+(T5_DEF_012-T5_DEF_012))
    .function F5_FN_013(a) (((F5_FN_007(a)+3)-3)+(T5_DEF_013-T5_DEF_013))
    .function F5_FN_014(a,b) (((F5_FN_008(a,b)*5)/5)+(T5_DEF_014-T5_DEF_014))
    .function F5_FN_015(a,b,c) ((((F5_FN_009(a,b,c)+3)*10)/10-3)+(T5_DEF_015-T5_DEF_015))
    .function F5_FN_016(a,b,c,d) (((F5_FN_004(a,b,c,d)+(3+3))-(3+3))+(T5_DEF_016-T5_DEF_016))
    .function F5_FN_017(a,b,c,d,e) (((F5_FN_005(a,b,c,d,e)+(9*3))-(9*3))+(T5_DEF_017-T5_DEF_017))
    .function F5_FN_018(a,b,c,d,e,f) (((F5_FN_012(a,b,c,d,e,f)+10+6+9)-10-6-9)+(T5_DEF_018-T5_DEF_018))
    .function F5_FN_019(a) (((F5_FN_001(a)+6)-6)+(T5_DEF_019-T5_DEF_019))
    .function F5_FN_020(a,b) (((F5_FN_002(a,b)*7)/7)+(T5_DEF_020-T5_DEF_020))
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04 with full TEST-05 randomization + varied function replacement
    ; GENERATED: seed = 1470561003
    ; GENERATED: replaced define refs = 18 / 36
    ; GENERATED: functions = 20 (chained=10)

    OR A',(IX+((($14-9)-9)+9+9)) ; @BT DD B6 14
    LD A',(DE')            ; @BT 1A
    LD C',SET 5,(IY+$62)   ; @BT FD CB 62 E9
    SBC A',IYH             ; @BT FD 9C
    XOR A',D'              ; @BT AA
    SUB A',D'              ; @BT 92
    SET 6,C'               ; @BT CB F1
    SET 1,H'               ; @BT CB CC
    LD C',RLC (IX+((($29-(7+6))+6)+7)) ; @BT DD CB 29 01
    LD D',RES 2,(IY+$18)   ; @BT FD CB 18 92
    LD C',RRC (IX+((($35-10)+1)-1+10)) ; @BT DD CB 35 09
    LD A',RRC (IX+(($15-(5+8))+5+8)) ; @BT DD CB 15 0F
    LD C',RL (IY+((($32-(4+1))+4)+1)) ; @BT FD CB 32 11
    LD (IX+((($64-7)+(7-4))+4)),C' ; @BT DD 71 64
    LD D',H'               ; @BT 54
    SET 0,(IY+$0D)         ; @BT FD CB 0D C6
    RL (IX+((($23-6)+(6-3))+3)) ; @BT DD CB 23 16
    CP A',IYL              ; @BT FD BD
    LD B',RR (IX+(($2B-(5+1))+5+1)) ; @BT DD CB 2B 18
    LD C',SET 3,(IX+$53)   ; @BT DD CB 53 D9
    LD E',IYL              ; @BT FD 5D
    OR A',(((T5_DEF_027-(11+4))+(11+4))) ; @BT F6 78
    BIT 5,D'               ; @BT CB 6A
    BIT 2,E'               ; @BT CB 53
    LD D',RL (IY+((($42-(11+7))+(11+7)))) ; @BT FD CB 42 12
    LD D',RLC (IX+((($39-(10+4))+4)+10)) ; @BT DD CB 39 02
    CP A',E'               ; @BT BB
    ADD A',IYL             ; @BT FD 85
    SET 6,B'               ; @BT CB F0
    BIT 1,B'               ; @BT CB 48
    JR EQU,((($E0-6)+(6-2))+2) ; @BT 28 E0
    BIT 1,D'               ; @BT CB 4A
    LD D',SET 3,(IX+$63)   ; @BT DD CB 63 DA
    RLD                    ; @BT ED 6F
    LD IYL,E'              ; @BT FD 6B
    CP A',(HL')            ; @BT BE
    LD H',RL (IY+(($62-(11+1))+11+1)) ; @BT FD CB 62 14
    LDD                    ; @BT ED A8
    INC C'                 ; @BT 0C
    LD A',RES 7,(IX+$03)   ; @BT DD CB 03 BF
    RES 4,C'               ; @BT CB A1
    LD B',RES 6,(IX+$0D)   ; @BT DD CB 0D B0
    ADC HL',HL'            ; @BT ED 6A
    PUSH (((F5_FN_001(T5_DEF_017)-(7+1))+7)+1) ; @BT ED 8A 98 6A
    AND A',IXH             ; @BT DD A4
    LD H',RES 5,(IX+$47)   ; @BT DD CB 47 AC
    LD C',A'               ; @BT 4F
    CPIR                   ; @BT ED B1
    SET 1,(HL')            ; @BT CB CE
    RST ((($00-(10+2))+2)+10) ; @BT C7
    RES 7,(IY+$67)         ; @BT FD CB 67 BE
    MUL D',E               ; @BT ED 30
    LD IYH,E'              ; @BT FD 63
    OUT (C'),H'            ; @BT ED 61
    LD H',SLA (IY+((($24-7)-4)+7+4)) ; @BT FD CB 24 24
    LDDX                   ; @BT ED AC
    LD A',SLL (IY+((($5A-3)+(3+5))-5)) ; @BT FD CB 5A 37
    RLC (HL')              ; @BT CB 06
    OUT (C'),0             ; @BT ED 71
    AND A',A'              ; @BT A7
    LD B',SET 4,(IX+$49)   ; @BT DD CB 49 E0
    CP A',(IY+((($1A-(8+1))+8)+1)) ; @BT FD BE 1A
    RES 4,H'               ; @BT CB A4
    BIT 6,D'               ; @BT CB 72
    LD L',RRC (IX+((($75-7)-8)+7+8)) ; @BT DD CB 75 0D
    ADD DE',(((T5_DEF_003-3)+(3+7))-7) ; @BT ED 35 A9 39
    LD E',SET 7,(IY+$0E)   ; @BT FD CB 0E FB
    INC H'                 ; @BT 24
    JR NEQ,(((T5_DEF_026-(8+1))+8)+1) ; @BT 20 C9
    ADC A',IXL             ; @BT DD 8D
    RES 6,(IY+$49)         ; @BT FD CB 49 B6
    LD A',RES 0,(IY+$5C)   ; @BT FD CB 5C 87
    LD L',SET 2,(IX+$0D)   ; @BT DD CB 0D D5
    LD H',RLC (IX+((($59-2)+(2+9))-9)) ; @BT DD CB 59 04
    SET 4,B'               ; @BT CB E0
    SUB A',(IY+((($7B-(11+9))+(11+9)))) ; @BT FD 96 7B
    INC DE'                ; @BT 13
    AND A',IXL             ; @BT DD A5
    LD B',SET 1,(IX+$37)   ; @BT DD CB 37 C8
    DEC L'                 ; @BT 2D
    LD B',IXL              ; @BT DD 45
    LD (((($EDD2-(2+1))+2)+1)),DE' ; @BT ED 53 D2 ED
    SRA D'                 ; @BT CB 2A
    LD E',RR (IX+((($5B-4)-8)+4+8)) ; @BT DD CB 5B 1B
    AND A',IYH             ; @BT FD A4
    LD (IY+((($32-8)-(9-1))+(9-1)+8)),C' ; @BT FD 71 32
    LD H',RES 1,(IY+$32)   ; @BT FD CB 32 8C
    SET 7,H'               ; @BT CB FC
    JP (IX)                ; @BT DD E9
    SRL D'                 ; @BT CB 3A
    AND A',(HL')           ; @BT A6
    SET 4,(IY+$05)         ; @BT FD CB 05 E6
    RES 2,E'               ; @BT CB 93
    LD B',RES 0,(IX+$69)   ; @BT DD CB 69 80
    ADC A',D'              ; @BT 8A
    LD H',RRC (IY+((($68-(10+8))+8)+10)) ; @BT FD CB 68 0C
    LD H',RES 6,(IY+$50)   ; @BT FD CB 50 B4
    LD D',D'               ; @BT 52
    SET 2,C'               ; @BT CB D1
    LD A',A'               ; @BT 7F
    LD A',C'               ; @BT 79
    LD D',RL (IX+((($3F-3)+(3-9))+9)) ; @BT DD CB 3F 12
    BIT 6,(HL')            ; @BT CB 76
    LD C',SRA (IX+((($7D-2)-9)+2+9)) ; @BT DD CB 7D 29
    CP A',A'               ; @BT BF
    SBC A',C'              ; @BT 99
    BIT 2,C'               ; @BT CB 51
    LD (HL'),D'            ; @BT 72
    ADD A',IYH             ; @BT FD 84
    LD (HL'),L'            ; @BT 75
    RET LSS                ; @BT D8
    LD D',SET 6,(IX+$75)   ; @BT DD CB 75 F2
    RES 4,(HL')            ; @BT CB A6
    LD C',RES 3,(IY+$0E)   ; @BT FD CB 0E 99
    LD D',SET 5,(IX+$6F)   ; @BT DD CB 6F EA
    LD IXH,D'              ; @BT DD 62
    RES 1,E'               ; @BT CB 8B
    PIXELDN                ; @BT ED 93
    RRC D'                 ; @BT CB 0A
    LD A',RES 1,(IX+$5F)   ; @BT DD CB 5F 8F
    RES 7,L'               ; @BT CB BD
    LD C',L'               ; @BT 4D
    LD A',RL (IX+((($0F-(3+1))+3)+1)) ; @BT DD CB 0F 17
    LD E',SRA (IY+((($20-(3+6))+6)+3)) ; @BT FD CB 20 2B
    RLC H'                 ; @BT CB 04
    BIT 0,E'               ; @BT CB 43
    LD H',SET 1,(IX+$77)   ; @BT DD CB 77 CC
    MIRROR A'              ; @BT ED 24
    RST (($08-(10+2))+10+2) ; @BT CF
    SET 5,(IY+$23)         ; @BT FD CB 23 EE
    LD H',SET 1,(IY+$7A)   ; @BT FD CB 7A CC
    BIT 0,L'               ; @BT CB 45
    ADD A',L'              ; @BT 85
    LD C',SET 7,(IX+$6B)   ; @BT DD CB 6B F9
    OTDR                   ; @BT ED BB
    BIT 1,L'               ; @BT CB 4D
    LD E',SET 1,(IX+$67)   ; @BT DD CB 67 CB
    RLC A'                 ; @BT CB 07
    BIT 3,H'               ; @BT CB 5C
    SET 2,(IX+$46)         ; @BT DD CB 46 D6
    SET 7,(IY+$5F)         ; @BT FD CB 5F FE
    LD D',SLL (IY+((($0A-3)+(3+5))-5)) ; @BT FD CB 0A 32
    LD H',SLL (IX+((($27-6)-2)+6+2)) ; @BT DD CB 27 34
    LD E',RES 2,(IY+$28)   ; @BT FD CB 28 93
    INC IYH                ; @BT FD 24
    LD A',RES 0,(IX+$59)   ; @BT DD CB 59 87
    RET POS                ; @BT F0
    LDDR                   ; @BT ED B8
    SET 5,(HL')            ; @BT CB EE
    LD B',E'               ; @BT 43
    LD A',(((($4221-10)-(1-1))+(1-1)+10)) ; @BT 3A 21 42
    PUSH IX                ; @BT DD E5
    LD IXL,C'              ; @BT DD 69
    SET 3,E'               ; @BT CB DB
    JR LSS,((($9B-4)-3)+4+3) ; @BT 38 9B
    IN A',(((T5_DEF_024-(8+6))+8+6)) ; @BT DB 91
    LD D',C'               ; @BT 51
    LD A',SET 4,(IX+$39)   ; @BT DD CB 39 E7
    SET 0,(HL')            ; @BT CB C6
    ADD A',(HL')           ; @BT 86
    RRC E'                 ; @BT CB 0B
    SRL A'                 ; @BT CB 3F
    BIT 6,C'               ; @BT CB 71
    LD L',SLA (IY+((($34-11)-(1-1))+(1-1)+11)) ; @BT FD CB 34 25
    LD (IY+((($35-4)+4)-4+4)),D' ; @BT FD 72 35
    LDIR                   ; @BT ED B0
    RES 4,L'               ; @BT CB A5
    LD C',SET 4,(IX+$59)   ; @BT DD CB 59 E1
    INC IYL                ; @BT FD 2C
    RLC (IX+((($41-4)+(4-2))+2)) ; @BT DD CB 41 06
    CALL POS,((($B890-(4+4))+4)+4) ; @BT F4 90 B8
    OR A',H'               ; @BT B4
    RET NEG                ; @BT F8
    CP A',H'               ; @BT BC
    AND A',B'              ; @BT A0
    LD H',RRC (IX+((($65-(7+8))+(7+8)))) ; @BT DD CB 65 0C
    LD A',B'               ; @BT 78
    LD B',SRL (IX+(($73-(7+5))+7+5)) ; @BT DD CB 73 38
    RES 7,B'               ; @BT CB B8
    CP A',IYH              ; @BT FD BC
    BIT 5,H'               ; @BT CB 6C
    LD L',B'               ; @BT 68
    LD IYL,A'              ; @BT FD 6F
    RL (HL')               ; @BT CB 16
    LD H',SET 2,(IY+$00)   ; @BT FD CB 00 D4
    LD D',SLA (IX+((($01-2)-3)+2+3)) ; @BT DD CB 01 22
    LD IYH,IYH             ; @BT FD 64
    SRA A'                 ; @BT CB 2F
    LD D',RES 6,(IY+$30)   ; @BT FD CB 30 B2
    SUB A',((($4C-4)-(5-1))+(5-1)+4) ; @BT D6 4C
    RES 7,(IX+$64)         ; @BT DD CB 64 BE
    LD E',B'               ; @BT 58
    SRL C'                 ; @BT CB 39
    RES 0,C'               ; @BT CB 81
    LD H',SET 3,(IX+$03)   ; @BT DD CB 03 DC
    POP BC'                ; @BT C1
    ADD BC',(((T5_DEF_021-7)+(7+5))-5) ; @BT ED 36 87 37
    EXX                    ; @BT D9
    PIXELAD                ; @BT ED 94
    RES 5,(IY+$2B)         ; @BT FD CB 2B AE
    LD E',E'               ; @BT 5B
    JP (((F5_FN_002(T5_DEF_012,11)-(10+1))+10)+1) ; @BT C3 89 28
    LD H',SLL (IY+((($2A-8)+(8-3))+3)) ; @BT FD CB 2A 34
    CALL EQU,((($B9A1-5)+1)-1+5) ; @BT CC A1 B9
    LD A',I                ; @BT ED 57
    LD L',SRA (IY+(($40-(9+4))+9+4)) ; @BT FD CB 40 2D
    ADD A',(((T5_DEF_028-3)+(3-4))+4) ; @BT C6 08
    OUT (C'),C'            ; @BT ED 49
    SET 1,B'               ; @BT CB C8
    SET 4,D'               ; @BT CB E2
    LD C',RES 6,(IY+$20)   ; @BT FD CB 20 B1
    LD D',(IY+(($7D-(9+2))+9+2)) ; @BT FD 56 7D
    IN L',(C')             ; @BT ED 68
    CP A',C'               ; @BT B9
    LD H',SET 4,(IY+$0C)   ; @BT FD CB 0C E4
    LD IXH,E'              ; @BT DD 63
    LD L',SLA (IX+((($31-3)+9)-9+3)) ; @BT DD CB 31 25
    LD L',L'               ; @BT 6D
    SBC A',(($A1-8)+8)     ; @BT DE A1
    LD B',SET 2,(IX+$3D)   ; @BT DD CB 3D D0
    LD H',SET 0,(IX+$71)   ; @BT DD CB 71 C4
    LD E',A'               ; @BT 5F
    JP NEQ,(((T5_DEF_020-4)-1)+4+1) ; @BT C2 34 23
    SBC A',IXL             ; @BT DD 9D
    LD L',RES 2,(IY+$48)   ; @BT FD CB 48 95
    BIT 6,A'               ; @BT CB 77
    LD D',RES 0,(IY+$0C)   ; @BT FD CB 0C 82
    RES 5,D'               ; @BT CB AA
    INIR                   ; @BT ED B2
    RES 1,L'               ; @BT CB 8D
    LD L',SET 7,(IX+$2B)   ; @BT DD CB 2B FD
    SRA C'                 ; @BT CB 29
    RES 5,C'               ; @BT CB A9
    LD C',H'               ; @BT 4C
    SL1 B'                 ; @BT CB 30
    BRLC DE',B             ; @BT ED 2C
    BIT 3,A'               ; @BT CB 5F
    BIT 7,(HL')            ; @BT CB 7E
    DEC (IX+(($5C-3)+3))   ; @BT DD 35 5C
    DEC DE'                ; @BT 1B
    LD D',RLC (IY+((($3C-11)-(5-1))+(5-1)+11)) ; @BT FD CB 3C 02
    LD E',(HL')            ; @BT 5E
    RLC D'                 ; @BT CB 02
    LD D',RES 2,(IX+$15)   ; @BT DD CB 15 92
    LD H',RLC (IY+((($5C-2)+(2+5))-5)) ; @BT FD CB 5C 04
    LD (((($EBB0-(5+5))+(5+5)))),A' ; @BT 32 B0 EB
    SET 2,D'               ; @BT CB D2
    LD H',SET 5,(IY+$12)   ; @BT FD CB 12 EC
    SL1 (IY+((($7D-7)+(7-8))+8)) ; @BT FD CB 7D 36
    LD C',RL (IX+((($2F-10)+(10+8))-8)) ; @BT DD CB 2F 11
    RES 5,(IX+$28)         ; @BT DD CB 28 AE
    SET 5,(IX+$20)         ; @BT DD CB 20 EE
    CP A',IXH              ; @BT DD BC
    LD C',SET 2,(IX+$4D)   ; @BT DD CB 4D D1
    ADD IX,IX              ; @BT DD 29
    BIT 3,D'               ; @BT CB 5A
    LD BC',(((F5_FN_003(T5_DEF_010,2,7)-4)-4)+4+4) ; @BT ED 4B 87 48
    LD D',SRL (IX+(($13-6)+6)) ; @BT DD CB 13 3A
    LD C',RES 2,(IY+$08)   ; @BT FD CB 08 91
    LD A',SLA (IY+((($54-9)+2)-2+9)) ; @BT FD CB 54 27
    LD L',SET 3,(IY+$16)   ; @BT FD CB 16 DD
    LD (HL'),H'            ; @BT 74
    LD H',RES 1,(IX+$2F)   ; @BT DD CB 2F 8C
    AND A',C'              ; @BT A1
    LD IXH,IXL             ; @BT DD 65
    ADD IY,DE'             ; @BT FD 19
    LD D',RES 5,(IX+$27)   ; @BT DD CB 27 AA
    SET 0,(IX+$0A)         ; @BT DD CB 0A C6
    SRA L'                 ; @BT CB 2D
    RES 3,(IX+$6C)         ; @BT DD CB 6C 9E
    RES 3,(HL')            ; @BT CB 9E
    LD L',SET 0,(IY+$04)   ; @BT FD CB 04 C5
    SRA E'                 ; @BT CB 2B
    LD L',SET 1,(IX+$07)   ; @BT DD CB 07 CD
    BIT 1,C'               ; @BT CB 49
    ADD A',(IX+((($64-7)-(2-1))+(2-1)+7)) ; @BT DD 86 64
    RR H'                  ; @BT CB 1C
    SLA (IY+((($1B-4)+(4-2))+2)) ; @BT FD CB 1B 26
    OUT ((((T5_DEF_036-9)+(9+7))-7)),A' ; @BT D3 75
    LDWS                   ; @BT ED A5
    BIT 5,C'               ; @BT CB 69
    LD A',(BC')            ; @BT 0A
    LD B',SET 1,(IY+$3A)   ; @BT FD CB 3A C8
    LD A',D'               ; @BT 7A
    LD B',RES 4,(IY+$04)   ; @BT FD CB 04 A0
    LD D',L'               ; @BT 55
    DEC IYH                ; @BT FD 25
    LD SP,IY               ; @BT FD F9
    LD D',SET 4,(IX+$69)   ; @BT DD CB 69 E2
    SET 3,(IX+$64)         ; @BT DD CB 64 DE
    LD B',(IY+((($1A-(10+3))+3)+10)) ; @BT FD 46 1A
    AND A',(IX+((($47-1)+(1-3))+3)) ; @BT DD A6 47
    LD A',SLA (IX+((($51-6)+(6-6))+6)) ; @BT DD CB 51 27
    LD C',IXL              ; @BT DD 4D
    LD B',SET 6,(IX+$55)   ; @BT DD CB 55 F0
    OR A',C'               ; @BT B1
    LD H',RES 7,(IY+$56)   ; @BT FD CB 56 BC
    LD IXL,(((T5_DEF_002-11)+9)-9+11) ; @BT DD 2E D0
    LD L',SLL (IY+((($3A-(8+1))+8)+1)) ; @BT FD CB 3A 35
    LD E',RES 3,(IX+$2B)   ; @BT DD CB 2B 9B
    RES 3,D'               ; @BT CB 9A
    SET 4,A'               ; @BT CB E7
    SET 0,B'               ; @BT CB C0
    LD A',RES 5,(IY+$7A)   ; @BT FD CB 7A AF
    LD E',SET 0,(IX+$61)   ; @BT DD CB 61 C3
    SUB A',(HL')           ; @BT 96
    ADC A',(((F5_FN_004(T5_DEF_034,15,16,9)-(7+7))+(7+7))) ; @BT CE 53
    LDIX                   ; @BT ED A4
    LD A',SRL (IY+((($66-11)+(11+2))-2)) ; @BT FD CB 66 3F
    RES 1,A'               ; @BT CB 8F
    LD DE',((((F5_FN_005(T5_DEF_035,13,10,5,12)-(4+4))+(4+4)))) ; @BT ED 5B C9 6C
    EX (SP),IX             ; @BT DD E3
    POP IX                 ; @BT DD E1
    LD A',SET 3,(IY+$36)   ; @BT FD CB 36 DF
    LD IYL,D'              ; @BT FD 6A
    DEC IXL                ; @BT DD 2D
    LD H',SRA (IX+((($2D-5)-(6-1))+(6-1)+5)) ; @BT DD CB 2D 2C
    LD L',RES 6,(IY+$60)   ; @BT FD CB 60 B5
    RES 7,E'               ; @BT CB BB
    INC HL'                ; @BT 23
    LD L',RL (IX+((($6F-(4+1))+4)+1)) ; @BT DD CB 6F 15
    LD A',SET 6,(IY+$48)   ; @BT FD CB 48 F7
    SBC A',IXH             ; @BT DD 9C
    INC L'                 ; @BT 2C
    ADD IY,SP              ; @BT FD 39
    BIT 7,L'               ; @BT CB 7D
    BIT 7,C'               ; @BT CB 79
    LD B',IYL              ; @BT FD 45
    RRC H'                 ; @BT CB 0C
    LD E',IXH              ; @BT DD 5C
    BIT 5,(IX+$04)         ; @BT DD CB 04 6E
    LD A',SET 6,(IX+$45)   ; @BT DD CB 45 F7
    RR (IX+((($68-9)-(9-1))+(9-1)+9)) ; @BT DD CB 68 1E
    LD E',SET 6,(IY+$08)   ; @BT FD CB 08 F3
    RLA                    ; @BT 17
    SET 7,(IX+$5C)         ; @BT DD CB 5C FE
    RET GTE                ; @BT D0
    XOR A',E'              ; @BT AB
    LD DE',((($6DDA-6)+(6-4))+4) ; @BT ED 5B DA 6D
    DEC B'                 ; @BT 05
    SET 0,H'               ; @BT CB C4
    ADC A',(IX+((($2B-9)+(9-2))+2)) ; @BT DD 8E 2B
    LD (IX+((($73-5)+4)-4+5)),(($88-6)+6) ; @BT DD 36 73 88
    RST ((($38-2)+(2-3))+3) ; @BT FF
    SET 5,L'               ; @BT CB ED
    BIT 2,D'               ; @BT CB 52
    AND A',L'              ; @BT A5
    LD A',IXH              ; @BT DD 7C
    LD B',RRC (IX+((($25-10)+9)-9+10)) ; @BT DD CB 25 08
    LD E',SET 2,(IY+$70)   ; @BT FD CB 70 D3
    RRC (HL')              ; @BT CB 0E
    IN E',(C')             ; @BT ED 58
    BIT 5,L'               ; @BT CB 6D
    LD B',SET 7,(IX+$5B)   ; @BT DD CB 5B F8
    BIT 3,(IY+$4B)         ; @BT FD CB 4B 5E
    LD L',RES 6,(IX+$5D)   ; @BT DD CB 5D B5
    DEC HL'                ; @BT 2B
    DAA                    ; @BT 27
    LD E',SLL (IX+((($17-6)-(4-1))+(4-1)+6)) ; @BT DD CB 17 33
    LD E',H'               ; @BT 5C
    RLC (IX+((($41-(3+1))+3)+1)) ; @BT DD CB 41 06
    LD E',SET 3,(IY+$76)   ; @BT FD CB 76 DB
    SBC A',L'              ; @BT 9D
    LD A',SET 5,(IY+$42)   ; @BT FD CB 42 EF
    LD L',RLC (IX+((($69-(6+1))+6)+1)) ; @BT DD CB 69 05
    LD D',RES 6,(IX+$2D)   ; @BT DD CB 2D B2
    OUT (C'),B'            ; @BT ED 41
    LD D',RES 4,(IY+$24)   ; @BT FD CB 24 A2
    LD E',RES 5,(IX+$37)   ; @BT DD CB 37 AB
    BSRF DE',B             ; @BT ED 2B
    RST ((($28-(7+4))+(7+4))) ; @BT EF
    SET 4,C'               ; @BT CB E1
    ADD HL',((($400F-11)+(11-5))+5) ; @BT ED 34 0F 40
    BIT 7,(IX+$40)         ; @BT DD CB 40 7E
    BIT 7,A'               ; @BT CB 7F
    LD (DE'),A'            ; @BT 12
    LD L',RL (IY+((($72-(8+9))+(8+9)))) ; @BT FD CB 72 15
    LD H',A'               ; @BT 67
    XOR A',L'              ; @BT AD
    LD B',L'               ; @BT 45
    SRL (IY+((($57-6)-(1-1))+(1-1)+6)) ; @BT FD CB 57 3E
    LD B',RES 1,(IY+$72)   ; @BT FD CB 72 88
    RES 7,C'               ; @BT CB B9
    LD L',RES 1,(IX+$3F)   ; @BT DD CB 3F 8D
    SET 2,L'               ; @BT CB D5
    INC E'                 ; @BT 1C
    LD B',SLL (IY+((($6A-8)-(6-1))+(6-1)+8)) ; @BT FD CB 6A 30
    LD E',SRL (IX+((($23-3)-(9-1))+(9-1)+3)) ; @BT DD CB 23 3B
    EX DE',HL'             ; @BT EB
    RES 2,C'               ; @BT CB 91
    CALL GTE,((($B66E-(1+1))+1)+1) ; @BT D4 6E B6
    XOR A',IYH             ; @BT FD AC
    LD A',SET 5,(IX+$3F)   ; @BT DD CB 3F EF
    XOR A',B'              ; @BT A8
    LD L',SET 5,(IY+$22)   ; @BT FD CB 22 ED
    LD E',RES 1,(IX+$1F)   ; @BT DD CB 1F 8B
    LD L',E'               ; @BT 6B
    LD C',SRL (IY+((($06-8)+(8-10))+10)) ; @BT FD CB 06 39
    RETN                   ; @BT ED 45
    LD IYH,A'              ; @BT FD 67
    IN A',(C')             ; @BT ED 78
    SLA H'                 ; @BT CB 24
    LD L',(IX+((($07-3)+2)-2+3)) ; @BT DD 6E 07
    LD E',SRA (IX+((($1D-2)+(2-4))+4)) ; @BT DD CB 1D 2B
    LD A',SET 2,(IY+$30)   ; @BT FD CB 30 D7
    LD L',SET 4,(IY+$1C)   ; @BT FD CB 1C E5
    LD A',SET 0,(IY+$24)   ; @BT FD CB 24 C7
    LD E',RES 6,(IY+$40)   ; @BT FD CB 40 B3
    RES 6,(HL')            ; @BT CB B6
    INC (IX+((($5A-3)+(3+2))-2)) ; @BT DD 34 5A
    RL (IY+((($26-(5+1))+5)+1)) ; @BT FD CB 26 16
    BIT 3,B'               ; @BT CB 58
    ADC A',H'              ; @BT 8C
    LD A',SET 7,(IY+$4E)   ; @BT FD CB 4E FF
    OUT (C'),A'            ; @BT ED 79
    RST ((($00-(11+4))+4)+11) ; @BT C7
    LD A',IXL              ; @BT DD 7D
    LD A',RES 2,(IY+$68)   ; @BT FD CB 68 97
    SET 0,L'               ; @BT CB C5
    LD C',RES 5,(IY+$1A)   ; @BT FD CB 1A A9
    BIT 2,H'               ; @BT CB 54
    SBC A',(IX+(($51-(7+2))+7+2)) ; @BT DD 9E 51
    LD D',SET 1,(IY+$5A)   ; @BT FD CB 5A CA
    SUB A',C'              ; @BT 91
    JP (C')                ; @BT ED 98
    LD D',RES 7,(IY+$36)   ; @BT FD CB 36 BA
    LD D',IXL              ; @BT DD 55
    LD IX,((((T5_DEF_030-8)+6)-6+8)) ; @BT DD 2A 3C B4
    RES 1,(IX+$30)         ; @BT DD CB 30 8E
    LD IY,((F5_FN_006(T5_DEF_029,17,12,4,17,14)-3)+3) ; @BT FD 2A 07 D1
    LD C',SLA (IX+((($71-4)-2)+4+2)) ; @BT DD CB 71 21
    RLC B'                 ; @BT CB 00
    LD C',SET 4,(IY+$5C)   ; @BT FD CB 5C E1
    LD (HL'),E'            ; @BT 73
    LD B',RES 7,(IY+$16)   ; @BT FD CB 16 B8
    RES 0,E'               ; @BT CB 83
    BSRL DE',B             ; @BT ED 2A
    SET 7,C'               ; @BT CB F9
    LD E',SET 4,(IX+$79)   ; @BT DD CB 79 E3
    RES 7,H'               ; @BT CB BC
    RST                    ; @BT C7
    LD E',SET 7,(IX+$0B)   ; @BT DD CB 0B FB
    SLA C'                 ; @BT CB 21
    SET 0,C'               ; @BT CB C1
    LD IYL,(((F5_FN_007(T5_DEF_019)-9)+(9-5))+5) ; @BT FD 2E 26
    NEXTREG (($F1-(5+9))+5+9),((($F1-4)+(4-8))+8) ; @BT ED 91 F1 F1
    LD B',SET 0,(IY+$34)   ; @BT FD CB 34 C0
    EX (SP),HL'            ; @BT E3
    LDIRX                  ; @BT ED B4
    LD A',RES 3,(IX+$6B)   ; @BT DD CB 6B 9F
    SET 1,C'               ; @BT CB C9
    SET 7,D'               ; @BT CB FA
    BIT 0,H'               ; @BT CB 44
    LD H',RES 5,(IY+$4A)   ; @BT FD CB 4A AC
    SRA H'                 ; @BT CB 2C
    LD C',RES 7,(IX+$23)   ; @BT DD CB 23 B9
    XOR A',(((F5_FN_008(T5_DEF_008,15)-(9+8))+(9+8))) ; @BT EE D3
    RRC (IY+((($0C-(3+1))+3)+1)) ; @BT FD CB 0C 0E
    DEC C'                 ; @BT 0D
    LD IYH,D'              ; @BT FD 62
    LD B',SET 0,(IX+$31)   ; @BT DD CB 31 C0
    RES 2,D'               ; @BT CB 92
    ADD A',(IY+((($6D-11)-(1-1))+(1-1)+11)) ; @BT FD 86 6D
    RL A'                  ; @BT CB 17
    LD IYH,IYL             ; @BT FD 65
    SET 5,H'               ; @BT CB EC
    CALL NEQ,((($B77F-(11+9))+9)+11) ; @BT C4 7F B7
    SBC A',D'              ; @BT 9A
    LD E',SET 3,(IX+$73)   ; @BT DD CB 73 DB
    RES 2,L'               ; @BT CB 95
    RLC E'                 ; @BT CB 03
    SET 7,(HL')            ; @BT CB FE
    RES 4,E'               ; @BT CB A3
    CPL                    ; @BT 2F
    LD A',SRL (IX+(($63-(5+8))+5+8)) ; @BT DD CB 63 3F
    BIT 4,L'               ; @BT CB 65
    LD E',SET 2,(IX+$6D)   ; @BT DD CB 6D D3
    CPDR                   ; @BT ED B9
    LD IXL,A'              ; @BT DD 6F
    RES 1,H'               ; @BT CB 8C
    ADD A',IXL             ; @BT DD 85
    SET 6,A'               ; @BT CB F7
    LD (IY+((($2C-2)+(2-5))+5)),A' ; @BT FD 77 2C
    LD A',SET 0,(IX+$21)   ; @BT DD CB 21 C7
    ADD HL,A'              ; @BT ED 31
    DEC (IY+(($68-(7+4))+7+4)) ; @BT FD 35 68
    RET PE                 ; @BT E8
    JP GTE,((($2223-2)+(2-9))+9) ; @BT D2 23 22
    BIT 6,E'               ; @BT CB 73
    IM ((2-(7+2))+7+2)     ; @BT ED 5E
    OR A',E'               ; @BT B3
    LD L',RES 3,(IY+$4E)   ; @BT FD CB 4E 9D
    LD H',RES 4,(IX+$41)   ; @BT DD CB 41 A4
    LD C',(IY+((($47-3)-(1-1))+(1-1)+3)) ; @BT FD 4E 47
    NOP                    ; @BT 00
    ADD HL',HL'            ; @BT 29
    DJNZ ((($65-(3+6))+(3+6))) ; @BT 10 65
    IM (((0-(5+2))+(5+2))) ; @BT ED 46
    BIT 4,(HL')            ; @BT CB 66
    SBC A',A'              ; @BT 9F
    LD IXL,B'              ; @BT DD 68
    OR A',A'               ; @BT B7
    LD (HL'),A'            ; @BT 77
    LD SP,((((T5_DEF_011-8)+(8-3))+3)) ; @BT ED 7B 09 B1
    JP LSS,((($2001-(8+1))+8)+1) ; @BT DA 01 20
    RES 1,D'               ; @BT CB 8A
    LD L',SET 7,(IY+$2E)   ; @BT FD CB 2E FD
    LD D',RES 5,(IY+$2A)   ; @BT FD CB 2A AA
    LD H',RL (IX+(($5F-(10+9))+10+9)) ; @BT DD CB 5F 14
    LD D',SET 0,(IX+$51)   ; @BT DD CB 51 C2
    RR (HL')               ; @BT CB 1E
    LD B',RES 3,(IX+$7B)   ; @BT DD CB 7B 98
    LD A',SRA (IY+((($60-(2+4))+(2+4)))) ; @BT FD CB 60 2F
    CP A',IXL              ; @BT DD BD
    SET 4,(HL')            ; @BT CB E6
    SL1 (HL')              ; @BT CB 36
    SET 2,E'               ; @BT CB D3
    LD H',B'               ; @BT 60
    LD A',SET 3,(IX+$33)   ; @BT DD CB 33 DF
    AND A',H'              ; @BT A4
    LD E',RES 3,(IY+$2E)   ; @BT FD CB 2E 9B
    BIT 0,(HL')            ; @BT CB 46
    CP A',L'               ; @BT BD
    LD IXL,IXH             ; @BT DD 6C
    LD H',SET 3,(IY+$06)   ; @BT FD CB 06 DC
    RR L'                  ; @BT CB 1D
    LD H',SRL (IY+((($36-8)+5)-5+8)) ; @BT FD CB 36 3C
    SET 6,H'               ; @BT CB F4
    LD (IX+((($6A-5)-(1-1))+(1-1)+5)),E' ; @BT DD 73 6A
    LD B',IYH              ; @BT FD 44
    RES 2,B'               ; @BT CB 90
    LD IXL,IXL             ; @BT DD 6D
    BIT 4,(IY+$69)         ; @BT FD CB 69 66
    SLA (HL')              ; @BT CB 26
    OUTI                   ; @BT ED A3
    DI                     ; @BT F3
    INC IXH                ; @BT DD 24
    DEC H'                 ; @BT 25
    SET 2,(HL')            ; @BT CB D6
    BIT 2,A'               ; @BT CB 57
    SRA (IY+((($39-6)+8)-8+6)) ; @BT FD CB 39 2E
    SLA A'                 ; @BT CB 27
    BIT 7,H'               ; @BT CB 7C
    BIT 1,H'               ; @BT CB 4C
    SRL E'                 ; @BT CB 3B
    BIT 3,(IX+$48)         ; @BT DD CB 48 5E
    LD (HL'),B'            ; @BT 70
    LD A',(IX+((($55-9)+(9-3))+3)) ; @BT DD 7E 55
    SUB A',L'              ; @BT 95
    LD D',RR (IX+(($4B-(3+3))+3+3)) ; @BT DD CB 4B 1A
    LD E',RES 4,(IY+$34)   ; @BT FD CB 34 A3
    LD L',SRL (IY+(($46-4)+4)) ; @BT FD CB 46 3D
    LD B',(IX+((($11-(6+5))+5)+6)) ; @BT DD 46 11
    LD B',RRC (IY+((($28-(6+1))+6)+1)) ; @BT FD CB 28 08
    LD B',SET 2,(IY+$40)   ; @BT FD CB 40 D0
    SET 0,E'               ; @BT CB C3
    PUSH BC'               ; @BT C5
    INDR                   ; @BT ED BA
    SET 7,A'               ; @BT CB FF
    RST ((($10-4)+(4-8))+8) ; @BT D7
    POP DE'                ; @BT D1
    XOR A',C'              ; @BT A9
    RES 0,(IX+$12)         ; @BT DD CB 12 86
    LD D',SET 3,(IY+$66)   ; @BT FD CB 66 DA
    LD ((((F5_FN_009(T5_DEF_014,6,17)-8)+7)-7+8)),IY ; @BT FD 22 05 F1
    LD ((((F5_FN_010(T5_DEF_033,4,3,7)-9)+3)-3+9)),SP ; @BT ED 73 F4 EF
    LD L',SET 4,(IX+$19)   ; @BT DD CB 19 E5
    DEC (HL')              ; @BT 35
    LD A',(((F5_FN_011(T5_DEF_016,19,14,12,10)-(3+9))+9)+3) ; @BT 3A C3 00
    LD A',RES 7,(IY+$06)   ; @BT FD CB 06 BF
    LD E',IYH              ; @BT FD 5C
    CPI                    ; @BT ED A1
    CALL LSS,((($B44C-(10+1))+10)+1) ; @BT DC 4C B4
    LD A',RES 1,(IY+$62)   ; @BT FD CB 62 8F
    LD L',RES 7,(IY+$66)   ; @BT FD CB 66 BD
    LD D',SRL (IY+(($16-(5+2))+5+2)) ; @BT FD CB 16 3A
    BIT 4,A'               ; @BT CB 67
    LD L',((($91-(5+1))+5)+1) ; @BT 2E 91
    LD B',C'               ; @BT 41
    LD B',H'               ; @BT 44
    LD L',SRL (IX+((($43-(6+1))+(6+1)))) ; @BT DD CB 43 3D
    SL1 H'                 ; @BT CB 34
    LD C',RR (IX+((($3B-10)+(10-4))+4)) ; @BT DD CB 3B 19
    LD D',SET 6,(IY+$78)   ; @BT FD CB 78 F2
    LD C',SET 1,(IY+$4A)   ; @BT FD CB 4A C9
    XOR A',(HL')           ; @BT AE
    ADC A',IXH             ; @BT DD 8C
    CCF                    ; @BT 3F
    LD H',C'               ; @BT 61
    LD B',SLA (IX+((($61-(2+1))+2)+1)) ; @BT DD CB 61 20
    LD E',RRC (IX+((($55-(4+1))+(4+1)))) ; @BT DD CB 55 0B
    LD IYL,C'              ; @BT FD 69
    SLA B'                 ; @BT CB 20
    LD A',RES 4,(IX+$71)   ; @BT DD CB 71 A7
    EI                     ; @BT FB
    LD E',SET 5,(IY+$02)   ; @BT FD CB 02 EB
    HALT                   ; @BT 76
    SUB A',E'              ; @BT 93
    RRD                    ; @BT ED 67
    SET 4,L'               ; @BT CB E5
    DEC BC'                ; @BT 0B
    RR B'                  ; @BT CB 18
    PUSH DE'               ; @BT D5
    LD E',SLL (IY+((($1A-(1+1))+1)+1)) ; @BT FD CB 1A 33
    ADC A',IYL             ; @BT FD 8D
    SL1 A'                 ; @BT CB 37
    LD C',RES 2,(IX+$05)   ; @BT DD CB 05 91
    JR ((($F7-11)+(11-1))+1) ; @BT 18 F7
    LD H',SET 0,(IY+$74)   ; @BT FD CB 74 C4
    LD B',SET 5,(IY+$52)   ; @BT FD CB 52 E8
    RES 0,A'               ; @BT CB 87
    ADD IX,DE'             ; @BT DD 19
    RES 0,B'               ; @BT CB 80
    BIT 0,A'               ; @BT CB 47
    LD A',SET 2,(IX+$2D)   ; @BT DD CB 2D D7
    LD D',RES 0,(IX+$09)   ; @BT DD CB 09 82
    LD I,A'                ; @BT ED 47
    LD A',E'               ; @BT 7B
    LD D',IYL              ; @BT FD 55
    LD (IY+((($3B-9)+5)-5+9)),H' ; @BT FD 74 3B
    IN F',(C')             ; @BT ED 70
    RES 3,(IY+$6F)         ; @BT FD CB 6F 9E
    IN C',(C')             ; @BT ED 48
    RL L'                  ; @BT CB 15
    DEC E'                 ; @BT 1D
    SETAE                  ; @BT ED 95
    LD D',(IX+((($74-(2+7))+(2+7)))) ; @BT DD 56 74
    XOR A',IYL             ; @BT FD AD
    LD E',((($B1-5)+(5-5))+5) ; @BT 1E B1
    LD (IY+((($2F-(8+5))+5)+8)),B' ; @BT FD 70 2F
    SET 6,D'               ; @BT CB F2
    INC IXL                ; @BT DD 2C
    DEC IX                 ; @BT DD 2B
    BIT 2,L'               ; @BT CB 55
    LD BC',(((($4776-(9+8))+8)+9)) ; @BT ED 4B 76 47
    ADD A',A'              ; @BT 87
    SET 0,D'               ; @BT CB C2
    LD A',SRA (IX+((($5D-6)+(6-9))+9)) ; @BT DD CB 5D 2F
    LD C',RES 1,(IY+$02)   ; @BT FD CB 02 89
    LD A',IYL              ; @BT FD 7D
    LD D',IYH              ; @BT FD 54
    BIT 1,A'               ; @BT CB 4F
    LD B',SET 5,(IX+$4F)   ; @BT DD CB 4F E8
    SET 3,(IY+$67)         ; @BT FD CB 67 DE
    ADC HL',SP             ; @BT ED 7A
    BIT 5,(HL')            ; @BT CB 6E
    LD IXH,(((F5_FN_012(T5_DEF_001,4,19,12,5,11)-9)+4)-4+9) ; @BT DD 26 18
    LD C',SET 1,(IX+$47)   ; @BT DD CB 47 C9
    POP HL'                ; @BT E1
    SLA D'                 ; @BT CB 22
    LD IXH,C'              ; @BT DD 61
    LD L',RES 4,(IY+$54)   ; @BT FD CB 54 A5
    INC D'                 ; @BT 14
    LD (IX+((($61-6)+(6-3))+3)),B' ; @BT DD 70 61
    LD B',RES 2,(IX+$75)   ; @BT DD CB 75 90
    LD A',RLC (IX+((($09-(4+3))+3)+4)) ; @BT DD CB 09 07
    SET 4,(IX+$02)         ; @BT DD CB 02 E6
    LD B',RES 4,(IX+$01)   ; @BT DD CB 01 A0
    LD D',SET 2,(IY+$60)   ; @BT FD CB 60 D2
    LD L',SET 5,(IX+$1F)   ; @BT DD CB 1F ED
    SET 7,L'               ; @BT CB FD
    SBC HL',BC'            ; @BT ED 42
    LD L',SET 3,(IX+$13)   ; @BT DD CB 13 DD
    RES 5,B'               ; @BT CB A8
    LD D',IXH              ; @BT DD 54
    RES 0,(IY+$15)         ; @BT FD CB 15 86
    LD A',SLL (IX+(($57-(9+3))+9+3)) ; @BT DD CB 57 37
    SET 2,B'               ; @BT CB D0
    LD C',IYH              ; @BT FD 4C
    RES 6,(IX+$46)         ; @BT DD CB 46 B6
    LD D',RRC (IY+((($48-4)+(4-3))+3)) ; @BT FD CB 48 0A
    SUB A',IYL             ; @BT FD 95
    ADD HL',SP             ; @BT 39
    IND                    ; @BT ED AA
    LD L',SET 6,(IX+$25)   ; @BT DD CB 25 F5
    LD D',SET 7,(IY+$7E)   ; @BT FD CB 7E FA
    LD H',SLA (IX+((($21-11)-(8-1))+(8-1)+11)) ; @BT DD CB 21 24
    LD L',SLL (IX+((($37-6)-(5-1))+(5-1)+6)) ; @BT DD CB 37 35
    RRC L'                 ; @BT CB 0D
    RES 0,(IX+$12)         ; @BT DD CB 12 86
    LD D',SET 7,(IX+$7B)   ; @BT DD CB 7B FA
    SET 5,D'               ; @BT CB EA
    ADC A',B'              ; @BT 88
    LD H',RES 7,(IX+$53)   ; @BT DD CB 53 BC
    OR A',D'               ; @BT B2
    SET 4,E'               ; @BT CB E3
    RES 3,C'               ; @BT CB 99
    LD ((((T5_DEF_004-(5+7))+7)+5)),BC' ; @BT ED 43 C1 EC
    LD C',RES 3,(IX+$0B)   ; @BT DD CB 0B 99
    LD B',RR (IY+((($2E-(1+1))+1)+1)) ; @BT FD CB 2E 18
    LD A',RES 4,(IY+$74)   ; @BT FD CB 74 A7
    LD C',SLA (IY+((($74-10)+6)-6+10)) ; @BT FD CB 74 21
    BSLA DE',B             ; @BT ED 28
    LD A',RL (IY+(($12-(3+9))+3+9)) ; @BT FD CB 12 17
    RES 6,B'               ; @BT CB B0
    SWAPNIB                ; @BT ED 23
    SRA (HL')              ; @BT CB 2E
    LD B',RES 6,(IY+$10)   ; @BT FD CB 10 B0
    RRC (IX+((($09-(7+9))+(7+9)))) ; @BT DD CB 09 0E
    RRC B'                 ; @BT CB 08
    LD B',SET 3,(IY+$46)   ; @BT FD CB 46 D8
    SET 5,C'               ; @BT CB E9
    LD A',RRC (IY+((($18-7)+(7-10))+10)) ; @BT FD CB 18 0F
    LD B',IXH              ; @BT DD 44
    LD H',(HL')            ; @BT 66
    SBC HL',SP             ; @BT ED 72
    LD H',((($AE-10)-(4-1))+(4-1)+10) ; @BT 26 AE
    LD C',RES 4,(IX+$11)   ; @BT DD CB 11 A1
    SET 5,B'               ; @BT CB E8
    LD B',SET 3,(IX+$43)   ; @BT DD CB 43 D8
    LD E',RES 0,(IX+$19)   ; @BT DD CB 19 83
    LD ((($EEE3-4)+4)),HL' ; @BT 22 E3 EE
    BIT 1,(IY+$0F)         ; @BT FD CB 0F 4E
    LD A',R                ; @BT ED 5F
    LD E',RL (IY+((($52-(1+1))+1)+1)) ; @BT FD CB 52 13
    LD H',D'               ; @BT 62
    LD D',E'               ; @BT 53
    SLA (IX+((($18-(6+9))+(6+9)))) ; @BT DD CB 18 26
    LD L',C'               ; @BT 69
    XOR A',(IX+((($2B-5)+(5-9))+9)) ; @BT DD AE 2B
    BIT 3,L'               ; @BT CB 5D
    LD C',D'               ; @BT 4A
    LD E',SET 6,(IX+$05)   ; @BT DD CB 05 F3
    SET 1,D'               ; @BT CB CA
    LD B',RES 2,(IY+$78)   ; @BT FD CB 78 90
    BIT 0,D'               ; @BT CB 42
    DEC IY                 ; @BT FD 2B
    SET 5,A'               ; @BT CB EF
    INC B'                 ; @BT 04
    LD C',SET 6,(IY+$68)   ; @BT FD CB 68 F1
    LD D',RES 1,(IX+$0F)   ; @BT DD CB 0F 8A
    LD B',RES 0,(IY+$6C)   ; @BT FD CB 6C 80
    LD (IY+((($38-(4+8))+(4+8)))),E' ; @BT FD 73 38
    ADD IX,SP              ; @BT DD 39
    INC IX                 ; @BT DD 23
    OR A',B'               ; @BT B0
    RR E'                  ; @BT CB 1B
    SET 1,(IX+$28)         ; @BT DD CB 28 CE
    XOR A',H'              ; @BT AC
    LD E',RES 2,(IX+$25)   ; @BT DD CB 25 93
    BIT 6,H'               ; @BT CB 74
    SET 3,C'               ; @BT CB D9
    BIT 4,(IX+$66)         ; @BT DD CB 66 66
    SL1 E'                 ; @BT CB 33
    BIT 2,B'               ; @BT CB 50
    LD B',SLL (IX+((($67-6)+5)-5+6)) ; @BT DD CB 67 30
    SET 6,L'               ; @BT CB F5
    IN H',(C')             ; @BT ED 60
    RES 6,L'               ; @BT CB B5
    ADD A',D'              ; @BT 82
    SET 1,L'               ; @BT CB CD
    LD L',RES 4,(IX+$51)   ; @BT DD CB 51 A5
    RES 5,H'               ; @BT CB AC
    LD IYL,IYH             ; @BT FD 6C
    RET NEQ                ; @BT C0
    RST ((($18-5)+(5-3))+3) ; @BT DF
    RES 4,B'               ; @BT CB A0
    ADC HL',DE'            ; @BT ED 5A
    CP A',(IX+((($11-4)-(7-1))+(7-1)+4)) ; @BT DD BE 11
    INI                    ; @BT ED A2
    ADD A',C'              ; @BT 81
    LD D',SET 2,(IX+$5D)   ; @BT DD CB 5D D2
    POP AF'                ; @BT F1
    IN D',(C')             ; @BT ED 50
    LD C',SET 3,(IY+$56)   ; @BT FD CB 56 D9
    INC IY                 ; @BT FD 23
    RES 0,H'               ; @BT CB 84
    LD B',SET 7,(IY+$5E)   ; @BT FD CB 5E F8
    CALL NEG,(($B55D-(10+5))+10+5) ; @BT FC 5D B5
    NEG                    ; @BT ED 44
    LD C',RES 5,(IX+$17)   ; @BT DD CB 17 A9
    LD A',RES 6,(IX+$7D)   ; @BT DD CB 7D B7
    LD E',C'               ; @BT 59
    BIT 6,(IY+$25)         ; @BT FD CB 25 76
    LD H',RES 0,(IX+$29)   ; @BT DD CB 29 84
    XOR A',IXL             ; @BT DD AD
    RST ((($30-6)+4)-4+6)  ; @BT F7
    LD A',RR (IX+((($1B-(11+7))+(11+7)))) ; @BT DD CB 1B 1F
    LD (IX+((($67-5)+(5-9))+9)),D' ; @BT DD 72 67
    LDI                    ; @BT ED A0
    LD D',RRC (IX+(($45-(8+7))+8+7)) ; @BT DD CB 45 0A
    LD C',IYL              ; @BT FD 4D
    LD B',RLC (IX+((($19-9)+7)-7+9)) ; @BT DD CB 19 00
    OR A',IXL              ; @BT DD B5
    SUB A',H'              ; @BT 94
    SET 1,A'               ; @BT CB CF
    RES 5,L'               ; @BT CB AD
    LD B',RES 5,(IY+$0A)   ; @BT FD CB 0A A8
    BIT 7,D'               ; @BT CB 7A
    LD D',SET 5,(IY+$72)   ; @BT FD CB 72 EA
    ADC A',(IY+((($34-(11+3))+3)+11)) ; @BT FD 8E 34
    SBC A',IYL             ; @BT FD 9D
    ADD BC,A'              ; @BT ED 33
    LD H',RES 3,(IX+$3B)   ; @BT DD CB 3B 9C
    RES 3,H'               ; @BT CB 9C
    LD C',RRC (IY+((($38-4)+(4-8))+8)) ; @BT FD CB 38 09
    LD D',SET 1,(IX+$57)   ; @BT DD CB 57 CA
    RRA                    ; @BT 1F
    CP A',D'               ; @BT BA
    RES 3,E'               ; @BT CB 9B
    LD E',SET 1,(IY+$6A)   ; @BT FD CB 6A CB
    LD HL',(($9771-4)+4)   ; @BT 2A 71 97
    LD L',RR (IX+(($7B-(8+9))+8+9)) ; @BT DD CB 7B 1D
    LD L',RES 2,(IX+$45)   ; @BT DD CB 45 95
    JP EQU,(((F5_FN_013(T5_DEF_031)-9)-(5-1))+(5-1)+9) ; @BT CA 56 25
    LD C',SET 5,(IX+$5F)   ; @BT DD CB 5F E9
    XOR A',IXH             ; @BT DD AC
    POP IY                 ; @BT FD E1
    LD H',RR (IY+((($6E-(7+4))+(7+4)))) ; @BT FD CB 6E 1C
    LD C',SRA (IY+(($00-6)+6)) ; @BT FD CB 00 29
    LD B',D'               ; @BT 42
    TEST ((F5_FN_014(T5_DEF_013,13)-(9+2))+9+2) ; @BT ED 27 7A
    LD L',RES 5,(IX+$57)   ; @BT DD CB 57 AD
    SET 2,(IY+$49)         ; @BT FD CB 49 D6
    LD L',RES 1,(IY+$42)   ; @BT FD CB 42 8D
    RET EQU                ; @BT C8
    LD H',(IY+((($4B-(7+5))+5)+7)) ; @BT FD 66 4B
    RES 0,(HL')            ; @BT CB 86
    LD C',SRL (IX+(($03-9)+9)) ; @BT DD CB 03 39
    ADD A',IXH             ; @BT DD 84
    LD E',L'               ; @BT 5D
    LD A',RLC (IY+(($0C-(6+9))+6+9)) ; @BT FD CB 0C 07
    RES 0,D'               ; @BT CB 82
    OUTINB                 ; @BT ED 90
    LD A',(HL')            ; @BT 7E
    LD D',SRA (IX+((($0D-6)+(6-2))+2)) ; @BT DD CB 0D 2A
    SET 3,A'               ; @BT CB DF
    SET 6,(HL')            ; @BT CB F6
    LD SP,IX               ; @BT DD F9
    LD E',SET 4,(IY+$7C)   ; @BT FD CB 7C E3
    SET 6,(IY+$41)         ; @BT FD CB 41 F6
    LD A',SET 1,(IX+$27)   ; @BT DD CB 27 CF
    RR D'                  ; @BT CB 1A
    RLC C'                 ; @BT CB 01
    LD C',(IX+((($3E-2)+(2-9))+9)) ; @BT DD 4E 3E
    LD D',RES 1,(IY+$12)   ; @BT FD CB 12 8A
    SET 3,B'               ; @BT CB D8
    LD H',RES 2,(IY+$38)   ; @BT FD CB 38 94
    BIT 4,E'               ; @BT CB 63
    LD A',RR (IY+((($1E-10)-9)+10+9)) ; @BT FD CB 1E 1F
    BIT 6,L'               ; @BT CB 75
    RLC (IY+(($44-(7+7))+7+7)) ; @BT FD CB 44 06
    RET PO                 ; @BT E0
    RES 2,H'               ; @BT CB 94
    LD L',RES 0,(IX+$39)   ; @BT DD CB 39 85
    LD (IX+((($70-(7+6))+(7+6)))),L' ; @BT DD 75 70
    LD B',SET 4,(IY+$4C)   ; @BT FD CB 4C E0
    RL B'                  ; @BT CB 10
    RES 3,L'               ; @BT CB 9D
    ADD HL',BC'            ; @BT 09
    SUB A',(IX+((($78-(9+6))+(9+6)))) ; @BT DD 96 78
    LD B',RL (IY+((($22-5)+(5-7))+7)) ; @BT FD CB 22 10
    LD L',RES 5,(IY+$5A)   ; @BT FD CB 5A AD
    RES 0,L'               ; @BT CB 85
    LD D',RES 4,(IX+$21)   ; @BT DD CB 21 A2
    DEC IXH                ; @BT DD 25
    LD HL',(((T5_DEF_022-4)+4)) ; @BT 2A 60 96
    ADC A',(HL')           ; @BT 8E
    LD E',SRL (IY+((($26-(10+1))+10)+1)) ; @BT FD CB 26 3B
    RES 1,C'               ; @BT CB 89
    OTIR                   ; @BT ED B3
    EX (SP),IY             ; @BT FD E3
    RL H'                  ; @BT CB 14
    LD H',SET 7,(IY+$1E)   ; @BT FD CB 1E FC
    BIT 1,(IX+$0C)         ; @BT DD CB 0C 4E
    LD E',RES 5,(IY+$3A)   ; @BT FD CB 3A AB
    LDPIRX                 ; @BT ED B7
    LD C',SET 7,(IY+$6E)   ; @BT FD CB 6E F9
    LD C',RES 1,(IX+$7F)   ; @BT DD CB 7F 89
    RES 4,D'               ; @BT CB A2
    SET 1,E'               ; @BT CB CB
    OUT (C'),L'            ; @BT ED 69
    OR A',IXH              ; @BT DD B4
    LD D',((($58-(7+1))+7)+1) ; @BT 16 58
    JP NEG,((($2112-9)+(9-4))+4) ; @BT FA 12 21
    BIT 0,B'               ; @BT CB 40
    LD L',H'               ; @BT 6C
    SET 2,H'               ; @BT CB D4
    LD E',RES 1,(IY+$22)   ; @BT FD CB 22 8B
    LD IY,(((($CFF6-(8+7))+7)+8)) ; @BT FD 2A F6 CF
    LD L',RES 3,(IX+$4B)   ; @BT DD CB 4B 9D
    LD C',RR (IY+((($3E-(4+1))+4)+1)) ; @BT FD CB 3E 19
    LD D',RES 3,(IX+$1B)   ; @BT DD CB 1B 9A
    RES 5,(HL')            ; @BT CB AE
    LD SP,(((F5_FN_015(T5_DEF_007,14,12)-(2+1))+2)+1) ; @BT ED 7B 1A B2
    RES 6,A'               ; @BT CB B7
    LD IYL,B'              ; @BT FD 68
    LD D',RES 3,(IY+$1E)   ; @BT FD CB 1E 9A
    LD H',SET 7,(IX+$1B)   ; @BT DD CB 1B FC
    RES 3,A'               ; @BT CB 9F
    BIT 2,(IY+$2D)         ; @BT FD CB 2D 56
    LD IYH,B'              ; @BT FD 60
    BIT 1,(HL')            ; @BT CB 4E
    LD C',RES 7,(IY+$26)   ; @BT FD CB 26 B9
    LD H',RR (IX+((($6B-3)+(3-3))+3)) ; @BT DD CB 6B 1C
    SBC HL',HL'            ; @BT ED 62
    LD A',RES 3,(IY+$6E)   ; @BT FD CB 6E 9F
    LD H',RES 0,(IY+$2C)   ; @BT FD CB 2C 84
    LD H',RES 6,(IX+$4D)   ; @BT DD CB 4D B4
    LD C',RES 4,(IY+$14)   ; @BT FD CB 14 A1
    LD D',B'               ; @BT 50
    LD IYH,C'              ; @BT FD 61
    DEC IYL                ; @BT FD 2D
    BSRA DE',B             ; @BT ED 29
    SET 0,(IX+$0A)         ; @BT DD CB 0A C6
    SET 5,E'               ; @BT CB EB
    RES 0,(IY+$15)         ; @BT FD CB 15 86
    BIT 5,(IY+$07)         ; @BT FD CB 07 6E
    ADC A',E'              ; @BT 8B
    SLA E'                 ; @BT CB 23
    RES 2,(IY+$51)         ; @BT FD CB 51 96
    ADC A',C'              ; @BT 89
    INC A'                 ; @BT 3C
    LD (BC'),A'            ; @BT 02
    LD IXL,D'              ; @BT DD 6A
    SET 0,A'               ; @BT CB C7
    RST ((($20-5)-(2-1))+(2-1)+5) ; @BT E7
    LD E',RES 4,(IX+$31)   ; @BT DD CB 31 A3
    LD E',RES 0,(IY+$1C)   ; @BT FD CB 1C 83
    LD L',SET 1,(IY+$0A)   ; @BT FD CB 0A CD
    LD L',A'               ; @BT 6F
    NEXTREG ((($DA-(4+3))+3)+4),A' ; @BT ED 92 DA
    LD E',RES 6,(IX+$3D)   ; @BT DD CB 3D B3
    RES 3,B'               ; @BT CB 98
    LD A',H'               ; @BT 7C
    RES 2,(HL')            ; @BT CB 96
    RR (IY+((($6B-(5+7))+7)+5)) ; @BT FD CB 6B 1E
    LD IYL,IYL             ; @BT FD 6D
    OUTD                   ; @BT ED AB
    IM (((1-6)+(6-4))+4)   ; @BT ED 56
    LDDRX                  ; @BT ED BC
    JP PE,((($2667-1)+(1-11))+11) ; @BT EA 67 26
    RES 5,E'               ; @BT CB AB
    JP POS,((($2445-10)-8)+10+8) ; @BT F2 45 24
    LD H',SRA (IY+((($30-6)+(6-4))+4)) ; @BT FD CB 30 2C
    RL E'                  ; @BT CB 13
    SBC A',B'              ; @BT 98
    LD A',SET 7,(IX+$4B)   ; @BT DD CB 4B FF
    IN B',(C')             ; @BT ED 40
    RES 7,A'               ; @BT CB BF
    LD D',SLL (IX+((($07-(3+1))+3)+1)) ; @BT DD CB 07 32
    LD B',RES 1,(IX+$6F)   ; @BT DD CB 6F 88
    RES 4,(IY+$0D)         ; @BT FD CB 0D A6
    LD R,A'                ; @BT ED 4F
    ADD DE,A'              ; @BT ED 32
    SRA (IX+((($36-9)+6)-6+9)) ; @BT DD CB 36 2E
    SET 1,(IY+$2B)         ; @BT FD CB 2B CE
    LD H',SET 2,(IX+$7D)   ; @BT DD CB 7D D4
    LD L',(HL')            ; @BT 6E
    LD IXH,A'              ; @BT DD 67
    LD H',SET 6,(IX+$15)   ; @BT DD CB 15 F4
    LD L',RES 7,(IX+$63)   ; @BT DD CB 63 BD
    DEC D'                 ; @BT 15
    AND A',(IY+(($50-(2+4))+2+4)) ; @BT FD A6 50
    RRCA                   ; @BT 0F
    LD C',SET 6,(IX+$65)   ; @BT DD CB 65 F1
    SET 7,B'               ; @BT CB F8
    ADC A',A'              ; @BT 8F
    RLC L'                 ; @BT CB 05
    RET                    ; @BT C9
    SUB A',IXH             ; @BT DD 94
    LD E',RR (IY+((($5E-3)+(3+8))-8)) ; @BT FD CB 5E 1B
    AND A',((($FF-8)+(8-3))+3) ; @BT E6 FF
    LD B',RES 5,(IX+$07)   ; @BT DD CB 07 A8
    LD D',SLA (IY+((($04-10)+(10+7))-7)) ; @BT FD CB 04 22
    SET 4,H'               ; @BT CB E4
    ADD A',H'              ; @BT 84
    ADD A',E'              ; @BT 83
    LD C',RLC (IY+((($2C-9)+(9+1))-1)) ; @BT FD CB 2C 01
    LD IXH,B'              ; @BT DD 60
    OR A',L'               ; @BT B5
    OR A',(HL')            ; @BT B6
    SET 6,(IX+$3E)         ; @BT DD CB 3E F6
    LD C',SLL (IX+((($77-(2+3))+(2+3)))) ; @BT DD CB 77 31
    BIT 4,D'               ; @BT CB 62
    BIT 3,(HL')            ; @BT CB 5E
    LD E',IXL              ; @BT DD 5D
    RES 6,D'               ; @BT CB B2
    OR A',IYH              ; @BT FD B4
    LD IYH,(((T5_DEF_005-8)+(8-11))+11) ; @BT FD 26 6E
    JP (HL')               ; @BT E9
    RES 6,E'               ; @BT CB B3
    SET 0,(IY+$0D)         ; @BT FD CB 0D C6
    OR A',(IY+((($1D-2)+(2-11))+11)) ; @BT FD B6 1D
    LD (((F5_FN_016(T5_DEF_006,8,12,11)-9)+9)),IX ; @BT DD 22 16 F2
    LD E',RES 7,(IY+$46)   ; @BT FD CB 46 BB
    LD E',D'               ; @BT 5A
    LD C',C'               ; @BT 49
    CALL PE,((F5_FN_017(T5_DEF_025,19,6,5,7)-10)+10) ; @BT EC B2 BA
    LD B',A'               ; @BT 47
    LD L',RES 0,(IY+$3C)   ; @BT FD CB 3C 85
    RRC C'                 ; @BT CB 09
    LD H',SET 4,(IX+$09)   ; @BT DD CB 09 E4
    SBC A',(HL')           ; @BT 9E
    JR GTE,(((T5_DEF_009-(11+8))+8)+11) ; @BT 30 B2
    LD A',RES 5,(IX+$77)   ; @BT DD CB 77 AF
    LD E',(IX+((($21-(6+4))+(6+4)))) ; @BT DD 5E 21
    LD H',(IX+(($48-(11+9))+11+9)) ; @BT DD 66 48
    LD B',SLA (IY+((($64-5)+2)-2+5)) ; @BT FD CB 64 20
    SRA B'                 ; @BT CB 28
    RES 1,B'               ; @BT CB 88
    AND A',E'              ; @BT A3
    OUT (C'),D'            ; @BT ED 51
    LD H',L'               ; @BT 65
    LD A',SET 1,(IY+$2A)   ; @BT FD CB 2A CF
    LD B',RES 7,(IX+$13)   ; @BT DD CB 13 B8
    INC BC'                ; @BT 03
    LD SP,HL'              ; @BT F9
    CP A',B'               ; @BT B8
    BIT 3,E'               ; @BT CB 5B
    BIT 1,E'               ; @BT CB 4B
    JP (IY)                ; @BT FD E9
    LD H',SET 5,(IX+$0F)   ; @BT DD CB 0F EC
    ADD A',B'              ; @BT 80
    LD D',A'               ; @BT 57
    LD IXL,E'              ; @BT DD 6B
    AND A',D'              ; @BT A2
    SET 7,E'               ; @BT CB FB
    SL1 L'                 ; @BT CB 35
    LD L',RLC (IY+((($6C-(9+8))+(9+8)))) ; @BT FD CB 6C 05
    AND A',IYL             ; @BT FD A5
    RES 7,D'               ; @BT CB BA
    RES 6,H'               ; @BT CB B4
    RES 4,A'               ; @BT CB A7
    SBC A',H'              ; @BT 9C
    DEC A'                 ; @BT 3D
    SBC HL',DE'            ; @BT ED 52
    LD E',RLC (IX+((($49-4)-(3-1))+(3-1)+4)) ; @BT DD CB 49 03
    SUB A',IXL             ; @BT DD 95
    PUSH HL'               ; @BT E5
    LD A',RES 2,(IX+$65)   ; @BT DD CB 65 97
    BIT 7,E'               ; @BT CB 7B
    LD H',H'               ; @BT 64
    LD A',L'               ; @BT 7D
    BIT 2,(IX+$2A)         ; @BT DD CB 2A 56
    LD C',(HL')            ; @BT 4E
    INC SP                 ; @BT 33
    SL1 D'                 ; @BT CB 32
    LD H',SRL (IX+((($33-2)+(2-2))+2)) ; @BT DD CB 33 3C
    SUB A',B'              ; @BT 90
    BIT 5,A'               ; @BT CB 6F
    BIT 4,H'               ; @BT CB 64
    BIT 6,B'               ; @BT CB 70
    BIT 3,C'               ; @BT CB 59
    BIT 4,B'               ; @BT CB 60
    ADD IX,BC'             ; @BT DD 09
    SET 3,H'               ; @BT CB DC
    SUB A',IYH             ; @BT FD 94
    RLC (IY+((($44-2)+4)-4+2)) ; @BT FD CB 44 06
    BIT 5,E'               ; @BT CB 6B
    LD C',RES 0,(IY+$7C)   ; @BT FD CB 7C 81
    LD L',(IY+((($0A-3)+5)-5+3)) ; @BT FD 6E 0A
    LD E',SLA (IY+((($14-(4+8))+8)+4)) ; @BT FD CB 14 23
    CP A',(((F5_FN_018(T5_DEF_023,2,6,6,12,15)-(10+6))+(10+6))) ; @BT FE E2
    LD D',RR (IY+((($4E-6)+6)-6+6)) ; @BT FD CB 4E 1A
    RES 5,A'               ; @BT CB AF
    SUB A',A'              ; @BT 97
    LD E',RLC (IY+((($4C-5)+(5-9))+9)) ; @BT FD CB 4C 03
    DEC SP                 ; @BT 3B
    ADC HL',BC'            ; @BT ED 4A
    LD E',(IY+((($2A-(6+2))+(6+2)))) ; @BT FD 5E 2A
    LD C',B'               ; @BT 48
    LD H',SET 6,(IY+$18)   ; @BT FD CB 18 F4
    PUSH IY                ; @BT FD E5
    LD (IX+((($6D-(9+1))+9)+1)),H' ; @BT DD 74 6D
    EX AF',AF              ; @BT 08
    LD H',RES 4,(IY+$44)   ; @BT FD CB 44 A4
    RR A'                  ; @BT CB 1F
    LD A',(IY+((($5E-(4+6))+(4+6)))) ; @BT FD 7E 5E
    LD C',IXH              ; @BT DD 4C
    ADD HL',DE'            ; @BT 19
    RL D'                  ; @BT CB 12
    LD A',RES 6,(IY+$00)   ; @BT FD CB 00 B7
    LD B',RL (IX+((($1F-(8+8))+8)+8)) ; @BT DD CB 1F 10
    SET 3,(HL')            ; @BT CB DE
    ADD IY,IY              ; @BT FD 29
    RES 1,(IY+$33)         ; @BT FD CB 33 8E
    LD (HL'),(((T5_DEF_032-8)+7)-7+8) ; @BT 36 94
    LD E',RL (IX+((($4F-(9+9))+9)+9)) ; @BT DD CB 4F 13
    BIT 6,(IX+$22)         ; @BT DD CB 22 76
    SRL (HL')              ; @BT CB 3E
    RES 1,(HL')            ; @BT CB 8E
    LD B',SRL (IY+((($76-(6+1))+6)+1)) ; @BT FD CB 76 38
    SL1 C'                 ; @BT CB 31
    LD E',SET 5,(IX+$7F)   ; @BT DD CB 7F EB
    CPD                    ; @BT ED A9
    BIT 0,(IX+$6E)         ; @BT DD CB 6E 46
    BIT 7,B'               ; @BT CB 78
    ADD IY,BC'             ; @BT FD 09
    LD L',SET 6,(IY+$28)   ; @BT FD CB 28 F5
    LD B',RLC (IY+((($1C-(4+7))+7)+4)) ; @BT FD CB 1C 00
    SET 6,E'               ; @BT CB F3
    BIT 5,B'               ; @BT CB 68
    SBC A',(IY+((($5A-(5+6))+6)+5)) ; @BT FD 9E 5A
    LD D',(HL')            ; @BT 56
    LD (IY+((($3E-9)-5)+9+5)),L' ; @BT FD 75 3E
    PUSH AF'               ; @BT F5
    JP PO,((($2778-2)+(2-6))+6) ; @BT E2 78 27
    LD L',SET 0,(IX+$01)   ; @BT DD CB 01 C5
    OUT (C'),E'            ; @BT ED 59
    LD L',RR (IY+((($7E-9)+(9-8))+8)) ; @BT FD CB 7E 1D
    LD L',SET 2,(IY+$10)   ; @BT FD CB 10 D5
    LD C',SET 0,(IY+$44)   ; @BT FD CB 44 C1
    RES 4,(IX+$0A)         ; @BT DD CB 0A A6
    LD (IX+(($5E-(3+2))+3+2)),A' ; @BT DD 77 5E
    OR A',IYL              ; @BT FD B5
    INC (IY+((($66-10)+(10+6))-6)) ; @BT FD 34 66
    XOR A',(IY+((($34-(1+1))+1)+1)) ; @BT FD AE 34
    RRC A'                 ; @BT CB 0F
    LD E',RES 7,(IX+$43)   ; @BT DD CB 43 BB
    LD E',RRC (IY+(($58-(3+2))+3+2)) ; @BT FD CB 58 0B
    SET 2,A'               ; @BT CB D7
    LD B',(HL')            ; @BT 46
    LD (HL'),C'            ; @BT 71
    BIT 0,(IY+$71)         ; @BT FD CB 71 46
    LD C',RES 0,(IX+$79)   ; @BT DD CB 79 81
    RES 2,(IX+$4E)         ; @BT DD CB 4E 96
    SLA L'                 ; @BT CB 25
    INC (HL')              ; @BT 34
    LD C',E'               ; @BT 4B
    LD B',(($61-(4+3))+4+3) ; @BT 06 61
    BIT 0,C'               ; @BT CB 41
    RL C'                  ; @BT CB 11
    LD H',RES 2,(IX+$35)   ; @BT DD CB 35 94
    LD D',SET 0,(IY+$54)   ; @BT FD CB 54 C2
    SRL H'                 ; @BT CB 3C
    LD IX,(((T5_DEF_018-(9+5))+5)+9) ; @BT DD 2A 55 B5
    CALL (($BCD4-6)+6)     ; @BT CD D4 BC
    RR C'                  ; @BT CB 19
    SCF                    ; @BT 37
    LD L',RRC (IY+(($78-(4+7))+4+7)) ; @BT FD CB 78 0D
    LD L',D'               ; @BT 6A
    RES 6,C'               ; @BT CB B1
    LD C',SET 2,(IY+$50)   ; @BT FD CB 50 D1
    LD B',B'               ; @BT 40
    BIT 4,C'               ; @BT CB 61
    SET 3,D'               ; @BT CB DA
    LD H',RES 3,(IY+$3E)   ; @BT FD CB 3E 9C
    LD A',IYH              ; @BT FD 7C
    LD B',SRA (IX+((($6D-5)+3)-3+5)) ; @BT DD CB 6D 28
    SRL B'                 ; @BT CB 38
    SRL (IX+((($54-1)+(1-2))+2)) ; @BT DD CB 54 3E
    LD B',SET 6,(IY+$58)   ; @BT FD CB 58 F0
    LD C',(($BA-(10+8))+10+8) ; @BT 0E BA
    LD D',SET 4,(IY+$6C)   ; @BT FD CB 6C E2
    SRL L'                 ; @BT CB 3D
    LD (IY+(($41-11)+11)),((($DE-8)+8)-8+8) ; @BT FD 36 41 DE
    LD IXH,IXH             ; @BT DD 64
    ADC A',L'              ; @BT 8D
    BIT 2,(HL')            ; @BT CB 56
    SL1 (IX+((($7A-5)+6)-6+5)) ; @BT DD CB 7A 36
    LD L',SRA (IX+((($3D-(11+3))+(11+3)))) ; @BT DD CB 3D 2D
    BIT 7,(IY+$43)         ; @BT FD CB 43 7E
    LD B',RES 3,(IY+$7E)   ; @BT FD CB 7E 98
    ADC A',IYH             ; @BT FD 8C
    LD H',E'               ; @BT 63
    LD E',SLA (IX+((($11-6)+(6-6))+6)) ; @BT DD CB 11 23
    LD A',SET 4,(IY+$3C)   ; @BT FD CB 3C E7
    LD C',SLL (IY+((($7A-8)+(8-9))+9)) ; @BT FD CB 7A 31
    SET 3,L'               ; @BT CB DD
    CALL PO,(((T5_DEF_015-7)-(1-1))+(1-1)+7) ; @BT E4 C3 BB
    XOR A',A'              ; @BT AF
    RETI                   ; @BT ED 4D
    RES 7,(HL')            ; @BT CB BE
    SBC A',E'              ; @BT 9B
    LD C',SET 0,(IX+$41)   ; @BT DD CB 41 C1
    LD E',SET 0,(IY+$64)   ; @BT FD CB 64 C3
    LD C',RES 6,(IX+$1D)   ; @BT DD CB 1D B1
    RLCA                   ; @BT 07
    LD D',RES 7,(IX+$33)   ; @BT DD CB 33 BA
    LD D',SRA (IY+((($10-5)+6)-6+5)) ; @BT FD CB 10 2A
    RES 2,A'               ; @BT CB 97
    LD B',SRA (IY+(($70-11)+11)) ; @BT FD CB 70 28

    .db "<05"               ; @BT END
