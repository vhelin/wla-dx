.struct Point
        x db
        y db
.endst

.ramsection "vars" bank 0 slot 1 returnorg keep
        player db
        hp dw
.ends

        .db _sizeof_Point
        .db _sizeof_player
        .db _sizeof_hp

        .db _sizeof_ahead

.ramsection "vars2" bank 0 slot 1 returnorg keep
        ahead db
.ends
