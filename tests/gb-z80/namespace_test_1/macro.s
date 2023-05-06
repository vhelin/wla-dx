
        .section "someSection" free
someLabel:
someLabel2:
        ret
        .ends

        .macro "someMacro"
        call someLabel2
        .endm

        .macro IncrementHELLOMOTO
        .redefine HELLOMOTO \1+1
        .endm
        
