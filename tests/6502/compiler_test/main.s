
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 2
           BANKSIZE $2000
           BANKS 2
        .ENDRO

        .BANK 00  SLOT 0
        .ORG    0

.define none    $00

; @BT linked.rom

.db "02>"      ; @BT TEST-02 02 START
.if 0 == 1
	jp pohoih, pojp, egr eg,erg, ege,rg,egr,egr,eg,erg,th,rt,ru,uy,eur,j,yjet,ye,ye,iye,ie,eu,e,i
	.db 1,2,3,4,5,6,9,23,43,5,5,3,3,4,45,6,4,3,2, none, none, none
   .DB   none, none, none, none, none, none, none, none, none, none, none, none, none, none
.else
   .DB   7     ; @BT 07
.endif
.db "<02"      ; @BT END

.STRUCT water
name   ds 8
age    db
weight dw
.ENDST

.db "01>"                                                      ; @BT TEST-01 01 START
.DSTRUCT waterdrop1 INSTANCEOF water DATA "tingle", 40, 120    ; @BT 74 69 6E 67 6C 65 00 00 28 78 00
.DSTRUCT waterdrop2, water, "tingle", 40, 120                  ; @BT 74 69 6E 67 6C 65 00 00 28 78 00
.DSTRUCT waterdrop3, water, "somedrop"                         ; @BT 73 6F 6D 65 64 72 6F 70 00 00 00 
.db $ff, $ff                                                   ; @BT FF FF
.db "<01"                                                      ; @BT END

        .db "03>"                   ; @BT TEST-03 03 START
        .db "TEST\r\n\0\t"          ; @BT 54 45 53 54 0D 0A 00 09
        .db "<03"                   ; @BT END

        .define STRING_A = "HI \"TIM\""
        
        .db "04>"                   ; @BT TEST-04 04 START
        .db STRING_A                ; @BT 48 49 20 22 54 49 4D 22
        .db "<04"                   ; @BT END

        .db "05>"                   ; @BT TEST-05 05 START
        .db "\x01\x02\x03\x04HI\0"  ; @BT 01 02 03 04 48 49 00
        .db "<05"                   ; @BT END

        .db "06>"                   ; @BT TEST-06 06 START
        .db "\x12"                  ; @BT 12
        .db "<06"                   ; @BT END

        .macro db_string
label_\1:
        .db "\x01B\1\0\2"
        .endm
        
        .db "07>"                   ; @BT TEST-07 07 START
        db_string "HI", 1           ; @BT 01 42 48 49 00 31
        .db "<07"                   ; @BT END
        

        
.MACRO make_rst
  .ORG (?1 * $0003 + $203)
  .SECTION "BOOT_RESET_\1_?1" SIZE $0003 FORCE KEEP
boot_rst_\1_?1:
      .db "\1_?1"
  .ENDS
.ENDM

        .org $200
        .db "08>"             ; @BT TEST-08 08 START

        .REPEAT 8 INDEX I
        make_rst &I           ; @BT 49 5F 30 49 5F 31 49 5F 32 49 5F 33 49 5F 34 49 5F 35 49 5F 36 49 5F 37
        .ENDR

        .db "<08"             ; @BT END
        

        .macro string_macro
        .db "\\1\?1"
        .endm
        
        .db "09>"             ; @BT TEST-09 09 START
        string_macro &I       ; @BT 5C 31 3F 31
        .db "<09"             ; @BT END

        .db "10>"             ; @BT TEST-10 10 START
        .fopen "data.bin" fp
        .fsize fp t
        .repeat t
        .fread fp d
        .db d                 ; @BT 41 42 43
        .endr
        .undefine t, d
        .db "<10"             ; @BT END

        .ftell fp fp_position
        .if fp_position != 3
        .fail "fp_position should be 3!"
        .endif
        .fclose fp

        .macro read_data
        .fread \1 d
        .db d
        .fread \1 d
        .db d
        .fread \1 d
        .db d
        .fseek \1 1 start
        .fread \1 d
        .db d
        .ftell \1 fp_position
        .if fp_position != 2
        .fail "fp_position should be 2!"
        .endif
        .fseek \1 -3 end
        .fread \1 d
        .db d
        .fseek \1 1 current
        .fread \1 d
        .db d
        .ftell \1 fp_position
        .if fp_position != 3
        .fail "fp_position should be 3!"
        .endif
        .endm
        
        .db "11>"             ; @BT TEST-11 11 START
        .fopen "data.bin" fp
        read_data fp          ; @BT 41 42 43 42 41 43
        .fclose fp
        .undefine d
        .db "<11"             ; @BT END

        .db "12>"             ; @BT TEST-12 12 START
        ; once we had a bug in WLA that '}' didn't work with .db
        .db "{}", "{", "}"    ; @BT 7B 7D 7B 7D
        .db "hello".length    ; @BT 05
        .db "<12"             ; @BT END
