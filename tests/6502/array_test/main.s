
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

        // map!
        .DB "01>"
        .ARRAYOUT NAME MyArray INDEX '6' DEFINITION MAPPING
        .DB MAPPING
        .ARRAYOUT NAME MyArray INDEX '6' DEFINITION MAPPING
        .DB MAPPING
        .ARRAYOUT NAME MyArray INDEX '8' DEFINITION MAPPING
        .DB MAPPING
        .ARRAYOUT MyArray '2' MAPPING
        .DB MAPPING
        .ARRAYOUT MyArray '7' MAPPING
        .DB MAPPING
        .ARRAYOUT MyArray '5' MAPPING
        .DB MAPPING
        .DB "<01"

        .ARRAYIN MyArray 'A' $ccddeeff
        .ARRAYIN MyArray 'B' ($00110011 | $ff00ff00)

        .DB "02>"
        .ARRAYOUT MyArray 'A' VALUE_A
        .ARRAYOUT MyArray 'B' VALUE_B
        .DD VALUE_A, VALUE_B
        .DB "<02"

        .DB "03>"
        .ARRAYDB NAME MyArray INDICES '6', '6', '8', '2', '7', '5'
        .DB "<03"

        .ARRAYIN MyArray 'C' $1234
        .ARRAYIN MyArray 'D' $abcd
        .ARRAYIN MyArray 0 $5678
        
        .DB "04>"
        .ARRAYDW NAME MyArray INDICES 'C', 'D', 0
        .DB "<04"

        .DB "05>"
        .ARRAYDB NAME MyArray INDICES "668275"
        .DB "<05"
        
        .ARRAYIN MyArray 1 $123456
        .ARRAYIN MyArray 2 ($111111 | $222222)

        .DB "06>"
        .ARRAYDL NAME MyArray INDICES 0+1+2+3-5, 1+1+1+1-1-1
        .DB "<06"
        
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

        .DB "07>"
        .ARRAYDD NAME MyArray INDICES "C128", 0
        .DB "<07"

        // define an array for mapping ASCII values into
        // less bits
        .ARRAYDEFINE NAME MapArray SIZE 4
        
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
        
        .DB "08>"
        .FILTER MapInto4Bits "BAGED", 0
        .DB "<08"
        
