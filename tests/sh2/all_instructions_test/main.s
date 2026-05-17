; @BT linked.rom

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

.db "01>" ; @BT TEST-01 01 START

  nop                         ; @BT $00 $09
  clrt                        ; @BT $00 $08
  sett                        ; @BT $00 $18
  mov r1,r2                   ; @BT $62 $13
  mov #1,r15                  ; @BT $EF $01
  mov r15,r0                  ; @BT $60 $F3
  mov.l r0,@-r15              ; @BT $2F $06
  mov #$7f,r3                 ; @BT $E3 $7F
  add #$ff,r3                 ; @BT $73 $FF
  add r1,r2                   ; @BT $32 $1C
  mov.b @r1,r2                ; @BT $62 $10
  mov.w r2,@r3                ; @BT $23 $21
  mov.l @r4+,r5               ; @BT $65 $46
  mov.l r6,@-r7               ; @BT $27 $66
  mov.b @(3,r4),r0            ; @BT $84 $43
  mov.w r0,@(6,r5)            ; @BT $81 $53
  mov.l @(8,r8),r9            ; @BT $59 $82
  mov.l r10,@(12,r11)         ; @BT $1B $A3
  mov.b @(r0,r1),r2           ; @BT $02 $1C
  mov.w r3,@(r0,r4)           ; @BT $04 $35
  mov.b @(4,gbr),r0           ; @BT $C4 $04
  mov.l r0,@(8,gbr)           ; @BT $C2 $02
  mova @(8,pc),r0             ; @BT $C7 $02
  mov.w @(6,pc),r1            ; @BT $91 $03
  mov.l @(8,pc),r2            ; @BT $D2 $02
  movt r0                     ; @BT $00 $29
  swap.b r1,r2                ; @BT $62 $18
  xtrct r3,r4                 ; @BT $24 $3D
  cmp/eq #1,r0                ; @BT $88 $01
  cmp/eq r1,r2                ; @BT $32 $10
  cmp/pl r3                   ; @BT $43 $15
  dt r15                      ; @BT $4F $10
  div0u                       ; @BT $00 $19
  dt r4                       ; @BT $44 $10
  exts.b r5,r6                ; @BT $66 $5E
  mac.w @r1+,@r2+             ; @BT $42 $1F
  mul.l r3,r4                 ; @BT $04 $37
  muls.w r5,r6                ; @BT $26 $5F
  and #$12,r0                 ; @BT $C9 $12
  and r1,r2                   ; @BT $22 $19
  and.b #$34,@(r0,gbr)        ; @BT $CD $34
  tas.b @r1                   ; @BT $41 $1B
  rotcl r2                    ; @BT $42 $24
  shll16 r3                   ; @BT $43 $28

  bt SH2_BT_TARGET            ; @BT $89 $01
  nop                         ; @BT $00 $09
  nop                         ; @BT $00 $09
SH2_BT_TARGET:
  bf SH2_BF_TARGET            ; @BT $8B $01
  nop                         ; @BT $00 $09
  nop                         ; @BT $00 $09
SH2_BF_TARGET:
  bra SH2_BRA_TARGET          ; @BT $A0 $01
  nop                         ; @BT $00 $09
  nop                         ; @BT $00 $09
SH2_BRA_TARGET:
  bsr SH2_BSR_TARGET          ; @BT $B0 $01
  nop                         ; @BT $00 $09
  nop                         ; @BT $00 $09
SH2_BSR_TARGET:
  bf 127                      ; @BT $8B $7F
  bt -128                     ; @BT $89 $80
  bra 2047                    ; @BT $A7 $FF
  nop                         ; @BT $00 $09
  bsr -2048                   ; @BT $B8 $00
  nop                         ; @BT $00 $09
  braf @r1                    ; @BT $01 $23
  nop                         ; @BT $00 $09
  bsrf @r2                    ; @BT $02 $03
  nop                         ; @BT $00 $09
  jmp @r3                     ; @BT $43 $2B
  nop                         ; @BT $00 $09
  jsr @r4                     ; @BT $44 $0B
  nop                         ; @BT $00 $09
  rts                         ; @BT $00 $0B
  nop                         ; @BT $00 $09

  ldc r1,sr                   ; @BT $41 $0E
  ldc r2,gbr                  ; @BT $42 $1E
  ldc r3,vbr                  ; @BT $43 $2E
  ldc.l @r4+,sr               ; @BT $44 $07
  lds r5,mach                 ; @BT $45 $0A
  lds.l @r6+,pr               ; @BT $46 $26
  stc sr,r7                   ; @BT $07 $02
  stc.l gbr,@-r8              ; @BT $48 $13
  sts mach,r9                 ; @BT $09 $0A
  sts.l pr,@-r10              ; @BT $4A $22
  trapa #$20                  ; @BT $C3 $20
  rte                         ; @BT $00 $2B
  nop                         ; @BT $00 $09
  sleep                       ; @BT $00 $1B
  clrmac                      ; @BT $00 $28

.db "<01" ; @BT END

.db "02>" ; @BT TEST-02 02 START

SH2_T2_START:
  bt SH2_T2_BT_TARGET         ; @BT $89 $02
  nop                         ; @BT $00 $09
  mov #$22,r0                 ; @BT $E0 $22
  add #$01,r0                 ; @BT $70 $01
SH2_T2_BT_TARGET:
  bf/s SH2_T2_BFS_TARGET      ; @BT $8F $02
  nop                         ; @BT $00 $09
  mov #$33,r1                 ; @BT $E1 $33
  add #$02,r1                 ; @BT $71 $02
SH2_T2_BFS_TARGET:
  bra SH2_T2_BRA_TARGET       ; @BT $A0 $03
  nop                         ; @BT $00 $09
  mov #$44,r2                 ; @BT $E2 $44
  add #$03,r2                 ; @BT $72 $03
  mov r2,r3                   ; @BT $63 $23
SH2_T2_BRA_TARGET:
  bsr SH2_T2_BSR_TARGET       ; @BT $B0 $03
  nop                         ; @BT $00 $09
  mov #$55,r4                 ; @BT $E4 $55
  add #$04,r4                 ; @BT $74 $04
  mov r4,r5                   ; @BT $65 $43
SH2_T2_BSR_TARGET:
SH2_T2_BACK_TARGET:
  clrt                        ; @BT $00 $08
  bf SH2_T2_BACK_TARGET       ; @BT $8B $FD
  bt/s SH2_T2_BTS_TARGET      ; @BT $8D $01
  nop                         ; @BT $00 $09
  nop                         ; @BT $00 $09
SH2_T2_BTS_TARGET:
  mov.w @(SH2_T2_WORD_POOL,pc),r1 ; @BT $91 $02
  mov.l @(SH2_T2_LONG_POOL,pc),r2 ; @BT $D2 $02
  mova @(SH2_T2_LONG_POOL,pc),r0  ; @BT $C7 $01
  nop                         ; @BT $00 $09
SH2_T2_WORD_POOL:
  .dw $1357                   ; @BT $13 $57
  .align 4                    ; @BT $00
SH2_T2_LONG_POOL:
  .dd $2468ACE0               ; @BT $24 $68 $AC $E0

.db "<02" ; @BT END

; TEST 03 define/function expression operands.
.define T3_ZERO (9/3-3)
.define T3_ONE (T3_ZERO+1)
.define T3_TWO (T3_ONE+1)
.define T3_THREE (T3_TWO+T3_ONE)
.define T3_FOUR (T3_TWO*T3_TWO)
.define T3_EIGHT (T3_FOUR*T3_TWO)
.define T3_IMM12 ($10+2)
.define T3_IMM34 (T3_IMM12+T3_IMM12+T3_IMM12-2)
.define T3_IMM7F ($80-T3_ONE)
.define T3_NEG1 (T3_ZERO-T3_ONE)
.define T3_TRAP ($10*T3_TWO)
.define T3_GBR_BYTE (T3_THREE+T3_ONE)
.define T3_GBR_WORD (T3_GBR_BYTE*T3_TWO)
.define T3_GBR_LONG (T3_GBR_WORD)
.define T3_DISP_BYTE (T3_THREE)
.define T3_DISP_WORD (T3_THREE*T3_TWO)
.define T3_DISP_LONG (T3_TWO*T3_FOUR)

.function F3_ZERO() T3_ONE-T3_ONE
.function F3_PAIR(a) a+a
.function F3_IMM34() F3_PAIR(T3_IMM12)+T3_IMM12-T3_TWO
.function F3_IMM7F() T3_IMM7F+F3_ZERO()
.function F3_NEG1() F3_ZERO()-T3_ONE
.function F3_TRAP() T3_TRAP
.function F3_GBR_WORD() F3_PAIR(T3_GBR_BYTE)
.function F3_GBR_LONG() F3_GBR_WORD()
.function F3_DISP_LONG() F3_PAIR(T3_FOUR)

.db "03>" ; @BT TEST-03 03 START

  mov #T3_ONE,r15             ; @BT $EF $01
  mov #F3_IMM7F(),r3          ; @BT $E3 $7F
  add #F3_NEG1(),r3           ; @BT $73 $FF
  cmp/eq #T3_ONE,r0           ; @BT $88 $01
  and #T3_IMM12,r0            ; @BT $C9 $12
  and.b #F3_IMM34(),@(r0,gbr) ; @BT $CD $34
  or #F3_IMM34(),r0           ; @BT $CB $34
  xor #T3_IMM12,r0            ; @BT $CA $12
  trapa #F3_TRAP()            ; @BT $C3 $20
  mov.b @(T3_GBR_BYTE,gbr),r0 ; @BT $C4 $04
  mov.w @(F3_GBR_WORD(),gbr),r0 ; @BT $C5 $04
  mov.l @(F3_GBR_LONG(),gbr),r0 ; @BT $C6 $02
  mov.b r0,@(T3_GBR_BYTE,gbr) ; @BT $C0 $04
  mov.w r0,@(F3_GBR_WORD(),gbr) ; @BT $C1 $04
  mov.l r0,@(F3_GBR_LONG(),gbr) ; @BT $C2 $02
  mov.b @(T3_DISP_BYTE,r4),r0 ; @BT $84 $43
  mov.w r0,@(T3_DISP_WORD,r5) ; @BT $81 $53
  mov.l @(F3_DISP_LONG(),r8),r9 ; @BT $59 $82
  mov.l r10,@(F3_DISP_LONG()+T3_FOUR,r11) ; @BT $1B $A3
  mov.w @(SH2_T3_WORD_POOL,pc),r6 ; @BT $96 $02
  mov.l @(SH2_T3_LONG_POOL,pc),r7 ; @BT $D7 $02
  mova @(SH2_T3_LONG_POOL,pc),r0  ; @BT $C7 $01
  nop                         ; @BT $00 $09
SH2_T3_WORD_POOL:
  .dw $55AA                   ; @BT $55 $AA
  .align 4                    ; @BT $00 $00
SH2_T3_LONG_POOL:
  .dd $11223344               ; @BT $11 $22 $33 $44

.db "<03" ; @BT END
