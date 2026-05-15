;
; Z80 sound driver for the Mega Drive HELLO WORLD example.
;
; The 68000 copies this program into Z80 RAM after the text is visible, then
; releases the Z80. The Z80 plays two PSG beeps, silences the PSG, and idles.
;

.INCDIR "../../../include/megadrive"

.INCLUDE "../../../memorymaps/mdz80.i"
.INCLUDE "z80.inc"

.ROMBANKSIZE $2000
.ROMBANKS 1

.EMPTYFILL $00

.BANK 0 SLOT 0
.ORG $0000
Start:
  di
  ld sp, $1FE0
  call psg_silence
  call psg_beep_high
  call delay_gap
  call psg_beep_low
  call psg_silence

Idle:
  jp Idle

psg_beep_high:
  ld a, $90
  ld (REG_MD_Z80_PSG), a
  ld a, $80
  ld (REG_MD_Z80_PSG), a
  ld a, $10
  ld (REG_MD_Z80_PSG), a
  call delay_beep
  ld a, $9F
  ld (REG_MD_Z80_PSG), a
  ret

psg_beep_low:
  ld a, $90
  ld (REG_MD_Z80_PSG), a
  ld a, $80
  ld (REG_MD_Z80_PSG), a
  ld a, $18
  ld (REG_MD_Z80_PSG), a
  call delay_beep
  ld a, $9F
  ld (REG_MD_Z80_PSG), a
  ret

psg_silence:
  ld a, $9F
  ld (REG_MD_Z80_PSG), a
  ld a, $BF
  ld (REG_MD_Z80_PSG), a
  ld a, $DF
  ld (REG_MD_Z80_PSG), a
  ld a, $FF
  ld (REG_MD_Z80_PSG), a
  ret

delay_beep:
  ld bc, $FFFF
DelayBeepLoop:
  dec bc
  ld a, b
  or c
  jr nz, DelayBeepLoop
  ret

delay_gap:
  ld de, $C000
DelayGapLoop:
  dec de
  ld a, d
  or e
  jr nz, DelayGapLoop
  ret
