        .MEMORYMAP
        DEFAULTSLOT 0
        SLOTSIZE $20
        SLOT 0 $8000
        SLOT 1 $8020
        SLOT 2 $8040
        .ENDME

        .ROMBANKMAP
        BANKSTOTAL 20
        BANKSIZE $20
        BANKS 20
        .ENDRO

        .EMPTYFILL $00

        .BANK 7 SLOT 0
        .ORG $0000

        .SECTION "Refs" FORCE
        .DW force_spanned_label
        .DB :force_spanned_label
        .DW semisuper_spanned_label
        .DB :semisuper_spanned_label
        .DW semisub_spanned_label
        .DB :semisub_spanned_label
        .DW semifree_spanned_label
        .DB :semifree_spanned_label
        .DW free_spanned_label
        .DB :free_spanned_label
        .DW superfree_spanned_label
        .DB :superfree_spanned_label
        .DW overwrite_spanned_label
        .DB :overwrite_spanned_label
        .DW multi_span_middle
        .DB :multi_span_middle
        .DW slot_range_middle
        .DB :slot_range_middle
        .ENDS

        .BANK 0 SLOT 0
        .ORG $1C

        .SECTION "ForceSpan" FORCE SPAN 0:0/8:1
force_start_label:
        .DB :force_spanned_label, $F0, $F1, $F2
force_spanned_label:
        .DB :force_start_label, $F4, $F5, $F6
        .ENDS

        .BANK 1 SLOT 0
        .ORG $1C

        .SECTION "SemiSuperSpan" SEMISUPERFREE BANKS 1 WINDOW $801C $8024 SPAN 1:0/9:1
semisuper_start_label:
        .DB :semisuper_start_label, $A0, $A1, $A2
semisuper_spanned_label:
        .DB :semisuper_spanned_label, $A4, $A5, $A6
        .ENDS

        .BANK 2 SLOT 0
        .ORG $1F

        .SECTION "SemiSubSpan" SEMISUBFREE WINDOW $801C $8024 SPAN 2:0/10:1
semisub_start_label:
        .DB :semisub_start_label, $B0, $B1, $B2
semisub_spanned_label:
        .DB :semisub_spanned_label, $B4, $B5, $B6
        .ENDS

        .BANK 3 SLOT 0
        .ORG $1C

        .SECTION "SemiFreeSpan" SEMIFREE SPAN 3:0/11:1
semifree_start_label:
        .DB :semifree_start_label, $C0, $C1, $C2
semifree_spanned_label:
        .DB :semifree_spanned_label, $C4, $C5, $C6
        .ENDS

        .BANK 4 SLOT 0
        .ORG $1C

        .SECTION "FreeSpan" FREE WINDOW $801C $8024 SPAN 4:0/12:1
free_start_label:
        .DB :free_start_label, $D0, $D1, $D2
free_spanned_label:
        .DB :free_spanned_label, $D4, $D5, $D6
        .ENDS

        .BANK 5 SLOT 0
        .ORG $1C

        .SECTION "SuperFreeSpan" SUPERFREE WINDOW $801C $8024 SPAN 5:0/13:1
superfree_start_label:
        .DB :superfree_start_label, $E0, $E1, $E2
superfree_spanned_label:
        .DB bank(superfree_spanned_label)+0, $E4, $E5, $E6
        .ENDS

        .BANK 6 SLOT 0
        .ORG $1C

        .SECTION "OverwriteSpan" OVERWRITE SPAN 6:0/14:1
overwrite_start_label:
        .DB bank(overwrite_start_label), $90, $91, $92
overwrite_spanned_label:
        .DB :overwrite_spanned_label, $94, $95, $96
        .ENDS

        ; The inherited bank is ignored: SPAN's first bank is the start bank.
        .BANK 7 SLOT 0
        .ORG $1C

        .SECTION "MultiBankSpan" FORCE SPAN 15-17 SLOT 1
multi_span_start:
        .DB :multi_span_start, $71, $72, $73
multi_span_middle:
        .DB :multi_span_middle
        .DB $01, $02, $03, $04, $05, $06, $07, $08, $09, $0A
        .DB $0B, $0C, $0D, $0E, $0F, $10, $11, $12, $13, $14
        .DB $15, $16, $17, $18, $19, $1A, $1B, $1C, $1D, $1E, $1F
multi_span_end:
        .DB :multi_span_end, $E1, $E2, $E3
        .ENDS

        .SECTION "SlotRangeSpan" FORCE ORG $1E SPAN 18-19:0-1
slot_range_start:
        .DB :slot_range_middle, $51
slot_range_middle:
        .DB :slot_range_middle, $52
        .ENDS
