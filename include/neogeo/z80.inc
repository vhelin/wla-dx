;
; Neo Geo Z80 port definitions and helper macros
;

.DEFINE PORT_SOUND_COMMAND   $00
.DEFINE PORT_YM2610_A_ADDR   $04
.DEFINE PORT_YM2610_A_DATA   $05
.DEFINE PORT_YM2610_B_ADDR   $06
.DEFINE PORT_YM2610_B_DATA   $07
.DEFINE PORT_BANK_2K         $08
.DEFINE PORT_BANK_4K         $09
.DEFINE PORT_BANK_8K         $0A
.DEFINE PORT_BANK_16K        $0B
.DEFINE PORT_SOUND_REPLY     $0C
.DEFINE PORT_NMI_DISABLE     $18

.MACRO NG_Z80_CLEAR_COMMAND
  xor a
  out (PORT_SOUND_REPLY), a
  out (PORT_SOUND_COMMAND), a
.ENDM

.MACRO NG_Z80_REPLY
  ld a, \1
  out (PORT_SOUND_REPLY), a
.ENDM

.MACRO NG_Z80_ENABLE_NMI
  xor a
  in a, (PORT_BANK_2K)
.ENDM

.MACRO NG_Z80_DISABLE_NMI
  xor a
  in a, (PORT_NMI_DISABLE)
.ENDM

.MACRO NG_Z80_INIT_LINEAR_BANKS
  ld a, $1E
  in a, (PORT_BANK_2K)
  ld a, $0E
  in a, (PORT_BANK_4K)
  ld a, $06
  in a, (PORT_BANK_8K)
  ld a, $02
  in a, (PORT_BANK_16K)
.ENDM

; NEO-ZMC / NEO-ZMC2 bank switching helpers.
;
; The NEO-ZMC mapper selects a physical bank by the value in A at the
; moment an `in a, (port)` is executed against one of the bank-control
; ports. The read result is discarded; only the side effect (bank
; select) matters.
;
; PORT_BANK_16K ($0B) maps the $8000-$BFFF 16 KiB window.
; PORT_BANK_8K  ($0A) maps the $C000-$DFFF 8 KiB window.
; PORT_BANK_4K  ($09) maps the $E000-$EFFF 4 KiB window.
; PORT_BANK_2K  ($08) maps the $F000-$F7FF 2 KiB window.
;
; Each macro takes a single argument: the bank number to map into the
; corresponding window.

.MACRO NG_Z80_SET_BANK_16K
  ld a, \1
  in a, (PORT_BANK_16K)
.ENDM

.MACRO NG_Z80_SET_BANK_8K
  ld a, \1
  in a, (PORT_BANK_8K)
.ENDM

.MACRO NG_Z80_SET_BANK_4K
  ld a, \1
  in a, (PORT_BANK_4K)
.ENDM

.MACRO NG_Z80_SET_BANK_2K
  ld a, \1
  in a, (PORT_BANK_2K)
.ENDM