
;
; ZX81 Memory Map (1KB, 16KB and 56KB RAM)
;

.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 START $0000 SIZE $2000 NAME "ROM"      ; 8KB Basic ROM
SLOT 1 START $4000 SIZE  $400 NAME "RAM-1KB"  ; If the system has  1KB RAM
SLOT 2 START $4000 SIZE $4000 NAME "RAM-16KB" ; If the system has 16KB RAM
SLOT 3 START $2000 SIZE $E000 NAME "RAM-56KB" ; If the system has 56KB RAM
.ENDME

;
; Let's target a ZX81 with 1KB RAM (we use .ROMBANKS to reserve space for the code
; and this data will be loaded into RAM starting from $4009)
;
	
.ROMBANKSIZE $400
.ROMBANKS       1
.EMPTYFILL      0

;
; The .P file starts from $4009
;

.SLOT 1
.ORGA $4009

;
; Found this code here: https://www.sinclairzxworld.com/viewtopic.php?t=438 (by Zac)
; More information about this can be found here: https://sebastien.andrivet.com/fr/posts/my-first-computer-sinclair-zx81/
;

;
; System variables
;

VERSN:          .db 0
E_PPC:          .dw 2
D_FILE:         .dw Display
DF_CC:          .dw Display+1              ; First character of display
VARS:           .dw Variables
DEST:           .dw 0
E_LINE:         .dw BasicEnd 
CH_ADD:         .dw BasicEnd+4             ; Simulate SAVE "X"
X_PTR:          .dw 0
STKBOT:         .dw BasicEnd+5
STKEND:         .dw BasicEnd+5             ; Empty stack
BREG:           .db 0
MEM:            .dw MEMBOT
UNUSED1:        .db 0
DF_SZ:          .db 2
S_TOP:          .dw $0002                  ; Top program line number
LAST_K:         .dw $FDBF
DEBOUN:         .db 15
MARGIN:         .db 55
NXTLIN:         .dw Line2                  ; Next line address
OLDPPC:         .dw 0
FLAGX:          .db 0
STRLEN:         .dw 0
T_ADDR:         .dw $0C8D
SEED:           .dw 0
FRAMES:         .dw $F5A3
COORDS:         .dw 0
PR_CC:          .db $BC
S_POSN:         .dw $1821
CDFLAG:         .db $40
PRBUFF:         .db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,$76 ; 32 Spaces + Newline
MEMBOT:         .db 0,0,0,0,0,0,0,0,0,0,$84,$20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0     ; 30 zeros
UNUNSED2:       .dw 0

;
; End of system variables
;
	
Program:        
Line1:          .db $00,$01                ; Line 1
                .dw Line1End-Line1Text     ; Line 1 length
Line1Text:      .db $EA                    ; REM             
   
; ===========================================================
; <ASM> (starts at $4082)
; ===========================================================

      LD A,1      ; start with 1
loop: RST $10     ; PRINT 
      INC A       ; increment A
      CP 36       ; stop at 36
      JP NZ,loop  ; if not 36, then goto/jump to LOOP
      RET         ; Return to BASIC

; ===========================================================
; </ASM>
; ===========================================================

            .db $76                        ; Newline (ASM ends to a newline)
Line1End:       
Line2:      .db $00,$02                    ; Line 2
            .dw Line2End-Line2Text
Line2Text:  .db $F9,$D4                    ; RAND USR
            .db $1D,$22,$21,$1D,$20        ; 16514
            .db $7E                        ; Number
            .db $8F,$01,$04,$00,$00        ; Numeric encoding
            .db $76                        ; Newline
Line2End: 

Display:    .db $76                        ; Newline
            .db $76 ; Line 0
            .db $76 ; Line 1
            .db $76 ; Line 2
            .db $76 ; Line 3
            .db $76 ; Line 4
            .db $76 ; Line 5
            .db $76 ; Line 6
            .db $76 ; Line 7
            .db $76 ; Line 8
            .db $76 ; Line 9 
            .db $76 ; Line 10
            .db $76 ; Line 11
            .db $76 ; Line 12
            .db $76 ; Line 13
            .db $76 ; Line 14
            .db $76 ; Line 15
            .db $76 ; Line 16
            .db $76 ; Line 17
            .db $76 ; Line 18
            .db $76 ; Line 19
            .db $76 ; Line 20
            .db $76 ; Line 21
            .db $76 ; Line 22
            .db $76 ; Line 23

;
; Variables area (empty)
;

Variables:      
VariablesEnd:   .db $80
BasicEnd:                              
