        .memorymap
          slotsize $8000
          defaultslot 0
          slot 0 $0000
        .endme

        .rombanksize $8000
        .rombanks 1

        .bank 0 slot 0
        .org 0

        .DEFINE CX4_CALC_BASE  $20
        .DEFINE CX4_CALC_STEP  3
        .DEFINE CX4_CALC_SCALE 2
        .DEFINE CX4_CALC_QBASE $120

        ; @BT result.rom

        .section "Cx4 tests" force

        .db "01>"                   ; @BT TEST-01 01 START
        nop                          ; @BT 00 00
        bra $24                      ; @BT 12 08
        bra.f $68                    ; @BT 34 0A
        beq $AC                      ; @BT 56 0C
        bsr.f $F0                    ; @BT 78 2A
        skipvc                       ; @BT 00 24
        skipeq                       ; @BT 01 26
        wait                         ; @BT 00 1C
        rts                          ; @BT 00 3C
        inc mar                      ; @BT 00 40
        clear                        ; @BT 00 F8
        halt                         ; @BT 00 FC
        .db "<01"                   ; @BT END

        .db "02>"                   ; @BT TEST-02 02 START
        mov a,r3                     ; @BT 63 60
        ld mar,$44                   ; @BT 44 66
        mov mdr,r5                   ; @BT 65 61
        mov p,r7                     ; @BT 07 63
        mov p,$89                    ; @BT 89 67
        mov pl,$12                   ; @BT 12 7C
        mov ph,$34                   ; @BT 34 7D
        mov r6,a                     ; @BT 66 E0
        mov r7,mdr                   ; @BT 67 E1
        .db "<02"                   ; @BT END

        .db "03>"                   ; @BT TEST-03 03 START
        cmp a,r1                     ; @BT 61 50
        cmpr a<<16,r2                ; @BT 62 4B
        add a<<1,r3                  ; @BT 63 81
        add a<<8,$7F                 ; @BT 7F 86
        subr a,r4                    ; @BT 64 88
        sub a<<16,$20                ; @BT 20 97
        mul r5                       ; @BT 65 98
        xnor a,$10                   ; @BT 10 A4
        xor a<<1,r6                  ; @BT 66 A9
        and a<<8,$11                 ; @BT 11 B6
        or a<<16,r7                  ; @BT 67 BB
        .db "<03"                   ; @BT END

        .db "04>"                   ; @BT TEST-04 04 START
        shlr r8                      ; @BT 68 C0
        shlr 3                       ; @BT 03 C4
        shar r9                      ; @BT 69 C8
        shar 4                       ; @BT 04 CC
        rotr r10                     ; @BT 6A D0
        rotr 5                       ; @BT 05 D4
        shll r11                     ; @BT 6B D8
        shll 6                       ; @BT 06 DC
        exts a                       ; @BT 00 59
        extw a                       ; @BT 00 5A
        swap a,r12                   ; @BT 0C F0
        .db "<04"                   ; @BT END

        .db "05>"                   ; @BT TEST-05 05 START
        rdram 0,a                    ; @BT 00 68
        rdram 2,$22                  ; @BT 22 6E
        rdrom a                      ; @BT 00 70
        rdrom $123                   ; @BT 23 75
        wrram 1,a                    ; @BT 00 E9
        wrram 2,$44                  ; @BT 44 EE
        .db "<05"                   ; @BT END

        .db "06>"                   ; @BT TEST-06 06 START
        jmp $134                     ; @BT 9A 08
        jsr.f $178                   ; @BT BC 2A
        shr a,r1                     ; @BT 61 C0
        asr a,2                      ; @BT 02 CC
        shl a,3                      ; @BT 03 DC
        sxb a                        ; @BT 00 59
        sxw a                        ; @BT 00 5A
        st r0,a                      ; @BT 60 E0
        st r1,mdr                    ; @BT 61 E1
        .db "<06"                   ; @BT END

        .db "07>"                   ; @BT TEST-07 07 START
        bra (CX4_CALC_BASE-(CX4_CALC_STEP+2))*2          ; @BT 1B 08
        mov p,CX4_CALC_BASE+CX4_CALC_STEP                ; @BT 23 67
        mov pl,(CX4_CALC_BASE+CX4_CALC_STEP)*CX4_CALC_SCALE ; @BT 46 7C
        add a<<8,CX4_CALC_BASE+5                         ; @BT 25 86
        rdrom CX4_CALC_QBASE+CX4_CALC_STEP               ; @BT 23 75
        .db "<07"                   ; @BT END

        .ends