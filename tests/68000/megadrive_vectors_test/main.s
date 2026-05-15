.INCLUDE "../../../memorymaps/md68k.i"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $10000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.MDVECTORS
  RESET Reset
  DEFAULT DefaultHandler
  HBLANK HBlankHandler
  VBLANK VBlankHandler
  TRAP0 Trap0Handler
.ENDMDVECTORS

.BANK 0 SLOT 0

.ORGA $0200
Reset:
  rts

.ORGA $0220
HBlankHandler:
  rts

.ORGA $0240
VBlankHandler:
  rts

.ORGA $0260
DefaultHandler:
  rts

.ORGA $0280
Trap0Handler:
  rts
