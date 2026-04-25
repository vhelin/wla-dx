CC65816 = $(WLAVALGRIND) wla-65816
CCCX4   = $(WLAVALGRIND) wla-cx4
LD      = $(WLAVALGRIND) wlalink
CFLAGS  = -o
LDFLAGS = -v -i -s

all: result.rom check aresfolder

# 1) Assemble the Cx4 microcode.
cx4_prog.o: cx4_prog.s
	$(CCCX4) $(CFLAGS) cx4_prog.o cx4_prog.s

# 2) Link it to a raw binary blob for .INCBIN.
cx4_prog.bin: cx4_prog.o cx4.link
	$(LD) -b cx4.link cx4_prog.bin

# 3) Assemble the 65816 side (depends on the blob for .INCBIN).
main.o: main.s cx4_prog.bin
	$(CC65816) $(CFLAGS) main.o main.s

# 4) Link the final SNES ROM.
result.rom: main.o linkfile makefile
	$(LD) $(LDFLAGS) linkfile result.rom

# 5) byte_tester signature check (toolchain verification).
check: result.rom
	byte_tester -s main.s

# 6) 3 KB zero-filled HG51B data-ROM sidecar.  Our microcode does not use
#    RDROM, so zeros are fine; the file only has to exist so ares'
#    loadHitachiDSP() sees a real HG51BS169 data ROM.
cx4.data.rom:
	perl -e 'print "\x00" x 3072' > cx4.data.rom

# 7) Package a folder-pak that ares 147 can load.  ares' SuperFamicom
#    loader reads zips as raw ROM bytes (it does NOT extract them), so
#    folder-paks are the only working format.  Use one of:
#      - drag & drop  result.sfc/  onto the ares window
#      - copy result.sfc/ into  $USERPROFILE/Emulation/Super Famicom/
#        and launch it from ares' game library
#      - run  make install-ares  below
aresfolder: result.rom manifest.bml cx4.data.rom
	mkdir -p result.sfc
	cp -f result.rom       result.sfc/program.rom
	cp -f cx4.data.rom     result.sfc/hg51bs169.data.rom
	cp -f manifest.bml     result.sfc/manifest.bml

# 8) Install the pak into ares 147's default Windows game library, so it
#    shows up in the library browser and can be launched with one click.
install-ares: aresfolder
	@dest="$$USERPROFILE/Emulation/Super Famicom/cx4-hello-world.sfc"; \
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
