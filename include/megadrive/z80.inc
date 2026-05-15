;
; Sega Mega Drive/Genesis Z80-side register definitions and helper macros
;

.DEFINE REG_MD_Z80_YM_A0       $4000
.DEFINE REG_MD_Z80_YM_D0       $4001
.DEFINE REG_MD_Z80_YM_A1       $4002
.DEFINE REG_MD_Z80_YM_D1       $4003
.DEFINE REG_MD_Z80_BANK        $6000
.DEFINE REG_MD_Z80_PSG         $7F11
.DEFINE REG_MD_Z80_68K_WINDOW  $8000

.MACRO MD_Z80_SET_BANK
  ld a, \1
  ld (REG_MD_Z80_BANK), a
.ENDM
