.INCDIR "../../../include/neogeo"

.INCLUDE "../../../memorymaps/neogeo68k.i"
.INCLUDE "macros.inc"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.NGHEADER
  SYSTEM CD
  NGH $4321
  USERENTRY Start
  CDDACMDPTR $F800
.ENDNG

.BANK 0 SLOT 0

.ORGA $0200
Start:
	rts
