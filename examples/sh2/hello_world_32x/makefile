ASM68K = wla-68000
ASMSH2 = wla-sh2
ASMFLAGS = -o
LD = wlalink
LDFLAGS = -v -s

MARS_STARTUP_BLOB ?= mars_startup.blob

ARES_AUTO_EXE := $(shell if command -v cygpath >/dev/null 2>&1 && [ -n "$$USERPROFILE" ]; then p=$$(cygpath -u "$$USERPROFILE/Downloads/ares-v147/ares.exe" 2>/dev/null); if [ -f "$$p" ]; then printf '%s' "$$p"; fi; fi)
ARES ?= $(if $(ARES_AUTO_EXE),$(ARES_AUTO_EXE),ares)
ARES_FLAGS ?=

TARGET = hello32x.32x
M68K_BIN = m68k.bin
BOOTSTRAP_BIN = bootstrap.bin
OFILES = main68k.o mainsh2.o
RUN_ROM = $(shell if command -v cygpath >/dev/null 2>&1; then cygpath -w "$(abspath $(TARGET))"; else printf '%s' "$(abspath $(TARGET))"; fi)

.PHONY: all check run clean

all: check

check: $(TARGET)
	test "$$(wc -c < "$(BOOTSTRAP_BIN)")" -eq 1040
	test "$$(wc -c < "$(M68K_BIN)")" -eq 1024
	test "$$(wc -c < "$(TARGET)")" -eq 131072

$(TARGET): mainsh2.o linkfile.sh2 makefile
	$(LD) $(LDFLAGS) linkfile.sh2 $(TARGET)

mainsh2.o: mainsh2.s $(BOOTSTRAP_BIN) $(M68K_BIN) makefile
	$(ASMSH2) $(ASMFLAGS) mainsh2.o mainsh2.s

$(M68K_BIN): main68k.o linkfile.68k makefile
	$(LD) $(LDFLAGS) linkfile.68k $(M68K_BIN)

main68k.o: main68k.s memorymap_hot68k.i makefile
	$(ASM68K) $(ASMFLAGS) main68k.o main68k.s

$(BOOTSTRAP_BIN): $(MARS_STARTUP_BLOB) makefile
	cp "$(MARS_STARTUP_BLOB)" $(BOOTSTRAP_BIN)
	test "$$(wc -c < "$(BOOTSTRAP_BIN)")" -eq 1040

run: $(TARGET)
	"$(ARES)" --system "Mega 32X" "$(RUN_ROM)" $(ARES_FLAGS)

clean:
	rm -f $(OFILES) core *~ $(TARGET) $(M68K_BIN) $(BOOTSTRAP_BIN) *.sym