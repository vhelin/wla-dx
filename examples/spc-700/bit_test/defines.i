;==============================================================
; Memory banking setup
;==============================================================
.MEMORYMAP
    DEFAULTSLOT 0
    ; SPC header
    SLOTSIZE $100
    SLOT 0 $0000
    ; 64KB RAM
    SLOTSIZE $10000
    SLOT 1 $0100
    ; DSP registers
    SLOTSIZE $80
    SLOT 2 $10180
    ; unused
    SLOTSIZE $40
    SLOT 3 $101C0
    ; 64 Extra RAM (Memory region used when the IPL ROM region is set to read-only)
    SLOTSIZE $40
    SLOT 4 $10200
.ENDME

.ROMBANKMAP
    BANKSTOTAL 5
    ; SPC header
    BANKSIZE $100
    BANKS 1
    ; 64KB RAM
    BANKSIZE $10000
    BANKS 1
    ; DSP registers
    BANKSIZE $80
    BANKS 1
    ; unused
    BANKSIZE $40
    BANKS 1
    ; 64 Extra RAM (Memory region used when the IPL ROM region is set to read-only)
    BANKSIZE $40
    BANKS 1
.ENDRO

.EMPTYFILL $00

.DEFINE PRG_BEGIN $8000

.ASCIITABLE
; Use default N to N map
.ENDA

.BANK 0 SLOT 0
; 00000h    33 File header "SNES-SPC700 Sound File Data v0.30"
.ASC "SNES-SPC700 Sound File Data v0.30"
; 00021h     2 26,26
.BYTE 26
.BYTE 26
; 00023h     1 26 = header contains ID666 information
.BYTE 26
;              27 = header contains no ID666 tag
; 00024h     1 Version minor (i.e. 30)
.BYTE 30
; 
; SPC700 Registers:
; 00025h     2 PC
.WORD PRG_BEGIN
; 00027h     1 A
.BYTE 0
; 00028h     1 X
.BYTE 0
; 00029h     1 Y
.BYTE 0
; 0002Ah     1 PSW
.BYTE 0
; 0002Bh     1 SP (lower byte)
.BYTE $FF
; 0002Ch     2 reserved
.WORD 0
; 
; ID666 Tag (text format):
; 0002Eh    32 Song title
.ASC "Bit Instructions"
; 0004Eh    32 Game title
.ORG $004E
.ASC "SPC-700 CPU Tests"
; 0006Eh    16 Name of dumper
.ORG $006E
.ASC "wla-spc700"
; 0007Eh    32 Comments
; 0009Eh    11 Date SPC was dumped (MM/DD/YYYY)
; 000A9h     3 Number of seconds to play song before fading out
.ORG $00A9
.ASC "120"
; 000ACh     5 Length of fade in milliseconds
.ORG $00AC
.ASC "5000"
; 000B1h    32 Artist of song
; 000D1h     1 Default channel disables (0 = enable, 1 = disable)
; 000D2h     1 Emulator used to dump SPC:
;              0 = unknown
; 	     1 = ZSNES
; 	     2 = Snes9x
; 000D3h    45 reserved (set to all 0's)

.BANK 1 SLOT 1
; Set the main entry point of the program to start at PRG_BEGIN ($8000)
.ORG PRG_BEGIN
