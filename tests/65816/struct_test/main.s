
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

        ; @BT result.rom

        .db "01>"                           ; @BT TEST-01 01 START
        .db chan1.dataPtr, chan1.dataPos    ; @BT 00 02
        .db chan2.dataPtr, chan2.dataPos    ; @BT 04 06
        .db chan3.dataPtr, chan3.dataPos    ; @BT 08 0A
        .db chan3.extra1, chan3.extra2      ; @BT 0C 0D
        .db "<01"                           ; @BT END

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

        .db "02>"                           ; @BT TEST-02 02 START
        .dw moomin1, phantom, nyanko,       ; @BT 00 80 02 80 03 80
        .dw enemy1.posX, enemy1.posY        ; @BT 04 80 05 80
        .dw posX, posY                      ; @BT 06 80 07 80
        .dw enemy2.posX, enemy2.posY        ; @BT 08 80 09 80
        .dw hello                           ; @BT 0A 80
        .db "<02"                           ; @BT END

        .STRUCT blob
        label1   dw
        label2   dw
        .ENDST

        .ENUM $1000
        blob1 INSTANCEOF blob
              INSTANCEOF blob
        blob2 INSTANCEOF blob
        .ENDE

        .db "03>"                           ; @BT TEST-03 03 START
        .dw blob1.label1, blob1.label2      ; @BT 00 10 02 10
        .dw label1, label2                  ; @BT 04 10 06 10
        .dw blob2.label1, blob2.label2      ; @BT 08 10 0A 10
        .db "<03"                           ; @BT END

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

        .STRUCT teststruct SIZE 8
        testbyte1       db ;dsb $8000
        testbyte2       db ;dsb $8000
        .ENDST

        .STRUCT testnest
            nest1 INSTANCEOF teststruct
        .ENDST

        .ENUM 0 EXPORT
            test1 INSTANCEOF teststruct
            test2 INSTANCEOF teststruct
            test3 INSTANCEOF teststruct
            test4 INSTANCEOF teststruct
        .ENDE

        .ramsection "RAMSection" bank 0 slot 0
            ram1 instanceof teststruct
            ram2 instanceof teststruct
            ram3 instanceof teststruct
            ram4 instanceof teststruct
        .ends

        .STRUCT a1 SIZE 8
            testbyte1   db
        .ENDST

        .STRUCT a2 SIZE 8
            testbyte2   db
        .ENDST

        .STRUCT both
            INSTANCEOF a1    ; name1 INSTANCEOF a1
            INSTANCEOF a2    ; name2 INSTANCEOF a2
        .ENDST

        .ENUM 0 export
            test    INSTANCEOF both
        .ENDE
