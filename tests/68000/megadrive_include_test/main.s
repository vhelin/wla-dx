.INCDIR "../../../include/megadrive"
.INCLUDE "../../../memorymaps/md68k.i"
.INCLUDE "68k.inc"
.INCLUDE "vdp_regs.inc"
.INCLUDE "macros.inc"
.INCLUDE "bios.inc"
.INCLUDE "jcart.inc"
.INCLUDE "eeprom.inc"
.INCLUDE "mcd_main.inc"
.INCLUDE "mcd_sub.inc"

.ROMBANKSIZE $4000
.ROMBANKS 1

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORGA $0000
Start:
  MD_DISABLE_INTERRUPTS
  MD_VDP_WRITE_REG MD_VDP_REG_MODE2, MD_VDP_MODE2_DISPLAY
  MD_VDP_SET_ADDR_VRAM_WRITE $1234
  MD_Z80_REQUEST_BUS
  MD_PSG_SILENCE
