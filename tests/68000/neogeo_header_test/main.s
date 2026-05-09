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
  NGH $1234
  BACKUPRAMPTR $00100000
  BACKUPRAMSIZE 2
  USERENTRY Start
  PLAYERSTART PlayerStart
  DEMOEND DemoEnd
  COINSOUND CoinSound
.ENDNG

.BANK 0 SLOT 0

.ORGA $0200
Start:
	rts

.ORGA $0220
PlayerStart:
	rts

.ORGA $0240
DemoEnd:
	rts

.ORGA $0260
CoinSound:
	rts