
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

        .BANK 0 SLOT 0
        .ORG 0

        // define a too small array for mapping "0123456789" -> 4-bits
        // it gets enlarged by out-of-bounds .ARRAYINs later...
        .ARRAYDEFINE NAME MyArray SIZE 4

        // define the mapping
        .ARRAYIN NAME MyArray INDEX '0' VALUE %0000
        .ARRAYIN NAME MyArray INDEX '1' VALUE %0001
        .ARRAYIN NAME MyArray INDEX '2' VALUE %0010
        .ARRAYIN NAME MyArray INDEX '3' VALUE %0011
        .ARRAYIN NAME MyArray INDEX '4' VALUE %0100
        .ARRAYIN MyArray '5' %0101
        .ARRAYIN MyArray '6' %0110
        .ARRAYIN MyArray '7' %0111
        .ARRAYIN MyArray '8' %1000
        .ARRAYIN MyArray '9' %1001

        ; @BT linked.rom

        // map!
        .DB "01>"                                               ; @BT TEST-01 01 START
        .ARRAYOUT NAME MyArray INDEX '6' DEFINITION MAPPING
        .DB MAPPING                                             ; @BT 06
        .ARRAYOUT NAME MyArray INDEX '6' DEFINITION MAPPING
        .DB MAPPING                                             ; @BT 06
        .ARRAYOUT NAME MyArray INDEX '8' DEFINITION MAPPING
        .DB MAPPING                                             ; @BT 08
        .ARRAYOUT MyArray '2' MAPPING
        .DB MAPPING                                             ; @BT 02
        .ARRAYOUT MyArray '7' MAPPING
        .DB MAPPING                                             ; @BT 07
        .ARRAYOUT MyArray '5' MAPPING
        .DB MAPPING                                             ; @BT 05
        .DB "<01"                                               ; @BT END

        .ARRAYIN MyArray 'A' $ccddeeff
        .ARRAYIN MyArray 'B' ($00110011 | $ff00ff00)

        .DB "02>"                       ; @BT TEST-02 02 START
        .ARRAYOUT MyArray 'A' VALUE_A
        .ARRAYOUT MyArray 'B' VALUE_B
        .DD VALUE_A                     ; @BT FF EE DD CC 
        .DD VALUE_B                     ; @BT 11 FF 11 FF
        .DB "<02"                       ; @BT END

        .DB "03>"                                                   ; @BT TEST-03 03 START
        .ARRAYDB NAME MyArray INDICES '6', '6', '8', '2', '7', '5'  ; @BT 06 06 08 02 07 05
        .DB "<03"                                                   ; @BT END

        .ARRAYIN MyArray 'C' $1234
        .ARRAYIN MyArray 'D' $abcd
        .ARRAYIN MyArray 0 $5678
        
        .DB "04>"                                   ; @BT TEST-04 04 START
        .ARRAYDW NAME MyArray INDICES 'C', 'D', 0   ; @BT 34 12 CD AB 78 56
        .DB "<04"                                   ; @BT END

        .DB "05>"                                   ; @BT TEST-05 05 START
        .ARRAYDB NAME MyArray INDICES "668275"      ; @BT 06 06 08 02 07 05
        .DB "<05"                                   ; @BT END
        
        .print "HELLO 01\n"
        .ARRAYIN MyArray 1 $123456
        .ARRAYIN MyArray 2 ($111111 | $222222)
        .print "HELLO 02\n"

        .DB "06>"                                               ; @BT TEST-06 06 START
        .ARRAYDL NAME MyArray INDICES 0+1+2+3-5, 1+1+1+1-1-1    ; @BT 56 34 12 33 33 33
        .DB "<06"                                               ; @BT END
        
        // redefine some of the mapping
        .ARRAYIN NAME MyArray INDEX '0' VALUE $00000000
        .ARRAYIN NAME MyArray INDEX '1' VALUE $11111111
        .ARRAYIN NAME MyArray INDEX '2' VALUE $22222222
        .ARRAYIN NAME MyArray INDEX '3' VALUE $33333333
        .ARRAYIN NAME MyArray INDEX '4' VALUE $44444444
        .ARRAYIN NAME MyArray INDEX '5' VALUE $55555555
        .ARRAYIN NAME MyArray INDEX '6' VALUE $66666666
        .ARRAYIN NAME MyArray INDEX '7' VALUE $77777777
        .ARRAYIN NAME MyArray INDEX '8' VALUE $88888888
        .ARRAYIN NAME MyArray INDEX '9' VALUE $99999999

        .DB "07>"                                   ; @BT TEST-07 07 START
        .ARRAYDD NAME MyArray INDICES "C128", 0     ; @BT 34 12 00 00 11 11 11 11 22 22 22 22 88 88 88 88 78 56 00 00
        .DB "<07"                                   ; @BT END

        // define an array for mapping ASCII values into
        // less bits
        .ARRAYDEFINE SIZE 4 NAME MapArray
        
        .ARRAYIN NAME MapArray INDEX 'A' VALUES %0000, %0001, %0010, \
            %0011, %0100, %0101, %0110
        .ARRAYIN NAME MapArray INDEX  0  VALUE %1111

        .MACRO MapInto4Bits
        .ARRAYOUT NAME MapArray INDEX \1 DEFINITION MAPPING
        .PRINT \2, ": ", \1, " -> ", MAPPING, "\n"
        .BITS 4 MAPPING
        .IF \1 == 0
            .ENDBITS
        .ENDIF
        .ENDM
        
        .DB "08>"                           ; @BT TEST-08 08 START
        .FILTER MapInto4Bits "BAGED", 0     ; @BT 10 64 3F
        .DB "<08"                           ; @BT END

        // same as TEST-08, but with different .ARRAYDEFINE
        .ARRAYDEF MapArray2 SIZE 4
        
        .ARRAYIN NAME MapArray2 INDEX 'A' VALUES %0000, %0001, %0010, \
            %0011, %0100, %0101, %0110
        .ARRAYIN NAME MapArray2 INDEX  0  VALUE %1111

        .MACRO MapInto4Bits2
        .ARRAYOUT NAME MapArray2 INDEX \1 DEFINITION MAPPING
        .PRINT \2, ": ", \1, " -> ", MAPPING, "\n"
        .BITS 4 MAPPING
        .IF \1 == 0
            .ENDBITS
        .ENDIF
        .ENDM
        
        .DB "09>"                           ; @BT TEST-09 09 START
        .FILTER MapInto4Bits2 "BAGED", 0    ; @BT 10 64 3F
        .DB "<09"                           ; @BT END

        // same as TEST-08, but with different .ARRAYDEFINE
        .ARRAYDEFINE MapArray3 4
        
        .ARRAYIN NAME MapArray3 INDEX 'A' VALUES %0000, %0001, %0010, \
            %0011, %0100, %0101, %0110
        .ARRAYIN NAME MapArray3 INDEX  0  VALUE %1111

        .MACRO MapInto4Bits3
        .ARRAYOUT NAME MapArray3 INDEX \1 DEFINITION MAPPING
        .PRINT \2, ": ", \1, " -> ", MAPPING, "\n"
        .BITS 4 MAPPING
        .IF \1 == 0
            .ENDBITS
        .ENDIF
        .ENDM
        
        .DB "10>"                           ; @BT TEST-10 10 START
        .FILTER MapInto4Bits3 "BAGED", 0    ; @BT 10 64 3F
        .DB "<10"                           ; @BT END
        
