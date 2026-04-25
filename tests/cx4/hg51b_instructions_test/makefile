CC65816 = $(WLAVALGRIND) wla-65816
CCCX4   = $(WLAVALGRIND) wla-cx4
LD      = $(WLAVALGRIND) wlalink
CFLAGS  = -o
LDFLAGS = -v -i -s

all: result.rom aresfolder

cx4_prog.o: cx4_prog.s
	$(CCCX4) $(CFLAGS) cx4_prog.o cx4_prog.s

cx4_prog.bin: cx4_prog.o cx4.link
	$(LD) -b cx4.link cx4_prog.bin

main.o: main.s cx4_prog.bin
	$(CC65816) $(CFLAGS) main.o main.s

result.rom: main.o linkfile makefile
	$(LD) $(LDFLAGS) linkfile result.rom

cx4.data.rom:
	perl -e 'for ($$i = 0; $$i < 1024; $$i++) { $$n = $$i & 0xF; print chr(0xA0 | $$n), chr(0xB0 | $$n), chr(0xC0 | $$n); }' > cx4.data.rom

aresfolder: result.rom manifest.bml cx4.data.rom
	mkdir -p result.sfc
	cp -f result.rom       result.sfc/program.rom
	cp -f cx4.data.rom     result.sfc/hg51bs169.data.rom
	cp -f manifest.bml     result.sfc/manifest.bml

install-ares: aresfolder
	@dest="$$USERPROFILE/Emulation/Super Famicom/cx4-instructions.sfc"; \
	dest_unix=$$(cygpath -u "$$dest"); \
	mkdir -p "$$dest_unix"; \
	cp -f result.sfc/program.rom        "$$dest_unix/program.rom"; \
	cp -f result.sfc/hg51bs169.data.rom "$$dest_unix/hg51bs169.data.rom"; \
	cp -f result.sfc/manifest.bml       "$$dest_unix/manifest.bml"; \
	echo "Installed to: $$dest"

clean:
	rm -f main.o cx4_prog.o cx4_prog.bin core *~ result.rom result.sym *.lst cx4.data.rom result.zip
	rm -rf result.sfc

clear: clean
