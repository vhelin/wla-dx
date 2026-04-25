        .MEMORYMAP
          SLOTSIZE $8000
          DEFAULTSLOT 0
          SLOT 0 START $0000
        .ENDME

        .ROMBANKSIZE $8000
        .ROMBANKS 1

        .BANK 0 SLOT 0

        .ORG $0134
MathInit:
        NOP
        RTS

        .ORG $02C0
RenderLoop:
        NOP
        RTS

        .ORG $0348
SoundPatch:
        NOP
        RTS

        .ORG $045C
OverlayTask:
        NOP
        RTS