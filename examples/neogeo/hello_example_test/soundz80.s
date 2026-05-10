;
; Z80 sound driver — Neo Geo hello example
;
; Listens for three beep commands from the 68k over REG_SOUND and
; plays a short SSG tone on YM2610 channel A for each:
;
;   $10  ->  low  tone  (SSG period coarse=$01 fine=$80)
;   $11  ->  mid  tone  (SSG period coarse=$00 fine=$C0)
;   $12  ->  high tone  (SSG period coarse=$00 fine=$60)
;
; BIOS housekeeping commands ($01 PREPARE_SWITCH, $03 RESET) are
; acknowledged and handled as required by the Neo Geo protocol.
;

.INCDIR "../../../include/neogeo"

.INCLUDE "../../../memorymaps/neogeoz80.i"
.INCLUDE "macros.inc"
.INCLUDE "z80.inc"

.ROMBANKMAP
  BANKSTOTAL 32
  BANKSIZE $0800
  BANKS 32
.ENDRO

.EMPTYFILL $00

; One byte in Z80 work RAM ($F800-$FFFF): the most-recently queued
; command posted by the NMI handler for the main loop to process.
.DEFINE PendingCmd  $F800
.DEFINE ResetWaitPc $FFFE
.DEFINE OpcodeJrSelf $FE18

.BANK 0 SLOT 0
.ORGA $0000

  di
  jp    z80_init

; RST 38h / mode-1 interrupt handler — not used, just return.
.ORGA $0038
  reti

; -------------------------------------------------------------------
; NMI handler ($0066) — called by the BIOS for every sound command.
; -------------------------------------------------------------------
.ORGA $0066
nmi_handler:
  in    a, (PORT_SOUND_COMMAND)

  cp    NG_SOUND_CMD_PREPARE_SWITCH
  jr    z, nmi_ack

  cp    NG_SOUND_CMD_RESET
  jr    z, nmi_do_reset

  ; Game command: store for the main loop and echo with bit 7 set.
  ld    (PendingCmd), a
  or    $80
  out   (PORT_SOUND_REPLY), a
  retn

nmi_ack:
  ; A still holds the received command; echo it with bit 7 set ($01 → $81).
  or    $80
  out   (PORT_SOUND_REPLY), a
  retn

nmi_do_reset:
  ; ACK the RESET command to the 68k BIOS before reinitialising.
  ld    a, NG_SOUND_CMD_RESET
  or    $80                   ; $83
  out   (PORT_SOUND_REPLY), a
  ; Switch to a clean stack and "return" into driver_start.
  di
  ld    sp, $FFFD
  ld    hl, driver_start
  push  hl
  retn                      ; pops driver_start address → PC = driver_start

; -------------------------------------------------------------------
; z80_init ($0100) — runs after power-on / hard reset.
; Selects bank 0 and enables NMIs, then halts waiting for the BIOS
; to send the RESET command ($03) via NMI.
; -------------------------------------------------------------------
.ORGA $0100
z80_init:
  im    1
  ld    sp, $FFFF            ; temporary stack at top of RAM
  NG_Z80_ENABLE_NMI          ; let the BIOS RESET command reach NMI

  ; Wait in RAM until the 68k sends a sound command NMI. This avoids
  ; depending on HALT wakeup behavior while the BIOS is still handing
  ; off to the cartridge.
  ld    bc, OpcodeJrSelf
  ld    (ResetWaitPc), bc
  ld    hl, ResetWaitPc
  ld    sp, hl
  push  hl
  ret

; -------------------------------------------------------------------
; driver_start — entered from nmi_do_reset.
; -------------------------------------------------------------------
driver_start:
  di
  ld    sp, $FFFD
  NG_Z80_INIT_LINEAR_BANKS
  NG_Z80_CLEAR_COMMAND

  xor   a
  ld    (PendingCmd), a

  call  ssg_mute

main_loop:
  ld    a, (PendingCmd)
  or    a
  jr    z, main_loop        ; nothing pending

  ld    b, a                ; save command
  xor   a
  ld    (PendingCmd), a     ; clear pending slot before processing
  ld    a, b                ; restore command

  cp    $10
  jr    z, beep_low
  cp    $11
  jr    z, beep_mid
  cp    $12
  jr    z, beep_high
  jr    main_loop            ; unknown command — ignore

beep_low:
  ld    b, $01               ; SSG period: coarse $01, fine $80 ≈ 710 Hz
  ld    c, $80
  call  ssg_beep
  jr    main_loop

beep_mid:
  ld    b, $00               ; SSG period: coarse $00, fine $C0 ≈ 1302 Hz
  ld    c, $C0
  call  ssg_beep
  jr    main_loop

beep_high:
  ld    b, $00               ; SSG period: coarse $00, fine $60 ≈ 1953 Hz
  ld    c, $60
  call  ssg_beep
  jr    main_loop

; -------------------------------------------------------------------
; ssg_beep — play SSG channel A at frequency (b=coarse, c=fine),
; hold for a short while, then silence the channel.
; Trashes A, HL.
; -------------------------------------------------------------------
ssg_beep:
  ; R0: SSG Ch-A fine tune
  xor   a
  out   (PORT_YM2610_A_ADDR), a
  ld    a, c
  out   (PORT_YM2610_A_DATA), a
  ; R1: SSG Ch-A coarse tune
  ld    a, 1
  out   (PORT_YM2610_A_ADDR), a
  ld    a, b
  out   (PORT_YM2610_A_DATA), a
  ; R7: mixer — tone A on, everything else off ($3E = %00111110)
  ld    a, 7
  out   (PORT_YM2610_A_ADDR), a
  ld    a, $3E
  out   (PORT_YM2610_A_DATA), a
  ; R8: Ch-A amplitude — maximum fixed level ($0F)
  ld    a, 8
  out   (PORT_YM2610_A_ADDR), a
  ld    a, $0F
  out   (PORT_YM2610_A_DATA), a

  ; Hold the note for a short period.
  ld    hl, $C000
ssg_beep_hold:
  dec   hl
  ld    a, h
  or    l
  jr    nz, ssg_beep_hold

  ; Silence: amplitude 0.
  ld    a, 8
  out   (PORT_YM2610_A_ADDR), a
  xor   a
  out   (PORT_YM2610_A_DATA), a
  ret

; -------------------------------------------------------------------
; ssg_mute — silence all three SSG channels.
; Trashes A, B.
; -------------------------------------------------------------------
ssg_mute:
  ; R7: disable all tones and noise.
  ld    a, 7
  out   (PORT_YM2610_A_ADDR), a
  ld    a, $3F
  out   (PORT_YM2610_A_DATA), a
  ; R8/R9/R10: amplitude 0 on all channels.
  ld    b, 8
ssg_mute_loop:
  ld    a, b
  out   (PORT_YM2610_A_ADDR), a
  xor   a
  out   (PORT_YM2610_A_DATA), a
  inc   b
  ld    a, b
  cp    11
  jr    nz, ssg_mute_loop
  ret
