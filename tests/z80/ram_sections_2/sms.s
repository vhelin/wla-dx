
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example for testing .ramsection's size
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $4000
   SLOT            0       $0000
   SLOT            1       $4000
   SLOT            2       $8000
   ; RAM area
   SLOTSIZE        $2000
   SLOT            3       $C000
   SLOT            4       $E000
   SLOT            5       $FF01
.ENDME

.RAMSECTION "Variables_1" SLOT 3 SIZE 64 KEEP RETURNORG
var_1           dw
var_2           dw
var_3           dw
.ENDS

.RAMSECTION "Variables_2" SLOT 3 APPENDTO "Variables_1" RETURNORG
new_1           db
new_2           db
.ENDS
        
.EMPTYFILL $AA

.ROMBANKMAP
BANKSTOTAL 4
BANKSIZE $4000
BANKS 4
.ENDRO

        ;------------------------------------------------------------------------
        ; TEST-01
        ;------------------------------------------------------------------------
        
        .struct object
        size DB
        life DB
        .endst
        
        .struct object2 size 4
        size DB
        life DB
        .endst

        .struct object3
        type DB
        .union
          size DB
          life DB
        .nextu
          posX DB
          posY DB
        .nextu
          subtype DB
        .endu
        varA DB
        varB DB
        .endst

        .struct object4 size 8
        type DB
        .union
          size DB
          life DB
        .nextu
          posX DB
          posY DB
        .nextu
          subtype DB
        .endu
        varA DB
        varB DB
        .endst

        .ramsection "objects" bank 0 slot 4 force
        objects1  instanceof object 2
        objects2  instanceof object count 4 size 4
        size      DB
        objectA   instanceof object size 8
        startfrom DB
        objectB   instanceof object 1 size 2
        objects3  instanceof object 2
        count     DB
        objects4  instanceof object2 size 8 count 2
        objects5  instanceof object2 count 3
        objects6  instanceof object3 2
        objects7  instanceof object4 4
        objects8  instanceof object4 count 2 size 16
        .ends

                                ; @BT linked.rom
        .db "01>"               ; @BT TEST-01 01 START
        .db _sizeof_objects1    ; @BT 04
        .db _sizeof_objects1.1  ; @BT 02
        .db _sizeof_objects1.2  ; @BT 02
        .db _sizeof_objects2    ; @BT 10
        .db _sizeof_objects2.1  ; @BT 04
        .db _sizeof_objects2.2  ; @BT 04
        .db _sizeof_objects2.3  ; @BT 04
        .db _sizeof_objects2.4  ; @BT 04
        .db _sizeof_objectA     ; @BT 08
        .db _sizeof_objectB     ; @BT 02
        .db _sizeof_objects3    ; @BT 04
        .db _sizeof_objects3.1  ; @BT 02
        .db _sizeof_objects3.2  ; @BT 02
        .db _sizeof_objects4    ; @BT 10
        .db _sizeof_objects4.1  ; @BT 08
        .db _sizeof_objects4.2  ; @BT 08
        .db _sizeof_objects5    ; @BT 0C
        .db _sizeof_objects5.1  ; @BT 04
        .db _sizeof_objects5.2  ; @BT 04
        .db _sizeof_objects5.3  ; @BT 04
        .db _sizeof_objects6    ; @BT 0A
        .db _sizeof_objects6.1  ; @BT 05
        .db _sizeof_objects6.2  ; @BT 05
        .db _sizeof_objects7    ; @BT 20
        .db _sizeof_objects7.1  ; @BT 08
        .db _sizeof_objects7.2  ; @BT 08
        .db _sizeof_objects7.3  ; @BT 08
        .db _sizeof_objects7.4  ; @BT 08
        .db _sizeof_objects8    ; @BT 20
        .db _sizeof_objects8.1  ; @BT 10
        .db _sizeof_objects8.2  ; @BT 10
        .db _paddingof_objects2.1 ; @BT 02
        .db _paddingof_objects2.2 ; @BT 02
        .db _paddingof_objects2.3 ; @BT 02
        .db _paddingof_objects2.4 ; @BT 02
        .db _paddingof_objectA    ; @BT 06
        .db _paddingof_objects4.1 ; @BT 06
        .db _paddingof_objects4.2 ; @BT 06
        .db _paddingof_objects5.1 ; @BT 02
        .db _paddingof_objects5.2 ; @BT 02
        .db _paddingof_objects5.3 ; @BT 02
        .db _paddingof_objects8.1 ; @BT 0B
        .db _paddingof_objects8.2 ; @BT 0B
        .db "<01"               ; @BT END

        .export _paddingof_objects2.1
        .export _paddingof_objects2.2
        .export _paddingof_objects2.3
        .export _paddingof_objects2.4
        .export _paddingof_objectA
        .export _paddingof_objects4.1
        .export _paddingof_objects4.2
        .export _paddingof_objects5.1
        .export _paddingof_objects5.2
        .export _paddingof_objects5.3
        .export _paddingof_objects8.1
        .export _paddingof_objects8.2
        
        ;------------------------------------------------------------------------
        ; TEST-02
        ;------------------------------------------------------------------------

        .db "02>"               ; @BT TEST-02 02 START
        .dw objects1            ; @BT 00 E0
        .dw objects1.1          ; @BT 00 E0
        .dw objects1.2          ; @BT 02 E0
        .dw objects2            ; @BT 04 E0
        .dw objects2.1          ; @BT 04 E0
        .dw objects2.2          ; @BT 08 E0
        .dw objects2.3          ; @BT 0C E0
        .dw objects2.4          ; @BT 10 E0
        .dw size                ; @BT 14 E0
        .dw objectA             ; @BT 15 E0
        .dw startfrom           ; @BT 1D E0
        .dw objectB             ; @BT 1E E0
        .dw objects3            ; @BT 20 E0
        .dw objects3.1          ; @BT 20 E0
        .dw objects3.2          ; @BT 22 E0
        .dw count               ; @BT 24 E0
        .dw objects4            ; @BT 25 E0
        .dw objects4.1          ; @BT 25 E0
        .dw objects4.2          ; @BT 2D E0
        .dw objects5            ; @BT 35 E0
        .dw objects5.1          ; @BT 35 E0
        .dw objects5.2          ; @BT 39 E0
        .dw objects5.3          ; @BT 3D E0
        .dw objects6            ; @BT 41 E0
        .dw objects6.1          ; @BT 41 E0
        .dw objects6.2          ; @BT 46 E0
        .dw objects7            ; @BT 4B E0
        .dw objects7.1          ; @BT 4B E0
        .dw objects7.2          ; @BT 53 E0
        .dw objects7.3          ; @BT 5B E0
        .dw objects7.4          ; @BT 63 E0
        .dw objects8            ; @BT 6B E0
        .dw objects8.1          ; @BT 6B E0
        .dw objects8.2          ; @BT 7B E0        
        .db "<02"               ; @BT END

        ;------------------------------------------------------------------------
        ; TEST-03
        ;------------------------------------------------------------------------
        
        .enum $C000 export
        .union
helloA: db
helloB: db
helloC: dw
        .nextu
helloD: db
        .endu
helloE: db
        objectsX1 instanceof object size 8 count 2
        objectsX2 instanceof object 2 size 8 startfrom 0
        objectsX3 instanceof object 2
helloF: db
        objectsX4 instanceof object2
        objectsX5 instanceof object3 size 16
helloG: dw
        objectsX6 instanceof object4 count 2
helloH: db
        .ende
        
        .db "03>"               ; @BT TEST-03 03 START
        .dw helloA              ; @BT 00 C0
        .dw helloB              ; @BT 01 C0
        .dw helloC              ; @BT 02 C0
        .dw helloD              ; @BT 00 C0
        .dw helloE              ; @BT 04 C0
        .dw objectsX1           ; @BT 05 C0
        .dw objectsX1.1         ; @BT 05 C0
        .dw objectsX1.1.size    ; @BT 05 C0
        .dw objectsX1.1.life    ; @BT 06 C0
        .dw objectsX1.2         ; @BT 0D C0
        .dw objectsX1.2.size    ; @BT 0D C0
        .dw objectsX1.2.life    ; @BT 0E C0
        .dw objectsX2           ; @BT 15 C0
        .dw objectsX2.0         ; @BT 15 C0
        .dw objectsX2.0.size    ; @BT 15 C0
        .dw objectsX2.0.life    ; @BT 16 C0
        .dw objectsX2.1         ; @BT 1D C0
        .dw objectsX2.1.size    ; @BT 1D C0
        .dw objectsX2.1.life    ; @BT 1E C0
        .dw objectsX3           ; @BT 25 C0
        .dw objectsX3.1         ; @BT 25 C0
        .dw objectsX3.1.size    ; @BT 25 C0
        .dw objectsX3.1.life    ; @BT 26 C0
        .dw objectsX3.2         ; @BT 27 C0
        .dw objectsX3.2.size    ; @BT 27 C0
        .dw objectsX3.2.life    ; @BT 28 C0
        .dw helloF              ; @BT 29 C0
        .dw objectsX4           ; @BT 2A C0
        .dw objectsX4.size      ; @BT 2A C0
        .dw objectsX4.life      ; @BT 2B C0
        .dw objectsX5           ; @BT 2E C0
        .dw objectsX5.type      ; @BT 2E C0
        .dw objectsX5.size      ; @BT 2F C0
        .dw objectsX5.life      ; @BT 30 C0
        .dw objectsX5.posX      ; @BT 2F C0
        .dw objectsX5.posY      ; @BT 30 C0
        .dw objectsX5.subtype   ; @BT 2F C0
        .dw objectsX5.varA      ; @BT 31 C0
        .dw objectsX5.varB      ; @BT 32 C0
        .dw helloG              ; @BT 3E C0
        .dw objectsX6           ; @BT 40 C0
        .dw objectsX6.1         ; @BT 40 C0
        .dw objectsX6.1.type    ; @BT 40 C0
        .dw objectsX6.1.size    ; @BT 41 C0
        .dw objectsX6.1.life    ; @BT 42 C0
        .dw objectsX6.1.posX    ; @BT 41 C0
        .dw objectsX6.1.posY    ; @BT 42 C0
        .dw objectsX6.1.subtype ; @BT 41 C0
        .dw objectsX6.1.varA    ; @BT 43 C0
        .dw objectsX6.1.varB    ; @BT 44 C0
        .dw objectsX6.2         ; @BT 48 C0
        .dw objectsX6.2.type    ; @BT 48 C0
        .dw objectsX6.2.size    ; @BT 49 C0
        .dw objectsX6.2.life    ; @BT 4A C0
        .dw objectsX6.2.posX    ; @BT 49 C0
        .dw objectsX6.2.posY    ; @BT 4A C0
        .dw objectsX6.2.subtype ; @BT 49 C0
        .dw objectsX6.2.varA    ; @BT 4B C0
        .dw objectsX6.2.varB    ; @BT 4C C0
        .dw helloH              ; @BT 50 C0
        .db _paddingof_objectsX1.1 ; @BT 06
        .db _paddingof_objectsX1.2 ; @BT 06
        .db _paddingof_objectsX2.0 ; @BT 06
        .db _paddingof_objectsX2.1 ; @BT 06
        .db _paddingof_objectsX4   ; @BT 02
        .db _paddingof_objectsX5   ; @BT 0B
        .db _paddingof_objectsX6.1 ; @BT 03
        .db _paddingof_objectsX6.2 ; @BT 03
        .db "<03"               ; @BT END

        ;------------------------------------------------------------------------
        ; TEST-04
        ;------------------------------------------------------------------------

        .ramsection "AlignTest" bank 2 slot 4 align 8
        object_1 instanceof object
        // 2
        extrabyte_1 db
        // 3
        .align 4
        // 4
        offset_4 db
        // 5
        object_2 instanceof object3
        // 10
        .align 8
        // 16
        extrabyte_2 dw
        // 18
        .align 4
        // 20
        extrabyte_3 db
        // 21
        extrabyte_4 db
        // 22
        extrabyte_5 db
        // 23
        extrabyte_6 db
        // 24
        .align 4
        // 24
        extrabyte_7 db
        // 25
        .ends
        
        /*
        01:e000 SECTIONSTART_AlignTest
        01:e000 object_1
        01:e000 object_1.size
        01:e001 object_1.life
        01:e002 extrabyte_1
        01:e004 offset_4
        01:e005 object_2
        01:e005 object_2.type
        01:e006 object_2.size
        01:e007 object_2.life
        01:e006 object_2.posX
        01:e007 object_2.posY
        01:e006 object_2.subtype
        01:e008 object_2.varA
        01:e009 object_2.varB
        01:e010 extrabyte_2
        01:e014 extrabyte_3
        01:e015 extrabyte_4
        01:e016 extrabyte_5
        01:e017 extrabyte_6
        01:e018 extrabyte_7
        01:e018 SECTIONEND_AlignTest
        */

        .db "04>"               ; @BT TEST-04 04 START
        .dw extrabyte_1         ; @BT 02 E0
        .dw extrabyte_2         ; @BT 10 E0
        .dw extrabyte_7         ; @BT 18 E0
        .db "<04"               ; @BT END
        
        ;------------------------------------------------------------------------
        ; TEST-05
        ;------------------------------------------------------------------------

        .ramsection "AlignTest2" bank 2 slot 4 align 8
        morebyte_1 db
        // 1
        .align 8
        // 8
        morebyte_2 db
        // 9
        .ends

        /*
        02:e020 SECTIONSTART_AlignTest2
        02:e020 morebyte_1
        02:e028 morebyte_2
        02:e029 SECTIONEND_AlignTest2
        */
        
        .db "05>"               ; @BT TEST-05 05 START
        .dw morebyte_1          ; @BT 20 E0
        .dw morebyte_2          ; @BT 28 E0
        .db "<05"               ; @BT END
