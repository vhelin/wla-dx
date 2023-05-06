
        .section "someSection" free
someLabel:
someLabel2:
        ret
        .ends

        .macro "someMacro"
        call someLabel2
        call someLabel2+2
        .endm
