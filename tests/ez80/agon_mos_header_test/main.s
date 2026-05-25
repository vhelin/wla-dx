; @BT linked.rom

.INCDIR "../../../include/agon"
.INCLUDE "../../../memorymaps/agon_mos.i"
.INCLUDE "mos.inc"

.ROMBANKMAP
  BANKSTOTAL 1
  BANKSIZE $1000000
  BANKS 1
.ENDRO

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG 0

.DB "01>"                       ; @BT TEST-01 01 START
AGON_MOS_HEADER AGON_MOS_RUNMODE_ADL ; @BT 4D 4F 53 00 01
AGON_MOS_HEADER_Z80              ; @BT 4D 4F 53 00 00
AGON_MOS_HEADER_ADL              ; @BT 4D 4F 53 00 01
AGON_MOS_EXEC_NAME "APP.BIN"     ; @BT 41 50 50 2E 42 49 4E 00
.DB "<01"                       ; @BT END

.ORG AGON_MOS_HEADER_OFFSET
.DB "02>"                       ; @BT TEST-02 02 START
AGON_MOS_HEADER_ADL              ; @BT 4D 4F 53 00 01
.ADL ON
start:
  jp start                       ; @BT C3 48 00 00
.DB "<02"                       ; @BT END