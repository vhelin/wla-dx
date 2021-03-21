
.MEMORYMAP
  SLOTSIZE $8000
  DEFAULTSLOT 0
  SLOT 0 START $0000 SIZE $8000 ; ROM
  SLOT 1 START $8000 SIZE $2000 ; RAM
.ENDME

.ROMBANKSIZE $8000
.ROMBANKS 8

        .STRUCT fm_channel_t
        dataPtr     dw  ; 0
        dataPos     dw  ; 2
        .ENDST

        .STRUCT fm_rhythm_channel_t
        INSTANCEOF fm_channel_t
        extra1  db
        extra2  db
        .ENDST

        .ENUM $0000
        chan1 INSTANCEOF fm_channel_t
        chan2 INSTANCEOF fm_channel_t
        chan3 INSTANCEOF fm_rhythm_channel_t
        .ENDE

        .BANK 0 SLOT 0
        .ORG $0000

        .db "01>"
        .db chan1.dataPtr, chan1.dataPos
        .db chan2.dataPtr, chan2.dataPos
        .db chan3.dataPtr, chan3.dataPos
        .db chan3.extra1, chan3.extra2
        .db "<01"

        .struct game_object
        posX db
        posY db
        .endst
        
        .RAMSECTION "vars 1" BANK 0 SLOT 1
        moomin1   DW
        phantom   DB
        nyanko    DB
        enemy1    INSTANCEOF game_object
                  INSTANCEOF game_object
        enemy2    INSTANCEOF game_object
        hello     DB
        .ENDS

        .db "02>"
        .dw moomin1, phantom, nyanko,
        .dw enemy1.posX, enemy1.posY
        .dw posX, posY
        .dw enemy2.posX, enemy2.posY
        .dw hello
        .db "<02"

        .STRUCT blob
        label1   dw
        label2   dw
        .ENDST

        .ENUM $1000
        blob1 INSTANCEOF blob
              INSTANCEOF blob
        blob2 INSTANCEOF blob
        .ENDE

        .db "03>"
        .dw blob1.label1, blob1.label2
        .dw label1, label2
        .dw blob2.label1, blob2.label2
        .db "<03"

        .STRUCT position_t
        pos_x  DW
        pos_y  DW
        .ENDST

        .STRUCT enemy_t
        id     DW
               INSTANCEOF position_t ; here we import fields from position_t
        health DW
        .ENDST

        .ENUM $A000 EXPORT
        nemesis INSTANCEOF enemy_t
        .ENDE
