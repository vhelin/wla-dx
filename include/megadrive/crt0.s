;
; Sega Mega Drive/Genesis minimal MC68000 startup stub
;
; Include 68k.inc and macros.inc first, then include this file from ROM code.
; The user program should provide a label named main.
;

_md_start:
  MD_DISABLE_INTERRUPTS
  lea $FFFFFE00, sp

  move.b REG_MD_IO_VERSION, d0
  andi.b #$0F, d0
  beq _md_skip_tmss
  MD_WRITE_TMSS
_md_skip_tmss:

  MD_Z80_REQUEST_BUS
  MD_Z80_HOLD_RESET
  lea _md_z80_driver, a0
  lea REG_MD_Z80_RAM, a1
  move.w #(_md_z80_driver_end-_md_z80_driver)-1, d7
_md_z80_copy:
  move.b (a0)+, (a1)+
  dbra d7, _md_z80_copy
  MD_Z80_RELEASE_RESET
  MD_Z80_RELEASE_BUS

  lea _md_vdp_init_table, a0
  move.w #((_md_vdp_init_table_end-_md_vdp_init_table)/2)-1, d7
_md_vdp_init:
  move.w (a0)+, REG_MD_VDP_CTRL
  dbra d7, _md_vdp_init

  MD_PSG_SILENCE
  move.b #$40, REG_MD_IO_CTRL1
  move.b #$40, REG_MD_IO_CTRL2
  move.b #$00, REG_MD_IO_CTRL3
  move.b #$40, REG_MD_IO_DATA1
  move.b #$40, REG_MD_IO_DATA2

  moveq #0, d0
  lea $FF0000, a0
  move.w #$3FFF, d7
_md_clear_wram:
  move.l d0, (a0)+
  dbra d7, _md_clear_wram

  moveq #0, d1
  moveq #0, d2
  moveq #0, d3
  moveq #0, d4
  moveq #0, d5
  moveq #0, d6
  moveq #0, d7
  movea.l d0, a0
  movea.l d0, a1
  movea.l d0, a2
  movea.l d0, a3
  movea.l d0, a4
  movea.l d0, a5
  movea.l d0, a6

  jsr main
_md_hang:
  bra _md_hang

_md_default_irq:
  rte

_md_z80_driver:
  .db $F3,$ED,$56,$31,$00,$20,$18,$FE
_md_z80_driver_end:

_md_vdp_init_table:
  .dw $8004,$8174,$8230,$832C,$8407,$8578,$8600,$8700
  .dw $8800,$8900,$8A00,$8B00,$8C81,$8D3F,$8E00,$8F02
  .dw $9001,$9100,$9200,$9300,$9400,$9500,$9600,$9700
_md_vdp_init_table_end:
