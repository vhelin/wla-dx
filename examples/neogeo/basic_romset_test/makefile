ASM68K = wla-68000
ASMZ80 = wla-z80
ASMFLAGS = -o
LD = wlalink
LDFLAGS = -v -s
PYTHON ?= $(shell if command -v python3 >/dev/null 2>&1; then printf python3; else printf python; fi)

NG_GAME_ID = basic
NG_SOFTLIST_DESC = WLA DX Neo Geo basic ROM-set test
NG_P1 = $(NG_GAME_ID)-p1.bin
NG_M1 = $(NG_GAME_ID)-m1.bin
NG_S1 = $(NG_GAME_ID)-s1.bin
NG_C1 = $(NG_GAME_ID)-c1.bin
NG_C2 = $(NG_GAME_ID)-c2.bin

BLANK_ROM_SCRIPT = ../../../include/neogeo/make_blank_rom.py

include ../../../include/neogeo/neogeo.mk

.PHONY: all check run clean

all: check

check: $(NG_ZIP)
	test "$$(wc -c < "$(NG_P1)")" -eq 1048576
	test "$$(wc -c < "$(NG_M1)")" -eq 65536
	test "$$(wc -c < "$(NG_S1)")" -eq 131072
	test "$$(wc -c < "$(NG_C1)")" -eq 524288
	test "$$(wc -c < "$(NG_C2)")" -eq 524288

$(NG_P1): main68k.o linkfile.68k makefile
	$(LD) $(LDFLAGS) linkfile.68k $(NG_P1)

$(NG_M1): soundz80.o linkfile.z80 makefile
	$(LD) $(LDFLAGS) linkfile.z80 $(NG_M1)

main68k.o: main68k.s makefile
	$(ASM68K) $(ASMFLAGS) main68k.o main68k.s

soundz80.o: soundz80.s makefile
	$(ASMZ80) $(ASMFLAGS) soundz80.o soundz80.s

$(NG_S1): $(BLANK_ROM_SCRIPT) makefile
	$(PYTHON) $(BLANK_ROM_SCRIPT) $(NG_S1) 0x20000

$(NG_C1): $(BLANK_ROM_SCRIPT) makefile
	$(PYTHON) $(BLANK_ROM_SCRIPT) $(NG_C1) 0x80000

$(NG_C2): $(BLANK_ROM_SCRIPT) makefile
	$(PYTHON) $(BLANK_ROM_SCRIPT) $(NG_C2) 0x80000

run: all neogeo-run

clean: neogeo-clean
	rm -f main68k.o soundz80.o core *~ $(NG_P1) $(NG_M1) $(NG_S1) $(NG_C1) $(NG_C2) *.sym