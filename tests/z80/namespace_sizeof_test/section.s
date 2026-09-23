.section "blob" force org $200
qux:
        .db 1, 2, 3, 4
        .db _sizeof_qux
.ends

.section "blob2" force org $210
        .db _sizeof_zed
zed:
        .db 9, 8
.ends
