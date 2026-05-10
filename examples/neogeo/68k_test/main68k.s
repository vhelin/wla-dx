;
; Neo Geo MC68000 executable instruction test
;
; Runs a broad set of safe MC68000 instruction-family checks on real
; emulated 68k state, then prints the number of passing subtests and
; failures on the fixed layer.
;

.INCDIR "../../../include/neogeo"

.INCLUDE "../../../memorymaps/neogeo68k.i"
.INCLUDE "macros.inc"
.INCLUDE "68k.inc"
.INCLUDE "bios.inc"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.DEFINE RAM_TEST       $100400
.DEFINE RAM_MOVEM      $100420
.DEFINE EXPECTED_COUNT $0018

.NGHEADER
  NGH $0043
  PROMSIZE AUTO
  BACKUPRAMPTR $00100000
  BACKUPRAMSIZE 2
  EYECATCHER GAME
  LOGOBANK 0
  USERENTRY _ng_userentry
  PLAYERSTART _ng_playerstart
  DEMOEND _ng_demoend
  COINSOUND _ng_coinsound
.ENDNG

.BANK 0 SLOT 0
.ORGA $0000

.INCLUDE "crt0.s"

main:
  move.b #1, REG_CRTFIX

  moveq  #0, d6              ; pass count
  moveq  #0, d5              ; fail count

  bsr.w  test_move_address
  bsr.w  test_add_family
  bsr.w  test_addx_family
  bsr.w  test_sub_family
  bsr.w  test_subx_family
  bsr.w  test_cmp_family
  bsr.w  test_logic_family
  bsr.w  test_neg_clr_tst
  bsr.w  test_mul_div
  bsr.w  test_shifts
  bsr.w  test_rotates
  bsr.w  test_bit_immediate
  bsr.w  test_bit_register
  bsr.w  test_decimal
  bsr.w  test_ext_swap
  bsr.w  test_exg
  bsr.w  test_movem
  bsr.w  test_movep
  bsr.w  test_branches_scc
  bsr.w  test_dbra
  bsr.w  test_jsr_jmp
  bsr.w  test_link_unlk
  bsr.w  test_chk_tas
  bsr.w  test_sr_ccr_misc

  bsr.w  draw_results

idle:
  NG_KICK_WATCHDOG
  bra.b  idle

record_fail:
  addq.w #1, d5
  rts

; -------------------------------------------------------------------
; Display helpers
; -------------------------------------------------------------------
draw_results:
  move.w #$70CD, d0          ; col 6, row 13
  lea    text_title, a0
  bsr.w  print_string

  move.w #$714F, d0          ; col 10, row 15
  lea    text_pass, a0
  bsr.w  print_string
  move.w #$71CF, d0          ; col 14, row 15
  move.w d6, d1
  bsr.w  print_hex_word

  move.w #$7151, d0          ; col 10, row 17
  lea    text_fail, a0
  bsr.w  print_string
  move.w #$71D1, d0          ; col 14, row 17
  move.w d5, d1
  bsr.w  print_hex_word

  move.w #$7153, d0          ; col 10, row 19
  lea    text_count, a0
  bsr.w  print_string
  move.w #$7213, d0          ; col 16, row 19
  move.w #EXPECTED_COUNT, d1
  bsr.w  print_hex_word
  rts

print_string:
  move.w d0, REG_VRAMADDR
  NG_VRAM_MOD $20
_print_string_next:
  moveq  #0, d1
  move.b (a0)+, d1
  beq.b  _print_string_done
  ori.w  #$F000, d1
  move.w d1, REG_VRAMRW
  bra.b  _print_string_next
_print_string_done:
  NG_VRAM_MOD 1
  rts

print_hex_word:
  move.w d0, REG_VRAMADDR
  NG_VRAM_MOD $20
  moveq  #3, d2
_print_hex_next:
  rol.w  #4, d1
  move.w d1, d3
  andi.w #$000F, d3
  cmpi.w #10, d3
  blt.b  _print_hex_digit
  addi.w #('A'-10), d3
  bra.b  _print_hex_emit
_print_hex_digit:
  addi.w #'0', d3
_print_hex_emit:
  ori.w  #$F000, d3
  move.w d3, REG_VRAMRW
  dbra   d2, _print_hex_next
  NG_VRAM_MOD 1
  rts

; -------------------------------------------------------------------
; 68k instruction-family tests
; -------------------------------------------------------------------
test_move_address:
  moveq  #$12, d0
  move.w #$3456, d1
  move.l #$12345678, d2
  movea.l #RAM_TEST, a0
  lea    8(a0), a1
  pea    (a1)
  movea.l (sp)+, a2
  move.l d2, (a0)
  move.l (a0), d3
  cmp.l  #$12345678, d3
  bne.w  record_fail
  cmpa.l a1, a2
  bne.w  record_fail
  addq.w #1, d6
  rts

test_add_family:
  moveq  #5, d0
  addq.w #3, d0
  addi.w #$10, d0
  add.w  d0, d0
  adda.w #4, a0
  cmpi.w #$0030, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_addx_family:
  moveq  #$0F, d0
  moveq  #$01, d1
  move.w #$0010, ccr          ; X set
  addx.b d1, d0
  cmpi.b #$11, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_sub_family:
  move.w #$40, d0
  subi.w #$10, d0
  subq.w #8, d0
  moveq  #4, d1
  sub.w  d1, d0
  suba.w #2, a0
  cmpi.w #$0024, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_subx_family:
  moveq  #$20, d0
  moveq  #$0F, d1
  move.w #$0010, ccr          ; X set
  subx.b d1, d0
  cmpi.b #$10, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_cmp_family:
  lea    RAM_TEST, a0
  move.w #$1234, (a0)+
  move.w #$1234, (a0)+
  lea    RAM_TEST, a0
  lea    RAM_TEST+2, a1
  cmpm.w (a0)+, (a1)+
  bne.w  record_fail
  move.w #$1234, d0
  cmpi.w #$1234, d0
  bne.w  record_fail
  movea.l #RAM_TEST, a2
  cmpa.l #RAM_TEST, a2
  bne.w  record_fail
  addq.w #1, d6
  rts

test_logic_family:
  move.w #$0F0F, d0
  andi.w #$00FF, d0
  ori.w  #$3300, d0
  eori.w #$00F0, d0
  not.w  d0
  cmpi.w #$CC00, d0
  bne.w  record_fail
  move.w #$AAAA, d1
  and.w  #$0FF0, d1
  or.w   #$0005, d1
  eor.w  #$00A5, d1
  cmpi.w #$0A00, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_neg_clr_tst:
  moveq  #5, d0
  neg.w  d0
  negx.w d0
  cmpi.w #4, d0
  bne.w  record_fail
  clr.w  d1
  tst.w  d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_mul_div:
  move.w #123, d0
  mulu.w #10, d0
  cmp.l  #1230, d0
  bne.w  record_fail
  move.l #1234, d1
  move.w #100, d2
  divu.w d2, d1
  cmp.l  #$0022000C, d1
  bne.w  record_fail
  move.w #-12, d3
  muls.w #-3, d3
  cmp.l  #36, d3
  bne.w  record_fail
  addq.w #1, d6
  rts

test_shifts:
  moveq  #1, d0
  asl.w  #4, d0
  lsr.w  #1, d0
  asr.w  #1, d0
  lsl.w  #2, d0
  cmpi.w #$0010, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_rotates:
  move.w #$8001, d0
  rol.w  #1, d0
  ror.w  #1, d0
  cmpi.w #$8001, d0
  bne.w  record_fail
  move.w #$0010, ccr
  move.w #$0001, d1
  roxl.w #1, d1
  roxr.w #1, d1
  cmpi.w #$0001, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_bit_immediate:
  lea    RAM_TEST, a0
  clr.b  (a0)
  bset.b #3, (a0)
  btst.b #3, (a0)
  beq.w  record_fail
  bchg.b #3, (a0)
  btst.b #3, (a0)
  bne.w  record_fail
  bset.l #31, d0
  btst.l #31, d0
  beq.w  record_fail
  bclr.l #31, d0
  btst.l #31, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_bit_register:
  lea    RAM_TEST, a0
  clr.b  (a0)
  moveq  #2, d1
  bset.b d1, (a0)
  btst.b d1, (a0)
  beq.w  record_fail
  bclr.b d1, (a0)
  btst.b d1, (a0)
  bne.w  record_fail
  moveq  #5, d2
  moveq  #0, d3
  bset.l d2, d3
  btst.l d2, d3
  beq.w  record_fail
  addq.w #1, d6
  rts

test_decimal:
  move.b #$15, d0
  move.b #$27, d1
  move.w #0, ccr
  abcd  d1, d0
  cmpi.b #$42, d0
  bne.w  record_fail
  move.b #$42, d2
  move.b #$15, d3
  move.w #0, ccr
  sbcd  d3, d2
  cmpi.b #$27, d2
  bne.w  record_fail
  move.b #$00, d4
  move.w #0, ccr
  nbcd  d4
  tst.b  d4
  bne.w  record_fail
  addq.w #1, d6
  rts

test_ext_swap:
  move.b #$80, d0
  ext.w  d0
  ext.l  d0
  cmp.l  #$FFFFFF80, d0
  bne.w  record_fail
  move.l #$12345678, d1
  swap   d1
  cmp.l  #$56781234, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_exg:
  move.l #$11112222, d0
  move.l #$33334444, d1
  exg    d0, d1
  cmp.l  #$33334444, d0
  bne.w  record_fail
  cmp.l  #$11112222, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_movem:
  lea    RAM_MOVEM, a0
  move.l #$11111111, d0
  move.l #$22222222, d1
  move.l #$33333333, d2
  movem.l d0-d2, (a0)
  cmp.l  #$11111111, (a0)+
  bne.w  record_fail
  cmp.l  #$22222222, (a0)+
  bne.w  record_fail
  cmp.l  #$33333333, (a0)+
  bne.w  record_fail
  addq.w #1, d6
  rts

test_movep:
  lea    RAM_TEST, a0
  clr.l  (a0)
  clr.l  4(a0)
  move.w #$1234, d0
  movep.w d0, 0(a0)
  moveq  #0, d1
  movep.w 0(a0), d1
  cmpi.w #$1234, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_branches_scc:
  moveq  #0, d0
  cmpi.w #0, d0
  beq.b  _branch_equal
  bra.w  record_fail
_branch_equal:
  seq    d1
  sne    d2
  cmpi.b #$FF, d1
  bne.w  record_fail
  tst.b  d2
  bne.w  record_fail
  addq.w #1, d6
  rts

test_dbra:
  moveq  #3, d0
  moveq  #0, d1
_dbra_loop:
  addq.w #1, d1
  dbra   d0, _dbra_loop
  cmpi.w #4, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

test_jsr_jmp:
  moveq  #0, d0
  bsr.b  _bsr_add_one
  jsr    _jsr_add_two
  lea    _jmp_done, a0
  jmp    (a0)
  bra.w  record_fail
_bsr_add_one:
  addq.w #1, d0
  rts
_jsr_add_two:
  addq.w #2, d0
  rts
_jmp_done:
  cmpi.w #3, d0
  bne.w  record_fail
  addq.w #1, d6
  rts

test_link_unlk:
  movea.l sp, a3
  link   a4, #-4
  move.l #$12345678, (sp)
  cmp.l  #$12345678, (sp)
  bne.b  _link_fail
  unlk   a4
  cmpa.l a3, sp
  bne.w  record_fail
  addq.w #1, d6
  rts
_link_fail:
  unlk   a4
  bra.w  record_fail

test_chk_tas:
  move.w #5, d0
  move.w #9, d1
  chk    d1, d0
  lea    RAM_TEST, a0
  clr.b  (a0)
  tas    (a0)
  move.b (a0), d2
  cmpi.b #$80, d2
  bne.w  record_fail
  addq.w #1, d6
  rts

test_sr_ccr_misc:
  nop
  move.w sr, d0
  move.w #$0004, ccr
  ori.b  #$01, ccr
  andi.b #$1F, ccr
  eori.b #$01, ccr
  trapv
  move.w #$2700, sr
  move.w sr, d1
  andi.w #$2700, d1
  cmpi.w #$2700, d1
  bne.w  record_fail
  addq.w #1, d6
  rts

text_title:
  .db "68000 TEST", $00
text_pass:
  .db "PASS", $00
text_fail:
  .db "FAIL", $00
text_count:
  .db "COUNT", $00
