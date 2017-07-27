; SNES memory map
; Based on the version by Tobias Pflug
;
; Supported versions (set MEMORYMAP_VERSION): 1
; Required definitions:
; - ROM_TYPE ("LOROM" or "HIROM")
;    - Or define LOROM or HIROM directly
; - KBIT (Rom size in KBit, power of 2, starting with 64)
;    - Or MBIT (Power of 2: 1, 2, 4, 8, 16, 32, ...)
;
; Example usage:
;     .DEFINE ROM_TYPE "HIROM"
;     .DEFINE MBIT 1
;
;     .INCLUDE "SNES.i"
;
;     .SNESNATIVEVECTOR
;         NMI    vblank
;     .ENDNATIVEVECTOR
;     .SNESEMUVECTOR
;         RESET  reset
;     .ENDEMUVECTOR

;     .BANK 0 SLOT 0
;     .ORG $0000
;
;     reset:
;         CLC ; Clear Carry to
;         XCE ; leave emulation mode
;     vblank: ; ...


.IFNDEF KBIT
    .IFNDEF MBIT
        .IFNDEF BANK_COUNT
            .PRINTT "\nEither define KBIT, MBIT or BANK_COUNT!\n"
            .FAIL
        .ELSE
            .IF BANK_COUNT == 0
                .PRINTT "\nBANK_COUNT can not be 0!\n"
                .FAIL
            .ENDIF
            .DEFINE KBIT BANK_COUNT * 1
        .ENDIF
    .ELSE
        .IF MBIT == 0
            .PRINTT "\nMBIT can not be 0!\n"
            .FAIL
        .ENDIF
        .DEFINE KBIT MBIT * 1024
    .ENDIF
.ENDIF
.IF KBIT == 0
    .PRINTT "\nKBIT can not be 0!\n"
    .FAIL
.ENDIF
.IF KBIT < 16
    .PRINTT "\nKBIT needs to be at least 16 or higher!\n"
    .FAIL
.ENDIF
; Check if power of two (SNES_ROM_SIZE is abused as a temp variable)
.DEFINE SNES_ROM_SIZE KBIT - 1
.REDEFINE SNES_ROM_SIZE KBIT & SNES_ROM_SIZE
.IF SNES_ROM_SIZE != 0
    .PRINTT "KBIT (or MBIT) needs to be in power of 2!\n"
    .FAIL
.ENDIF

.REDEFINE SNES_ROM_SIZE 0
.DEFINE TEMP 0
.REPEAT KBIT INDEX I ; Some crazy way to find the log2 of a number
    .IF TEMP == I
        ; TEMP = (TEMP + 1) * 2 - 1;
        .REDEFINE TEMP TEMP + 1
        .REDEFINE TEMP TEMP * 2
        .REDEFINE TEMP TEMP - 1
        .REDEFINE SNES_ROM_SIZE SNES_ROM_SIZE + 1
    .ENDIF
.ENDR
.REDEFINE SNES_ROM_SIZE SNES_ROM_SIZE - 4 ; Fix to the SNES value
.UNDEF TEMP
.SNESHEADER
    ROMSIZE SNES_ROM_SIZE
.ENDSNES

.IFNDEF ROM_TYPE
    .IFDEF LOROM
        .DEFINE ROM_TYPE "LOROM"
    .ENDIF
    .IFDEF HIROM
        .DEFINE ROM_TYPE "HIROM"
    .ENDIF
.ENDIF
.IFNDEF ROM_TYPE
    .PRINTT "\nROM_TYPE has to be defined: \"LOROM\" or \"HIROM\"\n"
    .PRINTT "Or just define LOROM or HIROM\n"
    .FAIL
.ENDIF

.IF ROM_TYPE == "LOROM"
    .HIROM
    .MEMORYMAP
        SLOTSIZE $8000
        DEFAULTSLOT 0
        SLOT 0 START $8000
    .ENDME
    .ROMBANKSIZE $8000
    .IFNDEF BANK_COUNT
        .DEFINE BANK_COUNT KBIT * 4 / 1024
    .ENDIF
.ENDIF
.IF ROM_TYPE == "HIROM"
    .HIROM
    .MEMORYMAP
        SLOTSIZE $10000
        DEFAULTSLOT 0
        SLOT 0 START $0000
    .ENDME
    .ROMBANKSIZE $10000
    .IFNDEF BANK_COUNT
        .DEFINE BANK_COUNT KBIT * 2 / 1024
    .ENDIF
.ENDIF
.ROMBANKS BANK_COUNT
