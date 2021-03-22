
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
