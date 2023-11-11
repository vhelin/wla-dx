
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -S -i

SFILES = setup.s linker_test.s
IFILES = defines.i
OFILES = setup.o linker_test.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

linker_test.o: linker_test.s defines.i
	$(CC) $(CFLAGS) linker_test.o linker_test.s

setup.o: setup.s defines.i
	$(CC) $(CFLAGS) setup.o setup.s

check: linked.rom
	byte_tester -s linker_test.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym *.lst
