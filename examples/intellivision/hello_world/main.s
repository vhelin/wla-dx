.INCLUDE "../../../memorymaps/intellivision_cart.i"

.ROMBANKSIZE $2000
.ROMBANKS 1

.EMPTYFILL $00

.DEFINE STIC_ENABLE      $0020
.DEFINE STIC_MODE        $0021
.DEFINE STIC_CS0         $0028
.DEFINE STIC_CS1         $0029
.DEFINE STIC_CS2         $002A
.DEFINE STIC_CS3         $002B
.DEFINE STIC_BORDER      $002C
.DEFINE STIC_HDELAY      $0030
.DEFINE STIC_VDELAY      $0031
.DEFINE STIC_BORDER_EXT  $0032
.DEFINE BACKTAB          $0200
.DEFINE BACKTAB_SIZE     240
.DEFINE STACK            $02F0
.DEFINE ISR_VEC          $0100
.DEFINE WTIMER           $0102
.DEFINE PSG_A_FINE       $01F0
.DEFINE PSG_A_COARSE     $01F4
.DEFINE PSG_MIXER        $01F8
.DEFINE PSG_A_VOL        $01FB
.DEFINE MIXER_TONE_A     $00FE
.DEFINE BEEP_PERIOD      $00A0
.DEFINE BEEP_FRAMES      8
.DEFINE BEEP_COUNT       3
.DEFINE HELLO_BACKTAB    $0268
.DEFINE HELLO_LEN        11
.DEFINE FG_WHITE         7

.BANK 0 SLOT 0
.ORGA $A000

; EXEC-friendly cartridge header. BIDECLE pointers are CPU word addresses.
RomHeader:
  .DW Zero/2, (Zero/2)>>8
  .DW Zero/2, (Zero/2)>>8
  .DW Main/2, (Main/2)>>8
  .DW Zero/2, (Zero/2)>>8
  .DW Ones/2, (Ones/2)>>8
  .DW Title/2, (Title/2)>>8
  .DW $03C0

Zero:
  .DW $0000
  .DW $0000

Ones:
  .DW 1, 1, 1, 1, 1

Title:
  .DW 126
  .DW $48, $45, $4C, $4C, $4F, $20, $57, $4F, $52, $4C, $44, 0


Main:
  DIS
  MVII #STACK, SP

  MVII #ISR, R0
  MVO R0, ISR_VEC
  SWAP R0
  MVO R0, ISR_VEC+1

  CALL ClearBacktab
  CALL DrawHelloWorld

  EIS

  MVII #2, R0
  CALL WaitFrames
  CALL PlayBeeps

Idle:
  B Idle


; EXEC dispatches here every vblank. Handshake the STIC immediately, then
; keep color-stack mode, a black border, and hidden MOBs.
ISR:
  MVO R0, STIC_ENABLE
  MVI STIC_MODE, R0

  CLRR R0
  MVO R0, STIC_CS0
  MVO R0, STIC_CS1
  MVO R0, STIC_CS2
  MVO R0, STIC_CS3
  MVO R0, STIC_BORDER
  MVO R0, STIC_HDELAY
  MVO R0, STIC_VDELAY
  MVO R0, STIC_BORDER_EXT

  MVII #$0000, R4
  MVII #16, R1
HideMobs:
  MVO@ R0, R4
  DECR R1
  BNEQ HideMobs

  MVI WTIMER, R0
  TSTR R0
  BEQ ISRDone
  DECR R0
  MVO R0, WTIMER
ISRDone:
  JR R5


WaitFrames:
  DIS
  MVO R0, WTIMER
  EIS
WaitFramesLoop:
  MVI WTIMER, R0
  TSTR R0
  BNEQ WaitFramesLoop
  JR R5


ClearBacktab:
  MVII #BACKTAB, R4
  MVII #BACKTAB_SIZE, R1
  CLRR R0
ClearLoop:
  MVO@ R0, R4
  DECR R1
  BNEQ ClearLoop
  JR R5


DrawHelloWorld:
  PSHR R5
  MVII #HELLO_BACKTAB, R4
  MVII #HelloText, R5
  MVII #HELLO_LEN, R1
DrawLoop:
  MVI@ R5, R0
  MVO@ R0, R4
  DECR R1
  BNEQ DrawLoop
  PULR R5
  JR R5


PlayBeeps:
  PSHR R5
  MVII #BEEP_PERIOD, R0
  MVO R0, PSG_A_FINE
  CLRR R0
  MVO R0, PSG_A_COARSE
  MVII #MIXER_TONE_A, R0
  MVO R0, PSG_MIXER

  MVII #BEEP_COUNT, R1
BeepLoop:
  MVII #$000F, R0
  MVO R0, PSG_A_VOL
  MVII #BEEP_FRAMES, R0
  CALL WaitFrames
  CLRR R0
  MVO R0, PSG_A_VOL
  MVII #BEEP_FRAMES, R0
  CALL WaitFrames
  DECR R1
  BNEQ BeepLoop

  PULR R5
  JR R5


; Color-stack BACKTAB words: ((ASCII-32) << 3) | white
HelloText:
  .DW (($48-$20)<<3)|FG_WHITE
  .DW (($45-$20)<<3)|FG_WHITE
  .DW (($4C-$20)<<3)|FG_WHITE
  .DW (($4C-$20)<<3)|FG_WHITE
  .DW (($4F-$20)<<3)|FG_WHITE
  .DW (($20-$20)<<3)|FG_WHITE
  .DW (($57-$20)<<3)|FG_WHITE
  .DW (($4F-$20)<<3)|FG_WHITE
  .DW (($52-$20)<<3)|FG_WHITE
  .DW (($4C-$20)<<3)|FG_WHITE
  .DW (($44-$20)<<3)|FG_WHITE
