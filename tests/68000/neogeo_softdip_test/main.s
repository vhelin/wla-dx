.INCDIR "../../../include/neogeo"

.INCLUDE "../../../memorymaps/neogeo68k.i"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.NGHEADER
  NGH $0001
  JPCONFIG SoftDIPJP
  USCONFIG SoftDIPJP
  EUCONFIG SoftDIPJP
  USERENTRY Start
.ENDNG

.BANK 0 SLOT 0

.ORGA $0200
Start:
	rts

.ORGA $0300
SoftDIPJP:
.NGSOFTDIP
  NAME "TEST GAME"
  SPECIAL $FF,$FF,$FF,$FF,$FF,$FF
  OPTIONS $24,$02,$00,$00,$00,$00,$00,$00,$00,$00
  TEXT "LIVES       "
  TEXT "1           "
  TEXT "2           "
  TEXT "3           "
  TEXT "4           "
  TEXT "HOW TO PLAY "
  TEXT "WITH        "
  TEXT "WITHOUT     "
.ENDNGSOFTDIP
