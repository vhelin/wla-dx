;
; Minimal Z80 sound CPU program for the Neo Geo 68k instruction test.
; It acknowledges BIOS sound housekeeping commands so crt0 can reset
; the sound CPU cleanly; the 68k instruction test itself is silent.
;

.INCDIR "../../../include/neogeo"

.INCLUDE "../../../memorymaps/neogeoz80.i"
.INCLUDE "macros.inc"
.INCLUDE "z80.inc"

.ROMBANKMAP
  BANKSTOTAL 32
  BANKSIZE $0800
  BANKS 32
.ENDRO

.EMPTYFILL $00

.DEFINE ResetWaitPc $FFFE
.DEFINE OpcodeJrSelf $FE18

.BANK 0 SLOT 0
.ORGA $0000

  di
  jp    z80_init

.ORGA $0038
  reti

.ORGA $0066
nmi_handler:
  in    a, (PORT_SOUND_COMMAND)

  cp    NG_SOUND_CMD_PREPARE_SWITCH
  jr    z, nmi_ack

  cp    NG_SOUND_CMD_RESET
  jr    z, nmi_do_reset

nmi_ack:
  or    $80
  out   (PORT_SOUND_REPLY), a
  retn

nmi_do_reset:
  ld    a, NG_SOUND_CMD_RESET
  or    $80
  out   (PORT_SOUND_REPLY), a
  di
  ld    sp, $FFFD
  ld    hl, driver_start
  push  hl
  retn

.ORGA $0100
z80_init:
  im    1
  ld    sp, $FFFF
  xor   a
  out   (PORT_BANK_2K), a

  ld    bc, OpcodeJrSelf
  ld    (ResetWaitPc), bc
  ld    hl, ResetWaitPc
  ld    sp, hl
  push  hl
  ret

driver_start:
  di
  ld    sp, $FFFD
  NG_Z80_INIT_LINEAR_BANKS
  NG_Z80_CLEAR_COMMAND

main_loop:
  jr    main_loop
