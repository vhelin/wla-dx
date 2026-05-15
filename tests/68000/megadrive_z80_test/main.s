.INCDIR "../../../include/megadrive"
.INCLUDE "../../../memorymaps/mdz80.i"
.INCLUDE "z80.inc"

.ROMBANKSIZE $2000
.ROMBANKS 1

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG $0000
Start:
  MD_Z80_SET_BANK $12
  ld a, (REG_MD_Z80_68K_WINDOW)
  ld (REG_MD_Z80_PSG), a
  jp Start