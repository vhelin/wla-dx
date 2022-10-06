
        .memorymap
          slotsize $8000
          defaultslot 1
          slot 0 $0000
          slot 1 $8000
        .endme

        .rombanksize $8000
        .rombanks 3

        .bank 0 slot 0

        ; @BT result.rom

        .section "Section1" force
        .db "01>"                   ; @BT TEST-01 01 START
        adc r3                      ; @BT 3D 53
        adc #15                     ; @BT 3F 5F
        add r1                      ; @BT 51
        .db "<01"                   ; @BT END

        .db "02>"                   ; @BT TEST-02 02 START
        alt3                        ; @BT 3F
        alt2                        ; @BT 3E
        alt1                        ; @BT 3D
        .db "<02"                   ; @BT END

        .db "03>"                   ; @BT TEST-03 03 START
        and r1                      ; @BT 71
        and #2                      ; @BT 3E 72
        .db "<03"                   ; @BT END

        .db "04>"                   ; @BT TEST-04 04 START
        bcc label_b - 3             ; @BT 0C 04
label_a:bcc label_a                 ; @BT 0C FE
        bcc label_b                 ; @BT 0C 03
        .db 1, 2, 3                 ; @BT 01 02 03
label_b:.db "<04"                   ; @BT END

        .db "05>"                   ; @BT TEST-05 05 START
        cmode                       ; @BT 3D 4E
        cache                       ; @BT 02
        .db "<05"                   ; @BT END

        .db "06>"                   ; @BT TEST-06 06 START
        from r15                    ; @BT BF
        ibt r7,#12h                 ; @BT A7 12
        iwt r4, #$1234              ; @BT F4 34 12
        iwt r8, #label_b            ; @BT F8 29 00
        .db "<06"                   ; @BT END

        .db "07>"                   ; @BT TEST-07 07 START
        lea r10, label_a            ; @BT FA 22 00
        link #3                     ; @BT 93
        .db "<07"                   ; @BT END

        .db "08>"                   ; @BT TEST-08 08 START
        lms r9, (88h)               ; @BT 3D A9 44 
        lms r1,($22+1+1)            ; @BT 3D A1 12
        lms r0, (label_a)           ; @BT 3D A0 11
        lms r14,(label_a+1*2)       ; @BT 3D AE 12
        .db "<08"                   ; @BT END

        .db "09>"                   ; @BT TEST-09 09 START
        merge                       ; @BT 70
        move r1, r15                ; @BT 2F 11
        move r15,r1                 ; @BT 21 1F
        move r7,r8                  ; @BT 28 17
        .db "<09"                   ; @BT END
        
        .db "10>"                   ; @BT TEST-10 10 START
        move r3, #127               ; @BT A3 7F
        move r10, #-128             ; @BT AA 80
        move r8, #$1234             ; @BT F8 34 12
        move r2, #label_a           ; @BT F2 22 00
        move r11, #label_a | $1200  ; @BT FB 22 12
        .db "<10"                   ; @BT END

        .db "11>"                   ; @BT TEST-11 11 START
        move r3, (127)              ; @BT 3D F3 7F 00
        move r10, (128)             ; @BT 3D AA 40
        move r8, ($1234)            ; @BT 3D F8 34 12
        move r2, (label_a)          ; @BT 3D F2 22 00
        move r11, (label_a | $1200) ; @BT 3D FB 22 12  
        .db "<11"                   ; @BT END

        .db "12>"                   ; @BT TEST-12 12 START
        .print "12>\n"              
        move ($A1),r10              ; @BT 3E FA A1 00
                                    ; sm ($A1), r10

        move ($A2), r11             ; @BT 3E AB 51
                                    ; sms ($A2), r11
                                    
        move (label_a),r12          ; @BT 3E FC 22 00
                                    ; sm ($22), r12
                                    ; NOTE: as WLA doesn't know that label_a is $22 it bails out to sm here...
                                    ; this is not what SNES Development Manual says, but then again WLA DX
                                    ; is not an official Nintendo/SNES assmebler...

        move (label_a | $1200), r13 ; @BT 3E FD 22 12
                                    ; sm ($1222), r13
        .print "<12\n"
        .db "<12"                   ; @BT END

        .db "13>"                   ; @BT TEST-13 13 START
        moveb r7, (r1)              ; @BT 17 3D 41
        moveb r0,(r3)               ; @BT 3D 43
        .db "<13"                   ; @BT END

        .db "14>"                   ; @BT TEST-14 14 START
        moveb (r11),r5              ; @BT B5 3D 3B
        moveb (r3), r0              ; @BT 3D 33
        .db "<14"                   ; @BT END

        .db "15>"                   ; @BT TEST-15 15 START
        moves r3, r9                ; @BT 23 B9
        movew r5, (r3)              ; @BT 15 43
        movew r0,(r6)               ; @BT 46
        .db "<15"                   ; @BT END

        .db "16>"                   ; @BT TEST-16 16 START
        movew (r10), r9             ; @BT B9 3A
        movew (r6),r0               ; @BT 36
        .db "<16"                   ; @BT END

        .db "17>"                   ; @BT TEST-17 17 START
        mult r5                     ; @BT 85
        mult #7                     ; @BT 3E 87
        nop                         ; @BT 01
        not                         ; @BT 4F
        or r13                      ; @BT CD
        .db "<17"                   ; @BT END

        .db "18>"                   ; @BT TEST-18 18 START
        rpix                        ; @BT 3D 4C
        sbc r2                      ; @BT 3D 62
        sbk                         ; @BT 90
        sex                         ; @BT 95
        .db "<18"                   ; @BT END

        .db "19>"                   ; @BT TEST-19 19 START
        sm (88h), r9                ; @BT 3E F9 88 00
        sm ($22+1+1),r1             ; @BT 3E F1 24 00
        sm (label_a), r0            ; @BT 3E F0 22 00
        sm (label_a+1*2), r14       ; @BT 3E FE 24 00
        .db "<19"                   ; @BT END

        .db "20>"                   ; @BT TEST-20 20 START
        sms (88h), r9               ; @BT 3E A9 44
        sms ($22+1+1),r1            ; @BT 3E A1 12
        sms (label_a), r0           ; @BT 3E A0 11
        sms (label_a+1*2),r14       ; @BT 3E AE 12
        .db "<20"                   ; @BT END

        .db "21>"                   ; @BT TEST-21 21 START
        stop                        ; @BT 00
        stw (r6)                    ; @BT 36
        sub r15                     ; @BT 6F
        sub #1                      ; @BT 3E 61
        swap                        ; @BT 4D
        to r10                      ; @BT 1A
        .db "<21"                   ; @BT END

        .db "22>"                   ; @BT TEST-22 22 START
        umult r0                    ; @BT 3D 80
        umult r15                   ; @BT 3D 8F
        .db "<22"                   ; @BT END
        .ends

        .db "23>"                   ; @BT TEST-23 23 START
        lms r9, (VALUE_88H)         ; @BT 3D A9 44
        lms r9, (VALUE_88H+1-1)     ; @BT 3D A9 44
        lms r1,(VALUE_22H)          ; @BT 3D A1 11
        lms r1,(VALUE_22H-1+1)      ; @BT 3D A1 11
        .db "<23"                   ; @BT END

        .define VALUE_22H 11h+11h
        .define VALUE_44H VALUE_22H*2
        .define VALUE_88H VALUE_44H*2
        
