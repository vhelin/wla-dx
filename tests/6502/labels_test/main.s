
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0

        ; @BT linked.rom
        
        .SECTION "Section01" FORCE
        .DB "01>"       ; @BT TEST-01 01 START
        jmp +           ; @BT 4C 0C 00
--      beq +           ; @BT F0 04
        nop             ; @BT EA
-       beq +           ; @BT F0 01
        nop             ; @BT EA
+       beq _f          ; @BT F0 02
        nop             ; @BT EA
        nop             ; @BT EA
__      nop             ; @BT EA
        beq _b          ; @BT F0 FD
        beq --          ; @BT F0 F1
        beq -           ; @BT F0 F2
        jmp -           ; @BT 4C 09 00
        beq _b + 1      ; @BT F0 F5
        .DB "<01"       ; @BT END
        .ENDS

        .SECTION "Section02" FORCE
        .DB "02>"               ; @BT TEST-02 02 START
label_01:
@child: nop                     ; @BT EA
        beq __label__           ; @BT F0 FD
label_02:
        jmp __label__ + 1       ; @BT 4C 26 00
        .DB "<02"               ; @BT END
        .ENDS
        
