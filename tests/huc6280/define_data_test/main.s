
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example testing different ways to define data with WLA
; written by ville helin <ville.helin@iki.fi> in 2021
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .memorymap
        defaultslot 0
        slotsize $2000
        slot 0 $0000
        slot 1 $2000
        slot 2 $4000
        slot 3 $6000
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

        .bank 0 slot 0
        .org 0

label_0:
        
        .section "TEST_01" FORCE KEEP
        .db "01>"
        .db 1, 2, 3
        .dsb 3, 2
        .dbm increment 1, 2, 3
        .db "<01"
        .ends

label_f:
        
        .section "TEST_02" FORCE KEEP
        .db "02>"
        .dw 1, 2, 3
        .dsw 3, 2
        .dwm increment 1, 2, 3
        .db "<02"
        .ends
        
label_27:

        .section "TEST_03" FORCE KEEP
        .db "03>"
        .dl 1, 2, 3
        .dsl 3, 2
        .dlm increment 1, 2, 3
        .db "<03"
        .ends
        
label_48:

        .section "TEST_04" FORCE KEEP
        .db "04>"
        .dd 1, 2, 3
        .dsd 3, 2
        .ddm increment 1, 2, 3
        .db "<04"
        .ends
        
label_72:

        .section "TEST_05" FORCE KEEP
        .db "05>"
        .db $aa
        .dw $bbcc
        .dl $ddeeff
        .dd $aabbccdd
        .db "<05"
        .ends
        
        .section "TEST_06" FORCE KEEP
        .db "06>"
        .db label_12+1
        .dw label_1234+1
        .dl label_1234+$560000
        .dd label_1234+$56000000
        .db "<06"
        .ends
        
        .section "TEST_07" FORCE KEEP
        .db "07>"
        .dbm increment, $fd
        .dwm increment, $fffd
        .dlm increment, $fffffd
        .ddm increment, $fffffffd
        .db "<07"
        .ends
        
        .section "TEST_08" FORCE KEEP
        .db "08>"
        .hex "010203aaBBcc", "DDee", "FF"
        .db "<08"
        .ends
        
        .ASCIITABLE
          MAP "A" TO "Z" = 0
          MAP "!" = $90
        .ENDA

        .section "TEST_09" FORCE KEEP
        .db "09>"
        .ASC $43, "ABC!", $41, $42, 'A'
        .ASCSTR $43, "ABC!", $41, $42, 'A'
        .db "<09"
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
        .db "10>"
        .dw label_182, label_0, label_f, label_27, label_48, label_72, label_196
        .db "<10"
        
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
        .db "11>"
        .dw label_196, label_19b
        .db "<11"
        
