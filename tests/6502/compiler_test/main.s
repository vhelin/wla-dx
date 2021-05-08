
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 00  SLOT 0
        .ORG    0

.define none    $00


.db "02>"
.if 0 == 1
	jp pohoih, pojp, egr eg,erg, ege,rg,egr,egr,eg,erg,th,rt,ru,uy,eur,j,yjet,ye,ye,iye,ie,eu,e,i
	.db 1,2,3,4,5,6,9,23,43,5,5,3,3,4,45,6,4,3,2, none, none, none
   .DB   none, none, none, none, none, none, none, none, none, none, none, none, none, none
.else
   .DB   7
.endif
.db "<02"

.STRUCT water
name   ds 8
age    db
weight dw
.ENDST

.db "01>"
.DSTRUCT waterdrop1 INSTANCEOF water DATA "tingle", 40, 120
.DSTRUCT waterdrop2, water, "tingle", 40, 120
.DSTRUCT waterdrop3, water, "somedrop"
.db $ff, $ff
.db "<01"

        .db "03>"
        .db "TEST\r\n\0\t"      ; @BT 54 45 53 54 0D 0A 00 09
        .db "<03"

        .define STRING_A = "HI \"TIM\""
        
        .db "04>"
        .db STRING_A            ; @BT 48 49 20 22 54 49 4D 22
        .db "<04"

        .db "05>"
        .db "\x01\x02\x03\x04HI\0" ; @BT 01 02 03 04 48 49 00
        .db "<05"

        .db "06>"
        .db "\x12"              ; @BT 12
        .db "<06"

        .macro db_string
label_\1:
        .db "\x01B\1\0\2"
        .endm
        
        .db "07>"
        db_string "HI", 1       ; @BT 01 42 48 49 00 31
        .db "<07"
        

        
.MACRO make_rst
  .ORG (?1 * $0003 + $203)
  .SECTION "BOOT_RESET_\1_?1" SIZE $0003 FORCE KEEP
boot_rst_\1_?1:
      .db "\1_?1"
  .ENDS
.ENDM

        .org $200
        .db "08>"

        .REPEAT 8 INDEX I
        make_rst @I
        .ENDR

        .db "<08"
        

        .macro string_macro
        .db "\\1\?1"
        .endm
        
        .db "09>"
        string_macro @I
        .db "<09"


    .db "10>"
    .fopen "data.bin" fp
    .fsize fp t
    .repeat t
    .fread fp d
    .db d
    .endr
    .undefine t, d
    .db "<10"
