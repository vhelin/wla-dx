ASM68K = wla-68000
ASMZ80 = wla-z80
ASMFLAGS = -o
LD = wlalink
LDFLAGS = -v -s

MAME_AUTO_EXE := $(shell if command -v cygpath >/dev/null 2>&1 && [ -n "$$USERPROFILE" ]; then p=$$(cygpath -u "$$USERPROFILE/Downloads/Mame/mame.exe" 2>/dev/null); if [ -f "$$p" ]; then printf '%s' "$$p"; fi; fi)
MAME ?= $(if $(MAME_AUTO_EXE),$(MAME_AUTO_EXE),mame)
MAME_FLAGS ?= -window

TARGET = hello.bin
Z80_TARGET = hello-z80.bin
OFILES = main68k.o soundz80.o
RUN_ROM = $(shell if command -v cygpath >/dev/null 2>&1; then cygpath -w "$(abspath $(TARGET))"; else printf '%s' "$(abspath $(TARGET))"; fi)

.PHONY: all check run clean

all: check

check: $(TARGET)
	test "$$(wc -c < "$(TARGET)")" -eq 131072
	test "$$(wc -c < "$(Z80_TARGET)")" -eq 8192

$(TARGET): main68k.o linkfile.68k makefile
	$(LD) $(LDFLAGS) linkfile.68k $(TARGET)

main68k.o: main68k.s $(Z80_TARGET) makefile
	$(ASM68K) $(ASMFLAGS) main68k.o main68k.s

$(Z80_TARGET): soundz80.o linkfile.z80 makefile
	$(LD) $(LDFLAGS) linkfile.z80 $(Z80_TARGET)

soundz80.o: soundz80.s makefile
	$(ASMZ80) $(ASMFLAGS) soundz80.o soundz80.s

run: $(TARGET)
	"$(MAME)" genesis -cart "$(RUN_ROM)" $(MAME_FLAGS)

clean:
	rm -f $(OFILES) core *~ $(TARGET) $(Z80_TARGET) *.sym