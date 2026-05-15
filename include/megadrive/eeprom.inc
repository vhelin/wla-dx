;
; Sega Mega Drive/Genesis serial EEPROM bit-bang helper macros
;
; Arguments are latch address, SDA bit and SCL bit unless noted otherwise.
;

.MACRO MD_EEPROM_WRITE_LATCH
  move.w #\2, \1
.ENDM

.MACRO MD_EEPROM_START
  move.w #((1 << \2) | (1 << \3)), \1
  move.w #(1 << \3), \1
.ENDM

.MACRO MD_EEPROM_STOP
  move.w #(1 << \3), \1
  move.w #((1 << \2) | (1 << \3)), \1
.ENDM

.MACRO MD_EEPROM_CLOCK_LOW
  move.w #(\2), \1
.ENDM

.MACRO MD_EEPROM_CLOCK_HIGH
  move.w #((\2) | (1 << \3)), \1
.ENDM

.MACRO MD_EEPROM_READ_WORD
  move.w (\1).l, \2
.ENDM
