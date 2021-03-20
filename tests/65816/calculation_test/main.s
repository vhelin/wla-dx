
        .memorymap
          slotsize $8000
          defaultslot 1
          slot 0 $0000
          slot 1 $8000
        .endme

        .lorom

        .rombanksize $8000
        .rombanks 3

        .bank 0 slot 0
        .org 0
        
        .section "Section1" force
        .DB "01>"
        AND #<$1020
        AND <$1020,X
        AND #>$1020
        AND >$1020,X
        AND #($1020 & $FF)
        AND ($1020 & $FF),X
        AND #($1020 # $100)
        AND ($1020 # $100),X
        .DB "<01"
        .ends

        .org $100

        .section "Section2" force
        .DB "02>"
        AND #<($1020 + 1)
        AND <($1020 + 1),X
        AND #>($1020 + 1)
        AND >($1020 + 1),X
        AND #(($1020 + 1) & $FF)
        AND (($1020 + 1) & $FF),X
        AND #(($1020 + 1) # $100)
        AND (($1020 + 1) # $100),X
        .DB "<02"
        .ends
        
        .org $200
        
        .section "Section3" force
        .DB "03>"
        AND #<label_205
label_205:
        AND <label_205,X
        AND #>label_205
        AND >label_205,X
        AND #(label_205 & $FF)
        AND (label_205 & $FF),X
        AND #(label_205 # $100)
        AND (label_205 # $100),X
        .DB "<03"
        .ends
        
label_216:
        .db "04>"
        .dw label_216, label_205, label_216+1, label_205+1
        .db "<04"

        .section "Section4" force
label_224:
        .db "05>"
        .db 2 ^ 3, 1 | 2, 1 << 1, 2 >> 1, ~1
        .db "<05"
        .ends

        .db "06>"
        .db (label_216 & 15) ^ 2, (label_216 & 15) | 1, (label_216 & 15) << 1, (label_216 & 15) >> 1, ~(label_216 & 15)
        .db "<06"
        
