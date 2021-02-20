
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example testing spc-700 parsing
; written by ville helin <ville.helin@iki.fi> in 2021
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
SlotSize $10000
DeFaUlTsLoT 0
slOt 0 $0000
.ENDME

.ROMBANKMAP
BANKStotal 1
BaNKSiZE $10000
BaNKs 1
.ENDRO

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; tests
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.bank 0 slot 0

// bits for settings
.define SETTINGS_BIT_AUDIO    0
.define SETTINGS_BIT_JOYSTICK 1
.define SETTINGS_BIT_KEYBOARD 2

// bits for cheat_codes
.define CHEAT_CODES_BIT_1 0
.define CHEAT_CODES_BIT_2 1
.define CHEAT_CODES_BIT_3 2

.org 0

.section "VARIABLES" force
settings:
    .db 0
cheat_codes:
    .db 0
.ends

.org $200

.section "TEST01" force
    .db "01>"
    bbc settings.SETTINGS_BIT_KEYBOARD,label_1
label_1:
    set1 settings.SETTINGS_BIT_AUDIO
    and1 C,/cheat_codes.CHEAT_CODES_BIT_1
    and1 C,cheat_codes.CHEAT_CODES_BIT_1
    .db "<01"
.ends

.section "TEST02" force
    .db "02>"
    bbc settings,SETTINGS_BIT_KEYBOARD,label_2
label_2:
    set1 settings,SETTINGS_BIT_AUDIO
    and1 C,/cheat_codes,CHEAT_CODES_BIT_1
    and1 C,cheat_codes,CHEAT_CODES_BIT_1
    .db "<02"
.ends

.define TCALL_0  0
.define TCALL_1  1
.define TCALL_15 15
        
.section "TEST03" force
    .db "03>"
    adc a, [SETTINGS_BIT_JOYSTICK+X]
    adc a, [SETTINGS_BIT_JOYSTICK]+Y
    tcall 0
    tcall TCALL_1 + 0
    tcall TCALL_1 * 10 - 10 + TCALL_15
    tclr1 !cheat_codes
    tset1 !cheat_codes
    .db "<03"
.ends
