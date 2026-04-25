; ---------------------------------------------------------------------------
; cx4_prog.s  -  Cx4 microcode that computes  R2 = R0*32 + R1
;
; This is the word offset into a 32x32 SNES BG tilemap (VMADDR is a word
; address, so offset = row*32 + col).  A single SHL+ADD replaces the
; 65816's software shift/add chain (this is exactly the kind of thing
; Mega Man X2/X3 used the Cx4 for).
;
; Inputs  (written by the 65816 before kicking the Cx4):
;   R0 = row    (0..31)
;   R1 = column (0..31)
; Output (read by the 65816 after HALT):
;   R2  = row*32 + column         (tilemap word offset)
;   R15 = $AB                     (done sentinel)
; ---------------------------------------------------------------------------

.MEMORYMAP
    SLOTSIZE $10000
    DEFAULTSLOT 0
    SLOT 0 $0000
.ENDME

.ROMBANKSIZE $10000
.ROMBANKS 1

.BANK 0 SLOT 0
.ORG 0

.SECTION "Cx4MulRoutine" FORCE
    MOV  A,r0         ; A <- row
    SHL  A,5          ; A <- row * 32
    ADD  A,r1         ; A <- row*32 + col
    MOV  r2,A         ; R2 <- tilemap offset

    MOV  A,$AB        ; A <- $AB
    MOV  r15,A        ; R15 <- $AB (done sentinel)
    HALT              ; stop
.ENDS
