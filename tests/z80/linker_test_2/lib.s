
        .DEFINE VALUE $332211

        .SECTION "LibrarySection" FREE
DoMaths:
        LD A, (VALUE & $FF)
        LD A, ((VALUE & $FF00) >> 8)
        LD A, ((VALUE & $FF0000) >> 16)

        .ENDS

        
