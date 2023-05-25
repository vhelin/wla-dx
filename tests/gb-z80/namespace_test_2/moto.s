        .section "barSection" free keep
barLabel:
        ld a, helo.VALUE
        .print helo.VALUE, "\n"
        ret
        .ends
        
