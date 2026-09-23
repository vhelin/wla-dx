baz:
        .db 0, 1, 2, 3, 4, 5, 6, 7

bar:
        ld hl, baz
        ld b, _sizeof_baz
        ld c, $be
        otir
        ret

        ld a, _sizeof_gap
        .db _sizeof_baz+1

gap:
        .db $11, $22, $33

endmark:
        .db $ff
