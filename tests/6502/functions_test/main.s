
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO


        .BANK 0 SLOT 0
        .ORG 0
        
        .DEFINE BIG_VALUE = $ABCD
        
        .DB "01>"
addr_01:.DB hibyte($1234), hibyte($FF00+$EE), hibyte(BIG_VALUE), hibyte(BIG_VALUE+$11), hibyte(addr_01), hibyte(addr_01+1)
        .DB hibyte($A000) + hibyte($0102)
        .DB "<01"

        .DB "02>"
addr_02:.DB lobyte($1234), lobyte($FF00+$EE), lobyte(BIG_VALUE), lobyte(BIG_VALUE+$11), lobyte(addr_02), lobyte(addr_02+1)
        .DB lobyte($A001) + lobyte($0102)
        .DB "<02"

        .BANK 1 SLOT 0
        .ORG 0

        .DB "03>"
addr_03:LDX loword(addr_03).w
        LDA #bank(addr_03).b
        .DB "<03"
        
        .BANK 0 SLOT 0
        .ORG $40

        .DEFINE BIGGEST_VALUE = $ABCDEF01
        
        .DB "04>"
addr_04:.DW hiword($12345678), hiword($FF000000+$EE0000), hiword(BIGGEST_VALUE), hiword(BIGGEST_VALUE+$110000)
        .DW hiword(addr_04), hiword(addr_04+$01234567)
        .DW hiword($A0110000) + hiword($01020304)
        .DB "<04"

        .DB "05>"
addr_05:.DW loword($12345678), loword($FF000201+$EE1111), loword(BIGGEST_VALUE), loword(BIGGEST_VALUE+$111234)
        .DW loword(addr_04), loword(addr_04+$01234567)
        .DW loword($A0110102) + loword($01020304)
        .DB "<05"
        
        .DB "06>"
addr_06:LDA #bank(addr_03) + 1 - 1
        LDA #bank(addr_06) + 1
        .DB 1 + BANK(addr_03) + 1
        .DB bank(addr_03) + (bank(addr_03) * 2) + BANK(addr_02)
        .DB "<06"

        .DB "07>"
        .DB BANK($2001), bank($2000-1), bank($1FFF + 1)
        .DB "<07"

        .DB "08>"
        .DB bankbyte($12345678), 1 + bankbyte($12345678 + 1)
        .DB "<08"
        
        .DB "09>"
        .DB bank(addr_03), bankbyte(addr_03 + 100)
        .DB "<09"
        
        .BANK 15 SLOT 0
        .ORG 0

        .DB "10>"
addr_10:.DB bank(addr_10), bankbyte(addr_10)
        .DB "<10"

        
