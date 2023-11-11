
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example testing different ways to define data with WLA
; written by ville helin <ville.helin@iki.fi> in 2021
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .memorymap
        defaultslot 0
        slotsize 2000h
        slot 0 $0000
        slot 1 0x2000
        slot 2 0x4000
        slot 3 6000h
        slot 4 $A000
        .endme

        .rombankmap
        bankstotal 4
        banksize $2000
        banks 4
        .endro

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; data definitions
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
        
        .macro increment
        .redefine _out \1+1
        .endm

        ; @BT result.rom

        .bank 0 slot 0
        .org 0

label_0:
        
        .section "TEST_01" FORCE KEEP
        .db "01>"               ; @BT TEST-01 01 START
        .db 1, 2, 3             ; @BT 01 02 03
        .dsb 3, 2               ; @BT 02 02 02
        .dbm increment 1, 2, 3  ; @BT 02 03 04
        .db "<01"               ; @BT END
        .ends

label_f:
        
        .section "TEST_02" FORCE KEEP
        .db "02>"               ; @BT TEST-02 02 START
        .dw 1, 2, 3             ; @BT 01 00 02 00 03 00
        .dsw 3, 2               ; @BT 02 00 02 00 02 00
        .dwm increment 1, 2, 3  ; @BT 02 00 03 00 04 00
        .db "<02"               ; @BT END
        .ends
        
label_27:

        .section "TEST_03" FORCE KEEP
        .db "03>"               ; @BT TEST-03 03 START
        .dl 1, 2, 3             ; @BT 01 00 00 02 00 00 03 00 00
        .dsl 3, 2               ; @BT 02 00 00 02 00 00 02 00 00
        .dlm increment 1, 2, 3  ; @BT 02 00 00 03 00 00 04 00 00
        .db "<03"               ; @BT END
        .ends
        
label_48:

        .section "TEST_04" FORCE KEEP
        .db "04>"               ; @BT TEST-04 04 START
        .dd 1, 2, 3             ; @BT 01 00 00 00 02 00 00 00 03 00 00 00
        .dsd 3, 2               ; @BT 02 00 00 00 02 00 00 00 02 00 00 00
        .ddm increment 1, 2, 3  ; @BT 02 00 00 00 03 00 00 00 04 00 00 00
        .db "<04"               ; @BT END
        .ends
        
label_72:

        .section "TEST_05" FORCE KEEP
        .db "05>"               ; @BT TEST-05 05 START
        .db $aa                 ; @BT AA
        .dw $bbcc               ; @BT CC BB
        .dl $ddeeff             ; @BT FF EE DD
        .dd $aabbccdd           ; @BT DD CC BB AA
        .db "<05"               ; @BT END
        .ends
        
        .section "TEST_06" FORCE KEEP
        .db "06>"                ; @BT TEST-06 06 START
        .db label_12+1           ; @BT 13
        .dw label_1234+1         ; @BT 35 12
        .dl label_1234+$560000   ; @BT 34 12 56
        .dd label_1234+$56000000 ; @BT 34 12 00 56
        .db "<06"                ; @BT END
        .ends
        
        .section "TEST_07" FORCE KEEP
        .db "07>"                 ; @BT TEST-07 07 START
        .dbm increment, $fd       ; @BT FE
        .dwm increment, $fffd     ; @BT FE FF
        .dlm increment, $fffffd   ; @BT FE FF FF
        .ddm increment, $fffffffd ; @BT FE FF FF FF
        .db "<07"                 ; @BT END
        .ends
        
        .section "TEST_08" FORCE KEEP
        .db "08>"                         ; @BT TEST-08 08 START
        .hex "010203aaBBcc", "DDee", "FF" ; @BT 01 02 03 AA BB CC DD EE FF
        .db "<08"                         ; @BT END
        .ends
        
        .ASCIITABLE
          MAP "A" TO "Z" = 0
          MAP "!" = $90
        .ENDA

        .section "TEST_09" FORCE KEEP
        .db "09>"                          ; @BT TEST-09 09 START
        .ASC $43, "ABC!", $41, $42, 'A'    ; @BT 02 00 01 02 90 00 01 00
        .ASCSTR $43, "ABC!", $41, $42, 'A' ; @BT 43 00 01 02 90 41 42 41
        .db "<09"                          ; @BT END
        .ends

        .org $12
label_12:

        .org $1234
label_1234:

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; label address tests
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»        
        
        .org $100

        .section "ManyNOPs" FORCE ; the size of this section is 13 ($0D) bytes
many_nops:
        nop
        nop
        nop
        beq many_nops
        nop
@loop_1:nop
        nop
        nop
        beq @loop_1
        nop
        nop
        .ends

        .section "BigDataSection" ; the size of this section is 7*16 ($70) bytes
data_a: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
data_b: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
data_c: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
data_d: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
data_e: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
data_f: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
data_g: .db 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
        .ends

        .section "LittleOne1" ; the size of this section is 5 ($05) bytes
little_one_1:
        nop
        .db "HI!"
        nop
        .ends
        
label_182:
        .db "10>"               ; @BT TEST-10 10 START
        .dw label_182, label_0, label_f, label_27, label_48, label_72, label_196 ; @BT 82 01 00 00 0F 00 27 00 48 00 72 00 96 01
        .db "<10"               ; @BT END
        
        .section "LittleOne2" returnorg ; the size of this section is 5 ($05) bytes
little_one_2:
        nop
        .db "YO!"
        nop
        .ends
        
label_196:
        nop
        nop
        beq label_196
        nop
label_19b:
        .db "11>"                ; @BT TEST-11 11 START
        .dw label_196, label_19b ; @BT 96 01 9B 01
        .db "<11"                ; @BT END

        .db "12>"                                                                                ; @BT TEST-12 12 START
        .dd %11111111111111111111111111111111, %10101010101010101010101010101010                 ; @BT FF FF FF FF AA AA AA AA
        .dd %11111111111111111111111111111111 - 0 + 0, %10101010101010101010101010101010 - 0 + 0 ; @BT FF FF FF FF AA AA AA AA
        .dd %11111111111111111111111111111111 & %11111111000000001111111100000000                ; @BT 00 FF 00 FF
        .dd %11111111000000001111111100000000 | %00000000111111110000000011111111                ; @BT FF FF FF FF
        .db "<12"                                                                                ; @BT END

        .db "13>"                                 ; @BT TEST-13 13 START
        .dd $fffffff0 + $f, $a0a0a0a0 | $0b0b0b0b ; @BT FF FF FF FF AB AB AB AB
        .dd 01234500h | 10000067h                 ; @BT 67 45 23 11
        .db "<13"                                 ; @BT END

        .db "14>"                        ; @BT TEST-14 14 START
        .dd V1, V1+1, V2, V2 | $00ff00ff ; @BT CC DD EE FF CD DD EE FF 00 FF 00 FF FF FF FF FF
        .db "<14"                        ; @BT END

        .db "15>"                        ; @BT TEST-15 15 START
        .dd V3, V3+1, V4, V4 + $01020304 ; @BT 04 03 02 01 05 03 02 01 AA AA AA AA AE AD AC AB
        .db "<15"                        ; @BT END

        .db "16>"          ; @BT TEST-16 16 START
        .dd W1, W2, W3, W4 ; @BT 78 56 34 12 F0 F0 F0 F0 04 03 02 01 AA AA AA AA
        .db "<16"          ; @BT END

        .db "17>"                                                                    ; @BT TEST-17 17 START
        .dd 0x12345678, 0b11110000111100001111000011110001                           ; @BT 78 56 34 12 F1 F0 F0 F0
        .dd 0x01020304 | 0x10203040, 0b11111111111111111111111111111111 & 0x0a0b0c0d ; @BT 44 33 22 11 0D 0C 0B 0A
        .db "<17"                                                                    ; @BT END
        
