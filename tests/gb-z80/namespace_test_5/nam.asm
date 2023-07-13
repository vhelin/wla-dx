.section "someSection" free keep
someLabel2
        ret
.ends

.macro "myMacro"
    call someLabel2
.endm
        
