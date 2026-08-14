; @BT linked.rom

.MEMORYMAP
  DEFAULTSLOT 0
  SLOT 0 $0000 $10000
.ENDME

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $10000
  BANKS 1
.ENDRO

.BANK 0 SLOT 0
.ORG 0

.DEFINE CART $5000

.db "01>" ; @BT TEST-01 01 START
.db $00                   ; @BT $00

  hlt                         ; @BT $00 $00
  sdbd                        ; @BT $00 $01
  eis                         ; @BT $00 $02
  dis                         ; @BT $00 $03
  tci                         ; @BT $00 $05
  clrc                        ; @BT $00 $06
  setc                        ; @BT $00 $07
  incr r0                     ; @BT $00 $08
  decr r1                     ; @BT $00 $11
  comr r2                     ; @BT $00 $1A
  negr r3                     ; @BT $00 $23
  adcr r4                     ; @BT $00 $2C
  gswd r0                     ; @BT $00 $30
  nop                         ; @BT $00 $34
  sin                         ; @BT $00 $36
  rswd r5                     ; @BT $00 $3D
  swap r0                     ; @BT $00 $40
  swap r0, 2                  ; @BT $00 $44
  sll r1                      ; @BT $00 $49
  sll r1, 2                   ; @BT $00 $4D
  movr r1, r2                 ; @BT $00 $8A
  addr r0, r7                 ; @BT $00 $C7
  jr r5                       ; @BT $00 $AF
  tstr r3                     ; @BT $00 $9B
  clrr r1                     ; @BT $01 $C9
  pshr r0                     ; @BT $02 $70
  pulr r2                     ; @BT $02 $B2
  incr sp                     ; @BT $00 $0E
  mvi $1234, r0               ; @BT $02 $80 $12 $34
  mvo r1, $1000               ; @BT $02 $41 $10 $00
  mvi@ r1, r2                 ; @BT $02 $8A
  mvo@ r3, r4                 ; @BT $02 $63
  mvii #$00AB, r0             ; @BT $02 $B8 $00 $AB
  mvi CART, r0                ; @BT $02 $80 $50 $00
  j $5000                     ; @BT $00 $04 $03 $50 $00 $00
  jsr r5, $5000               ; @BT $00 $04 $01 $50 $00 $00
  call $5000                  ; @BT $00 $04 $01 $50 $00 $00
  nopp                        ; @BT $02 $08 $00 $00

.db "<01" ; @BT END

.db "02>" ; @BT TEST-02 02 START

  beq CP1610_BEQ_TARGET       ; @BT $02 $04 $00 $01
  nop                         ; @BT $00 $34
CP1610_BEQ_TARGET:
  nop                         ; @BT $00 $34
  b CP1610_BEQ_TARGET         ; @BT $02 $20 $00 $03
  sdbd                        ; @BT $00 $01
  mvii #$1234, r1             ; @BT $02 $B9 $00 $34 $00 $12
  mvi DEST, r0                ; @BT $02 $80 $00 $43
  nop                         ; @BT $00 $34
DEST:

.db "<02" ; @BT END

.db "03>" ; @BT TEST-03 03 START

  sin 2                       ; @BT $00 $37
  bext 3, CP1610_BEXT_T       ; @BT $02 $13 $00 $01
  nop                         ; @BT $00 $34
CP1610_BEXT_T:
  nop                         ; @BT $00 $34
  jsr r4, DEST2               ; @BT $00 $04 $00 $00 $00 $52
  je $5000                    ; @BT $00 $04 $03 $51 $00 $00
  nop                         ; @BT $00 $34
DEST2:

.db "<03" ; @BT END

; TEST-04: every mnemonic again, operand values as number-only calculations.
.db "04>" ; @BT TEST-04 04 START

  hlt                         ; @BT $00 $00
  sdbd                        ; @BT $00 $01
  eis                         ; @BT $00 $02
  dis                         ; @BT $00 $03
  tci                         ; @BT $00 $05
  clrc                        ; @BT $00 $06
  setc                        ; @BT $00 $07
  incr r0                     ; @BT $00 $08
  decr r1                     ; @BT $00 $11
  comr r2                     ; @BT $00 $1A
  negr r3                     ; @BT $00 $23
  adcr r4                     ; @BT $00 $2C
  gswd r0                     ; @BT $00 $30
  nop                         ; @BT $00 $34
  sin                         ; @BT $00 $36
  sin (1+1)                   ; @BT $00 $37
  rswd r5                     ; @BT $00 $3D
  swap r0                     ; @BT $00 $40
  swap r0, (1+1)              ; @BT $00 $44
  sll r1                      ; @BT $00 $49
  sll r1, (2-0)               ; @BT $00 $4D
  rlc r2, (1*2)               ; @BT $00 $56
  sllc r3, (3-1)              ; @BT $00 $5F
  slr r0, (4/2)               ; @BT $00 $64
  sar r1, (8/4)               ; @BT $00 $6D
  rrc r2, (1+1)               ; @BT $00 $76
  sarc r3, (2*1)              ; @BT $00 $7F
  movr r1, r2                 ; @BT $00 $8A
  addr r0, r7                 ; @BT $00 $C7
  subr r2, r3                 ; @BT $01 $13
  cmpr r4, r5                 ; @BT $01 $65
  andr r6, r0                 ; @BT $01 $B0
  xorr r3, r3                 ; @BT $01 $DB
  jr r5                       ; @BT $00 $AF
  tstr r3                     ; @BT $00 $9B
  clrr r1                     ; @BT $01 $C9
  pshr r0                     ; @BT $02 $70
  pulr r2                     ; @BT $02 $B2
  incr sp                     ; @BT $00 $0E
  mvi ($1200+$34), r0         ; @BT $02 $80 $12 $34
  mvo r1, ($800+$800)         ; @BT $02 $41 $10 $00
  add ($1000+$234), r2        ; @BT $02 $C2 $12 $34
  sub ($2000/2), r3           ; @BT $03 $03 $10 $00
  cmp ($1230+$4), r4          ; @BT $03 $44 $12 $34
  and ($0FFF+1), r5           ; @BT $03 $85 $10 $00
  xor ($1235-1), r6           ; @BT $03 $C6 $12 $34
  mvi@ r1, r2                 ; @BT $02 $8A
  add@ r2, r3                 ; @BT $02 $D3
  sub@ r3, r4                 ; @BT $03 $1C
  cmp@ r4, r5                 ; @BT $03 $65
  and@ r5, r6                 ; @BT $03 $AE
  xor@ r1, r0                 ; @BT $03 $C8
  mvo@ r3, r4                 ; @BT $02 $63
  mvii #($A0+$0B), r0         ; @BT $02 $B8 $00 $AB
  mvoi r1, #($AB+$0)          ; @BT $02 $79 $00 $AB
  addi #($100-$55), r2        ; @BT $02 $FA $00 $AB
  subi #($50+$5B), r3         ; @BT $03 $3B $00 $AB
  cmpi #(171), r4             ; @BT $03 $7C $00 $AB
  andi #($FF-$54), r5         ; @BT $03 $BD $00 $AB
  xori #($A00+$AB-$A00), r6   ; @BT $03 $FE $00 $AB
  mvi ($4000+$1000), r0       ; @BT $02 $80 $50 $00
  j ($4800+$800)              ; @BT $00 $04 $03 $50 $00 $00
  je ($5000+0)                ; @BT $00 $04 $03 $51 $00 $00
  jd ($6000-$1000)            ; @BT $00 $04 $03 $52 $00 $00
  jsr r5, ($4F00+$100)        ; @BT $00 $04 $01 $50 $00 $00
  jsre r4, ($2800*2)          ; @BT $00 $04 $00 $51 $00 $00
  jsrd r6, ($A000/2)          ; @BT $00 $04 $02 $52 $00 $00
  call ($4000+$1000)          ; @BT $00 $04 $01 $50 $00 $00
  nopp                        ; @BT $02 $08 $00 $00
  sdbd                        ; @BT $00 $01
  mvii #($1200+$34), r1       ; @BT $02 $B9 $00 $34 $00 $12
  b T4_B+0                    ; @BT $02 $00 $00 $00
T4_B:
  bc T4_BC+0                  ; @BT $02 $01 $00 $00
T4_BC:
  beq T4_BEQ+0                ; @BT $02 $04 $00 $00
T4_BEQ:
  besc T4_BESC+0              ; @BT $02 $0F $00 $00
T4_BESC:
  bge T4_BGE+0                ; @BT $02 $0D $00 $00
T4_BGE:
  bgt T4_BGT+0                ; @BT $02 $0E $00 $00
T4_BGT:
  ble T4_BLE+0                ; @BT $02 $06 $00 $00
T4_BLE:
  blge T4_BLGE+0              ; @BT $02 $01 $00 $00
T4_BLGE:
  bllt T4_BLLT+0              ; @BT $02 $09 $00 $00
T4_BLLT:
  blt T4_BLT+0                ; @BT $02 $05 $00 $00
T4_BLT:
  bmi T4_BMI+0                ; @BT $02 $0B $00 $00
T4_BMI:
  bnc T4_BNC+0                ; @BT $02 $09 $00 $00
T4_BNC:
  bne T4_BNE+0                ; @BT $02 $0C $00 $00
T4_BNE:
  bneq T4_BNEQ+0              ; @BT $02 $0C $00 $00
T4_BNEQ:
  bnov T4_BNOV+0              ; @BT $02 $0A $00 $00
T4_BNOV:
  bnz T4_BNZ+0                ; @BT $02 $0C $00 $00
T4_BNZ:
  bov T4_BOV+0                ; @BT $02 $02 $00 $00
T4_BOV:
  bpl T4_BPL+0                ; @BT $02 $03 $00 $00
T4_BPL:
  busc T4_BUSC+0              ; @BT $02 $07 $00 $00
T4_BUSC:
  bz T4_BZ+0                  ; @BT $02 $04 $00 $00
T4_BZ:
  bze T4_BZE+0                ; @BT $02 $04 $00 $00
T4_BZE:
  bext (1+2), T4_BEXT+0       ; @BT $02 $13 $00 $00
T4_BEXT:
  nopp T4_NOPP+0              ; @BT $02 $08 $00 $00
T4_NOPP:
T4_BACK:
  nop                         ; @BT $00 $34
  b T4_BACK+0                 ; @BT $02 $20 $00 $03

.db "<04" ; @BT END

; TEST-05: same opcodes, values from numbers mixed with .DEFINEs.
.DEFINE T5_ZERO 0
.DEFINE T5_ONE 1
.DEFINE T5_TWO (T5_ONE+T5_ONE)
.DEFINE T5_THREE (T5_TWO+T5_ONE)
.DEFINE T5_HI $12
.DEFINE T5_LO $34
.DEFINE T5_ADDR_A (T5_HI*$100+T5_LO)
.DEFINE T5_ADDR_B ($800+$800)
.DEFINE T5_IMM ($A0+$0B)
.DEFINE T5_CART ($4000+$1000)

.db "05>" ; @BT TEST-05 05 START

  hlt                         ; @BT $00 $00
  sdbd                        ; @BT $00 $01
  eis                         ; @BT $00 $02
  dis                         ; @BT $00 $03
  tci                         ; @BT $00 $05
  clrc                        ; @BT $00 $06
  setc                        ; @BT $00 $07
  incr r0                     ; @BT $00 $08
  decr r1                     ; @BT $00 $11
  comr r2                     ; @BT $00 $1A
  negr r3                     ; @BT $00 $23
  adcr r4                     ; @BT $00 $2C
  gswd r0                     ; @BT $00 $30
  nop                         ; @BT $00 $34
  sin                         ; @BT $00 $36
  sin T5_ONE+T5_ONE           ; @BT $00 $37
  rswd r5                     ; @BT $00 $3D
  swap r0                     ; @BT $00 $40
  swap r0, T5_TWO             ; @BT $00 $44
  sll r1                      ; @BT $00 $49
  sll r1, T5_TWO+T5_ZERO      ; @BT $00 $4D
  rlc r2, T5_ONE*T5_TWO       ; @BT $00 $56
  sllc r3, T5_THREE-T5_ONE    ; @BT $00 $5F
  slr r0, T5_TWO              ; @BT $00 $64
  sar r1, T5_TWO              ; @BT $00 $6D
  rrc r2, T5_TWO              ; @BT $00 $76
  sarc r3, T5_TWO             ; @BT $00 $7F
  movr r1, r2                 ; @BT $00 $8A
  addr r0, r7                 ; @BT $00 $C7
  subr r2, r3                 ; @BT $01 $13
  cmpr r4, r5                 ; @BT $01 $65
  andr r6, r0                 ; @BT $01 $B0
  xorr r3, r3                 ; @BT $01 $DB
  jr r5                       ; @BT $00 $AF
  tstr r3                     ; @BT $00 $9B
  clrr r1                     ; @BT $01 $C9
  pshr r0                     ; @BT $02 $70
  pulr r2                     ; @BT $02 $B2
  incr sp                     ; @BT $00 $0E
  mvi T5_ADDR_A+T5_ZERO, r0   ; @BT $02 $80 $12 $34
  mvo r1, T5_ADDR_B-T5_ZERO   ; @BT $02 $41 $10 $00
  add T5_HI*$100+T5_LO, r2    ; @BT $02 $C2 $12 $34
  sub T5_ADDR_B+T5_ZERO, r3   ; @BT $03 $03 $10 $00
  cmp T5_ADDR_A, r4           ; @BT $03 $44 $12 $34
  and T5_ADDR_B, r5           ; @BT $03 $85 $10 $00
  xor T5_ADDR_A, r6           ; @BT $03 $C6 $12 $34
  mvi@ r1, r2                 ; @BT $02 $8A
  add@ r2, r3                 ; @BT $02 $D3
  sub@ r3, r4                 ; @BT $03 $1C
  cmp@ r4, r5                 ; @BT $03 $65
  and@ r5, r6                 ; @BT $03 $AE
  xor@ r1, r0                 ; @BT $03 $C8
  mvo@ r3, r4                 ; @BT $02 $63
  mvii #T5_IMM+T5_ZERO, r0    ; @BT $02 $B8 $00 $AB
  mvoi r1, #T5_IMM            ; @BT $02 $79 $00 $AB
  addi #T5_IMM, r2            ; @BT $02 $FA $00 $AB
  subi #T5_IMM, r3            ; @BT $03 $3B $00 $AB
  cmpi #T5_IMM, r4            ; @BT $03 $7C $00 $AB
  andi #T5_IMM, r5            ; @BT $03 $BD $00 $AB
  xori #T5_IMM, r6            ; @BT $03 $FE $00 $AB
  mvi T5_CART+T5_ZERO, r0     ; @BT $02 $80 $50 $00
  j T5_CART+T5_ZERO           ; @BT $00 $04 $03 $50 $00 $00
  je T5_CART                  ; @BT $00 $04 $03 $51 $00 $00
  jd T5_CART-T5_ZERO          ; @BT $00 $04 $03 $52 $00 $00
  jsr r5, T5_CART             ; @BT $00 $04 $01 $50 $00 $00
  jsre r4, T5_CART            ; @BT $00 $04 $00 $51 $00 $00
  jsrd r6, T5_CART            ; @BT $00 $04 $02 $52 $00 $00
  call T5_CART                ; @BT $00 $04 $01 $50 $00 $00
  nopp                        ; @BT $02 $08 $00 $00
  sdbd                        ; @BT $00 $01
  mvii #T5_ADDR_A, r1         ; @BT $02 $B9 $00 $34 $00 $12
  b T5_B+T5_ZERO              ; @BT $02 $00 $00 $00
T5_B:
  bc T5_BC+T5_ZERO            ; @BT $02 $01 $00 $00
T5_BC:
  beq T5_BEQ+T5_ZERO          ; @BT $02 $04 $00 $00
T5_BEQ:
  besc T5_BESC+T5_ZERO        ; @BT $02 $0F $00 $00
T5_BESC:
  bge T5_BGE+T5_ZERO          ; @BT $02 $0D $00 $00
T5_BGE:
  bgt T5_BGT+T5_ZERO          ; @BT $02 $0E $00 $00
T5_BGT:
  ble T5_BLE+T5_ZERO          ; @BT $02 $06 $00 $00
T5_BLE:
  blge T5_BLGE+T5_ZERO        ; @BT $02 $01 $00 $00
T5_BLGE:
  bllt T5_BLLT+T5_ZERO        ; @BT $02 $09 $00 $00
T5_BLLT:
  blt T5_BLT+T5_ZERO          ; @BT $02 $05 $00 $00
T5_BLT:
  bmi T5_BMI+T5_ZERO          ; @BT $02 $0B $00 $00
T5_BMI:
  bnc T5_BNC+T5_ZERO          ; @BT $02 $09 $00 $00
T5_BNC:
  bne T5_BNE+T5_ZERO          ; @BT $02 $0C $00 $00
T5_BNE:
  bneq T5_BNEQ+T5_ZERO        ; @BT $02 $0C $00 $00
T5_BNEQ:
  bnov T5_BNOV+T5_ZERO        ; @BT $02 $0A $00 $00
T5_BNOV:
  bnz T5_BNZ+T5_ZERO          ; @BT $02 $0C $00 $00
T5_BNZ:
  bov T5_BOV+T5_ZERO          ; @BT $02 $02 $00 $00
T5_BOV:
  bpl T5_BPL+T5_ZERO          ; @BT $02 $03 $00 $00
T5_BPL:
  busc T5_BUSC+T5_ZERO        ; @BT $02 $07 $00 $00
T5_BUSC:
  bz T5_BZ+T5_ZERO            ; @BT $02 $04 $00 $00
T5_BZ:
  bze T5_BZE+T5_ZERO          ; @BT $02 $04 $00 $00
T5_BZE:
  bext T5_THREE, T5_BEXT+T5_ZERO ; @BT $02 $13 $00 $00
T5_BEXT:
  nopp T5_NOPP+T5_ZERO        ; @BT $02 $08 $00 $00
T5_NOPP:
T5_BACK:
  nop                         ; @BT $00 $34
  b T5_BACK+T5_ZERO           ; @BT $02 $20 $00 $03

.db "<05" ; @BT END

; TEST-06: same opcodes, some operand values from .FUNCTIONs.
.DEFINE T6_HI $12
.DEFINE T6_LO $34
.FUNCTION F6_ADD(a, b) (a + b)
.FUNCTION F6_SUB(a, b) (a - b)
.FUNCTION F6_WORD(hi, lo) (hi * 256 + lo)
.FUNCTION F6_ZERO() (1-1)
.FUNCTION F6_ONE() (2/2)
.FUNCTION F6_TWO() F6_ADD(F6_ONE(), F6_ONE())
.FUNCTION F6_THREE() F6_ADD(F6_TWO(), F6_ONE())
.FUNCTION F6_ADDR_A() F6_WORD(T6_HI, T6_LO)
.FUNCTION F6_ADDR_B() F6_ADD($800, $800)
.FUNCTION F6_IMM() F6_ADD($A0, $0B)
.FUNCTION F6_CART() F6_ADD($4000, $1000)

.db "06>" ; @BT TEST-06 06 START

  hlt                         ; @BT $00 $00
  sdbd                        ; @BT $00 $01
  eis                         ; @BT $00 $02
  dis                         ; @BT $00 $03
  tci                         ; @BT $00 $05
  clrc                        ; @BT $00 $06
  setc                        ; @BT $00 $07
  incr r0                     ; @BT $00 $08
  decr r1                     ; @BT $00 $11
  comr r2                     ; @BT $00 $1A
  negr r3                     ; @BT $00 $23
  adcr r4                     ; @BT $00 $2C
  gswd r0                     ; @BT $00 $30
  nop                         ; @BT $00 $34
  sin                         ; @BT $00 $36
  sin F6_TWO()                ; @BT $00 $37
  rswd r5                     ; @BT $00 $3D
  swap r0                     ; @BT $00 $40
  swap r0, F6_TWO()           ; @BT $00 $44
  sll r1                      ; @BT $00 $49
  sll r1, F6_ADD(F6_ONE(), F6_ONE()) ; @BT $00 $4D
  rlc r2, F6_TWO()            ; @BT $00 $56
  sllc r3, F6_SUB(F6_THREE(), F6_ONE()) ; @BT $00 $5F
  slr r0, F6_TWO()            ; @BT $00 $64
  sar r1, F6_TWO()            ; @BT $00 $6D
  rrc r2, F6_TWO()            ; @BT $00 $76
  sarc r3, F6_TWO()           ; @BT $00 $7F
  movr r1, r2                 ; @BT $00 $8A
  addr r0, r7                 ; @BT $00 $C7
  subr r2, r3                 ; @BT $01 $13
  cmpr r4, r5                 ; @BT $01 $65
  andr r6, r0                 ; @BT $01 $B0
  xorr r3, r3                 ; @BT $01 $DB
  jr r5                       ; @BT $00 $AF
  tstr r3                     ; @BT $00 $9B
  clrr r1                     ; @BT $01 $C9
  pshr r0                     ; @BT $02 $70
  pulr r2                     ; @BT $02 $B2
  incr sp                     ; @BT $00 $0E
  mvi F6_ADDR_A(), r0         ; @BT $02 $80 $12 $34
  mvo r1, F6_ADDR_B()         ; @BT $02 $41 $10 $00
  add F6_WORD(T6_HI, T6_LO), r2 ; @BT $02 $C2 $12 $34
  sub F6_ADDR_B(), r3         ; @BT $03 $03 $10 $00
  cmp F6_ADDR_A(), r4         ; @BT $03 $44 $12 $34
  and F6_ADDR_B(), r5         ; @BT $03 $85 $10 $00
  xor F6_ADDR_A(), r6         ; @BT $03 $C6 $12 $34
  mvi@ r1, r2                 ; @BT $02 $8A
  add@ r2, r3                 ; @BT $02 $D3
  sub@ r3, r4                 ; @BT $03 $1C
  cmp@ r4, r5                 ; @BT $03 $65
  and@ r5, r6                 ; @BT $03 $AE
  xor@ r1, r0                 ; @BT $03 $C8
  mvo@ r3, r4                 ; @BT $02 $63
  mvii #F6_IMM(), r0          ; @BT $02 $B8 $00 $AB
  mvoi r1, #F6_IMM()          ; @BT $02 $79 $00 $AB
  addi #F6_IMM(), r2          ; @BT $02 $FA $00 $AB
  subi #F6_IMM(), r3          ; @BT $03 $3B $00 $AB
  cmpi #F6_IMM(), r4          ; @BT $03 $7C $00 $AB
  andi #F6_IMM(), r5          ; @BT $03 $BD $00 $AB
  xori #F6_IMM(), r6          ; @BT $03 $FE $00 $AB
  mvi F6_CART(), r0           ; @BT $02 $80 $50 $00
  j F6_CART()                 ; @BT $00 $04 $03 $50 $00 $00
  je F6_ADD($4000, $1000)     ; @BT $00 $04 $03 $51 $00 $00
  jd F6_SUB($6000, $1000)     ; @BT $00 $04 $03 $52 $00 $00
  jsr r5, F6_CART()           ; @BT $00 $04 $01 $50 $00 $00
  jsre r4, F6_CART()          ; @BT $00 $04 $00 $51 $00 $00
  jsrd r6, F6_CART()          ; @BT $00 $04 $02 $52 $00 $00
  call F6_CART()              ; @BT $00 $04 $01 $50 $00 $00
  nopp                        ; @BT $02 $08 $00 $00
  sdbd                        ; @BT $00 $01
  mvii #F6_ADDR_A(), r1       ; @BT $02 $B9 $00 $34 $00 $12
  b T6_B+F6_ZERO()            ; @BT $02 $00 $00 $00
T6_B:
  bc T6_BC+F6_ZERO()          ; @BT $02 $01 $00 $00
T6_BC:
  beq T6_BEQ+F6_ZERO()        ; @BT $02 $04 $00 $00
T6_BEQ:
  besc T6_BESC+F6_ZERO()      ; @BT $02 $0F $00 $00
T6_BESC:
  bge T6_BGE+F6_ZERO()        ; @BT $02 $0D $00 $00
T6_BGE:
  bgt T6_BGT+F6_ZERO()        ; @BT $02 $0E $00 $00
T6_BGT:
  ble T6_BLE+F6_ZERO()        ; @BT $02 $06 $00 $00
T6_BLE:
  blge T6_BLGE+F6_ZERO()      ; @BT $02 $01 $00 $00
T6_BLGE:
  bllt T6_BLLT+F6_ZERO()      ; @BT $02 $09 $00 $00
T6_BLLT:
  blt T6_BLT+F6_ZERO()        ; @BT $02 $05 $00 $00
T6_BLT:
  bmi T6_BMI+F6_ZERO()        ; @BT $02 $0B $00 $00
T6_BMI:
  bnc T6_BNC+F6_ZERO()        ; @BT $02 $09 $00 $00
T6_BNC:
  bne T6_BNE+F6_ZERO()        ; @BT $02 $0C $00 $00
T6_BNE:
  bneq T6_BNEQ+F6_ZERO()      ; @BT $02 $0C $00 $00
T6_BNEQ:
  bnov T6_BNOV+F6_ZERO()      ; @BT $02 $0A $00 $00
T6_BNOV:
  bnz T6_BNZ+F6_ZERO()        ; @BT $02 $0C $00 $00
T6_BNZ:
  bov T6_BOV+F6_ZERO()        ; @BT $02 $02 $00 $00
T6_BOV:
  bpl T6_BPL+F6_ZERO()        ; @BT $02 $03 $00 $00
T6_BPL:
  busc T6_BUSC+F6_ZERO()      ; @BT $02 $07 $00 $00
T6_BUSC:
  bz T6_BZ+F6_ZERO()          ; @BT $02 $04 $00 $00
T6_BZ:
  bze T6_BZE+F6_ZERO()        ; @BT $02 $04 $00 $00
T6_BZE:
  bext F6_THREE(), T6_BEXT+F6_ZERO() ; @BT $02 $13 $00 $00
T6_BEXT:
  nopp T6_NOPP+F6_ZERO()      ; @BT $02 $08 $00 $00
T6_NOPP:
T6_BACK:
  nop                         ; @BT $00 $34
  b T6_BACK+F6_ZERO()         ; @BT $02 $20 $00 $03

.db "<06" ; @BT END
