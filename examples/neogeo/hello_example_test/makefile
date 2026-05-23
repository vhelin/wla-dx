ASM68K = wla-68000
ASMZ80 = wla-z80
ASMFLAGS = -o
LD = wlalink
LDFLAGS = -v -s
PYTHON ?= $(shell if command -v python3 >/dev/null 2>&1; then printf python3; else printf python; fi)

NG_GAME_ID = hello
NG_SOFTLIST_DESC = WLA DX Neo Geo hello-world example
NG_P1 = hello-p1.bin
NG_M1 = hello-m1.bin
NG_S1 = hello-s1.bin
NG_C1 = hello-c1.bin
NG_C2 = hello-c2.bin

C_ROMS_STAMP = .c-roms.stamp

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

$(NG_S1): make_s1.py makefile
	$(PYTHON) make_s1.py $(NG_S1)

$(NG_C1) $(NG_C2): $(C_ROMS_STAMP)

$(C_ROMS_STAMP): make_c_roms.py makefile
	$(PYTHON) make_c_roms.py $(NG_C1) $(NG_C2)
	touch $(C_ROMS_STAMP)

run: all neogeo-run

clean: neogeo-clean
	rm -f main68k.o soundz80.o core *~ $(NG_P1) $(NG_M1) $(NG_S1) $(NG_C1) $(NG_C2) $(C_ROMS_STAMP) *.sym