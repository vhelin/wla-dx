.INCDIR "../../../include/neogeo"

.INCLUDE "../../../memorymaps/neogeo68k.i"
.INCLUDE "68k.inc"
.INCLUDE "fixprint.inc"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $100000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0

.ORGA $0066
.NGVECTORS
  VBLANK VBlankHandler
  TIMER TimerHandler
  EXTERNAL ExternalHandler
.ENDNGVECTORS

.ORGA $0200
VBlankHandler:
  rts

.ORGA $0220
TimerHandler:
  rts

.ORGA $0240
ExternalHandler:
  rts

.ORGA $0300
UsePrint:
  NG_PRINT $70CD, Text
  rts

NG_FIXPRINT_ROUTINE

Text:
  .db "OK", $00