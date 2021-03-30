
        .memorymap
          slotsize $8000
          defaultslot 1
          slot 0 $0000
          slot 1 $8000
        .endme

        .rombanksize $8000
        .rombanks 3

        .bank 0 slot 0

        .section "Section1" force
        .db "01>"
        adc r3
        adc #15
        add r1
        .db "<01"

        .db "02>"
        alt3
        alt2
        alt1
        .db "<02"

        .db "03>"
        and r1
        and #2
        .db "<03"

        .db "04>"
        bcc label_b - 3
label_a:bcc label_a
        bcc label_b
        .db 1, 2, 3
label_b:.db "<04"

        .db "05>"
        cmode
        cache
        .db "<05"

        .db "06>"
        from r15
        ibt r7,#12h
        iwt r4, #$1234
        iwt r8, #label_b
        .db "<06"

        .db "07>"
        lea r10, label_a
        link #3
        .db "<07"

        .db "08>"
        lms r9, (88h)
        lms r1,($22+1+1)
        lms r0, (label_a)
        lms r14,(label_a+1*2)
        .db "<08"

        .db "09>"
        merge
        move r1, r15
        move r15,r1
        move r7,r8
        .db "<09"
        
        .db "10>"
        move r3, #127
        move r10, #-128
        move r8, #$1234
        move r2, #label_a
        move r11, #label_a | $1200
        .db "<10"

        .db "11>"
        move r3, (127)
        move r10, (128)
        move r8, ($1234)
        move r2, (label_a)
        move r11, (label_a | $1200)
        .db "<11"

        .db "12>"
        move ($A1),r10
        move ($A2), r11
        move (label_a),r12
        move (label_a | $1200), r13
        .db "<12"

        .db "13>"
        moveb r7, (r1)
        moveb r0,(r3)
        .db "<13"

        .db "14>"
        moveb (r11),r5
        moveb (r3), r0
        .db "<14"

        .db "15>"
        moves r3, r9
        movew r5, (r3)
        movew r0,(r6)
        .db "<15"

        .db "16>"
        movew (r10), r9
        movew (r6),r0
        .db "<16"

        .db "17>"
        mult r5
        mult #7
        nop
        not
        or r13
        .db "<17"

        .db "18>"
        rpix
        sbc r2
        sbk
        sex
        .db "<18"

        .db "19>"
        sm (88h), r9
        sm ($22+1+1),r1
        sm (label_a), r0
        sm (label_a+1*2), r14
        .db "<19"

        .db "20>"
        sms (88h), r9
        sms ($22+1+1),r1
        sms (label_a), r0
        sms (label_a+1*2),r14
        .db "<20"

        .db "21>"
        stop
        stw (r6)
        sub r15
        sub #1
        swap
        to r10
        .db "<21"

        .db "22>"
        umult r0
        umult r15
        .db "<22"
        .ends

