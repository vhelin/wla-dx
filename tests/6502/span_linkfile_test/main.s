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

        .SECTION "ForceLinkSpan" FREE
force_start_label:
        .DB :force_spanned_label, $F0, $F1, $F2
force_spanned_label:
        .DB :force_start_label, $F4, $F5, $F6
        .ENDS

        .SECTION "SemiSuperLinkSpan" FREE
semisuper_start_label:
        .DB :semisuper_start_label, $A0, $A1, $A2
semisuper_spanned_label:
        .DB :semisuper_spanned_label, $A4, $A5, $A6
        .ENDS

        .SECTION "SemiSubLinkSpan" FREE
semisub_start_label:
        .DB :semisub_start_label, $B0, $B1, $B2
semisub_spanned_label:
        .DB :semisub_spanned_label, $B4, $B5, $B6
        .ENDS

        .SECTION "SemiFreeLinkSpan" FREE
semifree_start_label:
        .DB :semifree_start_label, $C0, $C1, $C2
semifree_spanned_label:
        .DB :semifree_spanned_label, $C4, $C5, $C6
        .ENDS

        .SECTION "FreeLinkSpan" FREE
free_start_label:
        .DB :free_start_label, $D0, $D1, $D2
free_spanned_label:
        .DB :free_spanned_label, $D4, $D5, $D6
        .ENDS

        .SECTION "SuperFreeLinkSpan" FREE
superfree_start_label:
        .DB :superfree_start_label, $E0, $E1, $E2
superfree_spanned_label:
        .DB bank(superfree_spanned_label)+0, $E4, $E5, $E6
        .ENDS

        .SECTION "OverwriteLinkSpan" FREE
overwrite_start_label:
        .DB bank(overwrite_start_label), $90, $91, $92
overwrite_spanned_label:
        .DB :overwrite_spanned_label, $94, $95, $96
        .ENDS

        .SECTION "MultiBankLinkSpan" FREE
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

        .SECTION "SlotRangeLinkSpan" FREE
slot_range_start:
        .DB :slot_range_middle, $51
slot_range_middle:
        .DB :slot_range_middle, $52
        .ENDS