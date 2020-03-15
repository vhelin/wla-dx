
;
; C64 memory map
;

.MEMORYMAP
        DEFAULTSLOT     2
        SLOTSIZE        $00FD
        SLOT 0          $0003   ; zero-page (variable space)
        SLOT 1          $0100   ; 6502-stack (variable space)
        SLOTSIZE        $FE00
        SLOT 2          $0200   ; main program RAM ("ROM")
.ENDME

;
; We'll use .ROMBANKS just to reserve space for the program, which in this
; case will later be loaded to RAM.
;

.ROMBANKSIZE            $FE00
.ROMBANKS               1

;
; Our program will start from $C000. Load this .PRG into an emulator,
; then type "SYS 49152" and it'll run.
;

.SLOT 2
.ORGA $C000

LoadAddress:

loop: inc $d020    ; Let's change the border color
      jmp loop

;
; We can mix code and variables as all this is loaded into RAM
;

var1: .db 0
var2: .db 0
var3: .db 0
