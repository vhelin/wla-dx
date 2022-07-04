
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
address_0x0200: 
        
; @BT result.rom

.section "TEST01" force
    .db "01>"                                       ; @BT TEST-01 01 START
    bbc settings.SETTINGS_BIT_KEYBOARD,label_1      ; @BT 53 00 00
label_1:
    set1 settings.SETTINGS_BIT_AUDIO                ; @BT 02 00
    and1 C,/cheat_codes.CHEAT_CODES_BIT_1           ; @BT 6A 01 00
    and1 C,cheat_codes.CHEAT_CODES_BIT_1            ; @BT 4A 01 00
    .db "<01"                                       ; @BT END
.ends

.section "TEST02" force
    .db "02>"                                       ; @BT TEST-02 02 START
    bbc settings,SETTINGS_BIT_KEYBOARD,label_2      ; @BT 53 00 00
label_2:
    set1 settings,SETTINGS_BIT_AUDIO                ; @BT 02 00
    and1 C,/cheat_codes,CHEAT_CODES_BIT_1           ; @BT 6A 01 00
    and1 C,cheat_codes,CHEAT_CODES_BIT_1            ; @BT 4A 01 00
    .db "<02"                                       ; @BT END
.ends

.define TCALL_0  0
.define TCALL_1  1
.define TCALL_15 15
        
.section "TEST03" force
    .db "03>"                                       ; @BT TEST-03 03 START
    adc a, [SETTINGS_BIT_JOYSTICK+X]                ; @BT 87 01
    adc a, [SETTINGS_BIT_JOYSTICK]+Y                ; @BT 97 01
    tcall 0                                         ; @BT 01
    tcall TCALL_1 + 0                               ; @BT 11
    tcall TCALL_1 * 10 - 10 + TCALL_15              ; @BT F1
    tclr1 !cheat_codes                              ; @BT 4E 01 00
    tset1 !cheat_codes                              ; @BT 0E 01 00
    .db "<03"                                       ; @BT END
.ends

.section "TEST04" force
        .db "04>"                   ; @BT TEST-04 04 START
        jmp !address_0x0200         ; @BT 5F 00 02
        jmp [!address_0x0200+x]     ; @BT 1F 00 02
        .db "<04"                   ; @BT END
.ends
        
        .section "TEST05" force
        .db "05>"               ; @BT TEST-05 05 START
        .db "Test\\"            ; @BT 54 65 73 74 5C
        .db "\\'"               ; @BT 5C 27
        .db "\\"                ; @BT 5C
        .db "T\\\"\\T"          ; @BT 54 5C 22 5C 54
        .db "<05"               ; @BT END
        .ends
        
