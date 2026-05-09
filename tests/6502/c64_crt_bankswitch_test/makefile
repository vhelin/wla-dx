CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -v

OFILES = bankswitch8k.o bankswitch16k.o bankswitch_easyflash_easycart.o bankswitch_c64gs.o bankswitch_rgcd.o bankswitch_gmod3.o hello_4k.o hello_8k.o hello_16k.o hello_ultimax4k.o hello_ultimax8k.o
CRTFILES = ocean.crt magicdesk.crt easyflash_easycart.crt gmod2.crt c64gs.crt comal80.crt rgcd.crt gmod3.crt normal4k.crt normal8k.crt normal16k.crt ultimax4k.crt ultimax8k.crt ultimax16k.crt simonsbasic.crt epyxfastload.crt

all: $(CRTFILES) check

ocean.crt: bankswitch8k.o bankswitch8k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 OCEAN bankswitch8k.linkfile ocean.crt

magicdesk.crt: bankswitch8k.o bankswitch8k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 MAGICDESK bankswitch8k.linkfile magicdesk.crt

gmod2.crt: bankswitch8k.o bankswitch8k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 GMOD2 bankswitch8k.linkfile gmod2.crt

rgcd.crt: bankswitch_rgcd.o bankswitch_rgcd.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 RGCD bankswitch_rgcd.linkfile rgcd.crt

gmod3.crt: bankswitch_gmod3.o bankswitch_gmod3.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 GMOD3 bankswitch_gmod3.linkfile gmod3.crt

easyflash_easycart.crt: bankswitch_easyflash_easycart.o bankswitch_easyflash_easycart.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 EASYFLASH bankswitch_easyflash_easycart.linkfile easyflash_easycart.crt

c64gs.crt: bankswitch_c64gs.o bankswitch_c64gs.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 C64GS bankswitch_c64gs.linkfile c64gs.crt

comal80.crt: bankswitch16k.o bankswitch16k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 COMAL80 bankswitch16k.linkfile comal80.crt

normal4k.crt: hello_4k.o hello_4k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 NORMAL4K hello_4k.linkfile normal4k.crt

normal8k.crt: hello_8k.o hello_8k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 NORMAL8K hello_8k.linkfile normal8k.crt

normal16k.crt: hello_16k.o hello_16k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 NORMAL16K hello_16k.linkfile normal16k.crt

ultimax4k.crt: hello_ultimax4k.o hello_ultimax4k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 ULTIMAX4K hello_ultimax4k.linkfile ultimax4k.crt

ultimax8k.crt: hello_ultimax8k.o hello_ultimax8k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 ULTIMAX8K hello_ultimax8k.linkfile ultimax8k.crt

ultimax16k.crt: hello_16k.o hello_16k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 ULTIMAX16K hello_16k.linkfile ultimax16k.crt

simonsbasic.crt: hello_16k.o hello_16k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 SIMONSBASIC hello_16k.linkfile simonsbasic.crt

epyxfastload.crt: hello_8k.o hello_8k.linkfile makefile
	$(LD) $(LDFLAGS) -t C64CRT -64 EPYXFASTLOAD hello_8k.linkfile epyxfastload.crt

bankswitch8k.o: bankswitch8k.s
	$(CC) $(CFLAGS) bankswitch8k.o bankswitch8k.s

bankswitch16k.o: bankswitch16k.s
	$(CC) $(CFLAGS) bankswitch16k.o bankswitch16k.s

bankswitch_easyflash_easycart.o: bankswitch_easyflash_easycart.s
	$(CC) $(CFLAGS) bankswitch_easyflash_easycart.o bankswitch_easyflash_easycart.s

bankswitch_c64gs.o: bankswitch_c64gs.s
	$(CC) $(CFLAGS) bankswitch_c64gs.o bankswitch_c64gs.s

bankswitch_rgcd.o: bankswitch_rgcd.s
	$(CC) $(CFLAGS) bankswitch_rgcd.o bankswitch_rgcd.s

bankswitch_gmod3.o: bankswitch_gmod3.s
	$(CC) $(CFLAGS) bankswitch_gmod3.o bankswitch_gmod3.s

hello_4k.o: hello_4k.s
	$(CC) $(CFLAGS) hello_4k.o hello_4k.s

hello_8k.o: hello_8k.s
	$(CC) $(CFLAGS) hello_8k.o hello_8k.s

hello_16k.o: hello_16k.s
	$(CC) $(CFLAGS) hello_16k.o hello_16k.s

hello_ultimax4k.o: hello_ultimax4k.s
	$(CC) $(CFLAGS) hello_ultimax4k.o hello_ultimax4k.s

hello_ultimax8k.o: hello_ultimax8k.s
	$(CC) $(CFLAGS) hello_ultimax8k.o hello_ultimax8k.s

check: $(CRTFILES)
	byte_tester testsfile

clean:
	rm -f $(OFILES) $(CRTFILES) *.sym *.lst core *~